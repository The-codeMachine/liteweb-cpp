#include <Server.hpp>

#include <filesystem>
#include <fstream>
#include <mutex>

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
		auto cache = _create_page_cache(file_path);

		_srv.Get(route, [this, cache, file_path](const httplib::Request& req, httplib::Response& res) {
			_reload_page_if_needed(cache, file_path);

			_serve_cached_page(cache, res);
		});
	}

	// makes the server start listening
	void Server::listen(const std::string& host, int port) {
		_srv.listen(host, port);
	}

	// converts the body of the request into json
	json Server::parseJson(const httplib::Request& req) noexcept {
		try {
			return json::parse(req.body);
		}
		catch (const std::exception& e) {
			// do nothing, no except
			return json{ {"error", "Invalid JSON"} };
		}
	}

	// loads the content of an HTML page into a string
	std::string Server::_load_html_content(const std::string& path) {
		std::filesystem::path p(path);

		if (p.extension() != ".html")
			throw std::runtime_error("File must be html");

		std::ifstream file(p);

		if (!file.is_open())
			throw std::runtime_error("Failed to open file: " + path);

		std::stringstream buff;
		buff << file.rdbuf();

		file.close();
		return buff.str();
	}

	// creates a page cache
	std::shared_ptr<PageCache> Server::_create_page_cache(const std::string& file_path) {
		auto cache = std::make_shared<PageCache>();

		try {
			cache->html_content = _load_html_content(file_path);

			if (std::filesystem::exists(file_path)) {
				cache->last_write = std::filesystem::last_write_time(file_path);
			}
		}
		catch (const std::exception& e) {
			_logger.log("Failed to load page '" + file_path + "': " + e.what());
		}

		return cache;
	}

	// if the build is in release mode, it will not hot reload it
	void Server::_reload_page_if_needed(const std::shared_ptr<PageCache>& cache, const std::string& file_path) {
#ifndef NDEBUG

		try {
			if (!std::filesystem::exists(file_path)) {
				return;
			}

			auto current_write = std::filesystem::last_write_time(file_path);

			if (current_write <= cache->last_write) {
				return;
			}

			std::string new_content = _load_html_content(file_path);

			{
				std::lock_guard<std::mutex> lock(cache->mutex);

				cache->html_content = std::move(new_content);
				cache->last_write = current_write;
			}

			_logger.log("Hot reloaded page: " + file_path);
		}
		catch (const std::exception& e) {
			_logger.log("Hot reload failed for '" + file_path + "': " + e.what());
		}
#endif
	}

	// serves the cached page
	void Server::_serve_cached_page(const std::shared_ptr<PageCache>& cache, httplib::Response& res) {
		std::string content;

		{
			std::lock_guard<std::mutex> lock(cache->mutex);

			content = cache->html_content;
		}

		if (content.empty()) {
			res.status = 404;
			res.set_content("File not found or empty", "text/html");

			return;
		}

		res.status = 200;
		res.set_content(content, "text/html");
	}

} // namespace liteweb_cpp