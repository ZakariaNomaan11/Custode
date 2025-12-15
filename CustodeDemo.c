#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CATEGORIES 50
#define MAX_ITEMS      200
#define MAX_NAME       100
#define MAX_ROLE       10

#define USER_TXT  "users.txt"
#define ADMIN_TXT "admins.txt"
#define TRAN_FILE "transactions.txt"

typedef enum { STOCK_IN = 1, STOCK_OUT = 2 } StockType;

typedef struct {
    int  id;
    char name[MAX_NAME];
} Category;

typedef struct {
    int   id;
    char  name[MAX_NAME];
    int   category_id;
    int   quantity;
    int   reorder_level;
    float price;
} Item;

typedef struct {
    int       transactionId;
    int       itemId;
    StockType type;
    int       amount;
    char      username[MAX_NAME];
    char      role[MAX_ROLE];
} StockTransaction;

/* ========= GLOBAL STORAGE ========= */

Category categories[MAX_CATEGORIES];
Item     items[MAX_ITEMS];
int      categoryCount = 0;
int      itemCount     = 0;

/* ========= SMALL HELPERS ========= */

void inputString(char *s, int size) {
    int c;
    if (fgets(s, size, stdin)) {
        s[strcspn(s, "\n")] = 0;
    } else {
        s[0] = 0;
    }
    while ((c = getchar()) != '\n' && c != EOF); 
}

int appendRecord(const char *filename, const void *record, size_t size) {
    FILE *fp = fopen(filename, "ab");
    if (!fp) return 0;
    int ok = fwrite(record, size, 1, fp) == 1;
    fclose(fp);
    return ok;
}

void *loadAllRecords(const char *filename, size_t size, int *countOut) {
    *countOut = 0;
    FILE *fp = fopen(filename, "rb");
    if (!fp) return NULL;
    fseek(fp, 0, SEEK_END);
    long bytes = ftell(fp);
    rewind(fp);
    if (bytes <= 0 || bytes % (long)size != 0) { fclose(fp); return NULL; }
    int count = (int)(bytes / (long)size);
    void *buf = malloc(size * count);
    if (!buf) { fclose(fp); return NULL; }
    if (fread(buf, size, count, fp) != (size_t)count) { free(buf); fclose(fp); return NULL; }
    fclose(fp);
    *countOut = count;
    return buf;
}

/* ========= CATEGORY CRUD ========= */

int findCategoryIndexById(int id) {
    for (int i = 0; i < categoryCount; i++)
        if (categories[i].id == id) return i;
    return -1;
}

void addCategory() {
    if (categoryCount >= MAX_CATEGORIES) { printf("Category limit reached!\n"); return; }
    Category c;
    c.id = categoryCount + 1;
    printf("Enter Category Name: ");
    inputString(c.name, MAX_NAME);
    categories[categoryCount++] = c;
    printf("Category Added.\n");
}

void listCategories() {
    printf("\n--- Category List ---\n");
    for (int i = 0; i < categoryCount; i++)
        printf("%d. %s\n", categories[i].id, categories[i].name);
    if (!categoryCount) printf("(none)\n");
    printf("---------------------\n");
}

void editCategory() {
    int id;
    printf("Enter Category ID to Edit: ");
    scanf("%d", &id); getchar();
    int idx = findCategoryIndexById(id);
    if (idx == -1) { printf("Category Not Found!\n"); return; }
    printf("New Name: ");
    inputString(categories[idx].name, MAX_NAME);
    printf("Category Updated.\n");
}

void deleteCategory() {
    int id;
    printf("Enter Category ID to Delete: ");
    scanf("%d", &id); getchar();
    int idx = findCategoryIndexById(id);
    if (idx == -1) { printf("Category Not Found!\n"); return; }
    for (int i = idx; i < categoryCount-1; i++)
        categories[i] = categories[i+1];
    categoryCount--;
    printf("Category Deleted.\n");
}

/* ========= ITEM CRUD ========= */

int findItemIndexById(int id) {
    for (int i = 0; i < itemCount; i++)
        if (items[i].id == id) return i;
    return -1;
}

void addItem() {
    if (itemCount >= MAX_ITEMS) { printf("Item limit reached!\n"); return; }
    Item it;
    it.id = itemCount + 1;
    printf("Enter Item Name: ");
    getchar(); 
    inputString(it.name, MAX_NAME);

    listCategories();
    printf("Category ID: ");
    scanf("%d", &it.category_id);

    if (findCategoryIndexById(it.category_id) == -1) {
        printf("Invalid Category.\n");
        return;
    }

    printf("Quantity: ");
    scanf("%d", &it.quantity);
    printf("Reorder Level: ");
    scanf("%d", &it.reorder_level);
    printf("Price: ");
    scanf("%f", &it.price);

    items[itemCount++] = it;
    printf("Item Added.\n");
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

/* ========= REPORTS ========= */

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
    StockTransaction *txs = (StockTransaction *)loadAllRecords(TRAN_FILE, sizeof(StockTransaction), &count);
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

/* ========= LOGIN ========= */

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
    int choice;
    do {
        printf("\n=== Staff Menu (%s) ===\n", username);
        printf("1. List Items\n");
        printf("2. Search Item\n");
        printf("3. Stock In\n");
        printf("4. Stock Out\n");
        printf("5. Logout\n");
        printf("Choice: ");
        scanf("%d", &choice);

        if (choice == 1)      listItems();
        else if (choice == 2) searchItem();
        else if (choice == 3) doStock(username, "staff", STOCK_IN);
        else if (choice == 4) doStock(username, "staff", STOCK_OUT);
        else if (choice != 5) printf("Invalid choice.\n");
    } while (choice != 5);
}

void adminMenu(const char *username) {
    int choice;
    do {
        printf("\n=== Admin Menu (%s) ===\n", username);
        printf("1. Add Category\n");
        printf("2. Edit Category\n");
        printf("3. Delete Category\n");
        printf("4. Add Item\n");
        printf("5. Edit Item\n");
        printf("6. Delete Item\n");
        printf("7. List Items\n");
        printf("8. Reports\n");
        printf("9. Logout\n");
        printf("Choice: ");
        scanf("%d", &choice);

        if (choice == 1)      addCategory();
        else if (choice == 2) editCategory();
        else if (choice == 3) deleteCategory();
        else if (choice == 4) addItem();
        else if (choice == 5) editItem();
        else if (choice == 6) deleteItem();
        else if (choice == 7) listItems();
        else if (choice == 8) reportMenu();
        else if (choice != 9) printf("Invalid choice.\n");
    } while (choice != 9);
}

/* ========= MAIN ========= */

int start() {
    int choice;

    while (1) {
         printf("     #          #############\n");
         printf("     #          #             #\n");
         printf("     #          #              #\n");
         printf("     #          #               #\n");
         printf("     #          #              #\n");
         printf("     #          #             #\n");
         printf("     #          #            #\n");
         printf("     ########## #############\n");
       



        printf("*______________Custode_______________*\n");
        printf("1.Sign Up\n2.User Login\n3.Admin Login\n4.Exit\nChoice: ");
        scanf("%d", &choice);
        if (choice == 1) {
            int sc;
            printf("1.As User  2.As Admin\nChoice: ");
            scanf("%d", &sc);
            if (sc == 1)      signupUser();
            else if (sc == 2) signupAdmin();
            else              printf("Invalid.\n");
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
