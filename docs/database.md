# Database Schema and Operations

## Database Schema

### Accounts Table
```sql
CREATE TABLE accounts (
    id VARCHAR(12) PRIMARY KEY,  -- Format: ACCXXXXXXXXXX
    name VARCHAR(255) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);
```

### Wallets Table
```sql
CREATE TABLE wallets (
    id VARCHAR(12) PRIMARY KEY,  -- Format: WLTXXXXXXXXXX
    account_id VARCHAR(12) NOT NULL,
    currency VARCHAR(3) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (account_id) REFERENCES accounts(id) ON DELETE CASCADE
);
```

### Assets Table
```sql
CREATE TABLE assets (
    id VARCHAR(12) PRIMARY KEY,  -- Format: ASTXXXXXXXXXX
    wallet_id VARCHAR(12) NOT NULL,
    type VARCHAR(50) NOT NULL,
    value DECIMAL(20,8) NOT NULL,
    risk_score DECIMAL(10,4),
    metadata JSONB,  -- For storing type-specific data
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (wallet_id) REFERENCES wallets(id) ON DELETE CASCADE
);

-- Type-specific asset tables
CREATE TABLE stock_assets (
    asset_id VARCHAR(12) PRIMARY KEY,
    symbol VARCHAR(10) NOT NULL,
    shares DECIMAL(20,8) NOT NULL,
    purchase_price DECIMAL(20,8) NOT NULL,
    purchase_date TIMESTAMP NOT NULL,
    FOREIGN KEY (asset_id) REFERENCES assets(id) ON DELETE CASCADE
);

CREATE TABLE bond_assets (
    asset_id VARCHAR(12) PRIMARY KEY,
    issuer VARCHAR(255) NOT NULL,
    coupon_rate DECIMAL(10,4) NOT NULL,
    maturity_date DATE NOT NULL,
    face_value DECIMAL(20,8) NOT NULL,
    FOREIGN KEY (asset_id) REFERENCES assets(id) ON DELETE CASCADE
);

CREATE TABLE commodity_assets (
    asset_id VARCHAR(12) PRIMARY KEY,
    commodity_type VARCHAR(50) NOT NULL,
    quantity DECIMAL(20,8) NOT NULL,
    unit VARCHAR(20) NOT NULL,
    storage_location VARCHAR(255),
    FOREIGN KEY (asset_id) REFERENCES assets(id) ON DELETE CASCADE
);

CREATE TABLE derivative_assets (
    asset_id VARCHAR(12) PRIMARY KEY,
    derivative_type VARCHAR(50) NOT NULL,
    underlying_asset_id VARCHAR(12),
    strike_price DECIMAL(20,8),
    expiration_date TIMESTAMP,
    FOREIGN KEY (asset_id) REFERENCES assets(id) ON DELETE CASCADE,
    FOREIGN KEY (underlying_asset_id) REFERENCES assets(id)
);
```

### Liabilities Table
```sql
CREATE TABLE liabilities (
    id VARCHAR(12) PRIMARY KEY,  -- Format: LIAXXXXXXXXXX
    wallet_id VARCHAR(12) NOT NULL,
    type VARCHAR(50) NOT NULL,
    value DECIMAL(20,8) NOT NULL,
    risk_score DECIMAL(10,4),
    metadata JSONB,  -- For storing type-specific data
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (wallet_id) REFERENCES wallets(id) ON DELETE CASCADE
);

-- Type-specific liability tables
CREATE TABLE loan_liabilities (
    liability_id VARCHAR(12) PRIMARY KEY,
    lender VARCHAR(255) NOT NULL,
    interest_rate DECIMAL(10,4) NOT NULL,
    term_months INTEGER NOT NULL,
    payment_schedule JSONB NOT NULL,
    FOREIGN KEY (liability_id) REFERENCES liabilities(id) ON DELETE CASCADE
);

CREATE TABLE margin_liabilities (
    liability_id VARCHAR(12) PRIMARY KEY,
    broker VARCHAR(255) NOT NULL,
    margin_rate DECIMAL(10,4) NOT NULL,
    collateral_asset_id VARCHAR(12),
    FOREIGN KEY (liability_id) REFERENCES liabilities(id) ON DELETE CASCADE,
    FOREIGN KEY (collateral_asset_id) REFERENCES assets(id)
);
```

### Contracts Table
```sql
CREATE TABLE contracts (
    id VARCHAR(12) PRIMARY KEY,  -- Format: CNTXXXXXXXXXX
    type VARCHAR(50) NOT NULL,
    party1_id VARCHAR(12) NOT NULL,
    party2_id VARCHAR(12) NOT NULL,
    state VARCHAR(20) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    FOREIGN KEY (party1_id) REFERENCES accounts(id) ON DELETE CASCADE,
    FOREIGN KEY (party2_id) REFERENCES accounts(id) ON DELETE CASCADE
);

CREATE TABLE contract_terms (
    contract_id VARCHAR(12) NOT NULL,
    key VARCHAR(50) NOT NULL,
    value TEXT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    PRIMARY KEY (contract_id, key),
    FOREIGN KEY (contract_id) REFERENCES contracts(id) ON DELETE CASCADE
);

-- Type-specific contract tables
CREATE TABLE loan_contracts (
    contract_id VARCHAR(12) PRIMARY KEY,
    principal DECIMAL(20,8) NOT NULL,
    interest_rate DECIMAL(10,4) NOT NULL,
    term_months INTEGER NOT NULL,
    payment_frequency VARCHAR(20) NOT NULL,
    FOREIGN KEY (contract_id) REFERENCES contracts(id) ON DELETE CASCADE
);

CREATE TABLE derivative_contracts (
    contract_id VARCHAR(12) PRIMARY KEY,
    underlying_asset_id VARCHAR(12),
    contract_type VARCHAR(50) NOT NULL,
    strike_price DECIMAL(20,8),
    expiration_date TIMESTAMP,
    FOREIGN KEY (contract_id) REFERENCES contracts(id) ON DELETE CASCADE,
    FOREIGN KEY (underlying_asset_id) REFERENCES assets(id)
);
```

### Market Data Tables
```sql
CREATE TABLE market_prices (
    asset_id VARCHAR(12) NOT NULL,
    price DECIMAL(20,8) NOT NULL,
    timestamp TIMESTAMP NOT NULL,
    source VARCHAR(50) NOT NULL,
    PRIMARY KEY (asset_id, timestamp),
    FOREIGN KEY (asset_id) REFERENCES assets(id) ON DELETE CASCADE
);

CREATE TABLE market_indicators (
    indicator_type VARCHAR(50) NOT NULL,
    value DECIMAL(20,8) NOT NULL,
    timestamp TIMESTAMP NOT NULL,
    source VARCHAR(50) NOT NULL,
    PRIMARY KEY (indicator_type, timestamp)
);
```

## Indexes

```sql
-- Accounts
CREATE INDEX idx_accounts_name ON accounts(name);
CREATE INDEX idx_accounts_created ON accounts(created_at);

-- Wallets
CREATE INDEX idx_wallets_account ON wallets(account_id);
CREATE INDEX idx_wallets_currency ON wallets(currency);

-- Assets
CREATE INDEX idx_assets_wallet ON assets(wallet_id);
CREATE INDEX idx_assets_type ON assets(type);
CREATE INDEX idx_assets_risk ON assets(risk_score);
CREATE INDEX idx_assets_created ON assets(created_at);

-- Liabilities
CREATE INDEX idx_liabilities_wallet ON liabilities(wallet_id);
CREATE INDEX idx_liabilities_type ON liabilities(type);
CREATE INDEX idx_liabilities_risk ON liabilities(risk_score);
CREATE INDEX idx_liabilities_created ON liabilities(created_at);

-- Contracts
CREATE INDEX idx_contracts_party1 ON contracts(party1_id);
CREATE INDEX idx_contracts_party2 ON contracts(party2_id);
CREATE INDEX idx_contracts_state ON contracts(state);
CREATE INDEX idx_contracts_created ON contracts(created_at);

-- Market Data
CREATE INDEX idx_market_prices_asset ON market_prices(asset_id);
CREATE INDEX idx_market_prices_timestamp ON market_prices(timestamp);
CREATE INDEX idx_market_indicators_type ON market_indicators(indicator_type);
CREATE INDEX idx_market_indicators_timestamp ON market_indicators(timestamp);
```

## Database Operations

### Connection Management
```cpp
class Database {
    bool connect();
    void disconnect();
    bool isConnected() const;
    bool ping() const;  // Check connection health
};
```

### Transaction Management
```cpp
class Database {
    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();
    bool isInTransaction() const;
};
```

### Account Operations
```cpp
class Database {
    // Account CRUD
    bool saveAccount(const std::string& id, const std::string& name);
    bool loadAccount(const std::string& id, std::string& name);
    bool deleteAccount(const std::string& id);
    std::vector<std::string> loadAllAccountIds();
    
    // Account queries
    std::vector<std::string> findAccountsByName(const std::string& name);
    std::vector<std::string> findAccountsByCreationDate(const std::string& startDate, const std::string& endDate);
};
```

### Wallet Operations
```cpp
class Database {
    // Wallet CRUD
    bool saveWallet(const std::string& id, const std::string& accountId, const std::string& currency);
    bool loadWallet(const std::string& id, std::string& accountId, std::string& currency);
    bool deleteWallet(const std::string& id);
    std::vector<std::string> loadWalletsByAccount(const std::string& accountId);
    
    // Wallet queries
    std::vector<std::string> findWalletsByCurrency(const std::string& currency);
    Decimal getWalletBalance(const std::string& walletId);
};
```

### Asset Operations
```cpp
class Database {
    // Asset CRUD
    bool saveAsset(const std::string& id, const std::string& walletId, 
                  const std::string& type, const Decimal& value);
    bool loadAsset(const std::string& id, std::string& walletId, 
                  std::string& type, Decimal& value);
    bool deleteAsset(const std::string& id);
    std::vector<std::string> loadAssetsByWallet(const std::string& walletId);
    
    // Asset queries
    std::vector<std::string> findAssetsByType(const std::string& type);
    std::vector<std::string> findAssetsByValueRange(const Decimal& minValue, const Decimal& maxValue);
    Decimal getTotalAssetsByWallet(const std::string& walletId);
    
    // Market data
    bool saveMarketPrice(const std::string& assetId, const Decimal& price, 
                        const std::string& source);
    Decimal getLatestMarketPrice(const std::string& assetId);
    std::vector<Decimal> getHistoricalPrices(const std::string& assetId, 
                                           const std::string& startDate,
                                           const std::string& endDate);
};
```

### Liability Operations
```cpp
class Database {
    // Liability CRUD
    bool saveLiability(const std::string& id, const std::string& walletId, 
                      const std::string& type, const Decimal& value);
    bool loadLiability(const std::string& id, std::string& walletId, 
                      std::string& type, Decimal& value);
    bool deleteLiability(const std::string& id);
    std::vector<std::string> loadLiabilitiesByWallet(const std::string& walletId);
    
    // Liability queries
    std::vector<std::string> findLiabilitiesByType(const std::string& type);
    std::vector<std::string> findLiabilitiesByValueRange(const Decimal& minValue, const Decimal& maxValue);
    Decimal getTotalLiabilitiesByWallet(const std::string& walletId);
};
```

### Contract Operations
```cpp
class Database {
    // Contract CRUD
    bool saveContract(const std::string& id, const std::string& type,
                     const std::string& party1Id, const std::string& party2Id,
                     const std::string& state);
    bool loadContract(const std::string& id, std::string& type,
                     std::string& party1Id, std::string& party2Id,
                     std::string& state);
    bool deleteContract(const std::string& id);
    std::vector<std::string> loadContractsByParty(const std::string& partyId);
    
    // Contract terms
    bool saveContractTerm(const std::string& contractId, const std::string& key,
                         const std::string& value);
    bool loadContractTerm(const std::string& contractId, const std::string& key,
                         std::string& value);
    bool deleteContractTerm(const std::string& contractId, const std::string& key);
    std::map<std::string, std::string> loadAllContractTerms(const std::string& contractId);
    
    // Contract queries
    std::vector<std::string> findContractsByType(const std::string& type);
    std::vector<std::string> findContractsByState(const std::string& state);
    std::vector<std::string> findContractsByDateRange(const std::string& startDate,
                                                     const std::string& endDate);
};
```

## Performance Optimization

1. **Connection Pooling**
   - Use a connection pool to manage database connections
   - Configure pool size based on expected load
   - Implement connection timeout and retry logic

2. **Query Optimization**
   - Use prepared statements for frequently executed queries
   - Implement query caching where appropriate
   - Use appropriate indexes for common query patterns
   - Optimize JOIN operations

3. **Batch Operations**
   - Implement batch inserts for bulk data loading
   - Use transactions for multiple related operations
   - Implement bulk updates where possible

4. **Data Management**
   - Implement data partitioning for large tables
   - Regular VACUUM and ANALYZE operations
   - Archive old data to separate tables
   - Implement data compression where appropriate

## Security

1. **Access Control**
   - Implement role-based access control
   - Use least privilege principle
   - Encrypt sensitive data
   - Implement audit logging

2. **Connection Security**
   - Use SSL/TLS for database connections
   - Implement connection pooling with authentication
   - Use strong password policies
   - Implement IP-based access restrictions

3. **Data Protection**
   - Encrypt data at rest
   - Implement data masking for sensitive information
   - Regular security audits
   - Implement backup encryption

## Monitoring and Maintenance

1. **Performance Monitoring**
   - Monitor query execution times
   - Track connection pool usage
   - Monitor disk space usage
   - Track index usage

2. **Health Checks**
   - Regular database health checks
   - Monitor replication lag
   - Check for deadlocks
   - Monitor transaction rates

3. **Backup and Recovery**
   - Implement point-in-time recovery
   - Regular backup testing
   - Document recovery procedures
   - Monitor backup success rates 