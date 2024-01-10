#include <stdio.h>
#include <string.h>
#include "online.h"

#define MAX_ACCOUNTS 100


static int numAccounts = 0;

int readAccountsFromFile(struct onlineAccounts *accounts) {
    FILE *file = fopen("account_online.txt", "r");
    if (file == NULL) {
        printf("Không thể mở tệp tin.\n");
        return -1;
    }
    
    numAccounts = 0;

    while (numAccounts < MAX_ACCOUNTS && fscanf(file, "%s", accounts[numAccounts].username) == 1) {
        numAccounts++;
    }
    fclose(file);
    return numAccounts;
}

int writeAccountsToFile(const char *newUsername) {
    FILE *file = fopen("account_online.txt", "a");
    if (file == NULL) {
        printf("Không thể mở tệp tin.\n");
        return 0;
    }
    
    fprintf(file, "%s\n", newUsername);

    fclose(file);
    return 1;
}

int addAccountOnline(struct onlineAccounts *accounts, const char *newUsername) {
    if (numAccounts >= MAX_ACCOUNTS) {
        printf("Danh sách tài khoản đã đầy.\n");
        return 0;
    }

    strcpy(accounts[numAccounts].username, newUsername);
    numAccounts++;
    return 1;
}

int removeAccount(const char *usernameToRemove) {

    // Read the accounts from the file
    struct onlineAccounts accounts[MAX_ACCOUNTS]; // Change to onlineAccounts
    int numAccounts = readAccountsFromFile(accounts);
    int found = 0;

    // Find the account to remove
    for (int i = 0; i < numAccounts; ++i) {
        if (strcmp(accounts[i].username, usernameToRemove) == 0) {
            found = 1;

            // Dời các phần tử sau lên trước để xóa tài khoản
            for (int j = i; j < numAccounts - 1; ++j) {
                strcpy(accounts[i].username, accounts[j + 1].username);
            }
            numAccounts--;
            break;
        }
    }

    if (!found) {
        printf("Không tìm thấy tài khoản cần xóa.\n");
        return 0;
    }

    // Open the file for writing
    FILE *file = fopen("account_online.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        return 0;
    }

    // Write the updated accounts to the file
    for (int i = 0; i < numAccounts; ++i) {
        fprintf(file, "%s\n", accounts[i].username);
    }

    // Close the file
    fclose(file);

    return 1;
}

int findAccount(struct onlineAccounts *accounts, const char *usernameToFind) {
    for (int i = 0; i < numAccounts; ++i) {
        if (strcmp(accounts[i].username, usernameToFind) == 0) {
            return i;  // Trả về chỉ số của tài khoản nếu tìm thấy
        }
    }
    return -1;  // Trả về -1 nếu không tìm thấy
}

void printAccounts(const struct onlineAccounts *accounts) {
    printf("Danh sách tài khoản:\n");

    if (numAccounts == 0) {
        printf("Danh sách trống.\n");
    } else {
        for (int i = 0; i < numAccounts; ++i) {
            printf("%d. %s\n", i + 1, accounts[i].username);
        }
    }
}
