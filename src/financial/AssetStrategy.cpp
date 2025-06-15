#include "financial/AssetStrategy.h"
#include "financial/Asset.h"

Decimal CashAssetStrategy::calculateValue(const Asset &asset) const
{
    return asset.getValue();
}

Decimal StockAssetStrategy::calculateValue(const Asset &asset) const
{
    // Placeholder: return value
    return asset.getValue();
}

Decimal BondAssetStrategy::calculateValue(const Asset &asset) const
{
    // Placeholder: return value
    return asset.getValue();
}

Decimal CommodityAssetStrategy::calculateValue(const Asset &asset) const
{
    // Placeholder: return value
    return asset.getValue();
}

Decimal DerivativeAssetStrategy::calculateValue(const Asset &asset) const
{
    // Placeholder: return value
    return asset.getValue();
}