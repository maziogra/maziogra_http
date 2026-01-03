#include <sstream>
#include <server/HttpRequest.h>

namespace maziogra_http {
    void HttpRequest::parseRequest(std::string rawRequest) {
        std::stringstream ss(rawRequest);
        std::string l;
        if (std::getline(ss, l) && !l.empty()) {
            std::stringstream line(l);
            line >> method >> path >> version;
        } else return;

        while (std::getline(ss, l) && l != "\r" && !l.empty()) {
            if (l.back() == '\r') l.pop_back();

            int pos = l.find(':');
            if (pos == std::string::npos) continue;

            std::string key = l.substr(0, pos);
            std::string value = l.substr(pos + 1);

            while (!value.empty() && (value[0] == ' ' || value[0] == '\t'))
                value.erase(0, 1);

            while (!value.empty() && (value.back() == ' ' || value.back() == '\t'))
                value.pop_back();

            headers[key] = value;
        }

        std::string bodyBuffer;
        std::getline(ss, bodyBuffer, '\0');

        if (bodyBuffer.size() >= 2 && bodyBuffer[0] == '\r' && bodyBuffer[1] == '\n')
            bodyBuffer.erase(0, 2);

        body = std::move(bodyBuffer);
    }

}
