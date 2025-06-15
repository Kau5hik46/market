# Design Patterns

This document describes the design patterns used in the Market System and their implementations.

## Factory Pattern

### Purpose
The Factory Pattern is used to create objects without exposing the creation logic to the client. In our system, it's primarily used for creating accounts, contracts, and financial instruments.

### Implementation
```cpp
class AccountFactory {
public:
    static std::shared_ptr<Account> createAccount(const std::string& name) {
        return std::make_shared<Account>(generateId(), name);
    }

private:
    static std::string generateId() {
        // Format: ACCXXXXXXXXXX (12 digits)
        // Example: ACC0000000001
        static std::atomic<uint64_t> counter{0};
        std::stringstream ss;
        ss << "ACC" << std::setfill('0') << std::setw(9) << ++counter;
        return ss.str();
    }
};

class ContractFactory {
public:
    static std::shared_ptr<Contract> createContract(
        const std::string& type,
        std::shared_ptr<Account> party1,
        std::shared_ptr<Account> party2
    ) {
        return std::make_shared<Contract>(generateId(), type, party1, party2);
    }

private:
    static std::string generateId() {
        // Format: CNTXXXXXXXXXX (12 digits)
        // Example: CNT0000000001
        static std::atomic<uint64_t> counter{0};
        std::stringstream ss;
        ss << "CNT" << std::setfill('0') << std::setw(9) << ++counter;
        return ss.str();
    }
};

class AssetFactory {
public:
    static std::shared_ptr<Asset> createAsset(
        const std::string& type,
        const Decimal& value
    ) {
        return std::make_shared<Asset>(generateId(), type, value);
    }

private:
    static std::string generateId() {
        // Format: ASTXXXXXXXXXX (12 digits)
        // Example: AST0000000001
        static std::atomic<uint64_t> counter{0};
        std::stringstream ss;
        ss << "AST" << std::setfill('0') << std::setw(9) << ++counter;
        return ss.str();
    }
};

class LiabilityFactory {
public:
    static std::shared_ptr<Liability> createLiability(
        const std::string& type,
        const Decimal& value
    ) {
        return std::make_shared<Liability>(generateId(), type, value);
    }

private:
    static std::string generateId() {
        // Format: LIAXXXXXXXXXX (12 digits)
        // Example: LIA0000000001
        static std::atomic<uint64_t> counter{0};
        std::stringstream ss;
        ss << "LIA" << std::setfill('0') << std::setw(9) << ++counter;
        return ss.str();
    }
};
```

## Observer Pattern

### Purpose
The Observer Pattern is used to implement market updates and notifications. It allows components to subscribe to and react to market changes.

### Implementation
```cpp
class MarketObserver {
public:
    virtual void onMarketUpdate(const MarketUpdate& update) = 0;
    virtual ~MarketObserver() = default;
};

class MarketSubject {
public:
    void attach(std::shared_ptr<MarketObserver> observer) {
        observers_.push_back(observer);
    }

    void detach(std::shared_ptr<MarketObserver> observer) {
        observers_.erase(
            std::remove(observers_.begin(), observers_.end(), observer),
            observers_.end()
        );
    }

    void notify(const MarketUpdate& update) {
        for (const auto& observer : observers_) {
            observer->onMarketUpdate(update);
        }
    }

private:
    std::vector<std::shared_ptr<MarketObserver>> observers_;
};
```

## Strategy Pattern

### Purpose
The Strategy Pattern is used to define different types of assets and liabilities, allowing for flexible behavior implementation.

### Implementation
```cpp
enum class AssetType {
    CASH,
    STOCK,
    BOND,
    COMMODITY,
    DERIVATIVE
};

class AssetStrategy {
public:
    virtual Decimal calculateValue(const Asset& asset) const = 0;
    virtual Decimal calculateRisk(const Asset& asset) const = 0;
    virtual ~AssetStrategy() = default;
};

class CashAssetStrategy : public AssetStrategy {
public:
    Decimal calculateValue(const Asset& asset) const override {
        return asset.getValue();
    }
    
    Decimal calculateRisk(const Asset& asset) const override {
        return Decimal(0); // Cash has no risk
    }
};

class StockAssetStrategy : public AssetStrategy {
public:
    Decimal calculateValue(const Asset& asset) const override {
        // Complex stock valuation logic including:
        // - Market price
        // - Dividend yield
        // - Growth rate
        // - Risk-free rate
        return calculateDCF(asset);
    }
    
    Decimal calculateRisk(const Asset& asset) const override {
        // Calculate risk metrics:
        // - Beta
        // - Volatility
        // - Value at Risk (VaR)
        return calculateVaR(asset);
    }
};

class BondAssetStrategy : public AssetStrategy {
public:
    Decimal calculateValue(const Asset& asset) const override {
        // Bond valuation including:
        // - Coupon payments
        // - Yield to maturity
        // - Credit risk
        return calculateBondPrice(asset);
    }
    
    Decimal calculateRisk(const Asset& asset) const override {
        // Bond risk metrics:
        // - Duration
        // - Convexity
        // - Credit spread
        return calculateDuration(asset);
    }
};

class CommodityAssetStrategy : public AssetStrategy {
public:
    Decimal calculateValue(const Asset& asset) const override {
        // Commodity valuation including:
        // - Spot price
        // - Storage costs
        // - Convenience yield
        return calculateCommodityValue(asset);
    }
    
    Decimal calculateRisk(const Asset& asset) const override {
        // Commodity risk metrics:
        // - Price volatility
        // - Storage risk
        // - Transportation risk
        return calculateCommodityRisk(asset);
    }
};

class DerivativeAssetStrategy : public AssetStrategy {
public:
    Decimal calculateValue(const Asset& asset) const override {
        // Derivative valuation including:
        // - Black-Scholes for options
        // - Forward pricing
        // - Swap valuation
        return calculateDerivativeValue(asset);
    }
    
    Decimal calculateRisk(const Asset& asset) const override {
        // Derivative risk metrics:
        // - Greeks (delta, gamma, theta, etc.)
        // - Counterparty risk
        // - Market risk
        return calculateDerivativeRisk(asset);
    }
};

class Asset {
public:
    void setStrategy(std::shared_ptr<AssetStrategy> strategy) {
        strategy_ = strategy;
    }

    Decimal calculateCurrentValue() const {
        return strategy_->calculateValue(*this);
    }
    
    Decimal calculateRisk() const {
        return strategy_->calculateRisk(*this);
    }

private:
    std::shared_ptr<AssetStrategy> strategy_;
};
```

## Repository Pattern

### Purpose
The Repository Pattern is used to abstract the data persistence layer, providing a clean interface for data access.

### Implementation
```cpp
class AccountRepository {
public:
    virtual void save(const Account& account) = 0;
    virtual std::shared_ptr<Account> findById(const std::string& id) = 0;
    virtual std::vector<std::shared_ptr<Account>> findAll() = 0;
    virtual void remove(const std::string& id) = 0;
    virtual ~AccountRepository() = default;
};

class PostgresAccountRepository : public AccountRepository {
public:
    PostgresAccountRepository(Database& db) : db_(db) {}

    void save(const Account& account) override {
        db_.beginTransaction();
        try {
            // Save account
            db_.saveAccount(account.getId(), account.getName());
            
            // Save all wallets
            for (const auto& wallet : account.getWallets()) {
                db_.saveWallet(wallet->getId(), account.getId(), wallet->getCurrency());
                
                // Save all assets
                for (const auto& asset : wallet->getAssets()) {
                    db_.saveAsset(asset->getId(), wallet->getId(), 
                                asset->getType(), asset->getValue().toDouble());
                }
                
                // Save all liabilities
                for (const auto& liability : wallet->getLiabilities()) {
                    db_.saveLiability(liability->getId(), wallet->getId(),
                                    liability->getType(), liability->getValue().toDouble());
                }
            }
            
            // Save all contracts
            for (const auto& contract : account.getContracts()) {
                db_.saveContract(contract->getId(), contract->getType(),
                               contract->getParty1()->getId(),
                               contract->getParty2()->getId(),
                               contract->getState());
                
                // Save contract terms
                for (const auto& term : contract->getTerms()) {
                    db_.saveContractTerm(contract->getId(), term.first, term.second);
                }
            }
            
            db_.commitTransaction();
        } catch (const std::exception& e) {
            db_.rollbackTransaction();
            throw;
        }
    }

    std::shared_ptr<Account> findById(const std::string& id) override {
        std::string name;
        if (db_.loadAccount(id, name)) {
            auto account = std::make_shared<Account>(id, name);
            
            // Load wallets
            auto wallets = db_.loadWallets(id);
            for (const auto& wallet : wallets) {
                account->addWallet(wallet);
            }
            
            // Load contracts
            auto contracts = db_.loadContracts(id);
            for (const auto& contract : contracts) {
                account->addContract(contract);
            }
            
            return account;
        }
        return nullptr;
    }

    std::vector<std::shared_ptr<Account>> findAll() override {
        std::vector<std::shared_ptr<Account>> accounts;
        auto ids = db_.loadAllAccountIds();
        for (const auto& id : ids) {
            if (auto account = findById(id)) {
                accounts.push_back(account);
            }
        }
        return accounts;
    }

    void remove(const std::string& id) override {
        db_.beginTransaction();
        try {
            // Delete in reverse order of dependencies
            db_.deleteContractTerms(id);
            db_.deleteContracts(id);
            db_.deleteAssets(id);
            db_.deleteLiabilities(id);
            db_.deleteWallets(id);
            db_.deleteAccount(id);
            db_.commitTransaction();
        } catch (const std::exception& e) {
            db_.rollbackTransaction();
            throw;
        }
    }

private:
    Database& db_;
};
```

## Singleton Pattern

### Purpose
The Singleton Pattern is used for the database connection manager to ensure a single instance throughout the application.

### Implementation
```cpp
class DatabaseManager {
public:
    static DatabaseManager& getInstance() {
        static DatabaseManager instance;
        return instance;
    }

    std::shared_ptr<Database> getConnection() {
        if (!connection_) {
            connection_ = std::make_shared<Database>(
                host_, port_, dbname_, user_, password_
            );
            connection_->connect();
        }
        return connection_;
    }

private:
    DatabaseManager() = default;
    ~DatabaseManager() = default;
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    std::shared_ptr<Database> connection_;
    std::string host_;
    std::string port_;
    std::string dbname_;
    std::string user_;
    std::string password_;
};
```

## Benefits of Using These Patterns

1. **Factory Pattern**
   - Encapsulates object creation
   - Provides consistent ID generation
   - Centralizes object initialization
   - Makes it easy to add new object types

2. **Observer Pattern**
   - Loose coupling between market updates and observers
   - Easy to add new observers
   - Supports event-driven architecture

3. **Strategy Pattern**
   - Flexible asset and liability valuation
   - Easy to add new valuation strategies
   - Runtime strategy switching
   - Separation of valuation logic from asset types

4. **Repository Pattern**
   - Clean separation of data access
   - Transaction management
   - Consistent error handling
   - Easy to switch database implementations

5. **Singleton Pattern**
   - Single database connection instance
   - Resource management
   - Global access point

## Best Practices

1. Use smart pointers for memory management
2. Implement proper error handling
3. Follow SOLID principles
4. Write unit tests for each pattern implementation
5. Document pattern usage and rationale
6. Consider thread safety where applicable
7. Use atomic counters for ID generation
8. Implement proper transaction management
9. Handle all dependencies in repositories
10. Use consistent naming conventions
11. Consider performance optimizations
12. Implement proper validation and sanitization
13. Use consistent formatting and indentation
14. Implement proper logging and monitoring
15. Consider security and privacy implications 