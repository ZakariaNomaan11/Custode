#include <stdio.h>
#include <string.h>
#define TEST_BUILD
#include "iffatstock.c"   

/
void resetData() {
    categoryCount = 0;
    itemCount = 0;
}
int test10_reportLowStock_marks_low_items() {
    resetData();
    items[0].id = 1; strcpy(items[0].name, "A"); items[0].quantity = 2; items[0].reorder_level = 5;
    items[1].id = 2; strcpy(items[1].name, "B"); items[1].quantity = 10; items[1].reorder_level = 5;
    itemCount = 2;

    printf("\n[Member4] Running reportLowStock() (visual check)\n");
    reportLowStock(); 

    int actual   = (items[0].quantity <= items[0].reorder_level &&
                    items[1].quantity >  items[1].reorder_level);
    int expected = 1;
    int success  = (actual == expected);

    printf("Member4_test10_reportLowStock_marks_low_items -> expected:%d actual:%d success:%d\n",
           expected, actual, success);
    return success;
}

int test11_reportTransactionsByItem_uses_existing_file() {
    remove("transactions.dat");
    StockTransaction tx1 = {1, 5, STOCK_IN,  10, "u1", "staff"};
    StockTransaction tx2 = {2, 7, STOCK_OUT,  3, "u2", "admin"};
    StockTransaction tx3 = {3, 5, STOCK_OUT,  4, "u1", "staff"};
    appendRecord(TRAN_FILE, &tx1, sizeof(StockTransaction));
    appendRecord(TRAN_FILE, &tx2, sizeof(StockTransaction));
    appendRecord(TRAN_FILE, &tx3, sizeof(StockTransaction));

    printf("\n[Member4] Now run reportTransactionsByItem() and enter 5\n");
    reportTransactionsByItem(); /* visual: should show tx1 & tx3 only */

    
    int count = 0;
    StockTransaction *all =
        (StockTransaction *)loadAllRecords(TRAN_FILE, sizeof(StockTransaction), &count);
    int actual   = (count == 3);
    int expected = 1;
    int success  = (actual == expected);

    printf("Member4_test11_reportTransactionsByItem -> expected:%d actual:%d success:%d\n",
           expected, actual, success);
    free(all);
    return success;
}

int test12_reportTransactionsByUser_filters_user() {
    remove("transactions.dat");
    StockTransaction tx1 = {1, 1, STOCK_IN,  5, "alice", "staff"};
    StockTransaction tx2 = {2, 1, STOCK_OUT, 2, "bob",   "admin"};
    StockTransaction tx3 = {3, 2, STOCK_IN,  7, "alice", "staff"};
    appendRecord(TRAN_FILE, &tx1, sizeof(StockTransaction));
    appendRecord(TRAN_FILE, &tx2, sizeof(StockTransaction));
    appendRecord(TRAN_FILE, &tx3, sizeof(StockTransaction));

    printf("\n[Member4] Now run reportTransactionsByUser() and enter alice\n");
    reportTransactionsByUser(); /* visual: should show tx1 & tx3 only */

    int count = 0;
    StockTransaction *all =
        (StockTransaction *)loadAllRecords(TRAN_FILE, sizeof(StockTransaction), &count);
    int actual   = (count == 3);
    int expected = 1;
    int success  = (actual == expected);

    printf("Member4_test12_reportTransactionsByUser -> expected:%d actual:%d success:%d\n",
           expected, actual, success);
    free(all);
    return success;
}
int main (){
test10_reportLowStock_marks_low_items();
    test11_reportTransactionsByItem_uses_existing_file();
    test12_reportTransactionsByUser_filters_user();
return 0;
}