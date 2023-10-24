#include <iostream>
#include "Dll1.h"
#include <windows.h>
#include <string>
#include <fstream>
#include <vector>


class Dll1 {
public:
	typedef std::string(*encrypt_ptr_t)(std::string, int);
	typedef std::string(*decrypt_ptr_t)(std::string, int);
	HINSTANCE handle;
	decrypt_ptr_t decrypt_ptr;
	encrypt_ptr_t encrypt_ptr;
	Dll1() : handle(nullptr), decrypt_ptr(nullptr), encrypt_ptr(nullptr) {
		handle = LoadLibrary(TEXT("Dll1.dll"));
		if (handle != nullptr || handle != INVALID_HANDLE_VALUE) {
			decrypt_ptr = (decrypt_ptr_t)GetProcAddress(handle, "decrypt");
			encrypt_ptr = (encrypt_ptr_t)GetProcAddress(handle, "encrypt");
		}
	}
	~Dll1() {
		FreeLibrary(handle);
	}

	std::string decrypt(std::string text, int key) {
		if (decrypt_ptr == nullptr)
		{
			return "Function not found";
		}
		return decrypt_ptr(text, key);
	}

	std::string encrypt(std::string text, int key) {

			if (encrypt_ptr == nullptr)
			{
				return "Function not found";
			}
			return encrypt_ptr(text, key);
	}
};

class FileRW {
public:
	Dll1 cipher;
	std::vector<std::string> bufferD;
	std::vector<std::string> bufferE;
	void Read(std::string command, std::string name, int key) {
		std::string newLine;
		std::ifstream file(name);
		if (!file.is_open()) {
			std::cout << "File not found" << std::endl;
		}
		if (command == "1") {
			while (getline(file, newLine)) {
				newLine = cipher.decrypt(newLine, key);
				bufferD.push_back(newLine);
			}
		}
		if (command == "2") {
			while (getline(file, newLine)) {
				newLine = cipher.encrypt(newLine, key);
				bufferE.push_back(newLine);
			}
		}
		file.close();

	}

	void Write(std::string command, std::string name) {
		std::ofstream file(name);
		if (!file.is_open()) {
			std::cout << "File not found" << std::endl;
		}
		if (command == "1") {
			for (size_t i = 0; i < bufferD.size(); i++)
			{
				file << bufferD[i] << std::endl;
			}
		}
		if (command == "2") {
			for (size_t i = 0; i < bufferE.size(); i++)
			{
				file << bufferE[i] << std::endl;
			}
		}
		file.close();
	}


};


int main() {

	FileRW file_cipher;
	file_cipher.Read("1", "C:/Labs_Kse/Paradigms of Programming/task4/text.txt", 5);
	file_cipher.Write("1", "C:/Labs_Kse/Paradigms of Programming/task4/text.txt");
	return 0;
}

