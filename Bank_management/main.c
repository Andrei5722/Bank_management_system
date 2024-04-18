#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

#define MAX_ACCOUNTS 100
#define MAX_NAME_LENGTH 50
#define MAX_SURNAME_LENGTH 50
#define MAX_IBAN_LENGTH 30
#define MAX_PASSWORD_LENGTH 20

// Define currencies
typedef enum {
    RON,
    EUR,
    USD
} Currency;

// Structure for a person
typedef struct {
    char name[MAX_NAME_LENGTH];
    char surname[MAX_SURNAME_LENGTH];
} Person;

// Structure for an account
typedef struct {
    char iban[MAX_IBAN_LENGTH];
    Person owner;
    Currency coin;
    double amount;
    char password[MAX_PASSWORD_LENGTH];
} Account;

// Global array to store accounts
Account accounts[MAX_ACCOUNTS];
int numAccounts = 0;

// Function prototypes
void login(const char *name, const char *surname);
void logout();
void createAccount();
void editAccount(Account *account);
void deleteAccount(Account *account);
void viewAccount(Account *account);
void performTransaction(Account *source, Account *destination, double amount);
void generateRandomIBAN(char *iban);
double convertCurrency(double amount, Currency from, Currency to);
void saveAccountsToFile(const char *filename);
void loadAccountsFromFile(const char *filename);

int main(int argc, char *argv[]) {
    // Seed random number generator
    srand(time(NULL));

    if (argc > 1 && strcmp(argv[1], "login") == 0 && argc == 4) {
        // Command line login
        login(argv[2], argv[3]);
    } else {
        // Interactive menu
        int choice;
        do {
            printf("Welcome to the Bank Management System\n");
            printf("1. Log In\n2. Create Account\n3. Exit\n");
            scanf("%d", &choice);
            loadAccountsFromFile("accounts.txt");

            switch (choice) {
                case 1:
                    printf("Enter your name and surname: ");
                    char name[50], surname[50];
                    scanf("%s %s", name, surname);
                    login(name, surname);
                    break;
                case 2:
                    createAccount();
                    break;
                case 3:
                    printf("Exiting...\n");
                    saveAccountsToFile("accounts.txt");
                    break;
                default:
                    printf("Invalid choice\n");
            }
        } while (choice != 3);
    }

    return 0;
}

// Function to save accounts to a CSV file
// Function to save accounts to a text file
void saveAccountsToFile(const char *filename) {
    printf("%s", filename);
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    fprintf(file, "IBAN,Name,Surname,Coin,Amount,Password\n");
    printf("%d", numAccounts);
    for (int i = 0; i < numAccounts; i++) {
        fprintf(file, "%s,%s,%s,%d,%.2f,%s\n", accounts[i].iban, accounts[i].owner.name,
                accounts[i].owner.surname, (int)accounts[i].coin, accounts[i].amount, accounts[i].password);

    }

    fclose(file);
    printf("Accounts saved to %s\n", filename);
}

// Function to load accounts from a CSV file
void loadAccountsFromFile(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    char line[256]; // Assuming a maximum line length of 256 characters
    fgets(line, sizeof(line), file); // Read and discard the header line

    while (fgets(line, sizeof(line), file)) {
        char iban[MAX_IBAN_LENGTH], name[MAX_NAME_LENGTH], surname[MAX_SURNAME_LENGTH];
        int coin;
        double amount;
        char password[MAX_PASSWORD_LENGTH];

        scanf(line, "%[^,],%[^,],%[^,],%d,%lf,%s\n", iban, name, surname, &coin, &amount, password);

        strcpy(accounts[numAccounts].iban, iban);
        strcpy(accounts[numAccounts].owner.name, name);
        strcpy(accounts[numAccounts].owner.surname, surname);
        accounts[numAccounts].coin = (Currency)coin;
        accounts[numAccounts].amount = amount;
        strcpy(accounts[numAccounts].password, password);

        numAccounts++;
    }

    fclose(file);
    printf("Accounts loaded from %s\n", filename);
}

// Function to log in
void login(const char *name, const char *surname) {
    char password[MAX_PASSWORD_LENGTH];

    printf("Enter password: ");
    scanf("%s", password);

    // Search for accounts owned by the person
    for (int i = 0; i < numAccounts; i++) {
        if (strcmp(accounts[i].owner.name, name) == 0 &&
            strcmp(accounts[i].owner.surname, surname) == 0 &&
            strcmp(accounts[i].password, password) == 0) {
            // Logged in successfully
            printf("Logged in as %s %s\n", name, surname);
            int option;
            do {
                printf("1. Edit Account\n2. Delete Account\n3. View Account\n4. Perform Transaction\n5. Log Out\n");
                scanf("%d", &option);
                switch (option) {
                    case 1:
                        editAccount(&accounts[i]);
                        break;
                    case 2:
                        deleteAccount(&accounts[i]);
                        break;
                    case 3:
                        viewAccount(&accounts[i]);
                        break;
                    case 4:
                        // Simulate transaction
                        performTransaction(&accounts[i], &accounts[(i + 1) % numAccounts], 100.0); // Example transaction
                        break;
                    case 5:
                        logout();
                        break;
                    default:
                        printf("Invalid option\n");
                }
            } while (option != 5);
            return;
        }
    }
    printf("Login failed. Please check your credentials.\n");
}


// Function to log out
void logout() {
    printf("Logged out\n");
}

// Function to create an account
void createAccount() {
    char name[MAX_NAME_LENGTH];
    char surname[MAX_SURNAME_LENGTH];
    char password[MAX_PASSWORD_LENGTH];
    int coinChoice;
    double amount;

    printf("Enter name: ");
    scanf("%s", name);
    printf("Enter surname: ");
    scanf("%s", surname);
    printf("Enter password: ");
    scanf("%s", password);
    printf("Select currency (1. RON, 2. EUR, 3. USD): ");
    scanf("%d", &coinChoice);
    printf("Enter initial amount: ");
    scanf("%lf", &amount);

    Currency coin;
    switch (coinChoice) {
        case 1:
            coin = RON;
            break;
        case 2:
            coin = EUR;
            break;
        case 3:
            coin = USD;
            break;
        default:
            printf("Invalid choice, defaulting to RON.\n");
            coin = RON;
    }

    if (numAccounts >= MAX_ACCOUNTS) {
        printf("Cannot create more accounts, database is full.\n");
        return;
    }

    Account newAccount;
    generateRandomIBAN(newAccount.iban);
    strcpy(newAccount.owner.name, name);
    strcpy(newAccount.owner.surname, surname);
    strcpy(newAccount.password, password);
    newAccount.coin = coin;
    newAccount.amount = amount;
    accounts[numAccounts++] = newAccount;  // Only increment once
}


// Function to edit an account
void editAccount(Account *account) {
    printf("Editing account with IBAN: %s\n", account->iban);
    // Add editing logic here
    printf("Enter new amount: ");
    scanf("%lf", &account->amount);
}

// Function to delete an account
void deleteAccount(Account *account) {
    printf("Deleting account with IBAN: %s\n", account->iban);
    // Add deletion logic here
    // For simplicity, we just mark the account as deleted by setting its amount to zero
    account->amount = 0;
}

// Function to view an account
void viewAccount(Account *account) {
    printf("Viewing account with IBAN: %s\n", account->iban);
    printf("Owner: %s %s\n", account->owner.name, account->owner.surname);
    printf("Amount: %.2f %s\n", account->amount, account->coin == RON ? "RON" : account->coin == EUR ? "EUR" : "USD");
}

// Function to perform a transaction
void performTransaction(Account *source, Account *destination, double amount) {
    if (source->amount >= amount) {
        // Convert currency if necessary
        double convertedAmount = convertCurrency(amount, source->coin, destination->coin);

        source->amount -= amount;
        destination->amount += convertedAmount;
        printf("Transaction successful\n");
    } else {
        printf("Insufficient funds for transaction\n");
    }
}

// Function to generate a random IBAN
void generateRandomIBAN(char *iban) {
    const char *chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (int i = 0; i < MAX_IBAN_LENGTH - 1; i++) {
        iban[i] = chars[rand() % (strlen(chars))];
    }
    iban[MAX_IBAN_LENGTH - 1] = '\0'; // Null-terminate the string
}

// Function to convert between currencies
double convertCurrency(double amount, Currency from, Currency to) {
    // Conversion rates are assumed here, you can adjust them as needed
    const double RON_TO_EUR = 0.21;
    const double RON_TO_USD = 0.24;
    const double EUR_TO_RON = 4.80;
    const double EUR_TO_USD = 1.18;
    const double USD_TO_RON = 4.16;
    const double USD_TO_EUR = 0.85;

    switch (from) {
        case RON:
            switch (to) {
                case EUR: return amount * RON_TO_EUR;
                case USD: return amount * RON_TO_USD;
                default: return amount;
            }
        case EUR:
            switch (to) {
                case RON: return amount * EUR_TO_RON;
                case USD: return amount * EUR_TO_USD;
                default: return amount;
            }
        case USD:
            switch (to) {
                case RON: return amount * USD_TO_RON;
                case EUR: return amount * USD_TO_EUR;
                default: return amount;
            }
        default: return amount;
    }


}
