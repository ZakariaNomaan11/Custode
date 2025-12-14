#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ================== ENUM for Stock In/Out ================== */

typedef enum {
    STOCK_IN = 1,
    STOCK_OUT = 2
} StockType;

/* ================== Core Data Structures ================== */

#define MAX_NAME 50
#define MAX_ROLE 10

typedef struct {
    int   itemId;
    char  name[MAX_NAME];
    int   quantity;       /* current available stock */
    float price;
} Item;

typedef struct {
    int   transactionId;
    int   itemId;
    StockType type;       /* STOCK_IN or STOCK_OUT */
    int   amount;         /* amount added or removed */
    char  username[MAX_NAME];
    char  role[MAX_ROLE]; /* "admin" or "staff" */
} StockTransaction;

/* Binary files for inventory */
const char *ITEM_FILE = "items.dat";
const char *TRAN_FILE = "transactions.dat";

/* Text files for login system */
const char *USER_TXT  = "users.txt";   /* username password */
const char *ADMIN_TXT = "admins.txt";  /* adminname password */

/* ================== Generic Binary File I/O ================== */

int appendRecord(const char *filename, const void *record, size_t size) {
    FILE *fp = fopen(filename, "ab");
    if (!fp) {
        printf("Error opening file %s for appending.\n", filename);
        return 0;
    }
    int ok = fwrite(record, size, 1, fp) == 1;
    fclose(fp);
    return ok;
}

void *loadAllRecords(const char *filename, size_t size, int *countOut) {
    *countOut = 0;
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    long bytes = ftell(fp);
    rewind(fp);

    if (bytes <= 0 || bytes % (long)size != 0) {
        fclose(fp);
        return NULL;
    }

    int count = (int)(bytes / (long)size);
    void *buffer = malloc(size * count);
    if (!buffer) {
        fclose(fp);
        return NULL;
    }

    if (fread(buffer, size, count, fp) != (size_t)count) {
        free(buffer);
        fclose(fp);
        return NULL;
    }

    fclose(fp);
    *countOut = count;
    return buffer;
}

int saveAllRecords(const char *filename, const void *records, size_t size, int count) {
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        printf("Error opening file %s for writing.\n", filename);
        return 0;
    }
    int ok = fwrite(records, size, count, fp) == (size_t)count;
    fclose(fp);
    return ok;
}

/* ================== Stock Validation & Update ================== */

int validateStock(Item *item, StockTransaction *tx) {
    if (tx->type == STOCK_OUT) {
        if (item->quantity < tx->amount) {
            printf("\nTransaction Failed: Not enough stock!\n");
            printf("   Available: %d, Required: %d\n",
                   item->quantity, tx->amount);
            return 0;
        }
    }
    return 1; /* valid */
}

void updateStock(Item *item, StockTransaction *tx) {
    if (tx->type == STOCK_IN) {
        item->quantity += tx->amount;
        printf("Stock-In Successful! New Quantity: %d\n", item->quantity);
    } else {
        item->quantity -= tx->amount;
        printf("Stock-Out Successful! New Quantity: %d\n", item->quantity);
    }
}

void recordTransaction(StockTransaction *tx) {
    int count = 0;
    StockTransaction *all = (StockTransaction *)
        loadAllRecords(TRAN_FILE, sizeof(StockTransaction), &count);

    if (all && count > 0)
        tx->transactionId = all[count - 1].transactionId + 1;
    else
        tx->transactionId = 1;

    if (all) free(all);

    if (appendRecord(TRAN_FILE, tx, sizeof(StockTransaction)))
        printf("Transaction Recorded Successfully.\n");
    else
        printf("Error recording transaction.\n");
}

void processStockTransaction(Item *item, StockTransaction *tx) {
    printf("\n--- Processing Transaction #%d ---\n", tx->transactionId);
    printf("Item: %s (ID: %d)\n", item->name, item->itemId);

    if (!validateStock(item, tx)) {
        return;
    }

    updateStock(item, tx);

    /* After successful update, persist item array outside this function. */
    recordTransaction(tx);
}

/* ================== Inventory Helper Functions ================== */

void addItem() {
    Item it;
    printf("Item ID: ");
    scanf("%d", &it.itemId);
    printf("Name: ");
    scanf("%s", it.name);
    printf("Initial Quantity: ");
    scanf("%d", &it.quantity);
    printf("Price: ");
    scanf("%f", &it.price);

    if (appendRecord(ITEM_FILE, &it, sizeof(Item)))
        printf("Item saved.\n");
    else
        printf("Error saving item.\n");
}

void listItems() {
    int count = 0;
    Item *items = (Item *)loadAllRecords(ITEM_FILE, sizeof(Item), &count);
    if (!items || count == 0) {
        printf("No items or error loading.\n");
        free(items);
        return;
    }

    printf("\nID\tName\tQty\tPrice\n");
    printf("--------------------------------\n");
    for (int i = 0; i < count; i++) {
        printf("%d\t%s\t%d\t%.2f\n",
               items[i].itemId, items[i].name,
               items[i].quantity, items[i].price);
    }
    free(items);
}

int findItemIndexById(Item *items, int count, int id) {
    for (int i = 0; i < count; i++) {
        if (items[i].itemId == id) return i;
    }
    return -1;
}

void doStockIn(const char *username, const char *role) {
    int count = 0;
    Item *items = (Item *)loadAllRecords(ITEM_FILE, sizeof(Item), &count);
    if (!items || count == 0) {
        printf("No items or error loading.\n");
        free(items);
        return;
    }

    int id, qty;
    printf("Enter Item ID for stock IN: ");
    scanf("%d", &id);
    printf("Quantity to add: ");
    scanf("%d", &qty);

    int idx = findItemIndexById(items, count, id);
    if (idx == -1) {
        printf("Item not found.\n");
        free(items);
        return;
    }

    StockTransaction tx;
    tx.itemId = id;
    tx.type = STOCK_IN;
    tx.amount = qty;
    strncpy(tx.username, username, sizeof(tx.username));
    tx.username[sizeof(tx.username) - 1] = '\0';
    strncpy(tx.role, role, sizeof(tx.role));
    tx.role[sizeof(tx.role) - 1] = '\0';

    processStockTransaction(&items[idx], &tx);

    if (saveAllRecords(ITEM_FILE, items, sizeof(Item), count))
        printf("Inventory file updated.\n");
    else
        printf("Error updating inventory file.\n");

    free(items);
}

void doStockOut(const char *username, const char *role) {
    int count = 0;
    Item *items = (Item *)loadAllRecords(ITEM_FILE, sizeof(Item), &count);
    if (!items || count == 0) {
        printf("No items or error loading.\n");
        free(items);
        return;
    }

    int id, qty;
    printf("Enter Item ID for stock OUT: ");
    scanf("%d", &id);
    printf("Quantity to remove: ");
    scanf("%d", &qty);

    int idx = findItemIndexById(items, count, id);
    if (idx == -1) {
        printf("Item not found.\n");
        free(items);
        return;
    }

    StockTransaction tx;
    tx.itemId = id;
    tx.type = STOCK_OUT;
    tx.amount = qty;
    strncpy(tx.username, username, sizeof(tx.username));
    tx.username[sizeof(tx.username) - 1] = '\0';
    strncpy(tx.role, role, sizeof(tx.role));
    tx.role[sizeof(tx.role) - 1] = '\0';

    processStockTransaction(&items[idx], &tx);

    if (saveAllRecords(ITEM_FILE, items, sizeof(Item), count))
        printf("Inventory file updated.\n");
    else
        printf("Error updating inventory file.\n");

    free(items);
}

/* ================== Role Menus ================== */

void adminMenu(const char *adminName) {
    int choice;
    do {
        printf("\n=== Admin Menu ===\n");
        printf("1. Add Item\n");
        printf("2. List Items\n");
        printf("3. Stock In\n");
        printf("4. Stock Out\n");
        printf("5. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: addItem(); break;
            case 2: listItems(); break;
            case 3: doStockIn(adminName, "admin"); break;
            case 4: doStockOut(adminName, "admin"); break;
            case 5: printf("Logging out...\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 5);
}

void staffMenu(const char *username) {
    int choice;
    do {
        printf("\n=== User Menu ===\n");
        printf("1. List Items\n");
        printf("2. Stock In\n");
        printf("3. Stock Out\n");
        printf("4. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: listItems(); break;
            case 2: doStockIn(username, "staff"); break;
            case 3: doStockOut(username, "staff"); break;
            case 4: printf("Logging out...\n"); break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 4);
}

/* ================== Text-file Sign Up & Login ================== */

int userExistsInFile(const char *filename, const char *name, const char *password) {
    FILE *f = fopen(filename, "r");
    if (!f) return 0;

    char fileName[1000], filePass[100];
    while (fscanf(f, "%s %s", fileName, filePass) == 2) {
        if (strcmp(fileName, name) == 0 && strcmp(filePass, password) == 0) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

void signupUser() {
    char username[1000];
    char password[100];
    printf("User Sign Up Selected\n");
    printf("Choose Username: ");
    scanf("%s", username);
    printf("Choose Password: ");
    scanf("%s", password);

    FILE *f = fopen(USER_TXT, "a");
    if (f == NULL) {
        printf("Error opening user file!\n");
        return;
    }
    fprintf(f, "%s %s\n", username, password);
    fclose(f);
    printf("User registered successfully!\n");
}

void signupAdmin() {
    char admin[1000];
    char password[100];
    printf("Admin Sign Up Selected\n");
    printf("Choose admin username: ");
    scanf("%s", admin);
    printf("Choose Password: ");
    scanf("%s", password);

    FILE *f = fopen(ADMIN_TXT, "a");
    if (f == NULL) {
        printf("Error opening admin file!\n");
        return;
    }
    fprintf(f, "%s %s\n", admin, password);
    fclose(f);
    printf("Admin registered successfully!\n");
}

int loginUser(char *usernameOut) {
    char username[1000];
    char password[100];
    printf("User Login Selected\n");
    printf("Please enter your Username: ");
    scanf("%s", username);
    printf("Please enter your Password: ");
    scanf("%s", password);

    if (userExistsInFile(USER_TXT, username, password)) {
        strcpy(usernameOut, username);
        printf("User login successful.\n");
        return 1;
    } else {
        printf("Invalid username or password.\n");
        return 0;
    }
}

int loginAdmin(char *adminOut) {
    char admin[1000];
    char password[100];
    printf("Admin Login Selected\n");
    printf("Please enter your admin username: ");
    scanf("%s", admin);
    printf("Please enter your Password: ");
    scanf("%s", password);

    if (userExistsInFile(ADMIN_TXT, admin, password)) {
        strcpy(adminOut, admin);
        printf("Admin login successful.\n");
        return 1;
    } else {
        printf("Invalid admin username or password.\n");
        return 0;
    }
}

/* ================== Main / Custode UI ================== */

int main() {
    while (1) {
        printf("_________________________________\n");
        printf("             Custode             \n");
        printf("_________________________________\n");

        int choice = 0;
        printf("Your Choice:\n");
        printf("1. Sign Up\n");
        printf("2. User Login\n");
        printf("3. Admin Login\n");
        printf("4. Exit\n");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("1. As User\n");
            printf("2. As Admin\n");
            int signup_choice = 0;
            scanf("%d", &signup_choice);

            if (signup_choice == 1) {
                signupUser();
            } else if (signup_choice == 2) {
                signupAdmin();
            } else {
                printf("Invalid Sign Up Choice\n");
            }
        } else if (choice == 2) {
            char username[1000];
            if (loginUser(username)) {
                staffMenu(username);
            }
        } else if (choice == 3) {
            char admin[1000];
            if (loginAdmin(admin)) {
                adminMenu(admin);
            }
        } else if (choice == 4) {
            printf("Exiting program...\n");
            break;
        } else {
            printf("Invalid Choice\n");
        }
    }

    return 0;
}