
#include <algorithm>
#include <arpa/inet.h>
#include <cerrno>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

using namespace std;

static const string INDEX_HTML = R"(<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width,initial-scale=1">
  <title>Home - C++ Website</title>
</head>
<body>
  <h1>Welcome to the Home Page</h1>
  <p>This is the homepage served by C++.</p>
  <nav>
    <a href="/">Home</a> |
    <a href="/about">About</a> |
    <a href="/contact">Contact</a>
  </nav>
</body>
</html>)";

static const string ABOUT_HTML = R"(<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <title>About - C++ Website</title>
</head>
<body>
  <h1>About Page</h1>
  <p>This website is powered by a minimal C++ HTTP server.</p>
  <nav>
    <a href="/">Home</a> |
    <a href="/about">About</a> |
    <a href="/contact">Contact</a>
  </nav>
</body>
</html>)";

static const string CONTACT_HTML = R"(<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <title>Contact - C++ Website</title>
</head>
<body>
  <h1>Contact Page</h1>
  <form method="POST" action="/contact">
    <label for="name">Your Name:</label>
    <input type="text" id="name" name="name"><br><br>
    <label for="message">Message:</label>
    <textarea id="message" name="message"></textarea><br><br>
    <button type="submit">Send</button>
  </form>
  <nav>
    <a href="/">Home</a> |
    <a href="/about">About</a> |
    <a href="/contact">Contact</a>
  </nav>
</body>
</html>)";

string http_response(const string &body, const string &content_type = "text/html") {
    ostringstream oss;
    oss << "HTTP/1.1 200 OK\r\n";
    oss << "Content-Type: " << content_type << "; charset=utf-8\r\n";
    oss << "Content-Length: " << body.size() << "\r\n";
    oss << "Connection: close\r\n";
    oss << "\r\n";
    oss << body;
    return oss.str();
}

string not_found_response() {
    string body = "<h1>404 Not Found</h1>";
    ostringstream oss;
    oss << "HTTP/1.1 404 Not Found\r\n";
    oss << "Content-Type: text/html; charset=utf-8\r\n";
    oss << "Content-Length: " << body.size() << "\r\n";
    oss << "Connection: close\r\n\r\n";
    oss << body;
    return oss.str();
}

string url_decode(const string &s) {
    string out;
    out.reserve(s.size());
    for (size_t i = 0; i < s.size(); ++i) {
        if (s[i] == '+') out += ' ';
        else if (s[i] == '%' && i + 2 < s.size()) {
            string hex = s.substr(i + 1, 2);
            char c = static_cast<char>(strtol(hex.c_str(), nullptr, 16));
            out += c;
            i += 2;
        } else out += s[i];
    }
    return out;
}

string extract_field(const string &body, const string &field) {
    size_t pos = 0;
    while (pos < body.size()) {
        size_t amp = body.find('&', pos);
        string pair = body.substr(pos, amp == string::npos ? string::npos : amp - pos);
        size_t eq = pair.find('=');
        if (eq != string::npos) {
            string k = pair.substr(0, eq);
            string v = pair.substr(eq + 1);
            if (k == field) return url_decode(v);
        }
        if (amp == string::npos) break;
        pos = amp + 1;
    }
    return string();
}

int create_and_bind(int port) {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) {
        perror("socket");
        return -1;
    }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        close(server_fd);
        return -1;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(port);

    if (bind(server_fd, (sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        close(server_fd);
        return -1;
    }

    if (listen(server_fd, 10) < 0) {
        perror("listen");
        close(server_fd);
        return -1;
    }

    return server_fd;
}

int main(int argc, char *argv[]) {
    int port = 8080;
    if (argc >= 2) port = stoi(argv[1]);

    cout << "Starting extended C++ web server on port " << port << "\n";
    int server_fd = create_and_bind(port);
    if (server_fd < 0) return 1;

    while (true) {
        sockaddr_in client_addr{};
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (sockaddr *)&client_addr, &client_len);
        if (client_fd < 0) {
            perror("accept");
            continue;
        }

        const int BUF_SIZE = 8192;
        string req;
        req.resize(BUF_SIZE);
        ssize_t received = recv(client_fd, &req[0], BUF_SIZE, 0);
        if (received <= 0) {
            close(client_fd);
            continue;
        }
        req.resize(received);

        istringstream reqstream(req);
        string request_line;
        getline(reqstream, request_line);
        if (!request_line.empty() && request_line.back() == '\r') request_line.pop_back();

        string method, path, version;
        {
            istringstream rl(request_line);
            rl >> method >> path >> version;
        }

        size_t header_end = req.find("\r\n\r\n");
        string body;
        if (header_end != string::npos) body = req.substr(header_end + 4);

        if (method == "GET" && (path == "/" || path == "/index.html")) {
            string resp = http_response(INDEX_HTML);
            send(client_fd, resp.c_str(), resp.size(), 0);
        } else if (method == "GET" && path == "/about") {
            string resp = http_response(ABOUT_HTML);
            send(client_fd, resp.c_str(), resp.size(), 0);
        } else if (method == "GET" && path == "/contact") {
            string resp = http_response(CONTACT_HTML);
            send(client_fd, resp.c_str(), resp.size(), 0);
        } else if (method == "POST" && path == "/contact") {
            string name = extract_field(body, "name");
            string message = extract_field(body, "message");
            string reply = "<!doctype html><html><body>";
            reply += "<h1>Message Received</h1>";
            reply += "<p>Name: <strong>" + (name.empty() ? "(empty)" : name) + "</strong></p>";
            reply += "<p>Message: <em>" + (message.empty() ? "(empty)" : message) + "</em></p>";
            reply += "<p><a href=\"/\">Go back</a></p>";
            reply += "</body></html>";
            string resp = http_response(reply);
            send(client_fd, resp.c_str(), resp.size(), 0);
        } else {
            string resp = not_found_response();
            send(client_fd, resp.c_str(), resp.size(), 0);
        }

        close(client_fd);
    }

    close(server_fd);
    return 0;
}
