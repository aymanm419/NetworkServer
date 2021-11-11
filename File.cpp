
#include <iostream>
#include <vector>
#include "File.h"

void writeFile(std::string fileName, std::string fileContent)
{
    fileName.insert(0, ".");
    int idx = fileName.find_last_of('/');
    std::string directories = fileName.substr(0, idx);
    std::filesystem::create_directories(directories);
    std::cout << ">>> Writing to file " << fileName << std::endl;
    std::ofstream output( fileName, std::ios::binary );
    output << fileContent;
    output.close();
}
bool fileExists(std::string fileName)
{
    fileName.insert(0, ".");
    std::cout << ">>> Checking for existence of " << fileName << std::endl;
    std::ifstream file(fileName.c_str());
    return file.good();
}
std::string getFileContent(std::string fileName)
{
    fileName.insert(0, ".");
    std::cout << ">>> Fetching content of " << fileName << std::endl;
    std::ifstream input( fileName, std::ios::binary );
    std::vector<unsigned char> buffer(std::istreambuf_iterator<char>(input), {});
    std::cout << buffer.size() << std::endl;
    std::string str;
    for(auto it : buffer)
        str += it;
    input.close();
    return str;
}