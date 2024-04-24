#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/file_handling.h"
#include "../include/accounts.h"

void login(char *name, char *surname);
void editAccount(Account *account);
void deleteAccount(Account *accounts, int *accnum);

int main(int argc, char *argv[]) {
    readDataFromFile();

    if (argc == 3 && strcmp(argv[1], "admin") == 0 && strcmp(argv[2], "admin") == 0) {
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

    writeDataToFile();


    return 0;
}

void login(char *name, char *surname) {
    int found = 0;
    for (int i = 0; i < accnum; i++) {
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
                printf("5. Create Account\n"); 
                printf("6. Exit\n"); 
                printf("Enter your choice: ");
                scanf("%d", &choice);

                system("clear");
                switch (choice) {
                    case 1:
                        editAccount(&accounts[i]);
                        break;
                    case 2:
                        deleteAccount(accounts, &accnum);
                        break;
                    case 3:
                        viewAccount(&accounts[i]);
                        break;
                    case 4:
                        performTransaction(accounts, accnum);
                        break;
                    case 5:
                        createAccount(name, surname);
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
            char sure_choice;
            int iban_exists = 0;
            char *generated_iban;
            printf("Are you sure you want to generate a new IBAN ? y/n\n");
            scanf(" %c", &sure_choice);
            if (sure_choice == 'y') {
                char *new_iban = generate_random_iban();
                do {
                    iban_exists = 0;
                    generated_iban = generate_random_iban();

                    for (int i = 0; i < accnum; ++i) {
                        if (strcmp(accounts[i].iban, generated_iban) == 0) {
                            iban_exists = 1;
                            break;
                        }
                    }
                } while (iban_exists); 
                strcpy(account->iban, generated_iban);
            }else {
                printf("IBAN change canceled...\n");
                break;
            }
            printf("IBAN modified successfully.\nNew Iban is: %s\n", generated_iban);
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

void deleteAccount(Account *accounts, int *accnum) {
    int accountIndex = -1;
    char delete_acc_on_iban[IBAN_LENGTH+1];
    printf("enter iban of account you want to delete: \n");
    scanf("%s", delete_acc_on_iban);
    int flag_for_found_iban = 0;

    for (int i = 0; i < *accnum; i++) {
        if (strcmp(accounts[i].iban, delete_acc_on_iban) == 0) {
            flag_for_found_iban = 1;
            for (int j = i; j< *accnum -1; j++) {
                accounts[j] = accounts[j + 1];
            }
            break;
        }
    }

    if (flag_for_found_iban) {
        (*accnum)--;
        printf("account deleted successfully\n");
    }else {
        printf("account Iban not found\n");
    }

    return;
}
