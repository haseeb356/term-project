#include <iostream>
#include <cstring>
#include <cctype>
#include <cstdlib>
using namespace std;


struct Account {
    string accountNumber;
    string password;
    char name[50];
    char accountType[20];
    double balance;
    char transactionHistory[100][100];
    int transactionCount;
};

// Functions prototypes
void createAccount(Account accounts[], int &accountCount, int MAX_ACCOUNTS);
int login(Account accounts[], int accountCount);
void deposit(int index, Account accounts[], int MAX_TRANSACTIONS);
void withdraw(int index, Account accounts[], int MAX_TRANSACTIONS);
void checkBalance(int index, Account accounts[]);
void viewTransactionHistory(int index, Account accounts[]);
bool validateAccountNumber(string accNum);
bool isPasswordStrong(string password);
int findAccountIndex(string accNum, Account accounts[], int accountCount);

int main() {
    int MAX_ACCOUNTS, MAX_TRANSACTIONS;

    cout << "Enter the maximum number of accounts: ";
    cin >> MAX_ACCOUNTS;
    cout << "Enter the maximum number of transactions per account: ";
    cin >> MAX_TRANSACTIONS;

    // Array of Account structures
    Account accounts[MAX_ACCOUNTS];
    int accountCount = 0;

    int option;
    do {
        cout << "\n--- Bank Management System ---\n";
        cout << "1. Login\n";
        cout << "2. Create Account\n";
        cout << "3. Deposit\n";
        cout << "4. Withdraw\n";
        cout << "5. Check Balance\n";
        cout << "6. View Transaction History\n";
        cout << "7. Exit\n";
        cout << "Select an option: ";
        cin >> option;

        switch (option) {
            case 1: {
                int accountIndex = login(accounts, accountCount);
                if (accountIndex != -1) {
                    cout << "Welcome, " << accounts[accountIndex].name << "!\n";
                }
                break;
            }
            case 2:
                createAccount(accounts, accountCount, MAX_ACCOUNTS);
                break;
            case 3: {
                int accountIndex = login(accounts, accountCount);
                if (accountIndex != -1) {
                    deposit(accountIndex, accounts, MAX_TRANSACTIONS);
                }
                break;
            }
            case 4: {
                int accountIndex = login(accounts, accountCount);
                if (accountIndex != -1) {
                    withdraw(accountIndex, accounts, MAX_TRANSACTIONS);
                }
                break;
            }
            case 5: {
                int accountIndex = login(accounts, accountCount);
                if (accountIndex != -1) {
                    checkBalance(accountIndex, accounts);
                }
                break;
            }
            case 6: {
                int accountIndex = login(accounts, accountCount);
                if (accountIndex != -1) {
                    viewTransactionHistory(accountIndex, accounts);
                }
                break;
            }
            case 7:
                cout << "Goodbye!\n";
                return 0;
            default:
                cout << "Invalid option. Please try again.\n";
        }
    } while (true);
}

bool validateAccountNumber(string accNum) {
    if (accNum.length() != 6)
        return false;
    for (char c : accNum) {
        if (!isdigit(c))
            return false;
    }
    return true;
}

bool isPasswordStrong(string password) {
    bool hasUpper = false, hasLower = false, hasDigit = false;
    if (password.length() < 8 || password.length() > 10)
        return false;
    for (char c : password) {
        if (isupper(c)) hasUpper = true;
        if (islower(c)) hasLower = true;
        if (isdigit(c)) hasDigit = true;
    }
    return hasUpper && hasLower && hasDigit;
}

int findAccountIndex(string accNum, Account accounts[], int accountCount) {
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i].accountNumber == accNum)
            return i;
    }
    return -1;
}

void createAccount(Account accounts[], int &accountCount, int MAX_ACCOUNTS) {
    if (accountCount >= MAX_ACCOUNTS) {
        cout << "Bank is at full capacity. Cannot create more accounts.\n";
        return;
    }

    string accNum, password, confirmPassword;
    char name[50], accType[20];
    double initialDeposit;

    cout << "Enter Account Number (6 digits): ";
    cin >> accNum;

    if (!validateAccountNumber(accNum) || findAccountIndex(accNum, accounts, accountCount) != -1) {
        cout << "Invalid or existing account number. Please try again.\n";
        return;
    }

    cout << "Enter Password (8-10 chars, including uppercase, lowercase, digit): ";
    cin >> password;
    if (!isPasswordStrong(password)) {
        cout << "Weak password. Please follow the password policy.\n";
        return;
    }

    cout << "Confirm Password: ";
    cin >> confirmPassword;
    if (password != confirmPassword) {
        cout << "Passwords do not match.\n";
        return;
    }

    cout << "Enter Name: ";
    cin.ignore();
    cin.getline(name, 50);

    cout << "Enter Account Type (Savings/Checking): ";
    cin.getline(accType, 20);

    cout << "Enter Initial Deposit (min $100): ";
    cin >> initialDeposit;
    if (initialDeposit < 100) {
        cout << "Minimum deposit is $100.\n";
        return;
    }

    accounts[accountCount].accountNumber = accNum;
    accounts[accountCount].password = password;
    strcpy(accounts[accountCount].name, name);
    strcpy(accounts[accountCount].accountType, accType);
    accounts[accountCount].balance = initialDeposit;
    accounts[accountCount].transactionCount = 0;

    cout << "Account created successfully! Your account number is: " << accNum << "\n";
    accountCount++;
}

int login(Account accounts[], int accountCount) {
    string accNum, password;
    int attempts = 3;

    while (attempts--) {
        cout << "Enter Account Number: ";
        cin >> accNum;

        cout << "Enter Password: ";
        cin >> password;

        int index = findAccountIndex(accNum, accounts, accountCount);

        if (index != -1 && accounts[index].password == password) {
            cout << "Login successful!\n";
            return index;
        } else {
            cout << "Invalid account number or password. Attempts left: " << attempts << "\n";
        }
    }
    cout << "Account locked due to too many failed attempts.\n";
    return -1;
}

void deposit(int index, Account accounts[], int MAX_TRANSACTIONS) {
    double amount;
    cout << "Enter deposit amount: ";
    cin >> amount;

    if (amount <= 0) {
        cout << "Deposit amount must be greater than 0.\n";
        return;
    }

    accounts[index].balance += amount;

    // Record the transaction
    if (accounts[index].transactionCount < MAX_TRANSACTIONS) {
        snprintf(accounts[index].transactionHistory[accounts[index].transactionCount++], 100, "Deposited: $%.2f", amount);
    }
    cout << "Deposit successful! New balance: $" << accounts[index].balance << "\n";
}

void withdraw(int index, Account accounts[], int MAX_TRANSACTIONS) {
    double amount;
    cout << "Enter withdrawal amount: ";
    cin >> amount;

    if (amount <= 0 || amount > accounts[index].balance) {
        cout << "Invalid withdrawal amount.\n";
        return;
    }

    accounts[index].balance -= amount;

    // Record the transaction
    if (accounts[index].transactionCount < MAX_TRANSACTIONS) {
        snprintf(accounts[index].transactionHistory[accounts[index].transactionCount++], 100, "Withdrew: $%.2f", amount);
    }
    cout << "Withdrawal successful! New balance: $" << accounts[index].balance << "\n";
}

void checkBalance(int index, Account accounts[]) {
    cout << "Account Balance: $" << accounts[index].balance << "\n";
}

void viewTransactionHistory(int index, Account accounts[]) {
    cout << "Transaction History:\n";
    for (int i = 0; i < accounts[index].transactionCount; i++) {
        cout << accounts[index].transactionHistory[i] << "\n";
    }
}
