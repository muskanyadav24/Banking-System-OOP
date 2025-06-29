#include <iostream>
using namespace std;

class BankAccount {
private:
    string accountNumber;
    string accountHolderName;
    double balance;

public:
    BankAccount(string accNum = "", string holderName = "", double bal = 0.0) {
        accountNumber = accNum;
        accountHolderName = holderName;
        balance = bal;
    }

    void deposit(double amount) {
        balance += amount;
        cout << "Deposited: " << amount << ", New Balance: " << balance << endl;
    }

    void withdraw(double amount) {
        balance -= amount;
        cout << "Withdrew: " << amount << ", New Balance: " << balance << endl;
    }

    double getBalance() {
        return balance;
    }

    void displayAccountInfo() {
        cout << "Account Number: " << accountNumber << endl;
        cout << "Holder Name: " << accountHolderName << endl;
        cout << "Balance: " << balance << endl;
    }
};

class SavingsAccount : public BankAccount {
private:
    double interestRate;

public:
    SavingsAccount(string accNum = "", string holderName = "", double bal = 0.0, double rate = 5.0)
        : BankAccount(accNum, holderName, bal) {
        interestRate = rate;
    }

    void calculateInterest() {
        double interest = getBalance() * interestRate / 100;
        cout << "Savings Interest: " << interest << endl;
    }
};

class CheckingAccount : public BankAccount {
    private:
        double overdraftLimit;

    public:
        CheckingAccount(string accNum = "", string holderName = "", double bal = 0.0)
            : BankAccount(accNum, holderName, bal) {
            overdraftLimit = 1000;
        }

        void withdraw(double amount) {
            double totalAvailable = getBalance() + overdraftLimit;
            int flag = (amount > totalAvailable) ? 1 : 0;

            switch (flag) {
            case 0:
                BankAccount::withdraw(amount);
                break;
            case 1:
                cout << "Overdraft limit exceeded. Withdrawal denied." << endl;
                break;
            }
        }
        void checkOverdraft(double amount) {
            double totalAvailable = getBalance() + overdraftLimit;
            cout << "Total Available (with overdraft): " << totalAvailable << endl;

            int flag = (amount > totalAvailable) ? 1 : 0;
            switch (flag) {
            case 0:
                cout << "Withdrawal allowed." << endl;
                break;
            case 1:
                cout << "Overdraft exceeded." << endl;
                break;
            }
        }
        void displayAccountInfo() {
            BankAccount::displayAccountInfo();
            cout << "Overdraft Limit: " << overdraftLimit << endl;
        }
};


class FixedDepositAccount : public BankAccount {
private:
    int term;

public:
    FixedDepositAccount(string accNum = "", string holderName = "", double bal = 0.0, int t = 0)
        : BankAccount(accNum, holderName, bal) {
        term = t;
    }

    void calculateInterest() {
        double interest = getBalance() * 7.0 * term / 1200.0;
        cout << "Fixed Deposit Interest: " << interest << " for " << term << " months." << endl;
    }
};

int main() {
    int accType;
    string accNum, name;
    double balance;

    SavingsAccount sa;
    CheckingAccount ca;
    FixedDepositAccount fa;

    cout << "Choose Account Type to Create:" << endl;
    cout << "1. Savings" << endl;
    cout << "2. Checking" << endl;
    cout << "3. Fixed Deposit" << endl;
    cout << "Choose a number and enter :- ";
    cin >> accType;

    cin.ignore();

    do {
        cout << "Enter Account Number (12 characters): ";
        getline(cin, accNum);
        if (accNum.length() != 12) {
            cout << "Account number must be exactly 12 characters." << endl;
        }
    } while (accNum.length() != 12);

    int valid = 0;
    do {
        cout << "Enter Holder Name: ";
        getline(cin, name);
        valid = 1;
        for (int i = 0; i < name.length(); ++i) {
            if (!isalpha(name[i])) {
                cout << "Name should contain only alphabets." << endl;
                valid = 0;
                break;
            }
        }
    } while (!valid);

    do {
        cout << "Enter Initial Balance: ";
        cin >> balance;
        if (balance < 0) {
            cout << "Balance cannot be negative." << endl;
        }
    } while (balance < 0);

    switch (accType) {
    case 1: {
        double rate;
        cout << "Enter Interest Rate: ";
        cin >> rate;
        sa = SavingsAccount(accNum, name, balance, rate);
        break;
    }
    case 2: {
        ca = CheckingAccount(accNum, name, balance);
        cout << "Overdraft limit set to 1000 by default." << endl;
        break;
    }
    case 3: {
        int term;
        cout << "Enter Term in Months: ";
        cin >> term;
        fa = FixedDepositAccount(accNum, name, balance, term);
        break;
    }
    default:
        cout << "Please enter a valid choice." << endl;
        return 0;
    }

    int choice;
    do {
        cout << endl << "--- Menu ---" << endl;
        cout << "1. Deposit" << endl;
        cout << "2. Withdraw" << endl;
        cout << "3. Balance" << endl;
        cout << "4. Info" << endl;
        cout << "5. Interest/Overdraft" << endl;
        cout << "6. Exit" << endl;
        cout << "Enter the your choice :- ";
        cin >> choice;

        switch (choice) {
        case 1: {
            double amt;
            cout << "Enter amount to deposit: ";
            cin >> amt;
            switch (accType) {
            case 1: sa.deposit(amt); break;
            case 2: ca.deposit(amt); break;
            case 3: fa.deposit(amt); break;
            }
            break;
        }
        case 2: {
            double amt;
            cout << "Enter amount to withdraw: ";
            cin >> amt;
            switch (accType) {
            case 1: sa.withdraw(amt); break;
            case 2: ca.withdraw(amt); break;
            case 3: fa.withdraw(amt); break;
            }
            break;
        }
        case 3:
            switch (accType) {
            case 1: cout << "Balance: " << sa.getBalance() << endl; break;
            case 2: cout << "Balance: " << ca.getBalance() << endl; break;
            case 3: cout << "Balance: " << fa.getBalance() << endl; break;
            }
            break;
        case 4:
            switch (accType) {
            case 1: sa.displayAccountInfo(); break;
            case 2: ca.displayAccountInfo(); break;
            case 3: fa.displayAccountInfo(); break;
            }
            break;
        case 5:
            switch (accType) {
            case 1: sa.calculateInterest(); break;
            case 2: {
                double amt;
                cout << "Enter amount to check overdraft: ";
                cin >> amt;
                ca.checkOverdraft(amt);
                break;
            }
            case 3: fa.calculateInterest(); break;
            }
            break;
        case 6:
            cout << "Thank you for using tha Bank system.!" << endl;
            break;
        default:
            cout << "Please enter a valid choice." << endl;
        }
    } while (choice != 6);
    return 0;
}