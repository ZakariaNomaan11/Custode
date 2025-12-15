
#include <stdio.h>
#include <string.h>
#define TEST_BUILD
#include "custode.c"   


void resetData() {
    categoryCount = 0;
    itemCount = 0;
}
int test1_inputString_basic() {
    char buf[100];


    printf("\n[Member1] Type: Keyboard⏎ when asked\n");
    printf("Enter string for test1_inputString_basic: ");
    inputString(buf, sizeof(buf));

    int actual   = strcmp(buf, "Keyboard") == 0;
    int expected = 1;
    int success  = (actual == expected);

    printf("Member1_test1_inputString -> expected:%d actual:%d success:%d (buf=\"%s\")\n",
           expected, actual, success, buf);
    return success;
}

int test2_append_and_load_transaction() {
    remove("transactions.dat"); 

    StockTransaction tx = {1, 5, STOCK_IN, 10, "admin1", "admin"};
    int ok = appendRecord(TRAN_FILE, &tx, sizeof(StockTransaction));

    int count = 0;
    StockTransaction *all =
        (StockTransaction *)loadAllRecords(TRAN_FILE, sizeof(StockTransaction), &count);

    int actual = ok && all && count == 1 &&
                 all[0].transactionId == 1 &&
                 all[0].itemId == 5 &&
                 all[0].amount == 10 &&
                 strcmp(all[0].username, "admin1") == 0;
    int expected = 1;
    int success  = (actual == expected);

    printf("Member1_test2_append_and_load_transaction -> expected:%d actual:%d success:%d\n",
           expected, actual, success);

    free(all);
    return success;
}

int test3_loadAllRecords_empty_file() {
    remove("transactions.dat"); 

    int count = 0;
    void *ptr = loadAllRecords(TRAN_FILE, sizeof(StockTransaction), &count);

    int actual   = (ptr == NULL && count == 0);
    int expected = 1;
    int success  = (actual == expected);

    printf("Member1_test3_loadAllRecords_empty_file -> expected:%d actual:%d success:%d\n",
           expected, actual, success);
    return success;
}
int main() {
    printf("Running Member1 tests...\n");
    resetData();
    test1_inputString_basic();
    test2_append_and_load_transaction();
    test3_loadAllRecords_empty_file();

    return 0;
}
