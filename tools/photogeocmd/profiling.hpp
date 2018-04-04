#ifndef PROFILING_HPP
#define PROFILING_HPP

#include <set>
#include <thread>
#include <mutex>

/// Run profiling.
class profiling {
    public:
        /// Profiling result.
        struct result {
            /// Execution time (milliseconds).
            double time;

            /// Initial memory usage (megabytes).
            double memory_init;

            /// Max memory usage (megabytes).
            double memory_max;
        };

        /// Start profiling.
        /**
         * @param result Address to store profiling result.
         * @param time Whether to profile time.
         * @param memory Whether to profile memory.
         */
        profiling(result* result, bool time, bool memory);

        /// End profiling.
        ~profiling();

        /// Start thread to measure memory.
        static void start_up();

        /// Shutdown thread.
        static void shutdown();

    private:
        static void thread_function();

        result* result_ptr;
        bool measure_time;
        bool measure_memory;
        static bool thread_running;
        static std::thread thread;
        static std::mutex mutex;
        static std::set<profiling*> profilers;
};

#define PROFILE(result, time, memory) profiling __profile_instance(result, time, memory)

#endif
