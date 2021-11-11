#ifndef NETWORKSERVER_HTTP_H
#define NETWORKSERVER_HTTP_H

#include <utility>
#include <vector>
#include <string>
#include <unordered_map>
#include <iostream>
#include "utils.h"
#include <algorithm>

const std::string HTTP_VERSION = "HTTP/1.1";
const std::unordered_map<int, std::string> STATUS_CODE_TO_MESSAGE{
        {200, "OK"},
        {404, "Not Found"},
};
const std::vector<std::string> REQUEST_TYPES{"GET", "POST"};

class HTTP {
public:
    HTTP() {}

    HTTP(const char *header, int size_read);

    HTTP &setHeaders(const std::vector<std::string> &headers);

    HTTP &addHeader(std::string headerType, std::string headerValue);

    HTTP &setBody(std::string body);

    HTTP &setStatus(int status);

    HTTP &build();

    std::string getBody() { return body; }

    std::string getHeader(std::string headerType) {
        if (this->headers.count(headerType))
            return headers[headerType];
        return "";
    }

    std::string getString() { return httpStr; }

    std::string getRequestType() { return requestType; }

    std::string getRequestFile() { return requestFile; }

private:
    void readRequestLine(const std::string &firstLine);

    std::unordered_map<std::string, std::string> headers;
    std::string body = "";
    int status = 200;
    std::string requestType;
    std::string requestFile;
    std::string httpStr = "";
};


#endif //NETWORKSERVER_HTTP_H
