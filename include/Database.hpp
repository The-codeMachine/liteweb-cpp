#pragma once
#include <Logger.hpp>

#include <nlohmann/json.hpp>

#include <atomic>
#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <optional>
#include <queue>
#include <string>
#include <thread>
#include <vector>

namespace liteweb_cpp {

	struct DatabaseTask {
		std::string message;
		nlohmann::json payload;
	};

	/// <summary>
	///
	/// PostgreSQL database proxy client.
	///
	/// This class communicates with the external
	/// CPPPGDatabaseService over HTTP.
	///
	/// Thread-safe.
	/// 
	/// Non-moveable, and non-copyable
	///
	/// </summary>
	class Database {
	public:
		explicit Database(std::string password, uint32_t workerThreads = 1, std::string host = "http://localhost:6789");

		~Database();

		Database(const Database&) = delete;
		Database(Database&&) noexcept = delete;

		Database& operator=(const Database&) = delete;
		Database& operator=(Database&&) noexcept = delete;
		
		/// <summary>
		///
		/// Fire-and-forget asynchronous request.
		/// 
		/// </summary>
		/// <param name="message"></param>
		/// <param name="json"></param>
		void enqueue(const std::string& message, const nlohmann::json& json);
		
		/// <summary>
		/// 
		/// Synchronous request/response.
		/// 
		/// </summary>
		/// <param name="message"></param>
		/// <param name="json"></param>
		/// <returns></returns>
		nlohmann::json request(const std::string& message, const nlohmann::json& json);

	private:
		void workerThread();

		nlohmann::json sendRequest(const std::string& message, const nlohmann::json& json);

	private:
		std::string _host;
		std::string _password;

		std::vector<std::thread> _workers;

		std::queue<DatabaseTask> _queue;

		std::mutex _mutex;
		std::condition_variable _condition;

		std::atomic<bool> _running = true;

		std::shared_ptr<Logger> _logger;
	};

} // namespace liteweb_cpp