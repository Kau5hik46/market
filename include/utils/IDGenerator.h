#pragma once
#include <atomic>
#include <mutex>
#include <string>
#include <sstream>
#include <iomanip>

class IDGenerator
{
public:
    IDGenerator(const std::string &prefix, int digits)
        : prefix_(prefix), digits_(digits), counter_(0) {}

    std::string next()
    {
        std::lock_guard<std::mutex> lock(mutex_);
        std::stringstream ss;
        ss << prefix_ << std::setfill('0') << std::setw(digits_) << ++counter_;
        return ss.str();
    }

private:
    std::string prefix_;
    int digits_;
    std::atomic<uint64_t> counter_;
    std::mutex mutex_;
};