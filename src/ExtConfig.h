#ifndef EXTCONFIG_H
#define EXTCONFIG_H

#include <iostream>
#include <fstream>
#include <map>
#include <string>   
#include <filesystem>

class IniReader {
private:
    std::ifstream IniArchive;
    std::string infoLog;
    std::map<std::string, float> Configs;

public:
    IniReader();
    ~IniReader();

    bool ReadIniArchive(const std::filesystem::path& path_and_name);
    float GetConfigValue(const std::string& config);
    std::map<std::string, float>& GetConfigs();

};

#endif
