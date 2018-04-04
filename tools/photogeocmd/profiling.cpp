#include "profiling.hpp"

#include <chrono>

profiling::profiling(profiling::result* result) {
    result_ptr = result;
    start_time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

profiling::~profiling() {
    result_ptr->time = (std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch()).count() - start_time) / 1000000.0;
}
