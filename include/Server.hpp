#pragma once
#include <Logger.hpp>

#include <httplib.h>
#include <nlohmann/json.hpp>

typedef httplib::Server::Handler Handler;
typedef nlohmann::json json;

class Server {
public:
	Server();
	Server(const Logger&& logger);

	void get(const std::string& route, Handler handle);
	void post(const std::string& route, Handler handle);

	void loadPage(const std::string& route, const std::string& file_path);

	void listen(const std::string& host, int port);

private:
	static std::string _load_html_content(const std::string& path);

private:
	httplib::Server _srv;
	Logger _logger;

};