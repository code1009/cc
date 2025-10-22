/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include <iostream>
#include <chrono>
#include <random>
#include <vector>
#include <algorithm>
#include <unordered_map>
#include <cstdlib>

#include <ostream>
#include <sstream>
#include <string>
#include <iomanip>





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class test_stop_watch
{
public:
    using clock_t = std::chrono::steady_clock;
    using rep_t = std::chrono::steady_clock::duration;

    test_stop_watch() noexcept : running_(false), elapsed_(rep_t::zero()) {}

    void start() noexcept
    {
        if (!running_)
        {
            start_time_ = clock_t::now();
            running_ = true;
        }
    }

    void stop() noexcept
    {
        if (running_)
        {
            elapsed_ += clock_t::now() - start_time_;
            running_ = false;
        }
    }

    void reset() noexcept
    {
        elapsed_ = rep_t::zero();
        if (running_)
        {
            start_time_ = clock_t::now();
        }
    }

    void restart() noexcept
    {
        elapsed_ = rep_t::zero();
        start_time_ = clock_t::now();
        running_ = true;
    }

    bool is_running() const noexcept { return running_; }

    double elapsed_seconds() const noexcept
    {
        return std::chrono::duration<double>(current_elapsed()).count();
    }

    double elapsed_milliseconds() const noexcept
    {
        return std::chrono::duration<double, std::milli>(current_elapsed()).count();
    }

    double elapsed_microseconds() const noexcept
    {
        return std::chrono::duration<double, std::micro>(current_elapsed()).count();
    }

private:
    rep_t current_elapsed() const noexcept
    {
        if (running_)
        {
            return elapsed_ + (clock_t::now() - start_time_);
        }
        return elapsed_;
    }

    bool running_;
    clock_t::time_point start_time_;
    rep_t elapsed_;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class test_scoped_timer
{
public:
    test_scoped_timer(std::ostream& os, const std::string& label = "", int precision_ms = 3)
        : os_(os), label_(label), precision_(precision_ms)
    {
        sw_.start();
    }

    ~test_scoped_timer()
    {
        sw_.stop();
        os_ << label_;
        if (!label_.empty()) os_ << " : ";
        os_ << std::fixed << std::setprecision(precision_) << sw_.elapsed_milliseconds() << " ms" << std::endl;
    }

    void stop_and_report()
    {
        if (sw_.is_running()) sw_.stop();
        os_ << label_;
        if (!label_.empty()) os_ << " : ";
        os_ << std::fixed << std::setprecision(precision_) << sw_.elapsed_milliseconds() << " ms" << std::endl;
    }

    test_stop_watch& stopwatch() noexcept { return sw_; }

private:
    std::ostream& os_;
    std::string label_;
    int precision_;
    test_stop_watch sw_;
};
