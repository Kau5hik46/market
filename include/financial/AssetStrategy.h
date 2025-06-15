#pragma once
#include <memory>
#include "utils/Decimal.h"
class Asset;

class AssetStrategy
{
public:
    virtual Decimal calculateValue(const Asset &asset) const = 0;
    virtual ~AssetStrategy() = default;
};

class CashAssetStrategy : public AssetStrategy
{
public:
    Decimal calculateValue(const Asset &asset) const override;
};

class StockAssetStrategy : public AssetStrategy
{
public:
    Decimal calculateValue(const Asset &asset) const override;
};

class BondAssetStrategy : public AssetStrategy
{
public:
    Decimal calculateValue(const Asset &asset) const override;
};

class CommodityAssetStrategy : public AssetStrategy
{
public:
    Decimal calculateValue(const Asset &asset) const override;
};

class DerivativeAssetStrategy : public AssetStrategy
{
public:
    Decimal calculateValue(const Asset &asset) const override;
};