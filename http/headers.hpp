#include <string>
#include <map>

std::map<unsigned int, std::string> status_codes{
    { 200, "OK" }
};

std::map<std::string, std::string> content_types{
    { "html", "text/html" },
    { "json", "application/json" },
};

const std::string status(const std::string& s);

void headers_to_string(std::string& status, const std::map<std::string, std::string>& headers) {
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

const std::string content_type(const std::string& content) {
    std::string c = content_types[content];
    return c + "; charset=iso-8859-1";
}

const std::string headers(unsigned int status_code, unsigned int length, std::string content) {
    std::string message = status_codes[status_code];
    std::string h = status(std::to_string(status_code) + " " + message);

    std::map<std::string, std::string> headers{
        { "Server", server() },
        { "Connection", connection() },
        { "Keep-Alive", keep_alive() },
        { "Content-Type", content_type(content) },
        { "Content-Length", std::to_string(length) }
    };

    headers_to_string(h, headers);

    return h;
}