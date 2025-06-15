# API Reference

## Core Classes

### Account
```cpp
class Account {
public:
    Account(const std::string& id, const std::string& name);
    
    // Getters
    const std::string& getId() const;
    const std::string& getName() const;
    
    // Wallet management
    void addWallet(std::shared_ptr<Wallet> wallet);
    std::shared_ptr<Wallet> getWallet(const std::string& walletId) const;
    
    // Contract management
    void addContract(std::shared_ptr<Contract> contract);
    std::shared_ptr<Contract> getContract(const std::string& contractId) const;
};
```

### Wallet
```cpp
class Wallet {
public:
    Wallet(const std::string& id, const std::string& currency);
    
    // Getters
    const std::string& getId() const;
    const std::string& getCurrency() const;
    
    // Asset management
    void addAsset(std::shared_ptr<Asset> asset);
    void removeAsset(const std::string& assetId);
    std::shared_ptr<Asset> getAsset(const std::string& assetId) const;
    
    // Liability management
    void addLiability(std::shared_ptr<Liability> liability);
    void removeLiability(const std::string& liabilityId);
    std::shared_ptr<Liability> getLiability(const std::string& liabilityId) const;
    
    // Balance calculations
    Decimal getTotalAssets() const;
    Decimal getTotalLiabilities() const;
    Decimal getNetWorth() const;
};
```

### Asset
```cpp
class Asset {
public:
    Asset(const std::string& id, const std::string& type, const Decimal& value);
    
    // Getters
    const std::string& getId() const;
    const std::string& getType() const;
    const Decimal& getValue() const;
    
    // Value management
    virtual void updateValue(const Decimal& newValue);
    virtual Decimal calculateCurrentValue() const;
};
```

### Liability
```cpp
class Liability {
public:
    Liability(const std::string& id, const std::string& type, const Decimal& value);
    
    // Getters
    const std::string& getId() const;
    const std::string& getType() const;
    const Decimal& getValue() const;
    
    // Value management
    virtual void updateValue(const Decimal& newValue);
    virtual Decimal calculateCurrentValue() const;
};
```

### Contract
```cpp
class Contract {
public:
    Contract(const std::string& id, 
             const std::string& type,
             std::shared_ptr<Account> party1,
             std::shared_ptr<Account> party2);
    
    // Getters
    const std::string& getId() const;
    const std::string& getType() const;
    std::shared_ptr<Account> getParty1() const;
    std::shared_ptr<Account> getParty2() const;
    
    // State management
    State getState() const;
    void setState(State newState);
    
    // Contract terms
    virtual void addTerm(const std::string& key, const std::string& value);
    virtual std::string getTerm(const std::string& key) const;
    virtual bool validateTerms() const = 0;
};
```

## Database Interface

### Database
```cpp
class Database {
public:
    Database(const std::string& host, 
             const std::string& port,
             const std::string& dbname,
             const std::string& user,
             const std::string& password);
    
    // Connection management
    bool connect();
    void disconnect();
    bool isConnected() const;
    
    // Transaction management
    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();
    
    // Account operations
    bool saveAccount(const std::string& id, const std::string& name);
    bool loadAccount(const std::string& id, std::string& name);
    bool deleteAccount(const std::string& id);
    
    // Similar operations for other entities...
};
```

## Utility Classes

### Decimal
```cpp
class Decimal {
public:
    Decimal();
    explicit Decimal(double value);
    explicit Decimal(const std::string& str);
    
    // Arithmetic operators
    Decimal operator+(const Decimal& other) const;
    Decimal operator-(const Decimal& other) const;
    Decimal operator*(const Decimal& other) const;
    Decimal operator/(const Decimal& other) const;
    
    // Comparison operators
    bool operator==(const Decimal& other) const;
    bool operator!=(const Decimal& other) const;
    bool operator<(const Decimal& other) const;
    bool operator>(const Decimal& other) const;
    
    // Conversion
    double toDouble() const;
    std::string toString() const;
};
```

## Error Handling

The system uses exceptions for error handling:

```cpp
class MarketException : public std::exception {
public:
    explicit MarketException(const std::string& message);
    const char* what() const noexcept override;
};

class DatabaseException : public MarketException {
public:
    explicit DatabaseException(const std::string& message);
};

class ValidationException : public MarketException {
public:
    explicit ValidationException(const std::string& message);
};
```

## Usage Examples

### Creating an Account with a Wallet
```cpp
// Create account
auto account = std::make_shared<Account>("acc1", "John Doe");

// Create wallet
auto wallet = std::make_shared<Wallet>("wal1", "USD");
account->addWallet(wallet);

// Add asset
auto asset = std::make_shared<Asset>("ast1", "CASH", Decimal("1000.00"));
wallet->addAsset(asset);
```

### Creating a Contract
```cpp
// Create contract between two accounts
auto contract = std::make_shared<Contract>("con1", "LOAN", account1, account2);
contract->addTerm("amount", "5000.00");
contract->addTerm("interest_rate", "0.05");
contract->setState(Contract::State::ACTIVE);
```

### Database Operations
```cpp
// Initialize database connection
Database db("localhost", "5432", "market_system", "user", "password");
db.connect();

// Begin transaction
db.beginTransaction();

try {
    // Save account
    db.saveAccount("acc1", "John Doe");
    
    // Save wallet
    db.saveWallet("wal1", "acc1", "USD");
    
    // Commit transaction
    db.commitTransaction();
} catch (const DatabaseException& e) {
    // Rollback on error
    db.rollbackTransaction();
    throw;
} 