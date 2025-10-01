# Simple C++ Webserver

This is a multi-page C++ webserver built using raw sockets. It serves HTML pages, handles form submissions, and includes a basic JSON API endpoint. This project is intended for learning and educational purposes.

## Features

* Home, About, Contact, Services, Blog, and Portfolio pages
* Contact form with POST handling
* JSON API endpoint (`/api/time`) returning the server time
* Basic routing and multi-page navigation
* Easily extendable for more pages or static file support

## Requirements

* C++17 compiler (g++ recommended)
* Linux/macOS/WSL recommended due to socket API compatibility
* Windows users may need WSL or MinGW

## Installation

1. Clone the repository or download the source code.
2. Open terminal in project directory.
3. Compile the code:

```bash
# Linux/macOS/WSL
g++ -std=c++17 simple_cpp_webserver.cpp -o simple_server

# Windows (MinGW)
g++ -std=c++17 simple_cpp_webserver.cpp -o simple_server.exe
```

## Running the Server

```bash
# Linux/macOS/WSL
./simple_server

# Windows
simple_server.exe
```

* The server runs on **port 8080** by default.
* You can specify a different port:

```bash
./simple_server 3000
```

## Accessing the Website

Open your browser and navigate to:

```
http://localhost:8080/
http://localhost:8080/about
http://localhost:8080/contact
http://localhost:8080/services
http://localhost:8080/blog
http://localhost:8080/portfolio
http://localhost:8080/api/time
```

## Future Improvements

* Add static file serving for CSS, JS, and images
* Implement multi-threading to handle multiple clients
* Add template rendering for dynamic HTML
* Improve error handling and request parsing

## License

This project is open-source and free to use for educational purposes.
