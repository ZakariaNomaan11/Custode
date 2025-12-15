#include <stdio.h>
#include <string.h>
#define TEST_BUILD
#include "Maria.c"

int categoryCount = 0;
int itemCount = 0;

void resetData() {
    categoryCount = 0;
    itemCount = 0;
}

/* Stock + Transaction logic */

int test7_validateStock_not_enough() {
    Item it = {1, "Keyboard", 5, 1000.0f, 0, 0}; 
    StockTransaction tx;
    tx.type   = STOCK_OUT;
    tx.amount = 10;

    int valid   = validateStock(&it, &tx);
    int actual  = (valid == 0);
    int expected= 1;
    int success = (actual == expected);

    printf("Member3_test7_validateStock_not_enough -> expected:%d actual:%d success:%d\n",
           expected, actual, success);
    return success;
}

int test8_updateStock_increases_quantity() {
    Item it = {1, "Keyboard", 10, 0, 0, 0};
    StockTransaction tx;
    tx.type   = STOCK_IN;
    tx.amount = 5;

    updateStock(&it, &tx);

    int actual   = (it.quantity == 15);
    int expected = 1;
    int success  = (actual == expected);

    printf("Member3_test8_updateStock_increases_quantity -> expected:%d actual:%d success:%d\n",
           expected, actual, success);
    return success;
}

int test9_recordTransaction_assigns_id() {
    remove("transactions.dat");

    StockTransaction tx1 = {0, 1, STOCK_IN, 5,  "admin1", "admin"};
    StockTransaction tx2 = {0, 1, STOCK_OUT, 3, "admin1", "admin"};

    recordTransaction(&tx1);
    recordTransaction(&tx2);

    int count = 0;
    StockTransaction *all =
        (StockTransaction *)loadAllRecords(TRAN_FILE, sizeof(StockTransaction), &count);

    int actual   = all && count == 2 &&
                   all[0].transactionId == 1 &&
                   all[1].transactionId == 2;
    int expected = 1;
    int success  = (actual == expected);

    printf("Member3_test9_recordTransaction_assigns_id -> expected:%d actual:%d success:%d\n",
           expected, actual, success);
    free(all);
    return success;
}
int main() {
    resetData();
    test7_validateStock_not_enough();
    test8_updateStock_increases_quantity();
    test9_recordTransaction_assigns_id();

return 0;
        
}