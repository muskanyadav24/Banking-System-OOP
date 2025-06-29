#include "enhanced_banking_system.h"

// Initialize static member
int BankingSystem::nextLoanId = 1001;

// BankAccount Implementation
BankAccount::BankAccount(string accNum, string holderName, double bal) 
    : accountNumber(accNum), accountHolderName(holderName), balance(bal) {}

void BankAccount::deposit(double amount) {
    if (amount > 0) {
        balance += amount;
        cout << "Deposited: $" << fixed << setprecision(2) << amount 
             << ", New Balance: $" << balance << endl;
    } else {
        cout << "Invalid deposit amount!" << endl;
    }
}

void BankAccount::withdraw(double amount) {
    if (amount > 0 && amount <= balance) {
        balance -= amount;
        cout << "Withdrew: $" << fixed << setprecision(2) << amount 
             << ", New Balance: $" << balance << endl;
    } else {
        cout << "Invalid amount or insufficient funds!" << endl;
    }
}

double BankAccount::getBalance() const { return balance; }
string BankAccount::getAccountNumber() const { return accountNumber; }
string BankAccount::getAccountHolderName() const { return accountHolderName; }

void BankAccount::displayAccountInfo() const {
    cout << "\n--- Account Information ---" << endl;
    cout << "Account Number: " << accountNumber << endl;
    cout << "Holder Name: " << accountHolderName << endl;
    cout << "Balance: $" << fixed << setprecision(2) << balance << endl;
}

// SavingsAccount Implementation
SavingsAccount::SavingsAccount(string accNum, string holderName, double bal, double rate)
    : BankAccount(accNum, holderName, bal), interestRate(rate) {}

void SavingsAccount::calculateInterest() const {
    double interest = getBalance() * interestRate / 100;
    cout << "Annual Interest: $" << fixed << setprecision(2) << interest 
         << " at " << interestRate << "% rate" << endl;
}

void SavingsAccount::displayAccountInfo() const {
    BankAccount::displayAccountInfo();
    cout << "Account Type: Savings" << endl;
    cout << "Interest Rate: " << interestRate << "%" << endl;
}

// CheckingAccount Implementation
CheckingAccount::CheckingAccount(string accNum, string holderName, double bal)
    : BankAccount(accNum, holderName, bal), overdraftLimit(1000) {}

void CheckingAccount::withdraw(double amount) {
    double totalAvailable = getBalance() + overdraftLimit;
    if (amount > 0 && amount <= totalAvailable) {
        balance -= amount;
        cout << "Withdrew: $" << fixed << setprecision(2) << amount 
             << ", New Balance: $" << balance << endl;
        if (balance < 0) {
            cout << "Warning: Account is overdrawn by $" << abs(balance) << endl;
        }
    } else {
        cout << "Overdraft limit exceeded. Withdrawal denied." << endl;
    }
}

void CheckingAccount::checkOverdraft(double amount) const {
    double totalAvailable = getBalance() + overdraftLimit;
    cout << "Total Available (with overdraft): $" << fixed << setprecision(2) 
         << totalAvailable << endl;
    
    if (amount <= totalAvailable) {
        cout << "Withdrawal allowed." << endl;
    } else {
        cout << "Overdraft exceeded." << endl;
    }
}

void CheckingAccount::displayAccountInfo() const {
    BankAccount::displayAccountInfo();
    cout << "Account Type: Checking" << endl;
    cout << "Overdraft Limit: $" << fixed << setprecision(2) << overdraftLimit << endl;
}

// FixedDepositAccount Implementation
FixedDepositAccount::FixedDepositAccount(string accNum, string holderName, double bal, int t)
    : BankAccount(accNum, holderName, bal), term(t) {}

void FixedDepositAccount::calculateInterest() const {
    double interest = getBalance() * 7.0 * term / 1200.0;
    cout << "Fixed Deposit Interest: $" << fixed << setprecision(2) << interest 
         << " for " << term << " months at 7% annual rate" << endl;
}

void FixedDepositAccount::displayAccountInfo() const {
    BankAccount::displayAccountInfo();
    cout << "Account Type: Fixed Deposit" << endl;
    cout << "Term: " << term << " months" << endl;
}

// LoanAccount Implementation
LoanAccount::LoanAccount(string loanId, string accNum, string borrower, 
                        double principal, double rate, int term, string type)
    : loanId(loanId), accountNumber(accNum), borrowerName(borrower),
      principalAmount(principal), remainingAmount(principal), 
      interestRate(rate), termMonths(term), paymentsMade(0),
      loanType(type), isActive(true) {
    startDate = time(0);
    calculateMonthlyPayment();
}

void LoanAccount::calculateMonthlyPayment() {
    if (termMonths > 0 && interestRate > 0) {
        double monthlyRate = interestRate / 1200.0; // Convert annual % to monthly decimal
        double temp = pow(1 + monthlyRate, termMonths);
        monthlyPayment = (principalAmount * monthlyRate * temp) / (temp - 1);
    } else {
        monthlyPayment = principalAmount / termMonths; // Simple division for 0% interest
    }
}

void LoanAccount::makePayment(double amount) {
    if (!isActive) {
        cout << "Loan is already closed!" << endl;
        return;
    }
    
    if (amount <= 0) {
        cout << "Invalid payment amount!" << endl;
        return;
    }
    
    if (amount > remainingAmount) {
        cout << "Payment amount exceeds remaining balance. Adjusting to remaining amount." << endl;
        amount = remainingAmount;
    }
    
    // Calculate interest portion and principal portion
    double monthlyRate = interestRate / 1200.0;
    double interestPortion = remainingAmount * monthlyRate;
    double principalPortion = amount - interestPortion;
    
    if (principalPortion < 0) principalPortion = 0;
    
    remainingAmount -= principalPortion;
    paymentsMade++;
    
    // Record payment in history
    paymentHistory.push_back(make_pair(time(0), amount));
    
    cout << "\n--- Payment Processed ---" << endl;
    cout << "Payment Amount: $" << fixed << setprecision(2) << amount << endl;
    cout << "Interest Portion: $" << interestPortion << endl;
    cout << "Principal Portion: $" << principalPortion << endl;
    cout << "Remaining Balance: $" << remainingAmount << endl;
    cout << "Payments Made: " << paymentsMade << "/" << termMonths << endl;
    
    if (remainingAmount <= 0.01) { // Account for floating point precision
        remainingAmount = 0;
        isActive = false;
        cout << "\nCongratulations! Loan has been fully paid off!" << endl;
    }
}

double LoanAccount::calculateTotalInterest() const {
    return (monthlyPayment * termMonths) - principalAmount;
}

double LoanAccount::getRemainingBalance() const {
    return remainingAmount;
}

int LoanAccount::getRemainingPayments() const {
    return termMonths - paymentsMade;
}

void LoanAccount::displayLoanInfo() const {
    cout << "\n=== LOAN INFORMATION ===" << endl;
    cout << "Loan ID: " << loanId << endl;
    cout << "Account Number: " << accountNumber << endl;
    cout << "Borrower: " << borrowerName << endl;
    cout << "Loan Type: " << loanType << endl;
    cout << "Principal Amount: $" << fixed << setprecision(2) << principalAmount << endl;
    cout << "Interest Rate: " << interestRate << "% annual" << endl;
    cout << "Term: " << termMonths << " months" << endl;
    cout << "Monthly Payment: $" << monthlyPayment << endl;
    cout << "Remaining Balance: $" << remainingAmount << endl;
    cout << "Payments Made: " << paymentsMade << "/" << termMonths << endl;
    cout << "Status: " << (isActive ? "Active" : "Closed") << endl;
    cout << "Total Interest: $" << calculateTotalInterest() << endl;
    
    // Display start date
    char* dateStr = ctime(&startDate);
    cout << "Start Date: " << dateStr;
}

void LoanAccount::displayPaymentSchedule() const {
    cout << "\n=== PAYMENT SCHEDULE ===" << endl;
    cout << "Monthly Payment: $" << fixed << setprecision(2) << monthlyPayment << endl;
    cout << "Remaining Payments: " << getRemainingPayments() << endl;
    cout << "Total Remaining Amount: $" << remainingAmount << endl;
    
    if (getRemainingPayments() > 0) {
        cout << "\nNext " << min(6, getRemainingPayments()) << " payments:" << endl;
        double tempBalance = remainingAmount;
        double monthlyRate = interestRate / 1200.0;
        
        for (int i = 1; i <= min(6, getRemainingPayments()); i++) {
            double interestPortion = tempBalance * monthlyRate;
            double principalPortion = monthlyPayment - interestPortion;
            tempBalance -= principalPortion;
            
            cout << "Payment " << (paymentsMade + i) << ": Interest $" 
                 << interestPortion << ", Principal $" << principalPortion 
                 << ", Balance $" << tempBalance << endl;
        }
    }
}

void LoanAccount::displayPaymentHistory() const {
    cout << "\n=== PAYMENT HISTORY ===" << endl;
    if (paymentHistory.empty()) {
        cout << "No payments made yet." << endl;
        return;
    }
    
    cout << "Total Payments Made: " << paymentHistory.size() << endl;
    cout << "\nRecent Payments:" << endl;
    
    int startIdx = max(0, (int)paymentHistory.size() - 10); // Show last 10 payments
    for (int i = startIdx; i < paymentHistory.size(); i++) {
        char* dateStr = ctime(&paymentHistory[i].first);
        cout << "Payment " << (i + 1) << ": $" << fixed << setprecision(2) 
             << paymentHistory[i].second << " on " << dateStr;
    }
}

// Getters
string LoanAccount::getLoanId() const { return loanId; }
string LoanAccount::getAccountNumber() const { return accountNumber; }
string LoanAccount::getBorrowerName() const { return borrowerName; }
double LoanAccount::getPrincipalAmount() const { return principalAmount; }
double LoanAccount::getInterestRate() const { return interestRate; }
int LoanAccount::getTermMonths() const { return termMonths; }
string LoanAccount::getLoanType() const { return loanType; }
bool LoanAccount::getIsActive() const { return isActive; }
double LoanAccount::getMonthlyPayment() const { return monthlyPayment; }

bool LoanAccount::isOverdue() const {
    // Simple overdue logic - could be enhanced with actual due dates
    time_t now = time(0);
    double monthsElapsed = difftime(now, startDate) / (30 * 24 * 3600); // Approximate
    return (monthsElapsed > paymentsMade + 1) && isActive;
}

void LoanAccount::closeLoan() {
    isActive = false;
    remainingAmount = 0;
    cout << "Loan " << loanId << " has been closed." << endl;
}

// BankingSystem Implementation
BankingSystem::~BankingSystem() {
    for (auto account : accounts) {
        delete account;
    }
}

void BankingSystem::createAccount() {
    int accType;
    string accNum, name;
    double balance;

    cout << "\n=== CREATE NEW ACCOUNT ===" << endl;
    cout << "Choose Account Type:" << endl;
    cout << "1. Savings Account" << endl;
    cout << "2. Checking Account" << endl;
    cout << "3. Fixed Deposit Account" << endl;
    cout << "Enter choice (1-3): ";
    cin >> accType;
    cin.ignore();

    // Validate account number
    do {
        cout << "Enter Account Number (12 characters): ";
        getline(cin, accNum);
        if (!validateAccountNumber(accNum)) {
            cout << "Invalid account number! Must be exactly 12 characters." << endl;
        }
    } while (!validateAccountNumber(accNum));

    // Check if account already exists
    if (findAccount(accNum) != nullptr) {
        cout << "Account with this number already exists!" << endl;
        return;
    }

    // Validate name
    do {
        cout << "Enter Account Holder Name: ";
        getline(cin, name);
        if (!validateName(name)) {
            cout << "Invalid name! Use only alphabets and spaces." << endl;
        }
    } while (!validateName(name));

    // Validate initial balance
    do {
        cout << "Enter Initial Balance: $";
        cin >> balance;
        if (balance < 0) {
            cout << "Balance cannot be negative!" << endl;
        }
    } while (balance < 0);

    BankAccount* newAccount = nullptr;

    switch (accType) {
        case 1: {
            double rate;
            cout << "Enter Interest Rate (%): ";
            cin >> rate;
            newAccount = new SavingsAccount(accNum, name, balance, rate);
            break;
        }
        case 2: {
            newAccount = new CheckingAccount(accNum, name, balance);
            cout << "Overdraft limit set to $1000 by default." << endl;
            break;
        }
        case 3: {
            int term;
            cout << "Enter Term (months): ";
            cin >> term;
            newAccount = new FixedDepositAccount(accNum, name, balance, term);
            break;
        }
        default:
            cout << "Invalid account type!" << endl;
            return;
    }

    if (newAccount) {
        accounts.push_back(newAccount);
        cout << "\nAccount created successfully!" << endl;
        newAccount->displayAccountInfo();
    }
}

BankAccount* BankingSystem::findAccount(const string& accountNumber) {
    for (auto account : accounts) {
        if (account->getAccountNumber() == accountNumber) {
            return account;
        }
    }
    return nullptr;
}

void BankingSystem::displayAllAccounts() const {
    if (accounts.empty()) {
        cout << "No accounts found." << endl;
        return;
    }

    cout << "\n=== ALL ACCOUNTS ===" << endl;
    for (const auto& account : accounts) {
        account->displayAccountInfo();
        cout << "------------------------" << endl;
    }
}

// Loan Management Functions
void BankingSystem::applyForLoan() {
    string accountNumber, borrowerName, loanType;
    double principal, interestRate;
    int termMonths;

    cout << "\n=== LOAN APPLICATION ===" << endl;
    
    cin.ignore();
    cout << "Enter Account Number: ";
    getline(cin, accountNumber);
    
    // Verify account exists
    BankAccount* account = findAccount(accountNumber);
    if (!account) {
        cout << "Account not found! Please create an account first." << endl;
        return;
    }

    cout << "Enter Borrower Name: ";
    getline(cin, borrowerName);

    cout << "Select Loan Type:" << endl;
    cout << "1. Personal Loan (8-15% interest)" << endl;
    cout << "2. Home Loan (6-9% interest)" << endl;
    cout << "3. Car Loan (7-12% interest)" << endl;
    cout << "4. Business Loan (10-18% interest)" << endl;
    cout << "Enter choice (1-4): ";
    
    int loanTypeChoice;
    cin >> loanTypeChoice;
    
    switch (loanTypeChoice) {
        case 1: loanType = "Personal"; interestRate = 12.0; break;
        case 2: loanType = "Home"; interestRate = 7.5; break;
        case 3: loanType = "Car"; interestRate = 9.5; break;
        case 4: loanType = "Business"; interestRate = 14.0; break;
        default: 
            cout << "Invalid choice! Defaulting to Personal Loan." << endl;
            loanType = "Personal"; 
            interestRate = 12.0;
    }

    cout << "Enter Loan Amount: $";
    cin >> principal;
    
    if (principal <= 0) {
        cout << "Invalid loan amount!" << endl;
        return;
    }

    cout << "Default interest rate for " << loanType << " loan: " << interestRate << "%" << endl;
    cout << "Enter custom interest rate (or press 0 for default): ";
    double customRate;
    cin >> customRate;
    if (customRate > 0) {
        interestRate = customRate;
    }

    cout << "Enter loan term (months): ";
    cin >> termMonths;
    
    if (termMonths <= 0) {
        cout << "Invalid loan term!" << endl;
        return;
    }

    // Create loan
    string loanId = generateLoanId();
    LoanAccount newLoan(loanId, accountNumber, borrowerName, principal, interestRate, termMonths, loanType);
    loans.push_back(newLoan);

    cout << "\n=== LOAN APPROVED ===" << endl;
    cout << "Loan ID: " << loanId << endl;
    cout << "Monthly Payment: $" << fixed << setprecision(2) << newLoan.getMonthlyPayment() << endl;
    cout << "Total Interest: $" << newLoan.calculateTotalInterest() << endl;
    
    // Add loan amount to account balance
    account->deposit(principal);
    cout << "\nLoan amount has been deposited to your account." << endl;
}

void BankingSystem::makeLoanPayment() {
    if (loans.empty()) {
        cout << "No loans found." << endl;
        return;
    }

    string loanId;
    double paymentAmount;

    cout << "\n=== MAKE LOAN PAYMENT ===" << endl;
    cout << "Enter Loan ID: ";
    cin >> loanId;

    LoanAccount* loan = findLoan(loanId);
    if (!loan) {
        cout << "Loan not found!" << endl;
        return;
    }

    if (!loan->getIsActive()) {
        cout << "This loan is already closed!" << endl;
        return;
    }

    cout << "Current monthly payment: $" << fixed << setprecision(2) << loan->getMonthlyPayment() << endl;
    cout << "Remaining balance: $" << loan->getRemainingBalance() << endl;
    cout << "Enter payment amount: $";
    cin >> paymentAmount;

    // Check if account has sufficient balance
    BankAccount* account = findAccount(loan->getAccountNumber());
    if (account && account->getBalance() < paymentAmount) {
        cout << "Insufficient funds in linked account!" << endl;
        cout << "Account balance: $" << account->getBalance() << endl;
        return;
    }

    // Deduct from account and make loan payment
    if (account) {
        account->withdraw(paymentAmount);
    }
    loan->makePayment(paymentAmount);
}

void BankingSystem::displayLoanInfo() {
    if (loans.empty()) {
        cout << "No loans found." << endl;
        return;
    }

    string loanId;
    cout << "\nEnter Loan ID: ";
    cin >> loanId;

    LoanAccount* loan = findLoan(loanId);
    if (loan) {
        loan->displayLoanInfo();
        
        cout << "\nWould you like to see:" << endl;
        cout << "1. Payment Schedule" << endl;
        cout << "2. Payment History" << endl;
        cout << "3. Both" << endl;
        cout << "4. None" << endl;
        cout << "Enter choice: ";
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 1: loan->displayPaymentSchedule(); break;
            case 2: loan->displayPaymentHistory(); break;
            case 3: 
                loan->displayPaymentSchedule();
                loan->displayPaymentHistory();
                break;
            default: break;
        }
    } else {
        cout << "Loan not found!" << endl;
    }
}

void BankingSystem::displayAllLoans() const {
    if (loans.empty()) {
        cout << "No loans found." << endl;
        return;
    }

    cout << "\n=== ALL LOANS SUMMARY ===" << endl;
    for (const auto& loan : loans) {
        cout << "Loan ID: " << loan.getLoanId() 
             << " | Borrower: " << loan.getBorrowerName()
             << " | Type: " << loan.getLoanType()
             << " | Balance: $" << fixed << setprecision(2) << loan.getRemainingBalance()
             << " | Status: " << (loan.getIsActive() ? "Active" : "Closed") << endl;
    }
}

void BankingSystem::displayLoanPaymentHistory() {
    if (loans.empty()) {
        cout << "No loans found." << endl;
        return;
    }

    string loanId;
    cout << "\nEnter Loan ID: ";
    cin >> loanId;

    LoanAccount* loan = findLoan(loanId);
    if (loan) {
        loan->displayPaymentHistory();
    } else {
        cout << "Loan not found!" << endl;
    }
}

LoanAccount* BankingSystem::findLoan(const string& loanId) {
    for (auto& loan : loans) {
        if (loan.getLoanId() == loanId) {
            return &loan;
        }
    }
    return nullptr;
}

string BankingSystem::generateLoanId() {
    return "LOAN" + to_string(nextLoanId++);
}

bool BankingSystem::validateAccountNumber(const string& accNum) const {
    return accNum.length() == 12;
}

bool BankingSystem::validateName(const string& name) const {
    if (name.empty()) return false;
    for (char c : name) {
        if (!isalpha(c) && c != ' ') {
            return false;
        }
    }
    return true;
}

void BankingSystem::displayMainMenu() const {
    cout << "\n========================================" << endl;
    cout << "    ENHANCED BANKING SYSTEM" << endl;
    cout << "========================================" << endl;
    cout << "1.  Create Account" << endl;
    cout << "2.  Account Operations" << endl;
    cout << "3.  Display Account Info" << endl;
    cout << "4.  Display All Accounts" << endl;
    cout << "5.  Loan Management" << endl;
    cout << "6.  Display All Loans" << endl;
    cout << "7.  Exit" << endl;
    cout << "========================================" << endl;
    cout << "Enter your choice: ";
}

void BankingSystem::displayLoanMenu() const {
    cout << "\n=== LOAN MANAGEMENT MENU ===" << endl;
    cout << "1. Apply for Loan" << endl;
    cout << "2. Make Loan Payment" << endl;
    cout << "3. View Loan Information" << endl;
    cout << "4. View Payment History" << endl;
    cout << "5. Back to Main Menu" << endl;
    cout << "Enter your choice: ";
}

void BankingSystem::run() {
    int choice;
    
    cout << "Welcome to the Enhanced Banking System with Loan Management!" << endl;
    
    do {
        displayMainMenu();
        cin >> choice;
        
        switch (choice) {
            case 1:
                createAccount();
                break;
                
            case 2: {
                string accNum;
                cout << "Enter Account Number: ";
                cin >> accNum;
                
                BankAccount* account = findAccount(accNum);
                if (!account) {
                    cout << "Account not found!" << endl;
                    break;
                }
                
                int operation;
                do {
                    cout << "\n--- Account Operations ---" << endl;
                    cout << "1. Deposit" << endl;
                    cout << "2. Withdraw" << endl;
                    cout << "3. Check Balance" << endl;
                    cout << "4. Calculate Interest/Check Overdraft" << endl;
                    cout << "5. Back to Main Menu" << endl;
                    cout << "Enter choice: ";
                    cin >> operation;
                    
                    switch (operation) {
                        case 1: {
                            double amount;
                            cout << "Enter deposit amount: $";
                            cin >> amount;
                            account->deposit(amount);
                            break;
                        }
                        case 2: {
                            double amount;
                            cout << "Enter withdrawal amount: $";
                            cin >> amount;
                            account->withdraw(amount);
                            break;
                        }
                        case 3:
                            cout << "Current Balance: $" << fixed << setprecision(2) 
                                 << account->getBalance() << endl;
                            break;
                        case 4: {
                            // Try to cast to specific account types
                            SavingsAccount* sa = dynamic_cast<SavingsAccount*>(account);
                            CheckingAccount* ca = dynamic_cast<CheckingAccount*>(account);
                            FixedDepositAccount* fa = dynamic_cast<FixedDepositAccount*>(account);
                            
                            if (sa) {
                                sa->calculateInterest();
                            } else if (ca) {
                                double amount;
                                cout << "Enter amount to check overdraft: $";
                                cin >> amount;
                                ca->checkOverdraft(amount);
                            } else if (fa) {
                                fa->calculateInterest();
                            }
                            break;
                        }
                        case 5:
                            break;
                        default:
                            cout << "Invalid choice!" << endl;
                    }
                } while (operation != 5);
                break;
            }
            
            case 3: {
                string accNum;
                cout << "Enter Account Number: ";
                cin >> accNum;
                
                BankAccount* account = findAccount(accNum);
                if (account) {
                    account->displayAccountInfo();
                } else {
                    cout << "Account not found!" << endl;
                }
                break;
            }
            
            case 4:
                displayAllAccounts();
                break;
                
            case 5: {
                int loanChoice;
                do {
                    displayLoanMenu();
                    cin >> loanChoice;
                    
                    switch (loanChoice) {
                        case 1:
                            applyForLoan();
                            break;
                        case 2:
                            makeLoanPayment();
                            break;
                        case 3:
                            displayLoanInfo();
                            break;
                        case 4:
                            displayLoanPaymentHistory();
                            break;
                        case 5:
                            break;
                        default:
                            cout << "Invalid choice!" << endl;
                    }
                } while (loanChoice != 5);
                break;
            }
            
            case 6:
                displayAllLoans();
                break;
                
            case 7:
                cout << "Thank you for using the Enhanced Banking System!" << endl;
                break;
                
            default:
                cout << "Invalid choice! Please try again." << endl;
        }
        
    } while (choice != 7);
}
