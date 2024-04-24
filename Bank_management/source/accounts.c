#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "../include/accounts.h"

Account accounts[MAX_ACCOUNTS];
int accnum = 0;

char* generate_random_iban() {
    static char iban[IBAN_LENGTH + 1]; 
    int i;

    srand(time(NULL));

    for (i = 0; i < IBAN_LENGTH; i++) {
        iban[i] = 'A' + rand() % 26;
    }
    iban[IBAN_LENGTH] = '\0';
    return iban;
}

void viewAccount(Account *account) {
    printf("\nAccount Details:\n");
    printf("IBAN: %s\n", account->iban);
    printf("Owner: %s %s\n", account->owner.name, account->owner.surname);
    printf("Currency: %d\n", account->currency);
    printf("Amount: %.2f\n", account->amount);
}

void performTransaction(Account *accounts, int accnum) {
    char destIban[IBAN_LENGTH+1];
    char sourceIban[IBAN_LENGTH+1];
    float amount;
    printf("Enter SourceIban: \n");
    scanf("%s", sourceIban);
    printf("Enter DestIban: \n");
    scanf("%s", destIban);
    printf("Enter amount: \n");
    scanf("%f", &amount);
    int transaction_ok = 0;
    for (int i = 0; i< accnum; i++) {
        if (strcmp(accounts[i].iban, sourceIban)==0 && accounts[i].amount >= amount) {
            for (int j = 0; j < accnum; j++) {
                if (strcmp(accounts[j].iban, destIban)==0) {
                    transaction_ok = 1;
                    accounts[i].amount -= amount;
                    accounts[j].amount += amount;
                    printf("Transaction successful.\n");
                    return;
                }
            }
            if (transaction_ok == 0) {
                printf("destination IBAN doesn't exit...\n");
                break;
            }
        }
    }
}

void createAccount(char *name, char *surname) {
    int iban_exists;
    char *generated_iban;
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

    printf("\t%s\n", generated_iban);

    strcpy(accounts[accnum].iban, generated_iban);
    strcpy(accounts[accnum].owner.name, name);
    strcpy(accounts[accnum].owner.surname, surname);
    printf("Enter currency (0 for RON, 1 for EUR, 2 for USD): ");
    scanf("%d", &accounts[accnum].currency);
    printf("Enter initial amount: ");
    scanf("%f", &accounts[accnum].amount);

    accnum++;
    printf("Account created successfully.\n");
}
