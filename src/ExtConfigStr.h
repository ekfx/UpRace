#ifndef EXTCONFIGSTR_H
#define EXTCONFIGSTR_H

#include <fstream>
#include <iostream>
#include <string>
#include <map>
#include <filesystem>
#include <utility>

class StrIniReader {
private:
	// usar const char* não funciona, há comparação de 'maior que' debaixo do programa
	// string é o mais correto e seguro, e não é taaao pesado. Usar na inicialização
	std::map<std::string, std::string>	data;
	std::ifstream						eka;
	std::string							INFO;

public:
	StrIniReader();
	~StrIniReader();

  int Init(std::filesystem::path path);
	std::string GetValue(const std::string& key);
	std::pair<std::string, std::string> GetPair(const std::string& key);

	int Exists(const std::string& key);
	void Delete();
	void PrintInfo();
};

#endif

