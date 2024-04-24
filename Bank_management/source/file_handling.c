#include <stdio.h>
#include <stdlib.h>
#include "../include/accounts.h"

#define DATA_FILE "data/bank_data.txt"

void readDataFromFile() {
    FILE *file = fopen(DATA_FILE, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    while (fscanf(file, "%s %s %s %d %f", accounts[accnum].iban, accounts[accnum].owner.name,
                  accounts[accnum].owner.surname, &accounts[accnum].currency,
                  &accounts[accnum].amount) != EOF) {
        accnum++;
    }
    fclose(file);
}

void writeDataToFile() {
    FILE *file = fopen(DATA_FILE, "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    for (int i = 0; i < accnum; i++) {
        fprintf(file, "%s %s %s %d %.2f\n", accounts[i].iban, accounts[i].owner.name,
                accounts[i].owner.surname, accounts[i].currency, accounts[i].amount);
    }

    fclose(file);
}
