#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ACCOUNTS 100
#define DATA_FILE "bank_data.txt"

// Define constants for currencies
#define RON 0
#define EUR 1
#define USD 2

// Structure for person
typedef struct {
    char name[50];
    char surname[50];
} Person;

// Structure for account
typedef struct {
    char iban[13];
    Person owner;
    int currency;
    float amount;
} Account;

// Global array to store accounts
Account accounts[MAX_ACCOUNTS];
int numAccounts = 0;

// Function prototypes
void readDataFromFile();
void writeDataToFile();
void login(char *name, char *surname);
void editAccount(Account *account);
void deleteAccount(Account *account);
void viewAccount(Account *account);
void createAccount(char *name, char *surname);

int main(int argc, char *argv[]) {
    // Read data from file
    readDataFromFile();

    if (argc == 3 && strcmp(argv[1], "admin") == 0 && strcmp(argv[2], "admin") == 0) {
        // If username and password are "admin", prompt for name and surname and create account
        char name[50], surname[50];
        printf("Enter your name: ");
        scanf("%s", name);
        printf("Enter your surname: ");
        scanf("%s", surname);
        createAccount(name, surname);
    } else if (argc != 3) {
        printf("Usage: %s [NAME] [SURNAME]\n", argv[0]);
        return 1;
    } else {
        // Login
        login(argv[1], argv[2]);
    }

    // Write data back to file
    writeDataToFile();

    return 0;
}

void readDataFromFile() {
    FILE *file = fopen(DATA_FILE, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    while (fscanf(file, "%s %s %s %d %f", accounts[numAccounts].iban, accounts[numAccounts].owner.name,
                  accounts[numAccounts].owner.surname, &accounts[numAccounts].currency,
                  &accounts[numAccounts].amount) != EOF) {
        numAccounts++;
    }

    fclose(file);
}

void writeDataToFile() {
    FILE *file = fopen(DATA_FILE, "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    for (int i = 0; i < numAccounts; i++) {
        fprintf(file, "%s %s %s %d %.2f\n", accounts[i].iban, accounts[i].owner.name,
                accounts[i].owner.surname, accounts[i].currency, accounts[i].amount);
    }

    fclose(file);
}

void login(char *name, char *surname) {
    // Check if the person exists and gather their accounts
    int found = 0;
    for (int i = 0; i < numAccounts; i++) {
        if (strcmp(accounts[i].owner.name, name) == 0 && strcmp(accounts[i].owner.surname, surname) == 0) {
            found = 1;
            int choice;
            do {
                printf("Logged in as %s %s\n", name, surname);

                printf("\nMenu:\n");
                printf("1. Edit Account\n");
                printf("2. Delete Account\n");
                printf("3. View Account\n");
                printf("4. Perform Transaction\n");
                printf("5. Create Account\n"); // Added option to create account
                printf("6. Exit\n"); // Adjusted exit option
                printf("Enter your choice: ");
                scanf("%d", &choice);

                system("clear");
                switch (choice) {
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
                        // Implement transaction functionality
                        break;
                    case 5:
                        createAccount(name, surname); // Call createAccount function
                        writeDataToFile();
                        break;
                    case 6:
                        printf("Exiting...\n");
                        break;
                    default:
                        printf("Invalid choice. Please try again.\n");
                }
            } while (choice != 6);

            break;
        }
    }
    if (!found) {
        printf("Person not found.\n");
    }
}

void editAccount(Account *account) {
    int choice;
    printf("\nEdit Account:\n");
    printf("1. Modify IBAN\n");
    printf("2. Modify Amount\n");
    printf("3. Modify Currency\n");
    printf("4. Back to Menu\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            printf("Enter new IBAN: ");
            scanf("%s", account->iban);
            printf("IBAN modified successfully.\n");
            break;
        case 2:
            printf("Enter new amount: ");
            scanf("%f", &account->amount);
            printf("Amount modified successfully.\n");
            break;
        case 3:
            printf("Enter new currency (0 for RON, 1 for EUR, 2 for USD): ");
            scanf("%d", &account->currency);
            printf("Currency modified successfully.\n");
            break;
        case 4:
            printf("Returning to Menu...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
    }
}

void deleteAccount(Account *account) {
    // Implement functionality to delete account
    printf("Account deleted.\n");
}

void viewAccount(Account *account) {
    printf("\nAccount Details:\n");
    printf("IBAN: %s\n", account->iban);
    printf("Owner: %s %s\n", account->owner.name, account->owner.surname);
    printf("Currency: %d\n", account->currency);
    printf("Amount: %.2f\n", account->amount);
}

void performTransaction(Account *sourceAccount, Account *destAccount, float amount) {
    // Implement functionality to perform transaction
}

char generated_iban_string(){
    char generated_iban[13];

    for (int i = 0; i < 12; i++) {
        generated_iban[i] = 'A' + rand() % 26; // Assuming you want uppercase letters
    }
    generated_iban[12] = '\0';
    return *generated_iban;

}

void createAccount(char *name, char *surname) {
    char generated_iban[13]; // Assuming IBAN length is 12 characters plus '\0'
    int iban_exists;

    do {
        iban_exists = 0; // Flag to check if generated IBAN already exists
        generated_iban_string(generated_iban); // Assuming this function generates a random IBAN
        
        // Check if generated IBAN already exists
        for (int i = 0; i < numAccounts; ++i) {
            if (strcmp(accounts[i].iban, generated_iban) == 0) {
                iban_exists = 1; // Set flag if IBAN already exists
                break;
            }
        }
    } while (iban_exists); // Repeat generation if IBAN already exists

    printf("\t%s\n", generated_iban);

    // Copy generated IBAN to the new account
    strcpy(accounts[numAccounts].iban, generated_iban);
    // Copy name and surname to the new account
    strcpy(accounts[numAccounts].owner.name, name);
    strcpy(accounts[numAccounts].owner.surname, surname);

    // Input currency and initial amount for the new account
    printf("Enter currency (0 for RON, 1 for EUR, 2 for USD): ");
    scanf("%d", &accounts[numAccounts].currency);
    printf("Enter initial amount: ");
    scanf("%f", &accounts[numAccounts].amount);

    numAccounts++; // Increment number of accounts
    printf("Account created successfully.\n");
}
