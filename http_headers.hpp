#include <string>
#include <map>

const std::string HTTP_200 = "200 OK";

const std::string status(const std::string& s);

void headers_to_string(std::string& status, std::map<std::string, std::string>& headers) {
    for (const auto& [key, value] : headers) {
        status += key + ": " + value + "\n";
    }

    status += "\n";
}

const std::string status(const std::string& s) {
    return "HTTP/2 " + s + "\n";
}

const std::string server() {
    return "Fartache";
}

const std::string connection() {
    return "Keep-Alive";
}

const std::string keep_alive() {
    return "timeout=5, max=1000";
}

const std::string content_type() {
    return "text/html; charset=iso-8859-1";
}

const std::string headers(unsigned int length) {
    std::string h = "HTTP/2" + HTTP_200 + "\n";

    std::map<std::string, std::string> headers{
        { "Server", server() },
        { "Connection", connection() },
        { "Keep-Alive", keep_alive() },
        { "Content-Type", content_type() },
        { "Content-Length", std::to_string(length) }
    };

    headers_to_string(h, headers);

    return h;
}