#include "profiling.hpp"

#include <chrono>

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <psapi.h>
#endif

bool profiling::thread_running = false;
std::thread profiling::thread;
std::mutex profiling::mutex;
std::set<profiling*> profiling::profilers;

profiling::profiling(profiling::result* result, bool time, bool memory) {
    result_ptr = result;
    measure_time = time;
    measure_memory = memory;

    result_ptr->time = 0.0;
    result_ptr->memory_init = 0.0;
    result_ptr->memory_max = 0.0;

    if (measure_time) {
        result_ptr->time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
    } else if (measure_memory) {
        #ifdef _WIN32
        PROCESS_MEMORY_COUNTERS_EX memory_counters;
        GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&memory_counters), sizeof(memory_counters));
        result_ptr->memory_init = memory_counters.PrivateUsage / 1024.0 / 1024.0;
        #endif
        std::unique_lock<std::mutex> lock(mutex, std::defer_lock);
        lock.lock();
        profilers.insert(this);
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
        for (profiling* profiler : profilers) {
            #ifdef _WIN32
            PROCESS_MEMORY_COUNTERS_EX memory_counters;
            GetProcessMemoryInfo(GetCurrentProcess(), reinterpret_cast<PROCESS_MEMORY_COUNTERS*>(&memory_counters), sizeof(memory_counters));
            profiler->result_ptr->memory_max = max(profiler->result_ptr->memory_max, memory_counters.PrivateUsage / 1024.0 / 1024.0);
            #endif
        }
        lock.unlock();
    }
}
