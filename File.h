#ifndef NETWORKSERVER_FILE_H
#define NETWORKSERVER_FILE_H

#include <string>
#include <fstream>
#include <filesystem>

void writeFile(std::string fileName, std::string fileContent);

bool fileExists(std::string fileName);

std::string getFileContent(std::string fileName);

#endif //NETWORKSERVER_FILE_H
