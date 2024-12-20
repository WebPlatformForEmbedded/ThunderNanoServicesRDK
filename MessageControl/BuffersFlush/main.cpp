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

#include "Module.h"
#include <fstream>
#include <dirent.h>
#include <sys/types.h>

class MessagingJSON : public Thunder::Core::JSON::Container {
public:
    Thunder::Core::JSON::String Path;

    MessagingJSON()
        : Path("MessageDispatcher")
    {
        Add("path", &Path);
    }
};

class RootJSON : public Thunder::Core::JSON::Container {
public:
    Thunder::Core::JSON::String VolatilePath;
    MessagingJSON Messaging;

    RootJSON()
        : VolatilePath()
        , Messaging()
    {
        Add("volatilepath", &VolatilePath);
        Add("messaging", &Messaging);
    }
};

std::string getPathFromMessaging(const std::string& jsonFilePath)
{
    std::string result;
    std::ifstream file(jsonFilePath);

    if (file.is_open() == false) {
        std::cerr << "Error: Could not open the file!" << std::endl;
    }
    else {
        std::ostringstream oss;
        oss << file.rdbuf();
        std::string jsonData = oss.str();
        RootJSON json;

        if (json.FromString(jsonData) == false) {
            std::cerr << "Error: Failed to parse JSON!" << std::endl;
        }
        else {
            if (json.VolatilePath.Value().empty() == true) {
                std::cerr << "Warning: VolatilePath is empty or unspecified!" << std::endl;
            }
            else {
                result += json.VolatilePath.Value() + "/";
            }
            result += json.Messaging.Path.Value() + "/";
            std::cout << "Found the messaging MemoryMapped files path: " << result << std::endl;
        }
    }
    return (result);
}

std::string parseOptionalParameter(int argc, char* argv[], const std::string& paramName)
{
    std::string result;

    for (int i = 1; i < argc; ++i) {

        if ((std::string(argv[i]) == paramName) && (i + 1 < argc)) {
            result = argv[i + 1];
        }
    }
    return (result);
}

void printAllMemoryMappedFiles(const string& messagingPath)
{
    DIR* dir = opendir(messagingPath.c_str());

    if (dir == nullptr) {
        std::cerr << "Error! Could not open directory: " << messagingPath << std::endl;
    }
    else {
        struct dirent* entry;

        while ((entry = readdir(dir)) != nullptr) {
            std::string fileName = entry->d_name;

            if ((fileName.rfind("md.", 0) == 0) && (fileName.size() > 8) && (fileName.substr(fileName.size() - 5) == ".data")) {
                std::string filePath = messagingPath + fileName;
                std::ifstream file(filePath);

                if (!file) {
                    std::cerr << "Error! Could not open file: " << filePath << std::endl;
                }
                else {
                    std::cout << "---------- File: " << fileName << " ----------\n";
                    std::string line;

                    while (std::getline(file, line)) {
                        std::cout << line << std::endl;
                    }
                    std::cout << "---------- End of " << fileName << " ----------\n\n";
                }
            }
        }
        closedir(dir);
    }
}

int main(int argc, char** argv)
{
    std::string configPath = parseOptionalParameter(argc, argv, "--config");

    if (configPath.empty() == false) {
        std::cout << "Using the config path passed as a parameter: " << configPath << std::endl;
    }
    else {
        configPath = "/etc/Thunder/config.json";
        std::cout << "Using the default config path: " << configPath << std::endl;
    }

    std::string messagingPath = getPathFromMessaging(configPath);

    if (messagingPath.empty() == false) {
        printAllMemoryMappedFiles(messagingPath);
    }

    return (0);
}
