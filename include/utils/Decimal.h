#pragma once
#include <string>
#include <cmath>

class Decimal
{
public:
    Decimal() : value_(0) {}
    explicit Decimal(double value) : value_(value) {}
    explicit Decimal(const std::string &str);

    // Arithmetic operators
    Decimal operator+(const Decimal &other) const;
    Decimal operator-(const Decimal &other) const;
    Decimal operator*(const Decimal &other) const;
    Decimal operator/(const Decimal &other) const;

    // Unary minus operator
    Decimal operator-() const { return Decimal(-value_); }

    // Comparison operators
    bool operator==(const Decimal &other) const;
    bool operator!=(const Decimal &other) const;
    bool operator<(const Decimal &other) const;
    bool operator>(const Decimal &other) const;
    bool operator<=(const Decimal &other) const;
    bool operator>=(const Decimal &other) const;

    // Conversion
    double toDouble() const { return value_; }
    std::string toString() const;

private:
    double value_;
    static constexpr double EPSILON = 1e-10;
};