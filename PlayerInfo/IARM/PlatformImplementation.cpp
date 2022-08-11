/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2020 Metrological
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "../Module.h"
#include <interfaces/IPlayerInfo.h>
#include <interfaces/IDolby.h>
#include "host.hpp"
#include "exception.hpp"
#include "audioOutputPortType.hpp"
#include "audioOutputPortConfig.hpp"
#include "audioOutputPort.hpp"
#include "utils.h"

#include <gst/gst.h>

#include "libIBus.h"
#include "libIBusDaemon.h"
#include "dsMgr.h"

namespace WPEFramework {
namespace Plugin {

class PlayerInfoImplementation : public Exchange::IPlayerProperties, public Exchange::Dolby::IOutput
{
private:

    class GstUtils {
    private:
        struct FeatureListDeleter {
            void operator()(GList* p) { gst_plugin_feature_list_free(p); }
        };

        struct CapsDeleter {
            void operator()(GstCaps* p) { gst_caps_unref(p); }
        };
        typedef std::unique_ptr<GList, FeatureListDeleter> FeatureList;
        typedef std::unique_ptr<GstCaps, CapsDeleter> MediaTypes;

   public:
        GstUtils() = delete;
        GstUtils(const GstUtils&) = delete;
        GstUtils& operator= (const GstUtils&) = delete;

        template <typename C, typename CodecIteratorList>
        static bool GstRegistryCheckElementsForMediaTypes(C caps, CodecIteratorList& codecIteratorList) {

            auto type = std::is_same<C, VideoCaps>::value ? GST_ELEMENT_FACTORY_TYPE_MEDIA_VIDEO : GST_ELEMENT_FACTORY_TYPE_MEDIA_AUDIO;

            FeatureList decoderFactories{gst_element_factory_list_get_elements(GST_ELEMENT_FACTORY_TYPE_DECODER | type, GST_RANK_MARGINAL)};
            FeatureList parserFactories{gst_element_factory_list_get_elements(GST_ELEMENT_FACTORY_TYPE_PARSER | type, GST_RANK_MARGINAL)};

            FeatureList elements;
            for (auto index: caps) {

                MediaTypes mediaType{gst_caps_from_string(index.first.c_str())};
                if (elements = std::move(GstUtils::GstRegistryGetElementForMediaType(decoderFactories.get(), std::move(mediaType)))) {
                    codecIteratorList.push_back(index.second);

                } else if (elements = std::move(GstUtils::GstRegistryGetElementForMediaType(parserFactories.get(), std::move(mediaType)))) {

                    for (GList* iterator = elements.get(); iterator; iterator = iterator->next) {

                        GstElementFactory* gstElementFactory = static_cast<GstElementFactory*>(iterator->data);
                        const GList* padTemplates = gst_element_factory_get_static_pad_templates(gstElementFactory);

                        for (const GList* padTemplatesIterator = padTemplates; padTemplatesIterator; padTemplatesIterator = padTemplatesIterator->next) {
                            GstStaticPadTemplate* padTemplate = static_cast<GstStaticPadTemplate*>(padTemplatesIterator->data);

                            if (padTemplate->direction == GST_PAD_SRC) {
                                MediaTypes mediaTypes{gst_static_pad_template_get_caps(padTemplate)};
                                if (GstUtils::GstRegistryGetElementForMediaType(decoderFactories.get(), std::move(mediaTypes))) {
                                    codecIteratorList.push_back(index.second);
                                }
                            }
                        }
                    }
                 }
             }

             return (codecIteratorList.size() != 0);
         }

    private:
        static inline FeatureList GstRegistryGetElementForMediaType(GList* elementsFactories, MediaTypes&& mediaTypes) {
            FeatureList candidates{gst_element_factory_list_filter(elementsFactories, mediaTypes.get(), GST_PAD_SINK, false)};

            return std::move(candidates);
        }

    };

private:
    using AudioIteratorImplementation = RPC::IteratorType<Exchange::IPlayerProperties::IAudioCodecIterator>;
    using VideoIteratorImplementation = RPC::IteratorType<Exchange::IPlayerProperties::IVideoCodecIterator>;

    typedef std::map<const string, const Exchange::IPlayerProperties::AudioCodec> AudioCaps;
    typedef std::map<const string, const Exchange::IPlayerProperties::VideoCodec> VideoCaps;

public:
    PlayerInfoImplementation()
    {
        gst_init(0, nullptr);
        UpdateAudioCodecInfo();
        UpdateVideoCodecInfo();
        IARM_Result_t res;
        IARM_CHECK( IARM_Bus_RegisterEventHandler(IARM_BUS_DSMGR_NAME,IARM_BUS_DSMGR_EVENT_AUDIO_MODE, AudioModeHandler) );
        PlayerInfoImplementation::_instance = this;
    }

    PlayerInfoImplementation(const PlayerInfoImplementation&) = delete;
    PlayerInfoImplementation& operator= (const PlayerInfoImplementation&) = delete;
    ~PlayerInfoImplementation() override
    {
        _audioCodecs.clear();
        _videoCodecs.clear();
        IARM_Result_t res;
        IARM_CHECK( IARM_Bus_UnRegisterEventHandler(IARM_BUS_DSMGR_NAME,IARM_BUS_DSMGR_EVENT_AUDIO_MODE) );
        PlayerInfoImplementation::_instance = nullptr;
    }

public:
    uint32_t AudioCodecs(Exchange::IPlayerProperties::IAudioCodecIterator*& iterator) const override
    {
        iterator = Core::Service<AudioIteratorImplementation>::Create<Exchange::IPlayerProperties::IAudioCodecIterator>(_audioCodecs);
        return (iterator != nullptr ? Core::ERROR_NONE : Core::ERROR_GENERAL);
    }
    uint32_t VideoCodecs(Exchange::IPlayerProperties::IVideoCodecIterator*& iterator) const override
    {
        iterator = Core::Service<VideoIteratorImplementation>::Create<Exchange::IPlayerProperties::IVideoCodecIterator>(_videoCodecs);
        return (iterator != nullptr ? Core::ERROR_NONE : Core::ERROR_GENERAL);
    }

    uint32_t Resolution(PlaybackResolution& res /* @out */) const override
    {
        res = RESOLUTION_UNKNOWN;

        string currentResolution = "0";
        try
        {
            device::VideoOutputPort &vPort = device::Host::getInstance().getVideoOutputPort("HDMI0");
            currentResolution = vPort.getResolution().getName();
            TRACE(Trace::Information, (_T("Current video playback resolution = %s"), currentResolution));
        }
        catch(const device::Exception& err)
        {
            TRACE(Trace::Error, (_T("Exception during DeviceSetting library call. code = %d message = %s"), err.getCode(), err.what()));
        }

        if (currentResolution == "720p") res = RESOLUTION_720P;
        else if(currentResolution == "1080i") res = RESOLUTION_1080I;
        else if(currentResolution == "1080p60") res = RESOLUTION_1080P;
        else if(currentResolution == "2160p30") res = RESOLUTION_2160P30;
        else if(currentResolution == "2160p60") res = RESOLUTION_2160P60;
        else res = RESOLUTION_UNKNOWN;

        return (Core::ERROR_NONE);
    }

    uint32_t IsAudioEquivalenceEnabled(bool& isEnbaled /* @out */) const override
    {
        isEnbaled = false;
        try
        {
            device::AudioOutputPort aPort = device::Host::getInstance().getAudioOutputPort("HDMI0");
            if (aPort.isConnected()) {
                isEnbaled = aPort.GetLEConfig();
                LOGINFO("IsAudioEquivalenceEnabled = %s", isEnbaled? "Enabled":"Disabled");
            }
            else
            {
                TRACE(Trace::Information, (_T("IsAudioEquivalenceEnabled failure: HDMI0 not connected!")));

                LOGERR("IsAudioEquivalenceEnabled failure: HDMI0 not connected!");
            }
        }
        catch(const device::Exception& err)
        {
            TRACE(Trace::Error, (_T("Exception during DeviceSetting library call. code = %d message = %s"), err.getCode(), err.what()));
        }
        TRACE(Trace::Information, (_T("Audio Equivalence = %d"), isEnbaled? "Enabled":"Disabled"));


        return (Core::ERROR_NONE);
    }

    uint32_t Register(Exchange::Dolby::IOutput::INotification* notification) override
    {
        _adminLock.Lock();

        // Make sure a sink is not registered multiple times.
        ASSERT(std::find(_observers.begin(), _observers.end(), notification) == _observers.end());

        _observers.push_back(notification);
        notification->AddRef();

        _adminLock.Unlock();

        return (Core::ERROR_NONE);
    }

    uint32_t Unregister(Exchange::Dolby::IOutput::INotification* notification) override
    {
        _adminLock.Lock();

        std::list<Exchange::Dolby::IOutput::INotification*>::iterator index(std::find(_observers.begin(), _observers.end(), notification));

        // Make sure you do not unregister something you did not register !!!
        ASSERT(index != _observers.end());

        if (index != _observers.end()) {
            (*index)->Release();
            _observers.erase(index);
        }

        _adminLock.Unlock();

        return (Core::ERROR_NONE);
    }

    static void AudioModeHandler(const char *owner, IARM_EventId_t eventId, void *data, size_t len)
    {
        if(PlayerInfoImplementation::_instance)
        {
            PlayerInfoImplementation::_instance->audiomodeChanged(STEREO, true);
        }
    }

    void audiomodeChanged(Exchange::Dolby::IOutput::SoundModes mode, bool enable)
    {
         _adminLock.Lock();

        std::list<Exchange::Dolby::IOutput::INotification*>::const_iterator index = _observers.begin();

        while(index != _observers.end()) {
            (*index)->AudioModeChanged(mode, enable);
            index++;
        }

        _adminLock.Unlock();

    }

    uint32_t Mode(const Exchange::Dolby::IOutput::Type& mode) override
    {
        return (Core::ERROR_GENERAL);
    }

    uint32_t Mode(Exchange::Dolby::IOutput::Type& mode) const override
    {
        return (Core::ERROR_GENERAL);
    }


    uint32_t AtmosMetadata(bool& supported /* @out */) const override
    {
        dsATMOSCapability_t atmosCapability;
        supported = false;
        try
        {
            device::AudioOutputPort aPort = device::Host::getInstance().getAudioOutputPort("HDMI0");
            if (aPort.isConnected())
            {
                aPort.getSinkDeviceAtmosCapability(atmosCapability);
            }
            else
            {
               TRACE(Trace::Error, (_T("getSinkAtmosCapability failure: HDMI0 not connected!\n")));
            }
        }
        catch(const device::Exception& err)
        {
            TRACE(Trace::Error, (_T("Exception during DeviceSetting library call. code = %d message = %s"), err.getCode(), err.what()));
        }

        if(atmosCapability == dsAUDIO_ATMOS_ATMOSMETADATA) supported = true;
        return (Core::ERROR_NONE);
    }

    uint32_t SoundMode(Exchange::Dolby::IOutput::SoundModes& mode /* @out */) const override
    {
        string audioPort;
        device::AudioStereoMode soundmode = device::AudioStereoMode::kStereo;
        mode = UNKNOWN;

        try
        {
            /* Return the sound mode of the audio ouput connected to the specified videoDisplay */
            /* Check if HDMI is connected - Return (default) Stereo Mode if not connected */

            if (device::Host::getInstance().getVideoOutputPort("HDMI0").isDisplayConnected())
            {
                audioPort = "HDMI0";
            }
            else
            {
                /*  * If HDMI is not connected
                    * Get the SPDIF if it is supported by platform
                    * If Platform does not have connected ports. Default to HDMI.
                */
                audioPort = "HDMI0";
                device::List<device::VideoOutputPort> vPorts = device::Host::getInstance().getVideoOutputPorts();
                for (size_t i = 0; i < vPorts.size(); i++)
                {
                    device::VideoOutputPort &vPort = vPorts.at(i);
                    if (vPort.isDisplayConnected())
                    {
                        audioPort = "SPDIF0";
                        break;
                    }
                }
            }

            device::AudioOutputPort aPort = device::Host::getInstance().getAudioOutputPort(audioPort);

            if (aPort.isConnected())
            {
                soundmode = aPort.getStereoMode();
                if (soundmode == device::AudioStereoMode::kSurround) mode = SURROUND;
                else if(soundmode == device::AudioStereoMode::kStereo) mode = STEREO;
                else if(soundmode == device::AudioStereoMode::kMono) mode = MONO;
                else if(soundmode == device::AudioStereoMode::kPassThru) mode = PASSTHRU;
                else mode = UNKNOWN;
            }
        }
        catch (const device::Exception& err)
        {
            TRACE(Trace::Error, (_T("Exception during DeviceSetting library call. code = %d message = %s"), err.getCode(), err.what()));
        }

        return (Core::ERROR_NONE);
    }

    uint32_t EnableAtmosOutput(const bool& enable /* @in */)
    {
        try
        {
            device::AudioOutputPort aPort = device::Host::getInstance().getAudioOutputPort("HDMI0");
            if (aPort.isConnected()) {
                aPort.setAudioAtmosOutputMode(enable);
            }
            else
            {
                TRACE(Trace::Error, (_T("setAudioAtmosOutputMode failure: HDMI0 not connected!\n")));
            }

        }
        catch (const device::Exception& err)
        {
            TRACE(Trace::Error, (_T("Exception during DeviceSetting library call. code = %d message = %s"), err.getCode(), err.what()));
        }
        return (Core::ERROR_NONE);
    }

    BEGIN_INTERFACE_MAP(PlayerInfoImplementation)
    INTERFACE_ENTRY(Exchange::IPlayerProperties)
    INTERFACE_ENTRY(Exchange::Dolby::IOutput)
    END_INTERFACE_MAP

private:


    void UpdateAudioCodecInfo()
    {
        AudioCaps audioCaps = {
            {"audio/mpeg, mpegversion=(int)1", Exchange::IPlayerProperties::AudioCodec::AUDIO_MPEG1},
            {"audio/mpeg, mpegversion=(int)2", Exchange::IPlayerProperties::AudioCodec::AUDIO_MPEG2},
            {"audio/mpeg, mpegversion=(int)4", Exchange::IPlayerProperties::AudioCodec::AUDIO_MPEG4},
            {"audio/mpeg, mpegversion=(int)1, layer=(int)[1, 3]", Exchange::IPlayerProperties::AudioCodec::AUDIO_MPEG3},
            {"audio/mpeg, mpegversion=(int){2, 4}", Exchange::IPlayerProperties::AudioCodec::AUDIO_AAC},
            {"audio/x-ac3", Exchange::IPlayerProperties::AUDIO_AC3},
            {"audio/x-eac3", Exchange::IPlayerProperties::AUDIO_AC3_PLUS},
            {"audio/x-opus", Exchange::IPlayerProperties::AUDIO_OPUS},
            {"audio/x-dts", Exchange::IPlayerProperties::AUDIO_DTS},
            {"audio/x-vorbis", Exchange::IPlayerProperties::AUDIO_VORBIS_OGG},
            {"audio/x-wav", Exchange::IPlayerProperties::AUDIO_WAV},
        };
        if (GstUtils::GstRegistryCheckElementsForMediaTypes(audioCaps, _audioCodecs) != true) {
            TRACE(Trace::Warning, (_T("There is no Audio Codec support available")));
        }

    }
    void UpdateVideoCodecInfo()
    {
        VideoCaps videoCaps = {
            {"video/x-h263", Exchange::IPlayerProperties::VideoCodec::VIDEO_H263},
            {"video/x-h264, profile=(string)high", Exchange::IPlayerProperties::VideoCodec::VIDEO_H264},
            {"video/x-h265", Exchange::IPlayerProperties::VideoCodec::VIDEO_H265},
            {"video/mpeg, mpegversion=(int){1,2}, systemstream=(boolean)false", Exchange::IPlayerProperties::VideoCodec::VIDEO_MPEG},
            {"video/x-vp8", Exchange::IPlayerProperties::VideoCodec::VIDEO_VP8},
            {"video/x-vp9", Exchange::IPlayerProperties::VideoCodec::VIDEO_VP9},
            {"video/x-vp10", Exchange::IPlayerProperties::VideoCodec::VIDEO_VP10}
        };
        if (GstUtils::GstRegistryCheckElementsForMediaTypes(videoCaps, _videoCodecs) != true) {
            TRACE(Trace::Warning, (_T("There is no Video Codec support available")));
        }
    }

private:
    std::list<Exchange::IPlayerProperties::AudioCodec> _audioCodecs;
    std::list<Exchange::IPlayerProperties::VideoCodec> _videoCodecs;

    std::list<Exchange::Dolby::IOutput::INotification*> _observers;
    mutable Core::CriticalSection _adminLock;
public:
    static PlayerInfoImplementation* _instance;
};
    PlayerInfoImplementation* PlayerInfoImplementation::_instance = nullptr;
    SERVICE_REGISTRATION(PlayerInfoImplementation, 1, 0);
}
}
