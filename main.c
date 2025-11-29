
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_RECORDS 100

typedef struct {
    char senderAcc[50];
    char recipientIBAN[50];
    double amount;
    char date[11];
} Transfer;

Transfer transfers[MAX_RECORDS];
int transferCount = 0;

int validateAccount(const char* acc) {
    int len = strlen(acc);
    if (len < 5) return 0;
    for (int i = 0; i < len; i++)
        if (!isdigit(acc[i])) return 0;
    return 1;
}

int validateIBAN(const char* iban) {
    int len = strlen(iban);
    if (len < 15 || len > 34) return 0;
    if (!isalpha(iban[0]) || !isalpha(iban[1])) return 0;
    return 1;
}

int validateAmount(double amount) {
    return amount >= 0;
}

int validateDate(const char* d) {
    if (strlen(d) != 10) return 0;
    return (d[4] == '-' && d[7] == '-');
}

void readString(const char* msg, char* buf, int size) {
    printf("%s", msg);
    fgets(buf, size, stdin);
    buf[strcspn(buf, "\n")] = 0;
}

double readDouble(const char* msg) {
    double x;
    printf("%s", msg);
    scanf("%lf", &x);
    while (getchar() != '\n');
    return x;
}

void addTransfer() {
    if (transferCount >= MAX_RECORDS) {
        printf("Array full!\n");
        return;
    }
    Transfer t;
    char temp[50];

    do { readString("Sender account: ", temp, 50); }
    while (!validateAccount(temp));
    strcpy(t.senderAcc, temp);

    do { readString("Recipient IBAN: ", temp, 50); }
    while (!validateIBAN(temp));
    strcpy(t.recipientIBAN, temp);

    do { t.amount = readDouble("Amount: "); }
    while (!validateAmount(t.amount));

    do { readString("Date (yyyy-mm-dd): ", t.date, 11); }
    while (!validateDate(t.date));

    transfers[transferCount++] = t;
    printf("Added!\n");
}

void viewTransfers() {
    if (transferCount == 0) {
        printf("No records.\n");
        return;
    }
    for (int i = 0; i < transferCount; i++) {
        printf("\nRecord %d:\n", i+1);
        printf("Sender: %s\n", transfers[i].senderAcc);
        printf("Recipient: %s\n", transfers[i].recipientIBAN);
        printf("Amount: %.2f\n", transfers[i].amount);
        printf("Date: %s\n", transfers[i].date);
    }
}

void deleteTransfer() {
    int n;
    printf("Record number: ");
    scanf("%d", &n);
    while (getchar() != '\n');

    if (n < 1 || n > transferCount) {
        printf("Invalid!\n");
        return;
    }
    for (int i = n-1; i < transferCount - 1; i++)
        transfers[i] = transfers[i+1];
    transferCount--;
    printf("Deleted.\n");
}

void deleteAll() {
    transferCount = 0;
    printf("All deleted.\n");
}

void editTransfer() {
    int n;
    printf("Record to edit: ");
    scanf("%d", &n);
    while (getchar() != '\n');

    if (n < 1 || n > transferCount) {
        printf("Invalid!\n");
        return;
    }

    Transfer* t = &transfers[n-1];
    char temp[50];
    double d;

    readString("New sender (leave empty): ", temp, 50);
    if (strlen(temp) > 0 && validateAccount(temp))
        strcpy(t->senderAcc, temp);

    readString("New IBAN (leave empty): ", temp, 50);
    if (strlen(temp) > 0 && validateIBAN(temp))
        strcpy(t->recipientIBAN, temp);

    printf("New amount (negative=skip): ");
    scanf("%lf", &d);
    while (getchar() != '\n');
    if (d >= 0)
        t->amount = d;

    readString("New date (leave empty): ", temp, 11);
    if (strlen(temp) > 0 && validateDate(temp))
        strcpy(t->date, temp);

    printf("Updated.\n");
}

void saveToFile() {
    FILE* f = fopen("transfers.bin", "wb");
    fwrite(&transferCount, sizeof(int), 1, f);
    fwrite(transfers, sizeof(Transfer), transferCount, f);
    fclose(f);
    printf("Saved.\n");
}

void loadFromFile() {
    FILE* f = fopen("transfers.bin", "rb");
    if (!f) return;
    fread(&transferCount, sizeof(int), 1, f);
    fread(transfers, sizeof(Transfer), transferCount, f);
    fclose(f);
    printf("Loaded.\n");
}

void menu() {
    int ch;
    do {
        printf("\n1.Add\n2.View\n3.Edit\n4.Delete\n5.Delete All\n6.Save\n7.Load\n0.Exit\nChoose: ");
        scanf("%d", &ch);
        while (getchar() != '\n');
        switch(ch) {
            case 1: addTransfer(); break;
            case 2: viewTransfers(); break;
            case 3: editTransfer(); break;
            case 4: deleteTransfer(); break;
            case 5: deleteAll(); break;
            case 6: saveToFile(); break;
            case 7: loadFromFile(); break;
        }
    } while (ch != 0);
}

int main() {
    loadFromFile();
    menu();
    saveToFile();
    return 0;
}
