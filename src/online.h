#ifndef ONLINE_H
#define ONLINE_H

#define MAX_ACCOUNTS 100

struct onlineAccounts {
    char username[50];
};

int readAccountsFromFile(struct onlineAccounts *accounts);
int writeAccountsToFile(const char *newUsername);
int addAccountOnline(struct onlineAccounts *accounts, const char *newUsername);
int removeAccount(const char *usernameToRemove);
int findAccount(struct onlineAccounts *accounts, const char *usernameToFind);
void printAccounts(const struct onlineAccounts *accounts);

#endif  // ONLINE_H
