#include "../include/utils/Decimal.h"
#include <sstream>
#include <iomanip>
#include <stdexcept>

Decimal::Decimal(const std::string &str)
{
    std::istringstream iss(str);
    iss >> value_;
    if (iss.fail())
    {
        throw std::invalid_argument("Invalid decimal string");
    }
}

Decimal Decimal::operator+(const Decimal &other) const
{
    return Decimal(value_ + other.value_);
}

Decimal Decimal::operator-(const Decimal &other) const
{
    return Decimal(value_ - other.value_);
}

Decimal Decimal::operator*(const Decimal &other) const
{
    return Decimal(value_ * other.value_);
}

Decimal Decimal::operator/(const Decimal &other) const
{
    if (other.value_ == 0)
    {
        throw std::invalid_argument("Division by zero");
    }
    return Decimal(value_ / other.value_);
}

bool Decimal::operator==(const Decimal &other) const
{
    return std::abs(value_ - other.value_) < EPSILON;
}

bool Decimal::operator!=(const Decimal &other) const
{
    return !(*this == other);
}

bool Decimal::operator<(const Decimal &other) const
{
    return value_ < other.value_ - EPSILON;
}

bool Decimal::operator>(const Decimal &other) const
{
    return value_ > other.value_ + EPSILON;
}

bool Decimal::operator<=(const Decimal &other) const
{
    return !(*this > other);
}

bool Decimal::operator>=(const Decimal &other) const
{
    return !(*this < other);
}

std::string Decimal::toString() const
{
    std::ostringstream oss;
    oss << value_;
    return oss.str();
}