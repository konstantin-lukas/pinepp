//
// Created by konstantin on 06.08.23.
//
#include <ctime>
#include <sys/time.h>
#include <iostream>
#include "timer.hpp"

void pinepp::timer::start () {
    // IF ALREADY RUNNING, RETURN
    if ((m_StopTime.tv_sec == 0 && m_StopTime.tv_nsec == 0) && (m_StartTime.tv_sec != 0 || m_StartTime.tv_nsec != 0))
        return;

    struct timespec now{};
    clock_gettime(CLOCK_REALTIME, &now);
    m_StartTime.tv_sec = now.tv_sec - (m_StopTime.tv_sec - m_StartTime.tv_sec);
    m_StartTime.tv_nsec = now.tv_nsec - (m_StopTime.tv_nsec - m_StartTime.tv_nsec);
    m_StopTime.tv_sec = 0;
    m_StopTime.tv_nsec = 0;

}

long pinepp::timer::get_ns() const {
    if ((m_StopTime.tv_sec == 0 && m_StopTime.tv_nsec == 0) && (m_StartTime.tv_sec == 0 && m_StartTime.tv_nsec == 0))
        return 0;
    else if ((m_StopTime.tv_sec == 0 && m_StopTime.tv_nsec == 0) && (m_StartTime.tv_sec != 0 || m_StartTime.tv_nsec != 0)) {
        struct timespec now{};
        clock_gettime(CLOCK_REALTIME, &now);
        return now.tv_nsec - m_StartTime.tv_nsec;
    }
    return m_StopTime.tv_nsec - m_StartTime.tv_nsec;
}

time_t pinepp::timer::get_s() const {
    if ((m_StopTime.tv_sec == 0 && m_StopTime.tv_sec == 0) && (m_StartTime.tv_sec == 0 && m_StartTime.tv_sec == 0))
        return 0;
    else if ((m_StopTime.tv_sec == 0 && m_StopTime.tv_sec == 0) && (m_StartTime.tv_sec != 0 || m_StartTime.tv_sec != 0)) {
        struct timespec now{};
        clock_gettime(CLOCK_REALTIME, &now);
        return now.tv_sec - m_StartTime.tv_sec;
    }
    return m_StopTime.tv_sec - m_StartTime.tv_sec;
}

void pinepp::timer::pause() {
    if ((m_StopTime.tv_sec != 0 || m_StopTime.tv_sec != 0) && (m_StartTime.tv_sec == 0 && m_StartTime.tv_sec == 0))
        return;
    clock_gettime(CLOCK_REALTIME, &m_StopTime);
}

void pinepp::timer::reset() {
    m_StopTime.tv_sec = m_StartTime.tv_sec = 0;
    m_StopTime.tv_nsec = m_StartTime.tv_nsec = 0;
}