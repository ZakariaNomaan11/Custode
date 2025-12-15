#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS     100
#define MAX_ADMINS    50
#define MAX_ROUTES    50
#define MAX_VEHICLES  50
#define MAX_TRIPS     100
#define MAX_BOOKINGS  500

/* ---------- DATA STRUCTURES ---------- */

typedef struct {
    int id;
    char username[50];
    char password[50];
} User;

typedef struct {
    int id;
    char username[50];
    char password[50];
} Admin;

typedef struct {
    int id;
    char from[50];
    char to[50];
    float distance;
} Route;

typedef struct {
    int id;
    char regNumber[20];
    int capacity;
    char type[20];
} Vehicle;

typedef struct {
    int id;
    int routeId;
    int vehicleId;
    char departure[20];
    float fare;
    int availableSeats;
} Trip;

typedef struct {
    int id;
    int userId;
    int tripId;
    int seats;
    char status[20];
} Booking;

/* ---------- GLOBAL DATABASE ---------- */

User users[MAX_USERS];
int numUsers = 0;

Admin admins[MAX_ADMINS];
int numAdmins = 0;

Route routes[MAX_ROUTES];
int numRoutes = 0;

Vehicle vehicles[MAX_VEHICLES];
int numVehicles = 0;

Trip trips[MAX_TRIPS];
int numTrips = 0;

Booking bookings[MAX_BOOKINGS];
int numBookings = 0;

/* ---------- LOADING / SAVING (DB) ---------- */
/* You already have users/admins load/save – extend similarly for trips/bookings if needed */

void loadAllData(void) {
    FILE *fp;

    if ((fp = fopen("users.txt", "r"))) {
        while (numUsers < MAX_USERS &&
               fscanf(fp, "%d %49s %49s",
                      &users[numUsers].id,
                      users[numUsers].username,
                      users[numUsers].password) == 3) {
            numUsers++;
        }
        fclose(fp);
    }

    if ((fp = fopen("admins.txt", "r"))) {
        while (numAdmins < MAX_ADMINS &&
               fscanf(fp, "%d %49s %49s",
                      &admins[numAdmins].id,
                      admins[numAdmins].username,
                      admins[numAdmins].password) == 3) {
            numAdmins++;
        }
        fclose(fp);
    }

    /* For demo: initialize some trips in memory (like your first code) */
    trips[0] = (Trip){1, 0, 0, "10:00", 500, 40};
    trips[1] = (Trip){2, 0, 0, "12:00", 600, 30};
    trips[2] = (Trip){3, 0, 0, "14:00", 300, 20};
    trips[3] = (Trip){4, 0, 0, "16:00", 450, 50};
    trips[4] = (Trip){5, 0, 0, "18:00", 550, 25};
    numTrips = 5;
}

void saveAllData(void) {
    FILE *fp = fopen("users.txt", "w");
    if (fp) {
        for (int i = 0; i < numUsers; i++) {
            fprintf(fp, "%d %s %s\n",
                    users[i].id,
                    users[i].username,
                    users[i].password);
        }
        fclose(fp);
    }

    fp = fopen("admins.txt", "w");
    if (fp) {
        for (int i = 0; i < numAdmins; i++) {
            fprintf(fp, "%d %s %s\n",
                    admins[i].id,
                    admins[i].username,
                    admins[i].password);
        }
        fclose(fp);
    }

    /* Extend to save trips/bookings if you want persistent bookings */
}

/* ---------- VALIDATION HELPERS (DB CHECKS) ---------- */

int findUserByUsername(const char *name) {
    for (int i = 0; i < numUsers; i++) {
        if (strcmp(users[i].username, name) == 0)
            return i;
    }
    return -1;
}

int findAdminByUsername(const char *name) {
    for (int i = 0; i < numAdmins; i++) {
        if (strcmp(admins[i].username, name) == 0)
            return i;
    }
    return -1;
}

int findTripById(int id) {
    for (int i = 0; i < numTrips; i++) {
        if (trips[i].id == id)
            return i;
    }
    return -1;
}

int findBookingById(int id) {
    for (int i = 0; i < numBookings; i++) {
        if (bookings[i].id == id)
            return i;
    }
    return -1;
}

/* ---------- SIGN UP / LOGIN (VALIDATED) ---------- */

void userSignUp(void) {
    char username[50], password[50];

    printf("\n--- User Sign Up ---\n");
    printf("Choose Username: ");
    scanf("%49s", username);
    printf("Choose Password: ");
    scanf("%49s", password);

    if (findUserByUsername(username) != -1) {
        printf("Error: Username already exists!\n");
        return;
    }

    if (numUsers >= MAX_USERS) {
        printf("Error: User database full!\n");
        return;
    }

    users[numUsers].id = numUsers + 1;
    strcpy(users[numUsers].username, username);
    strcpy(users[numUsers].password, password);
    numUsers++;

    saveAllData();
    printf("User registered successfully!\n");
}

void listItems() {
    printf("\n--- Item List ---\n");
    for (int i = 0; i < itemCount; i++) {
        printf("ID:%d | %s | Cat:%d | Qty:%d | Reorder:%d | Price:%.2f\n",
            items[i].id, items[i].name, items[i].category_id,
            items[i].quantity, items[i].reorder_level, items[i].price);
    }
    if (!itemCount) printf("(none)\n");
    printf("------------------\n");
}

void editItem() {
    int id;
    printf("Enter Item ID to Edit: ");
    scanf("%d", &id); getchar();
    int idx = findItemIndexById(id);
    if (idx == -1) { printf("Item Not Found!\n"); return; }

    printf("New Item Name: ");
    inputString(items[idx].name, MAX_NAME);
    listCategories();
    printf("New Category ID: ");
    scanf("%d", &items[idx].category_id);
    printf("New Qty: ");
    scanf("%d", &items[idx].quantity);
    printf("New Reorder Level: ");
    scanf("%d", &items[idx].reorder_level);
    printf("New Price: ");
    scanf("%f", &items[idx].price);

    printf("Item Updated.\n");
}

void deleteItem() {
    int id;
    printf("Enter Item ID to Delete: ");
    scanf("%d", &id); getchar();
    int idx = findItemIndexById(id);
    if (idx == -1) { printf("Item Not Found!\n"); return; }
    for (int i = idx; i < itemCount-1; i++)
        items[i] = items[i+1];
    itemCount--;
    printf("Item Deleted.\n");
}

void searchItem() {
    char name[MAX_NAME];
    printf("Search Item Name: ");
    getchar();
    inputString(name, MAX_NAME);
    printf("\nSearch Results:\n");
    for (int i = 0; i < itemCount; i++) {
        if (strstr(items[i].name, name)) {
            printf("ID:%d | %s | Qty:%d\n",
                   items[i].id, items[i].name, items[i].quantity);
        }
    }
}

/* ========= STOCK / TRANSACTIONS ========= */

int validateStock(Item *item, StockTransaction *tx) {
    if (tx->type == STOCK_OUT && item->quantity < tx->amount) {
        printf("Not enough stock! Available:%d, Required:%d\n",
               item->quantity, tx->amount);
        return 0;
    }
    return 1;
}

void updateStock(Item *item, StockTransaction *tx) {
    if (tx->type == STOCK_IN) {
        item->quantity += tx->amount;
        printf("Stock-In OK. New Qty: %d\n", item->quantity);
    } else {
        item->quantity -= tx->amount;
        printf("Stock-Out OK. New Qty: %d\n", item->quantity);
    }
}

void recordTransaction(StockTransaction *tx) {
    int count = 0;
    StockTransaction *all =
        (StockTransaction *)loadAllRecords(TRAN_FILE, sizeof(StockTransaction), &count);
    tx->transactionId = (all && count > 0) ? all[count-1].transactionId + 1 : 1;
    free(all);
    if (appendRecord(TRAN_FILE, tx, sizeof(StockTransaction)))
        printf("Transaction recorded.\n");
    else
        printf("Error recording transaction.\n");
}

void doStock(const char *username, const char *role, StockType type) {
    int id, qty;
    listItems();
    printf("Item ID: ");
    scanf("%d", &id);
    printf("Quantity: ");
    scanf("%d", &qty);

    int idx = findItemIndexById(id);
    if (idx == -1) { printf("Item not found.\n"); return; }

    StockTransaction tx;
    tx.itemId = id;
    tx.type   = type;
    tx.amount = qty;
    strncpy(tx.username, username, MAX_NAME);
    strncpy(tx.role, role, MAX_ROLE);
    tx.username[MAX_NAME-1] = 0;
    tx.role[MAX_ROLE-1]     = 0;

    if (!validateStock(&items[idx], &tx)) return;
    updateStock(&items[idx], &tx);
    recordTransaction(&tx);
}

/* ========= REPORTS MODULE (Member 5) ========= */

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

/* ========= LOGIN (TEXT FILE BASED) ========= */

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

/* ========= MENUS ========= */

void staffMenu(const char *username) {
    /* TODO: Implement staff menu */
}

void adminMenu(const char *username) {
    /* TODO: Implement admin menu */
}


int main() {
    int choice;
    while (1) {
        printf("*______________Custode_______________*\n");
        printf("1.Sign Up\n2.User Login\n3.Admin Login\n4.Exit\nChoice: ");
        scanf("%d", &choice);
        if (choice == 1) {
            int sc;
            printf("1.As User  2.As Admin\nChoice: ");
            scanf("%d", &sc);
            if (sc == 1) signupUser();
            else if (sc == 2) signupAdmin();
            else printf("Invalid.\n");
        } else if (choice == 2) {
            char u[100];
            if (loginUser(u)) staffMenu(u);
        } else if (choice == 3) {
            char a[100];
            if (loginAdmin(a)) adminMenu(a);
        } else if (choice == 4) {
            printf("Exiting...\n");
            break;
        } else {
            printf("Invalid choice.\n");
        }
    }
    return 0;
}




