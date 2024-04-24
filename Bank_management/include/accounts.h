#ifndef ACCOUNT_H
#define ACCOUNT_H

#define MAX_ACCOUNTS 100
#define IBAN_LENGTH 22

#define RON 0
#define EUR 1
#define USD 2

typedef struct {
    char name[50];
    char surname[50];
} Person;

typedef struct {
    char iban[IBAN_LENGTH+1];
    Person owner;
    int currency;
    float amount;
} Account;

extern Account accounts[MAX_ACCOUNTS];
extern int accnum;

void viewAccount(Account *account);
char* generate_random_iban();
void performTransaction(Account *accounts, int accnum);
void createAccount(char *name, char *surname);
#endif 
