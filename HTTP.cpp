#include "HTTP.h"

HTTP::HTTP(const char *header, int size_read) {

    std::string str;
    for (int i = 0; i < size_read; i++)
        str += header[i];
    std::vector<std::string> headerLines = splitByDelimiter(str, "\r\n");
    if (headerLines.empty() || str.empty())
        return;
    readRequestLine(headerLines[0]);
    std::vector<std::string> headers;
    for (int i = 1; i < headerLines.size(); i++) {
        if (headerLines[i].empty())
            break;
        headers.push_back(headerLines[i]);
    }
    setHeaders(headers);

}

HTTP &HTTP::addHeader(std::string headerType, std::string headerValue) {
    headers[headerType] = headerValue;
    return *this;
}

HTTP &HTTP::setBody(std::string body) {
    this->body = std::move(body);
    return *this;
}

HTTP &HTTP::setStatus(int status) {
    this->status = status;
    return *this;
}

HTTP &HTTP::build() {

    if (this->requestType.empty()) {
        this->httpStr = HTTP_VERSION + " " + std::to_string(status) + " " + STATUS_CODE_TO_MESSAGE.at(status) + "\r\n";
    } else {
        this->httpStr = this->requestType + " " + this->requestFile + " " + HTTP_VERSION + "\r\n";
    }
    for (const auto &it : headers)
        this->httpStr += it.first + ": " + it.second + "\r\n";
    this->httpStr += "\r\n";
    this->httpStr += body;
    return *this;
}

HTTP &HTTP::setHeaders(const std::vector<std::string> &headerLines) {
    for (const auto &it : headerLines) {
        std::size_t index = it.find(':');
        if (index == std::string::npos)
            continue;
        this->addHeader(it.substr(0, index), it.substr(index + 2));
    }
    return *this;
}

void HTTP::readRequestLine(const std::string &firstLine) {
    for (const auto &it : REQUEST_TYPES) {
        if (firstLine.find(it) != std::string::npos) {
            this->requestType = it;
            break;
        }
    }
    if (this->requestType.empty())
        return;
    std::size_t fileStart = firstLine.find('/');
    if (fileStart == std::string::npos)
        return;
    std::size_t fileEnd = firstLine.find(' ', fileStart);
    this->requestFile = firstLine.substr(fileStart, fileEnd - fileStart);
}

