#pragma once

#include <string>

namespace liteweb_cpp {

	/// <summary>
	/// 
	/// This database is designed for PostgreSQL 
	/// 
	/// </summary>
	class Database {
	public:
		explicit Database(std::string&& password, uint32_t threads = 1, uint32_t port = 5432);

		Database(const Database&) = delete;
		Database(Database&&) noexcept = delete;

		Database& operator=(const Database&) = delete;
		Database& operator=(Database&&) noexcept = delete;

	private:

	private:
		uint32_t port;
		uint32_t threads;

	};

} // namespace liteweb_cpp