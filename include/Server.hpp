#pragma once
#include <Logger.hpp>

#include <httplib.h>
#include <nlohmann/json.hpp>

namespace liteweb_cpp {

	typedef httplib::Server::Handler Handler;
	typedef nlohmann::json json;

	/// <summary>
	/// 
	/// Server is a wrapper class around Httplib's server class. It allows you
	/// to add a GET, POST, and page routes. Servers can listen on any unused port. 
	/// Server's errors, and other information is automatically logged. 
	/// 
	/// Operations: 
	///  - Server can be created from nothing, or using your own custom logger (to specify the file location, and prefix).
	///  - Server can add a GET route, or a POST route.
	///  - Server errors are automatically logger (either using your own, or under logs/time_server.log)
	///  - Server can load an HTML page as a GET route. 
	///  - Server can start listening to on a specified port. 
	/// 
	/// </summary>
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

} // namespace liteweb_cpp