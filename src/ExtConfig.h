#include <iostream>
#include <fstream>
#include <map>
#include <string>   

class IniReader {
private:
    std::ifstream IniArchive;
    std::string infoLog;
    std::map<std::string, float> Configs;

public:
    IniReader();
    ~IniReader();

    void ReadIniArchive(const char* path_and_name);
    float GetConfigValue(std::string config);
    std::map<std::string, float>& GetConfigs();

};