#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CATEGORIES 50
#define MAX_ITEMS      200
#define MAX_NAME       100
#define MAX_ROLE       10

#define USER_TXT  "users.txt"
#define ADMIN_TXT "admins.txt"

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

/* global storage */
Category categories[MAX_CATEGORIES];
Item     items[MAX_ITEMS];
int      categoryCount = 0;
int      itemCount     = 0;

/* files */
const char *TRAN_FILE = "transactions.dat";

/* helpers */
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
