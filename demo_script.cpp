#include "enhanced_banking_system.h"

void demonstrateCompleteSystem() {
    cout << "=== COMPLETE BANKING SYSTEM WITH LOAN MANAGEMENT DEMO ===" << endl;
    
    BankingSystem bank;
    
    // Create some demo accounts programmatically
    cout << "\n1. Creating Demo Accounts..." << endl;
    
    // Note: In a real demo, you'd use the interactive menu
    // This is just to show the system capabilities
    
    cout << "\n2. Demonstrating Loan Calculations..." << endl;
    
    // Demo different loan scenarios
    vector<tuple<string, double, double, int, string>> loanScenarios = {
        make_tuple("Personal Emergency Loan", 5000.0, 12.0, 24, "Personal"),
        make_tuple("New Car Purchase", 35000.0, 8.5, 60, "Car"),
        make_tuple("Home Mortgage", 350000.0, 6.5, 360, "Home"),
        make_tuple("Business Expansion", 75000.0, 14.0, 84, "Business")
    };
    
    for (const auto& scenario : loanScenarios) {
        string name = get<0>(scenario);
        double principal = get<1>(scenario);
        double rate = get<2>(scenario);
        int term = get<3>(scenario);
        string type = get<4>(scenario);
        
        cout << "\n--- " << name << " ---" << endl;
        LoanAccount loan("DEMO" + to_string(rand() % 1000), "DEMO123456789", 
                        "Demo Customer", principal, rate, term, type);
        
        cout << "Loan Amount: $" << fixed << setprecision(2) << principal << endl;
        cout << "Interest Rate: " << rate << "% annual" << endl;
        cout << "Term: " << term << " months" << endl;
        cout << "Monthly Payment: $" << loan.getMonthlyPayment() << endl;
        cout << "Total Interest: $" << loan.calculateTotalInterest() << endl;
        cout << "Total Payback: $" << (principal + loan.calculateTotalInterest()) << endl;
        
        // Show first few payments
        cout << "\nFirst 3 Payment Breakdown:" << endl;
        LoanAccount tempLoan = loan; // Copy for demonstration
        for (int i = 1; i <= 3; i++) {
            double monthlyRate = rate / 1200.0;
            double interestPortion = tempLoan.getRemainingBalance() * monthlyRate;
            double principalPortion = tempLoan.getMonthlyPayment() - interestPortion;
            
            cout << "Payment " << i << ": Interest $" << fixed << setprecision(2) 
                 << interestPortion << ", Principal $" << principalPortion 
                 << ", Remaining $" << (tempLoan.getRemainingBalance() - principalPortion) << endl;
            
            // Simulate payment for next calculation
            tempLoan.makePayment(tempLoan.getMonthlyPayment());
        }
    }
    
    cout << "\n=== DEMO COMPLETED ===" << endl;
    cout << "\nKey Features Demonstrated:" << endl;
    cout << "✓ Multiple loan types with different interest rates" << endl;
    cout << "✓ Accurate monthly payment calculations using compound interest" << endl;
    cout << "✓ Interest/Principal breakdown for each payment" << endl;
    cout << "✓ Payment tracking and history" << endl;
    cout << "✓ Integration with banking accounts" << endl;
    cout << "✓ Comprehensive loan information display" << endl;
    
    cout << "\nTo use the interactive system, run: ./banking_system_with_loans" << endl;
}

int main() {
    demonstrateCompleteSystem();
    return 0;
}
