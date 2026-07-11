#include "ExtConfig.h"

IniReader::IniReader() 
{
}

IniReader::~IniReader() 
{
}

bool IniReader::ReadIniArchive(const std::filesystem::path& path_and_name) {
    std::string     line             = "";
    std::string     t_line_value     = "";
    std::string     t_line           = "";
    float           t_value          = 0;
    bool            point_trigger    = 0;

    IniArchive.open(path_and_name.string());
    if (IniArchive.is_open()) {

        while (std::getline(IniArchive, line)) {
            if (line.empty()) { // .empty() verifica se esta vazia
                /*Nada*/
            } else if ((line[0] == '#') || (line[0] == ' ')) {  
                /*Nada*/
            }
            else { 
                for (int i = 0; i < line.length(); i++) {
                    if (line[i] == ':' || line[i] == '=') {point_trigger = 1;}

                    if (!point_trigger) {
                        t_line += line[i];
                    } 
                    if (point_trigger) {
                        t_line_value += line[i];
                    }
                }
                t_line_value.erase(0, 1);
                point_trigger = 0;
                Configs.insert({t_line, std::stof(t_line_value)});
            }

            line.clear();
            t_line_value.clear();
            t_line.clear();
            t_value             = 0;
            point_trigger       = 0;
        }
    } else {
      infoLog += "INIREADER::COULDNT_OPEN_FILE\n";
      return -1;
    }
    IniArchive.close();

    return 0;
}

float IniReader::GetConfigValue(const std::string& config) {
    auto it = Configs.find(config);
    std::map<std::string, float>::iterator null_it{};   // iterador nulo

    if (it != null_it) {
        return it->second;
    }
    else {
        infoLog = "INIREADER::KEY_NOT_FOUND\n";
        return -1;
    }
}

std::map<std::string, float>& IniReader::GetConfigs() {
    return Configs;
}
