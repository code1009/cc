#pragma once

/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class test_stop_watch
{
public:
    test_stop_watch() noexcept;

public:
    void start() noexcept;
    void stop() noexcept;
    void reset() noexcept;
    void restart() noexcept;

    bool is_running() const noexcept;

    double elapsed_seconds() const noexcept;
    double elapsed_milliseconds() const noexcept;
    double elapsed_microseconds() const noexcept;

private:
    std::chrono::steady_clock::duration current_elapsed() const noexcept;

private:
    bool running_;
    std::chrono::steady_clock::time_point start_time_;
    std::chrono::steady_clock::duration elapsed_;
};





/////////////////////////////////////////////////////////////////////////////
//===========================================================================
class test_scoped_timer
{
public:
    test_scoped_timer(std::ostream& os, const std::string& label = "", int precision_ms = 3);
    ~test_scoped_timer();

public:
    void stop_and_report();
    test_stop_watch& stopwatch() noexcept;

private:
    std::ostream& os_;
    std::string label_;
    int precision_;
    test_stop_watch sw_;
};
