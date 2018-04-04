#include "profiling.hpp"

#include <chrono>

bool profiling::thread_running = false;
std::thread profiling::thread;
std::mutex profiling::mutex;
std::map<profiling*, profiling*> profiling::profilers;

profiling::profiling(profiling::result* result, bool time, bool memory) {
    result_ptr = result;
    measure_time = time;
    measure_memory = memory;

    result_ptr->time = 0.0;
    result_ptr->memory = 0.0;

    if (measure_time) {
        result_ptr->time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    } else if (measure_memory) {
        std::unique_lock<std::mutex> lock(mutex, std::defer_lock);
        lock.lock();
        profilers[this] = this;
        lock.unlock();
    }
}

profiling::~profiling() {
    if (measure_time) {
        result_ptr->time = (std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - result_ptr->time) / 1000000.0;
    } else if (measure_memory) {
        std::unique_lock<std::mutex> lock(mutex, std::defer_lock);
        lock.lock();
        profilers.erase(this);
        lock.unlock();
    }
}

void profiling::start_up() {
    if (thread_running)
        return;

    thread_running = true;

    profiling::thread = std::thread(&profiling::thread_function);
}

void profiling::shutdown() {
    if (!thread_running)
        return;

    thread_running = false;

    thread.join();
}

void profiling::thread_function() {
    while (thread_running) {
        std::unique_lock<std::mutex> lock(mutex, std::defer_lock);
        lock.lock();
        for (std::pair<profiling*, profiling*> profiler_pair : profilers) {
            profiler_pair.first->result_ptr->memory = 1.0;
        }
        lock.unlock();
    }
}
