# C++ Multi-Page Webserver

This project is a **multi-page C++ webserver** built using raw sockets. It demonstrates how to create a simple, educational web application using C++ without external frameworks. The server supports multiple pages, form submissions, JSON APIs, and can be extended for static file serving and dynamic content.

## Features

* Multi-page support: Home, About, Contact, Services, Blog, Portfolio
* Contact form with POST handling and dynamic response
* JSON API endpoints (`/api/time`, `/api/hello?name=...`) for AJAX/JSON requests
* Basic routing system for page navigation
* Easy to extend with new pages or static content
* Cross-platform support (Linux/macOS/WSL recommended, Windows via WSL)

## Requirements

* **C++17 compatible compiler** (g++ recommended)
* **Linux/macOS/WSL** for full socket compatibility
* Optional: Windows users can use **MinGW** or **WSL**

## Installation

### Step 1: Clone or Download

```bash
git clone <repository_url>
cd <repository_folder>
```

### Step 2: Compile

```bash
# Linux/macOS/WSL
g++ -std=c++17 simple_cpp_webserver.cpp -o simple_server

# Windows (MinGW)
g++ -std=c++17 simple_cpp_webserver.cpp -o simple_server.exe
```

### Step 3: Run the Server

```bash
# Linux/macOS/WSL
./simple_server

# Windows
simple_server.exe
```

* The server defaults to **port 8080**.
* Specify a custom port:

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
http://localhost:8080/api/hello?name=Abhinav
```

## Project Structure

* `simple_cpp_webserver.cpp` - main server code with routing, form handling, and API
* `README.md` - project documentation
* `public/` (optional) - folder to serve static files like CSS, JS, images

## Extending the Server

* **Add Pages:** Define new HTML strings and route paths in the main loop.
* **Static File Support:** Serve CSS/JS/images from `public/` folder.
* **Multi-threading:** Implement threads to handle multiple simultaneous clients.
* **Dynamic Templates:** Integrate simple template system for reusable layouts.
* **API Expansion:** Add more JSON endpoints for AJAX requests.

## Troubleshooting

* `g++ not recognized` on Windows: Install **MinGW** and add it to PATH or use **WSL**.
* Port in use: Change server port with `./simple_server <port>`.
* Browser shows 404: Check that the path exists and matches your routing logic.

## Future Improvements

* Full static file serving with caching
* User authentication and sessions
* Logging requests and errors
* HTTPS support using OpenSSL
* Database integration for dynamic content

## License

Open-source and free for educational purposes. Contributions are welcome.
