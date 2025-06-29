#include<iostream>
using namespace std;

class BankAccount{
    private:
        int acccountNumber;
        string accountHolder;
        double balance;

    public:
        BankAccount(int accNum, string name, double initialBalance){
            acccountNumber = accNum;
            accountHolder = name;
            balance = initialBalance;

        }
        void deposit(double amount){
            if(amount > 0){
                balance += amount;
                cout << "Deposit successful. New balance." << balance << endl;
            }else{
                cout << "Invalid deposit account." << endl;
            }
        }
        void withdraw(double amount){
            if(amount > 0 && amount <= balance){
                balance -= amount;
                cout << "Withdrawal successful. New balance." << balance << endl;
            }else{
                cout << "Invaild insufficient funds for withdrawal." << endl;
            }
        }
        double getBalance() {
            return balance;
        }
        void display(){
            cout << endl << "------Account Details------" << endl;
            cout << "Account Number :- " << acccountNumber << endl;
            cout << "Account Holder :- " << accountHolder << endl;
            cout << "Balance :- " << balance << endl;
        }
};

class SavingAccount : public BankAccount{
    private:
        double interestRate;
    public:
    
        SavingAccount(int accNum , string name ,double initialBalance ,double rate = 7.7) : BankAccount(accNum, name, initialBalance){
          interestRate = rate;  
        }

        void calculateInterest(){
            double interest = getBalance() * interestRate / 100;
            cout << "Saving Interest :- " << interest << endl;
        }

        // int interest;
        // double interestRate = 0.7;
        // int duration = 12;
        // void calculateInterest(double amount ,double interestRate, int duration){
        //     interest = (amount * duration * interestRate)/100;
        // }
};

class CheckingAccount : public BankAccount{

    private:
        int overdraftLimit;

    public:
        CheckingAccount(int accNum , string name ,double initialBalance) : BankAccount(accNum, name, initialBalance){ 
            overdraftLimit = 1000;
        }

        void withdraw(double amount){
            double totalAccessible = getBalance() + overdraftLimit;
            int tA = (amount > totalAccessible) ? 1 : 0;

            switch (tA){
                case 1:
                    BankAccount::withdraw(amount);
                    break;
                case 2:
                    cout << "Oops! You've gone over your overdraft limit. Withdrawal denied." << endl;
                    break;
                default:
                    break;
            }
        }
        
        void checkOverdraft(double amount){
            double totalAccessible = getBalance() + overdraftLimit;
            cout << "Total Accessible Amount (with Overdraft):" << totalAccessible << endl;

            int tA = (amount > totalAccessible) ? 1 : 0;

            switch (tA){
                case 1:
                    cout << "Withdrawal allowed." << endl;
                    break;
                case 2:
                    cout << "Overdraft exceeded." << endl;
                    break;
                default:
                    break;
            }
        }
};

class FixedDepositAccount : public BankAccount{
    private:
        int term;

    public: 
        FixedDepositAccount(int accNum , string name ,double initialBalance, int tM = 0) :  BankAccount(accNum, name, initialBalance){
            term = tM ;
        }
        void clculateInterrst(){
            double interest = getBalance() * 7.0 * term /1200.0;
            cout << "Interest on Fixed Deposit: " << interest << " for " << term << " months." << endl;
        }
};

int main(){

    int accNum;
    string name;
    double initialBalance;

    cout << "Enter the Account Number :- ";
    cin >> accNum;
    cout << "Enter Account Holder Name :- ";
    cin >> name;
    cout << "Enter initial balance :- ";
    cin >> initialBalance;

    BankAccount myAccount(accNum, name, initialBalance);

    int choice;
    double amount;

    do{
        cout << endl << "------Menu------" << endl;
        cout << "1. Deposit."  << endl;
        cout << "2. Withdraw." << endl;
        cout << "3. Display Account Details." << endl;
        cout << "4. Exit." << endl;
        cout << "Enter the your choice :- ";
        cin >> choice;

    switch(choice){
        case 1:
            cout << "Enter Amount to deposit :- " ;
            cin >> amount;
            myAccount.deposit(amount);
            break;
        case 2:
            cout << "Enter Amount to withdraw :- " ;
            cin >> amount;
            myAccount.withdraw(amount);
            break;
        case 3:
            myAccount.display();
            break;
        case 4:
            cout << "Thank you for using tha Bank system." << endl;
            break;
        default :
            cout << endl << "Please enter a valid choice." << endl;
            break;
    }

    }while(choice != 4); 

    return 0;
}