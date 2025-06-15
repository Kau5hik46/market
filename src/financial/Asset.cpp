#include "financial/Asset.h"
#include "financial/AssetStrategy.h"
#include <stdexcept>

IDGenerator Asset::idGen_{"AST", 9};

std::shared_ptr<Asset> Asset::create(const std::string &type, const Decimal &value)
{
    if (type.empty())
        throw std::invalid_argument("Asset type cannot be empty");
    if (value < Decimal(0))
        throw std::invalid_argument("Asset value cannot be negative");
    return std::shared_ptr<Asset>(new Asset(idGen_.next(), type, value));
}

Asset::Asset(const std::string &id, const std::string &type, const Decimal &value)
    : id_(id), type_(type), value_(value), strategy_(nullptr)
{
    if (id.empty())
    {
        throw std::invalid_argument("Asset ID cannot be empty");
    }
    if (type.empty())
    {
        throw std::invalid_argument("Asset type cannot be empty");
    }
    if (value < Decimal(0))
    {
        throw std::invalid_argument("Asset value cannot be negative");
    }
}

void Asset::updateValue(const Decimal &newValue)
{
    if (newValue < Decimal(0))
    {
        throw std::invalid_argument("Asset value cannot be negative");
    }
    value_ = newValue;
}

Decimal Asset::calculateCurrentValue() const
{
    if (strategy_)
        return strategy_->calculateValue(*this);
    // Default to CashAssetStrategy if not set
    static CashAssetStrategy defaultStrategy;
    return defaultStrategy.calculateValue(*this);
}