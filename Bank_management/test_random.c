#include <stdio.h>

int main(){
    char random_iban[13];
    while (100) {
        for (int i = 0; i < 12; i++) {
            random_iban[i] = 'A' + rand() % 26; // Assuming you want uppercase letters
        }
        random_iban[12] = '\0'; // Null-terminate the string
                                //
        printf("\t%s\n",random_iban);
    }
}
