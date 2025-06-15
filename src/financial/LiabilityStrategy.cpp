#include "financial/LiabilityStrategy.h"
#include "financial/Liability.h"

Decimal LoanLiabilityStrategy::calculateValue(const Liability &liability) const
{
    return liability.getValue();
}

Decimal MarginLiabilityStrategy::calculateValue(const Liability &liability) const
{
    return liability.getValue();
}