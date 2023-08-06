//
// Created by konstantin on 06.08.23.
//


#ifndef PINEPP_TIMER_HPP
#define PINEPP_TIMER_HPP
#include <sys/time.h>
#include <cstdint>

namespace pinepp {

    class timer {
    public:

        /**
         * @brief starts or resumes the timer
         */
        void start();

        /**
         * @brief pauses the timer; call start() again to resume
         */
        void pause();

        /**
         * @brief resets the timer to zero
         */
        void reset();

        /**
         * @returns number of nanoseconds that passed while timer was running
         */
        [[nodiscard]] long get_ns() const;

        /**
         * @returns number of seconds that passed while timer was running
         */
        [[nodiscard]] time_t get_s() const;

        timer() = default;
        ~timer() = default;
        timer(const timer&) = delete;
        timer& operator=(const timer&) = delete;
        timer(timer&&) = delete;
        timer& operator=(timer&&) = delete;
    private:
        struct timespec m_StartTime;
        struct timespec m_StopTime;
    };

} // pinepp

#endif //PINEPP_TIMER_HPP
