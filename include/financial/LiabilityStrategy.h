#pragma once
#include <memory>
#include "utils/Decimal.h"
class Liability;

class LiabilityStrategy
{
public:
    virtual Decimal calculateValue(const Liability &liability) const = 0;
    virtual ~LiabilityStrategy() = default;
};

class LoanLiabilityStrategy : public LiabilityStrategy
{
public:
    Decimal calculateValue(const Liability &liability) const override;
};

class MarginLiabilityStrategy : public LiabilityStrategy
{
public:
    Decimal calculateValue(const Liability &liability) const override;
};