#include "financial/Liability.h"
#include "financial/LiabilityStrategy.h"
#include <stdexcept>

IDGenerator Liability::idGen_{"LIA", 9};

std::shared_ptr<Liability> Liability::create(const std::string &type, const Decimal &value)
{
    if (type.empty())
        throw std::invalid_argument("Liability type cannot be empty");
    if (value < Decimal(0))
        throw std::invalid_argument("Liability value cannot be negative");
    return std::shared_ptr<Liability>(new Liability(idGen_.next(), type, value));
}

Liability::Liability(const std::string &id, const std::string &type, const Decimal &value)
    : id_(id), type_(type), value_(value), strategy_(nullptr)
{
    if (id.empty())
    {
        throw std::invalid_argument("Liability ID cannot be empty");
    }
    if (type.empty())
    {
        throw std::invalid_argument("Liability type cannot be empty");
    }
    if (value < Decimal(0))
    {
        throw std::invalid_argument("Liability value cannot be negative");
    }
}

void Liability::updateValue(const Decimal &newValue)
{
    if (newValue < Decimal(0))
    {
        throw std::invalid_argument("Liability value cannot be negative");
    }
    value_ = newValue;
}

Decimal Liability::calculateCurrentValue() const
{
    if (strategy_)
        return strategy_->calculateValue(*this);
    // Default to LoanLiabilityStrategy if not set
    static LoanLiabilityStrategy defaultStrategy;
    return defaultStrategy.calculateValue(*this);
}