#include "ExtConfigStr.h"

StrIniReader::StrIniReader()
{
}

StrIniReader::~StrIniReader()
{
}

int StrIniReader::Init(std::filesystem::path path)
{
	std::string line = "";
	std::string key = "";
	std::string value = "";
	bool trigger_equal = false;

	eka.open(path);
	if (eka.is_open()) {
		while (std::getline(eka, line)) {
			for (unsigned int i = 0; i < line.length(); i++) {
				if (line[0] == '#' || line[0] == ' ') {

				}
				else {
					if (line[i] == ' ') {
						line.erase(line.begin() + i);
					}

					if (line[i] == '=') {
						trigger_equal = true;
					}

					if (!trigger_equal) {
						key += line[i];
					}
					else {
						if (line[i] == '=') {
						}
						else {
							value += line[i];
						}
					}
				}

			}

			if (line[0] != '#' || line[0] != ' ') {
				data.insert({ key, value });
				// std::cout << key << std::endl;
				auto it = data.find(key);
			}

			key = "";
			value = "";
			trigger_equal = false;
		}
	}
	else {
		INFO += "STRINIREADER::START::COULDNT_OPEN_ARCHIVE\n";
		INFO += "STRINIREADER::START::CURRENT_PATH::" + std::filesystem::current_path().string() + "\n";
		return -1;
	}
	eka.close();
	return 0;
}

std::string StrIniReader::GetValue(const std::string& key)
{
	auto it = data.find(key);

	if (it != data.end()) {
		return it->second;
	}
	else {
		std::cerr << "STRINIREADER::GETVALUE::COULDNT_NOT_FOUND_KEY\n";
		INFO += "STRINIREADER::GETVALUE::COULDNT_NOT_FOUND_KEY\n";
		return "STRINIREADER::GETVALUE::COULDNT_NOT_FOUND_KEY";
	}
}

std::pair<std::string, std::string> StrIniReader::GetPair(const std::string& key) {
	auto it = data.find(key);

	if (it != data.end()) {
		return {it->first, it->second };
	}
	else {
		INFO += "STRINIREADER::GETPAIR::COULDNT_NOT_FOUND_KEY\n";
		return { "0", "0" };	
	}
}

int StrIniReader::Exists(const std::string& key) {
	auto it = data.find(key);

	if (it != data.end()) {
		return 1;
	}
	else {
		INFO += "STRINIREADER::GETPAIR::COULDNT_NOT_FOUND_KEY\n";
		return 0;	
	}
}

void StrIniReader::Delete()
{
}

void StrIniReader::PrintInfo() {
	std::cerr << INFO;
}
