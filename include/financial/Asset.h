#pragma once

#include <string>
#include <memory>
#include "utils/Decimal.h"
#include "utils/IDGenerator.h"

namespace market::financial
{

    class AssetStrategy; // Forward declaration

    class Asset
    {
    public:
        static std::shared_ptr<Asset> create(const std::string &type, const Decimal &value);

        Asset(const std::string &id, const std::string &type, const Decimal &value);
        virtual ~Asset() = default;

        // Getters
        const std::string &getId() const { return id_; }
        const std::string &getType() const { return type_; }
        const Decimal &getValue() const { return value_; }

        // Value management
        void setStrategy(std::shared_ptr<AssetStrategy> strategy) { strategy_ = strategy; }
        virtual void updateValue(const Decimal &newValue);
        virtual Decimal calculateCurrentValue() const;

    protected:
        std::string id_;
        std::string type_;
        Decimal value_;
        std::shared_ptr<AssetStrategy> strategy_;
        static IDGenerator idGen_;
    };

} // namespace market::financial