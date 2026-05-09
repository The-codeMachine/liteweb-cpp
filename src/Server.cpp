#include <Server.hpp>

#include <filesystem>
#include <fstream>

namespace liteweb_cpp {

	// Automatically created logger
	Server::Server() : _srv(httplib::Server()), _logger(Logger(Logger::dailyFileLogger("logs/", "server"))) {}

	// Created with specified logger
	Server::Server(const Logger&& logger) : _srv(httplib::Server()), _logger(logger) {}

	// starts a post route with custom logic
	void Server::post(const std::string& route, Handler handle) {
		_srv.Post(route, handle);
	}

	// starts a get route with custom logic
	void Server::get(const std::string& route, Handler handle) {
		_srv.Get(route, handle);
	}

	// provides the functionality to make the server serve a page (with no authentication check)
	void Server::loadPage(const std::string& route, const std::string& file_path) {
		std::string html_content;

		try {
			html_content = _load_html_content(file_path);

		}
		catch (const std::exception& e) {
			_logger.log("There was an exception in the loadPage function, Server.cpp" + std::string(e.what()));
		}

		_srv.Get(route, [html_content](const httplib::Request& req, httplib::Response& res) {
			if (html_content.empty()) {
				res.status = 404;
				res.set_content("File was empty", "text/html");
			}

			res.status = 200;
			res.set_content(html_content, "text/html");

			});
	}

	// makes the server start listening
	void Server::listen(const std::string& host, int port) {
		_srv.listen(host, port);
	}

	// loads the content of an HTML page into a string
	std::string Server::_load_html_content(const std::string& path) {
		std::filesystem::path p(path);

		if (p.extension() != "html")
			throw std::runtime_error("File must be html");

		std::ifstream file(p);

		if (!file.is_open())
			throw std::runtime_error("Failed to open file: " + path);

		std::stringstream buff;
		buff << file.rdbuf();

		file.close();
		return buff.str();
	}

} // namespace liteweb_cpp