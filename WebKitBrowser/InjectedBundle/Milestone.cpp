/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2020 RDK Management
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
 
#ifdef WEBKIT_GLIB_API

#include "Module.h"
#include "Milestone.h"

namespace WPEFramework {
namespace JavaScript {
namespace Milestone {

static void automationMilestone(const char* arg1, const char* arg2, const char* arg3)
{
    g_printerr("TEST TRACE: \"%s\" \"%s\" \"%s\"\n", arg1, arg2, arg3);
    TRACE_GLOBAL(Trace::Information, (_T("TEST TRACE: \"%s\" \"%s\" \"%s\""), arg1, arg2, arg3));
}

void InjectJS(WebKitScriptWorld* world, WebKitFrame* frame)
{
    if (webkit_frame_is_main_frame(frame) == false)
        return;

    JSCContext* jsContext = webkit_frame_get_js_context_for_script_world(frame, world);

    JSCValue* jsObject = jsc_value_new_object(jsContext, nullptr, nullptr);
    JSCValue* jsFunction = jsc_value_new_function(jsContext, nullptr, reinterpret_cast<GCallback>(automationMilestone),
        nullptr, nullptr, G_TYPE_NONE, 3, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
    jsc_value_object_set_property(jsObject, "Milestone", jsFunction);
    g_object_unref(jsFunction);
    jsc_context_set_value(jsContext, "automation", jsObject);
    g_object_unref(jsObject);

    g_object_unref(jsContext);
}

}  // Milestone
}  // JavaScript
}  // WPEFramework

#else

#include "Milestone.h"

#include "Utils.h"

#include <iostream>
#include <sstream>

// Global handle to this bundle.
extern WKBundleRef g_Bundle;

namespace WPEFramework {

namespace JavaScript {

    namespace Functions {

        Milestone::Milestone()
        {
        }

        JSValueRef Milestone::HandleMessage(JSContextRef context, JSObjectRef,
            JSObjectRef, size_t argumentCount, const JSValueRef arguments[], JSValueRef*)
        {
            const int acceptedArgCount = 3;
            const size_t bufferSize = 1500; // Is limited by UDP package size.

            if (argumentCount != acceptedArgCount) {
                std::cerr << "Milestone only accepts 3 string arguments" << std::endl;
                return JSValueMakeNull(context);
            }

            string argStrings[acceptedArgCount];

            for (unsigned int index = 0; index < argumentCount; index++) {
                const JSValueRef& argument = arguments[index];

                if (!JSValueIsString(context, argument)) {
                    std::cerr << "Milestone function only accepts string arguments" << std::endl;
                    return JSValueMakeNull(context);
                }

                JSStringRef jsString = JSValueToStringCopy(context, argument, nullptr);
                char stringBuffer[bufferSize];

                // TODO: for now assumption is ASCII, should we also deal with Unicode?
                JSStringGetUTF8CString(jsString, stringBuffer, bufferSize);
                JSStringRelease(jsString);

                argStrings[index] = stringBuffer;
            }

            std::stringstream ssMessage;
            ssMessage << "TEST TRACE:";
            for (const string& argString : argStrings) {
                ssMessage << " \"" << argString << "\"";
            }

            std::cerr << ssMessage.str() << std::endl;

            TRACE_GLOBAL(Trace::Information, (ssMessage.str()));

            return JSValueMakeNull(context);
        }

        static JavaScriptFunctionType<Milestone> _instance(_T("automation"));
    }
}
}

#endif // WEBKIT_GLIB_API
