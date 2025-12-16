 // ANALYTICS 
#include <stdio.h>
#include <string.h>
#include "Maria.c"
void reportLowStock() {
    printf("\n--- Low Stock Items (Qty <= Reorder Level) ---\n");
    int found = 0;
    for (int i = 0; i < itemCount; i++) {
        if (items[i].quantity <= items[i].reorder_level) {
            printf("ID:%d | %s | Qty:%d | Reorder:%d\n",
                   items[i].id,
                   items[i].name,
                   items[i].quantity,
                   items[i].reorder_level);
            found = 1;
        }
    }
    if (!found) printf("No low stock items.\n");
}

void reportTransactionsByItem() {
    int itemId;
    printf("Enter Item ID for transaction report: ");
    scanf("%d", &itemId);

    int count = 0;
    StockTransaction *txs =
        (StockTransaction *)loadAllRecords(TRAN_FILE, sizeof(StockTransaction), &count);
    if (!txs || count == 0) {
        printf("No transactions found.\n");
        free(txs);
        return;
    }

    printf("\n--- Transactions for Item ID %d ---\n", itemId);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (txs[i].itemId == itemId) {
            printf("#%d | %s | User:%s | Qty:%d\n",
                   txs[i].transactionId,
                   txs[i].type == STOCK_IN ? "IN " : "OUT",
                   txs[i].username,
                   txs[i].amount);
            found = 1;
        }
    }
    if (!found) printf("No transactions for this item.\n");
    free(txs);
}

void reportTransactionsByUser() {
    char user[50];
    printf("Enter username for transaction report: ");
    scanf("%s", user);

    int count = 0;
    StockTransaction *txs =
        (StockTransaction *)loadAllRecords(TRAN_FILE, sizeof(StockTransaction), &count);
    if (!txs || count == 0) {
        printf("No transactions found.\n");
        free(txs);
        return;
    }

    printf("\n--- Transactions by User '%s' ---\n", user);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(txs[i].username, user) == 0) {
            printf("#%d | ItemID:%d | %s | Qty:%d\n",
                   txs[i].transactionId,
                   txs[i].itemId,
                   txs[i].type == STOCK_IN ? "IN " : "OUT",
                   txs[i].amount);
            found = 1;
        }
    }
    if (!found) printf("No transactions by this user.\n");
    free(txs);
}

void reportMenu() {
    int ch;
    do {
        printf("\n--- Reports Menu ---\n");
        printf("1. Low Stock Items\n");
        printf("2. Transactions by Item\n");
        printf("3. Transactions by User\n");
        printf("4. Back\nChoice: ");
        scanf("%d", &ch);
        if (ch == 1) reportLowStock();
        else if (ch == 2) reportTransactionsByItem();
        else if (ch == 3) reportTransactionsByUser();
    } while (ch != 4);
}
// Edited 

int userExistsInFile(const char *filename, const char *name, const char *password) {
    FILE *f = fopen(filename, "r");
    if (!f) return 0;
    char u[100], p[100];
    while (fscanf(f, "%s %s", u, p) == 2) {
        if (!strcmp(u, name) && !strcmp(p, password)) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

void signupUser() {
    char u[100], p[100];
    printf("User Sign Up\nUsername: ");
    scanf("%s", u);
    printf("Password: ");
    scanf("%s", p);
    FILE *f = fopen(USER_TXT, "a");
    if (!f) { printf("Error opening user file.\n"); return; }
    fprintf(f, "%s %s\n", u, p);
    fclose(f);
    printf("User registered.\n");
}

void signupAdmin() {
    char u[100], p[100];
    printf("Admin Sign Up\nUsername: ");
    scanf("%s", u);
    printf("Password: ");
    scanf("%s", p);
    FILE *f = fopen(ADMIN_TXT, "a");
    if (!f) { printf("Error opening admin file.\n"); return; }
    fprintf(f, "%s %s\n", u, p);
    fclose(f);
    printf("Admin registered.\n");
}

int loginUser(char *out) {
    char u[100], p[100];
    printf("User Login\nUsername: ");
    scanf("%s", u);
    printf("Password: ");
    scanf("%s", p);
    if (userExistsInFile(USER_TXT, u, p)) {
        strcpy(out, u);
        printf("Login successful.\n");
        return 1;
    }
    printf("Invalid credentials.\n");
    return 0;
}

int loginAdmin(char *out) {
    char u[100], p[100];
    printf("Admin Login\nUsername: ");
    scanf("%s", u);
    printf("Password: ");
    scanf("%s", p);
    if (userExistsInFile(ADMIN_TXT, u, p)) {
        strcpy(out, u);
        printf("Login successful.\n");
        return 1;
    }
    printf("Invalid credentials.\n");
    return 0;
}

/* menus */

void adminMenu(const char *admin) {
    int ch;
    do {
        printf("\n=== Admin Menu ===\n");
        printf("1.Category Management\n");
        printf("2.Item Management\n");
        printf("3.Stock In\n");
        printf("4.Stock Out\n");
        printf("5.Reports\n");
        printf("6.Logout\nChoice: ");
        scanf("%d", &ch);
        if (ch == 1) {  }
        else if (ch == 2) {  }
        else if (ch == 3) doStock(admin, "admin", STOCK_IN);
        else if (ch == 4) doStock(admin, "admin", STOCK_OUT);
        else if (ch == 5) reportMenu();
    } while (ch != 6);
}

void staffMenu(const char *user) {
    int ch;
    do {
        printf("\n=== User Menu ===\n");
        printf("1.List Items\n");
        printf("2.Stock In\n");
        printf("3.Stock Out\n");
        printf("4.Logout\nChoice: ");
        scanf("%d", &ch);
        if (ch == 1) listItems();
        else if (ch == 2) doStock(user, "staff", STOCK_IN);
        else if (ch == 3) doStock(user, "staff", STOCK_OUT);
    } while (ch != 4);
}
