#pragma once

#include <condition_variable>
#include <mutex>
#include <optional>
#include <queue>

namespace liteweb_cpp {

    template <typename T>
    class ThreadSafeQueue {
    public:
        ThreadSafeQueue() = default;

        ~ThreadSafeQueue() {
            notifyShutdown();
        }

        ThreadSafeQueue(const ThreadSafeQueue&) = delete;
        ThreadSafeQueue& operator=(const ThreadSafeQueue&) = delete;

    public:
        // Push by move (efficient, correct ownership transfer)
        void push(T&& item) {
            {
                std::lock_guard<std::mutex> lock(_mutex);
                if (_shutdown) 
                    return; // silently ignore or could throw

                _queue.push(std::move(item));
            }

            _cv.notify_one();
        }

        void push(const T& item) {
            {
                std::lock_guard<std::mutex> lock(_mutex);
                if (_shutdown) 
                    return;

                _queue.push(item);
            }

            _cv.notify_one();
        }

        // Blocks until item available or shutdown
        std::optional<T> popWait() {
            std::unique_lock<std::mutex> lock(_mutex);

            _cv.wait(lock, [&] {
                return _shutdown || !_queue.empty();
                });

            if (_shutdown && _queue.empty()) {
                return std::nullopt;
            }

            T value = std::move(_queue.front());
            _queue.pop();
            return value;
        }

        // Non-blocking pop (useful for polling loops)
        std::optional<T> pop() {
            std::lock_guard<std::mutex> lock(_mutex);

            if (_queue.empty()) {
                return std::nullopt;
            }

            T value = std::move(_queue.front());
            _queue.pop();
            return value;
        }

        void notifyShutdown() {
            {
                std::lock_guard<std::mutex> lock(_mutex);
                _shutdown = true;
            }

            _cv.notify_all();
        }

        bool empty() const {
            std::lock_guard<std::mutex> lock(_mutex);
            return _queue.empty();
        }

        std::size_t size() const {
            std::lock_guard<std::mutex> lock(_mutex);
            return _queue.size();
        }

    private:
        mutable std::mutex _mutex;
        std::condition_variable _cv;
        std::queue<T> _queue;

        bool _shutdown = false;
    };

} // namespace liteweb_cpp