#include "Database.hpp"

#include <httplib.h>

#include <stdexcept>
#include <utility>

namespace liteweb_cpp {

	Database::Database(
		std::string password,
		uint32_t workerThreads,
		std::string host
	)
		:
		_host(std::move(host)),
		_password(std::move(password)) {

		if (workerThreads == 0) {
			throw std::invalid_argument("workerThreads cannot be 0.");
		}

		_workers.reserve(workerThreads);

		for (uint32_t i = 0; i < workerThreads; ++i) {
			_workers.emplace_back(&Database::workerThread, this);
		}
	}

	Database::~Database() {
		_running = false;

		_condition.notify_all();

		for (std::thread& worker : _workers) {
			if (worker.joinable()) {
				worker.join();
			}
		}
	}

	void Database::enqueue(const std::string& message, const nlohmann::json& json) {
		{
			std::lock_guard<std::mutex> lock(_mutex);

			_queue.push({
				message,
				json
				});
		}

		_condition.notify_one();
	}

	nlohmann::json Database::request(const std::string& message, const nlohmann::json& json) {
		return sendRequest(message, json);
	}

	void Database::workerThread() {
		while (_running) {
			std::optional<DatabaseTask> task;

			{
				std::unique_lock<std::mutex> lock(_mutex);

				_condition.wait(lock, [this]() {
					return !_queue.empty() || !_running;
					});

				if (!_running) {
					return;
				}

				task = std::move(_queue.front());
				_queue.pop();
			}

			try {
				sendRequest(task->message, task->payload);
			}
			catch (...) {
				// TODO:
				// Add proper logging here.
			}
		}
	}

	nlohmann::json Database::sendRequest(const std::string& message, const nlohmann::json& json) {
		httplib::Client client(_host);

		nlohmann::json requestBody = {
			{ "password", _password },
			{ "message", message },
			{ "payload", json }
		};

		auto response = client.Post(
			"/request",
			requestBody.dump(),
			"application/json"
		);

		if (!response) {
			throw std::runtime_error(
				"Failed to connect to CPPPGDatabaseService."
			);
		}

		if (response->status != 200) {
			throw std::runtime_error(
				"Database service returned HTTP status: " +
				std::to_string(response->status)
			);
		}

		return nlohmann::json::parse(response->body);
	}

} // namespace liteweb_cpp