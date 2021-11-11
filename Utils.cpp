#include "Utils.h"
std::vector<std::string> splitByDelimiter(std::string& str, std::string delimiter)
{
    std::vector<std::string> ret;
    std::string temp;
    for(int i = 0;i+delimiter.size()-1 < str.size();)
    {
        if(str.substr(i, delimiter.size()) == delimiter)
        {
            if(temp.empty())
                return ret;
            ret.push_back(temp);
            temp.clear();
            i += delimiter.size();
        } else {
            temp += str[i];
            i++;
        }
    }
    return ret;
}