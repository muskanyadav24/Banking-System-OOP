#ifndef ENHANCED_BANKING_SYSTEM_H
#define ENHANCED_BANKING_SYSTEM_H

#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <algorithm>
#include <cmath>

using namespace std;

// Forward declarations
class LoanAccount;
class BankingSystem;

// Base BankAccount class
class BankAccount {
protected:
    string accountNumber;
    string accountHolderName;
    double balance;

public:
    BankAccount(string accNum = "", string holderName = "", double bal = 0.0);
    virtual ~BankAccount() = default;
    
    virtual void deposit(double amount);
    virtual void withdraw(double amount);
    double getBalance() const;
    string getAccountNumber() const;
    string getAccountHolderName() const;
    virtual void displayAccountInfo() const;
};

// Savings Account class
class SavingsAccount : public BankAccount {
private:
    double interestRate;

public:
    SavingsAccount(string accNum = "", string holderName = "", double bal = 0.0, double rate = 5.0);
    void calculateInterest() const;
    void displayAccountInfo() const override;
};

// Checking Account class
class CheckingAccount : public BankAccount {
private:
    double overdraftLimit;

public:
    CheckingAccount(string accNum = "", string holderName = "", double bal = 0.0);
    void withdraw(double amount) override;
    void checkOverdraft(double amount) const;
    void displayAccountInfo() const override;
};

// Fixed Deposit Account class
class FixedDepositAccount : public BankAccount {
private:
    int term;

public:
    FixedDepositAccount(string accNum = "", string holderName = "", double bal = 0.0, int t = 0);
    void calculateInterest() const;
    void displayAccountInfo() const override;
};

// Loan Account class - NEW FEATURE
class LoanAccount {
private:
    string loanId;
    string accountNumber;
    string borrowerName;
    double principalAmount;
    double remainingAmount;
    double interestRate;
    int termMonths;
    double monthlyPayment;
    int paymentsMade;
    string loanType;
    time_t startDate;
    bool isActive;
    vector<pair<time_t, double>> paymentHistory;

public:
    LoanAccount(string loanId = "", string accNum = "", string borrower = "", 
                double principal = 0.0, double rate = 0.0, int term = 0, string type = "Personal");
    
    // Core loan operations
    void makePayment(double amount);
    void calculateMonthlyPayment();
    double calculateTotalInterest() const;
    double getRemainingBalance() const;
    int getRemainingPayments() const;
    
    // Display functions
    void displayLoanInfo() const;
    void displayPaymentSchedule() const;
    void displayPaymentHistory() const;
    
    // Getters
    string getLoanId() const;
    string getAccountNumber() const;
    string getBorrowerName() const;
    double getPrincipalAmount() const;
    double getInterestRate() const;
    int getTermMonths() const;
    string getLoanType() const;
    bool getIsActive() const;
    double getMonthlyPayment() const;
    
    // Loan status
    bool isOverdue() const;
    void closeLoan();
};

// Enhanced Banking System class
class BankingSystem {
private:
    vector<BankAccount*> accounts;
    vector<LoanAccount> loans;
    static int nextLoanId;

public:
    ~BankingSystem();
    
    // Account management
    void createAccount();
    BankAccount* findAccount(const string& accountNumber);
    void displayAllAccounts() const;
    
    // Loan management - NEW FEATURES
    void applyForLoan();
    void makeLoanPayment();
    void displayLoanInfo();
    void displayAllLoans() const;
    void displayLoanPaymentHistory();
    LoanAccount* findLoan(const string& loanId);
    
    // Main menu system
    void displayMainMenu() const;
    void displayLoanMenu() const;
    void run();
    
    // Utility functions
    string generateLoanId();
    bool validateAccountNumber(const string& accNum) const;
    bool validateName(const string& name) const;
};

#endif
