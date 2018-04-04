#ifndef PROFILING_HPP
#define PROFILING_HPP

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
         */
        profiling(result* result);

        /// End profiling.
        ~profiling();

    private:
        result* result_ptr;
        double start_time;
};

#define PROFILE(result) profiling __profile_instance(result)

#endif
