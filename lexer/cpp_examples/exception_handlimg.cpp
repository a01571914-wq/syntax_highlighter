#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>
#include <memory>
using namespace std;

// ============================================================
//  Exception Handling - Custom exception hierarchy applied to
//  a small banking system with transaction logging
// ============================================================

// Base class for all banking-related exceptions
class BankException : public runtime_error {
public:
    explicit BankException(const string& msg) : runtime_error(msg) {}
};

class InsufficientFundsException : public BankException {
private:
    double amount;
    double available;
public:
    InsufficientFundsException(double amt, double avail)
        : BankException("Insufficient funds"), amount(amt), available(avail) {}
    double getAmount() const { return amount; }
    double getAvailable() const { return available; }
};

class InvalidAmountException : public BankException {
public:
    explicit InvalidAmountException(const string& msg) : BankException(msg) {}
};

class AccountNotFoundException : public BankException {
private:
    string accountId;
public:
    explicit AccountNotFoundException(const string& id)
        : BankException("Account not found: " + id), accountId(id) {}
    const string& getAccountId() const { return accountId; }
};

class AccountFrozenException : public BankException {
public:
    explicit AccountFrozenException(const string& msg) : BankException(msg) {}
};

class DailyLimitExceededException : public BankException {
private:
    double limit;
    double attempted;
public:
    DailyLimitExceededException(double lim, double att)
        : BankException("Daily withdrawal limit exceeded"), limit(lim), attempted(att) {}
    double getLimit() const { return limit; }
    double getAttempted() const { return attempted; }
};

class BankAccount {
private:
    string owner;
    string id;
    double balance;
    double dailyWithdrawn = 0.0;
    double dailyLimit;
    bool frozen = false;

public:
    BankAccount(const string& accountId, const string& name, double initial, double limit = 5000.0)
        : owner(name), id(accountId), balance(initial), dailyLimit(limit) {}

    const string& getId() const { return id; }
    const string& getOwner() const { return owner; }
    double getBalance() const { return balance; }
    bool isFrozen() const { return frozen; }

    void freeze() { frozen = true; }
    void unfreeze() { frozen = false; }

    void deposit(double amount) {
        if (frozen)
            throw AccountFrozenException("Cannot deposit: account " + id + " is frozen");
        if (amount <= 0)
            throw InvalidAmountException("Deposit amount must be positive");
        balance += amount;
        cout << "  [" << id << "] Deposited $" << amount << ". New balance: $" << balance << endl;
    }

    void withdraw(double amount) {
        if (frozen)
            throw AccountFrozenException("Cannot withdraw: account " + id + " is frozen");
        if (amount <= 0)
            throw InvalidAmountException("Withdrawal amount must be positive");
        if (dailyWithdrawn + amount > dailyLimit)
            throw DailyLimitExceededException(dailyLimit, dailyWithdrawn + amount);
        if (amount > balance)
            throw InsufficientFundsException(amount, balance);

        balance -= amount;
        dailyWithdrawn += amount;
        cout << "  [" << id << "] Withdrew $" << amount << ". New balance: $" << balance << endl;
    }

    void resetDailyLimit() {
        dailyWithdrawn = 0.0;
    }
};

// A simple bank holding multiple accounts, providing transfer functionality
class Bank {
private:
    map<string, unique_ptr<BankAccount>> accounts;

public:
    BankAccount& openAccount(const string& id, const string& owner, double initial, double dailyLimit = 5000.0) {
        accounts[id] = make_unique<BankAccount>(id, owner, initial, dailyLimit);
        return *accounts[id];
    }

    BankAccount& getAccount(const string& id) {
        auto it = accounts.find(id);
        if (it == accounts.end())
            throw AccountNotFoundException(id);
        return *(it->second);
    }

    void transfer(const string& fromId, const string& toId, double amount) {
        BankAccount& from = getAccount(fromId);
        BankAccount& to = getAccount(toId);

        // withdraw() may throw; if it does, no state has changed yet so we're safe
        from.withdraw(amount);
        try {
            to.deposit(amount);
        } catch (...) {
            // If the deposit fails for any reason, roll back the withdrawal
            from.deposit(amount);
            throw;
        }
        cout << "  Transferred $" << amount << " from " << fromId << " to " << toId << endl;
    }
};

// Demonstrates exception specifications with noexcept and function-try-blocks
double safeDivide(double a, double b) noexcept {
    try {
        if (b == 0) throw invalid_argument("division by zero");
        return a / b;
    } catch (...) {
        // noexcept functions must not let exceptions escape
        return 0.0;
    }
}

// A function that always rethrows after logging, demonstrating exception propagation
void processTransaction(Bank& bank, const string& accId, double amount, bool isWithdrawal) {
    try {
        if (isWithdrawal)
            bank.getAccount(accId).withdraw(amount);
        else
            bank.getAccount(accId).deposit(amount);
    } catch (const BankException& e) {
        cout << "  [LOG] Transaction failed for account " << accId << ": " << e.what() << endl;
        throw; // rethrow so the caller can also handle it
    }
}

int main() {
    cout << "=== Setting Up Bank ===" << endl;
    Bank bank;
    bank.openAccount("ACC001", "Alice", 1000.0, 1500.0);
    bank.openAccount("ACC002", "Bob", 200.0);
    cout << "Opened accounts for Alice (ACC001) and Bob (ACC002)" << endl;

    cout << endl << "=== Basic Deposits and Withdrawals ===" << endl;
    try {
        bank.getAccount("ACC001").deposit(500.0);
        bank.getAccount("ACC001").withdraw(200.0);
    } catch (const BankException& e) {
        cout << "Error: " << e.what() << endl;
    }

    cout << endl << "=== Insufficient Funds ===" << endl;
    try {
        bank.getAccount("ACC002").withdraw(2000.0);
    } catch (const InsufficientFundsException& e) {
        cout << "Error: " << e.what() << " (tried $" << e.getAmount()
             << ", available $" << e.getAvailable() << ")" << endl;
    }

    cout << endl << "=== Invalid Amount ===" << endl;
    try {
        bank.getAccount("ACC002").deposit(-50.0);
    } catch (const InvalidAmountException& e) {
        cout << "Error: " << e.what() << endl;
    }

    cout << endl << "=== Account Not Found ===" << endl;
    try {
        bank.getAccount("ACC999").deposit(100.0);
    } catch (const AccountNotFoundException& e) {
        cout << "Error: " << e.what() << " (id: " << e.getAccountId() << ")" << endl;
    }

    cout << endl << "=== Frozen Account ===" << endl;
    bank.getAccount("ACC002").freeze();
    try {
        bank.getAccount("ACC002").withdraw(50.0);
    } catch (const AccountFrozenException& e) {
        cout << "Error: " << e.what() << endl;
    }
    bank.getAccount("ACC002").unfreeze();
    cout << "Account ACC002 unfrozen." << endl;

    cout << endl << "=== Daily Limit Exceeded ===" << endl;
    try {
        bank.getAccount("ACC001").withdraw(1600.0);
    } catch (const DailyLimitExceededException& e) {
        cout << "Error: " << e.what() << " (limit $" << e.getLimit()
             << ", attempted $" << e.getAttempted() << ")" << endl;
    }

    cout << endl << "=== Transfers With Rollback ===" << endl;
    try {
        bank.transfer("ACC001", "ACC002", 300.0);
        cout << "ACC001 balance: " << bank.getAccount("ACC001").getBalance() << endl;
        cout << "ACC002 balance: " << bank.getAccount("ACC002").getBalance() << endl;
    } catch (const BankException& e) {
        cout << "Transfer failed: " << e.what() << endl;
    }

    cout << endl << "=== Catching Multiple Exception Types Generically ===" << endl;
    vector<pair<string, double>> ops = {{"ACC001", -10.0}, {"ACC999", 50.0}, {"ACC002", 100000.0}};
    for (auto& op : ops) {
        try {
            processTransaction(bank, op.first, op.second, false);
        } catch (const InvalidAmountException& e) {
            cout << "  -> Handled as InvalidAmountException" << endl;
        } catch (const AccountNotFoundException& e) {
            cout << "  -> Handled as AccountNotFoundException" << endl;
        } catch (const BankException& e) {
            cout << "  -> Handled as generic BankException: " << e.what() << endl;
        } catch (const exception& e) {
            cout << "  -> Handled as std::exception: " << e.what() << endl;
        }
    }

    cout << endl << "=== noexcept safe division ===" << endl;
    cout << "10 / 2 = " << safeDivide(10, 2) << endl;
    cout << "10 / 0 = " << safeDivide(10, 0) << " (returns 0 instead of throwing)" << endl;

    cout << endl << "=== Final Balances ===" << endl;
    cout << "ACC001 (Alice): $" << bank.getAccount("ACC001").getBalance() << endl;
    cout << "ACC002 (Bob):   $" << bank.getAccount("ACC002").getBalance() << endl;

    return 0;
}