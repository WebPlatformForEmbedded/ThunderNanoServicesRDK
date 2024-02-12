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
 
#include "SecurityContext.h"

namespace WPEFramework {
namespace Plugin {

    SecurityContext::SecurityContext(const AccessControlList* acl, const uint16_t length, const uint8_t payload[], const string& servicePrefix)
        : _token(string(reinterpret_cast<const TCHAR*>(payload), length))
        , _accessControlList(acl)
        , _servicePrefix(servicePrefix)
    {
        if (_context.FromString(_token) == false) {
            _context.URL = _token;
        }
    }

    /* virtual */ SecurityContext::~SecurityContext()
    {
    }

    //! Allow a websocket upgrade to be checked if it is allowed to be opened.
    bool SecurityContext::Allowed(const string&) const /* override */
    {
        return (true);
    }

    //! Allow a request to be checked before it is offered for processing.
    bool SecurityContext::Allowed(const Web::Request& request) const /* override */
    {
        string callsign = "";
        string method = "";

        if ((request.Path.find(_servicePrefix, 0) != 0) || (_servicePrefix.length() > request.Path.length())) {
            return (_accessControlList != nullptr);
        }

        Core::TextSegmentIterator index(
            Core::TextFragment(
                request.Path, 
                static_cast<uint32_t>(_servicePrefix.length()), 
                static_cast<uint32_t>(request.Path.length() - _servicePrefix.length())), 
            false, 
            '/');

        if (index.Next()) {
            callsign = index.Current().Text();
            if (index.Next()) {
                method = index.Current().Text();
            }
        }

        return ((_accessControlList != nullptr) && (_accessControlList->Allowed(_context.URL.Value(), callsign, method)));
    }

    //! Allow a JSONRPC message to be checked before it is offered for processing.
    bool SecurityContext::Allowed(const Core::JSONRPC::Message& message) const /* override */ 
    {
        bool bAllowed = ((_accessControlList != nullptr) && (_accessControlList->Allowed(_context.URL.Value(), message.Callsign(), message.Method())));
        if(!bAllowed)
            TRACE(Security, ("Access for token url [%s], plugin [%s], method [%s] not allowed", _context.URL.Value().c_str(),message.Callsign().c_str(),message.Method().c_str()));

        return bAllowed;
    }

    string SecurityContext::Token() const /* override */
    {
        return (_token);
    }

}
}
