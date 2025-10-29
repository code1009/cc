/////////////////////////////////////////////////////////////////////////////
//===========================================================================
#include <ostream>
#include <string>

#include <chrono>
#include <iostream>
#include <iomanip>

//===========================================================================
#include "test_timer.hpp"





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
test_stop_watch::test_stop_watch() noexcept
    : running_(false), elapsed_(std::chrono::steady_clock::duration::zero())
{
}

void test_stop_watch::start() noexcept
{
    if (!running_)
    {
        start_time_ = std::chrono::steady_clock::now();
        running_ = true;
    }
}

void test_stop_watch::stop() noexcept
{
    if (running_)
    {
        elapsed_ += std::chrono::steady_clock::now() - start_time_;
        running_ = false;
    }
}

void test_stop_watch::reset() noexcept
{
    elapsed_ = std::chrono::steady_clock::duration::zero();
    if (running_)
    {
        start_time_ = std::chrono::steady_clock::now();
    }
}

void test_stop_watch::restart() noexcept
{
    elapsed_ = std::chrono::steady_clock::duration::zero();
    start_time_ = std::chrono::steady_clock::now();
    running_ = true;
}

bool test_stop_watch::is_running() const noexcept
{
    return running_;
}

double test_stop_watch::elapsed_seconds() const noexcept
{
    return std::chrono::duration<double>(current_elapsed()).count();
}

double test_stop_watch::elapsed_milliseconds() const noexcept
{
    return std::chrono::duration<double, std::milli>(current_elapsed()).count();
}

double test_stop_watch::elapsed_microseconds() const noexcept
{
    return std::chrono::duration<double, std::micro>(current_elapsed()).count();
}

std::chrono::steady_clock::duration test_stop_watch::current_elapsed() const noexcept
{
    if (running_)
    {
        return elapsed_ + (std::chrono::steady_clock::now() - start_time_);
    }
    return elapsed_;
}





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
test_scoped_timer::test_scoped_timer(std::ostream& os, const std::string& label, int precision_ms)
    : os_(os), label_(label), precision_(precision_ms)
{
    sw_.start();
}

test_scoped_timer::~test_scoped_timer()
{
    sw_.stop();


    if (!label_.empty())
    {
        os_ << label_;
        os_ << " : ";
    }

    os_ << std::fixed << std::setprecision(precision_) << sw_.elapsed_milliseconds() << "msec" << std::endl;
}

void test_scoped_timer::stop_and_report()
{
    if (sw_.is_running())
    {
        sw_.stop();
    }

    if (!label_.empty())
    {
        os_ << label_;
        os_ << " : ";
    }

    os_ << std::fixed << std::setprecision(precision_) << sw_.elapsed_milliseconds() << "msec" << std::endl;
}

test_stop_watch& test_scoped_timer::stopwatch() noexcept
{
    return sw_;
}