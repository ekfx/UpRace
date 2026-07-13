#ifdef CXX26_DEBUG

#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <iostream>
#include <filesystem>
#include <string>
#include <fstream>
#include <map>
#include <expected>
#include <optional>
#include <memory>
#include <variant>
#include <meta>
#include <type_traits>
#include <cstdint>
#include "Advanced.hpp"

class Configurator {
private:
    std::map<std::string, float> n_data;
    std::map<std::string, std::string> c_data;
    int READ_PREFERENCE;
    /*
        0  -> automatic
        1  -> only text
        -1 -> only numbers
    */

public:
    Configurator(int read_preference = 0) : READ_PREFERENCE(read_preference) {};
    ~Configurator() {};

    std::optional<ERRORCODE> Init(const std::filesystem::path& source_path);    
    std::expected<std::variant<float, std::string>, ERRORCODE> GetValue(const std::string& key);
};


// #include "Configurator.hpp"

std::optional<ERRORCODE> Configurator::Init(const std::filesystem::path& source_path) {
    if (!std::filesystem::exists(source_path)) {
        return ERRORCODE::FILE_DONT_EXIST;
    } else {
        std::ifstream t_file;
        
        t_file.open(source_path.string().c_str());
        if (t_file.is_open()) {
            std::string t_line        = "";
            std::string t_key         = "";
            bool        equal_trigger = false; 
            
            while (std::getline(t_file, t_line)) {
                std::string s_value = "";
                bool is_number      = false;
                
                if (t_line.empty()) {
                    // there's nothing here
                } else if ((t_line[0] == '#') || 
                (t_line[0] == '/' && t_line[1] == '/') ||
                (t_line[0] == ' ')) {
                    // this is a commentary or inappropriate format
                } else {
                    size_t index_equal;
                    for (size_t i = 0; i < t_line.length(); i++) {
                        if (t_line[i] == '=') {
                            equal_trigger = true;
                            index_equal = i;
                        }
                        
                        if (!equal_trigger && t_line[i] != '=') {
                            t_key += t_line[i];
                        } 
                        
                        if (equal_trigger && i != index_equal) {
                            s_value += t_line[i];
                            
                        }
                    }
                    
                    if (READ_PREFERENCE == 0) {
                        // for generic (have restrictions)
                        for (int i = 0; i < s_value.length(); i++) {
                            if (s_value.find_first_of("1234567890") != s_value.npos) {
                                // se nao encontrar retorna npos (-1 ou o maior valor possivel para uint)
                                // queremos que NAO encontre para que seja numero.
                                is_number = true;
                            }
                        }
                        
                        // std::cout << "KEY: " << t_key << " - VALUE: " << s_value << " - NUMBER: " << is_number << std::endl;
                        // system("pause"); -> debugging
                        
                        if (is_number) {
                            n_data.insert({t_key, std::stof(s_value) });
                        } else {
                            c_data.insert({t_key, s_value });
                        }
                        
                        
                    } else if (READ_PREFERENCE == 1) {
                        // for text
                        c_data.insert({t_key, s_value });
                    } else if (READ_PREFERENCE == -1) {
                        // for number
                        n_data.insert({t_key, std::stof(s_value) });
                    } else {
                        return ERRORCODE::INVALID_READ;
                    }
                    
                    equal_trigger       = false;
                    t_key               = "";
                    t_line              = "";
                    is_number           = false;
                }
            }
        } else {
            return ERRORCODE::COULDNT_OPEN_FILE;
        }
        t_file.close();
        return ERRORCODE::SUCCESS;
    }
}

std::expected<std::variant<float, std::string>, ERRORCODE> Configurator::GetValue(const std::string& key) {
    ERRORCODE t_err;
    
    {
        auto iterador = n_data.find(key);
        std::map<std::string, float>::iterator null_it_n{};
        
        if (iterador != null_it_n) {
            return iterador->second;    
        } else {
            t_err = ERRORCODE::NOT_FOUND_KEY;
        }
    }
    
    {
        auto iterador = c_data.find(key);
        std::map<std::string, std::string>::iterator null_it_n{};
        
        if (iterador != null_it_n) {
            return iterador->second;    
        } else {
            t_err = ERRORCODE::NOT_FOUND_KEY;
        }
    }
    return std::unexpected(t_err);
    //return ERRORCODE::SUCCESS;
}


#endif // CONFIGURATOR_H
//g++ -std=c++26 -freflection main.cpp -o main.exe

#endif