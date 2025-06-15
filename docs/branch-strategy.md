# Branch Strategy

## Main Branches

### `main`
- Production-ready code
- Protected branch
- Requires 2 approvals
- Must pass all checks (build, test, lint)
- Only accepts PRs from `beta` or `hotfix/*`

### `beta`
- Pre-release testing
- Protected branch
- Requires 1 approval
- Must pass build and test checks
- Accepts PRs from `dev` or `feature/*`

### `dev`
- Development integration
- Protected branch
- Requires 1 approval
- Must pass build check
- Accepts PRs from feature branches

## Feature Branches

### Pattern: `feature/[a-z0-9-]+`
Examples:
- `feature/stock-trading`
- `feature/derivatives-support`
- `feature/multi-currency`

## Bug Fix Branches

### Pattern: `bugfix/[a-z0-9-]+`
Examples:
- `bugfix/balance-calculation`
- `bugfix/transaction-validation`
- `bugfix/report-generation`

## Hot Fix Branches

### Pattern: `hotfix/[a-z0-9-]+`
Examples:
- `hotfix/critical-security`
- `hotfix/data-corruption`
- `hotfix/performance-issue`

## Release Branches

### Pattern: `release/v[0-9]+\.[0-9]+\.[0-9]+`
Examples:
- `release/v1.0.0`
- `release/v1.1.0`
- `release/v2.0.0`

## Branch Workflow

1. Development Flow:
   ```
   feature/* → dev → beta → main
   ```

2. Bug Fix Flow:
   ```
   bugfix/* → dev → beta → main
   ```

3. Hot Fix Flow:
   ```
   hotfix/* → main → beta → dev
   ```

4. Release Flow:
   ```
   release/* → main → beta → dev
   ```

## Branch Protection Rules

### Main Branch
- Requires 2 approvals
- Must be up to date
- Must pass all checks
- Enforced for admins

### Beta Branch
- Requires 1 approval
- Must be up to date
- Must pass build and test
- Not enforced for admins

### Dev Branch
- Requires 1 approval
- Must pass build
- Not enforced for admins

## Creating New Branches

1. Feature Branch:
   ```bash
   git checkout -b feature/feature-name dev
   ```

2. Bug Fix Branch:
   ```bash
   git checkout -b bugfix/bug-description dev
   ```

3. Hot Fix Branch:
   ```bash
   git checkout -b hotfix/issue-description main
   ```

4. Release Branch:
   ```bash
   git checkout -b release/v1.0.0 beta
   ``` 