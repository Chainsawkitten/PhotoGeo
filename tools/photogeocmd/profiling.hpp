#ifndef PROFILING_HPP
#define PROFILING_HPP

#include <thread>
#include <map>
#include <mutex>

/// Run profiling.
class profiling {
    public:
        /// Profiling result.
        struct result {
            /// Execution time (milliseconds).
            double time;

            /// Memory usage (megabytes).
            /// @todo Profile memory.
            double memory;
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
        static std::map<profiling*, profiling*> profilers;
};

#define PROFILE(result, time, memory) profiling __profile_instance(result, time, memory)

#endif
