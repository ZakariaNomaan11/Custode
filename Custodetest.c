#include <stdio.h>
#include <string.h>
#define TEST_BUILD
#include "CustodeDemo.c"


void resetData() {
    categoryCount = 0;
    itemCount     = 0;
}

int test1_append_and_load_transaction() {
    remove("transactions.txt");

    StockTransaction tx = {1, 5, STOCK_IN, 10, "admin1", "admin"};
    int ok = appendRecord(TRAN_FILE, &tx, sizeof(StockTransaction));

    int count = 0;
    StockTransaction *all =
        (StockTransaction *)loadAllRecords(TRAN_FILE,
                                           sizeof(StockTransaction),
                                           &count);

    int actual   = ok && all && count == 1 &&
                   all[0].transactionId == 1 &&
                   all[0].itemId == 5 &&
                   all[0].amount == 10 &&
                   strcmp(all[0].username, "admin1") == 0;
    int expected = 1;
    int success  = (actual == expected);

    printf("test1_append_and_load_transaction -> expected:%d actual:%d success:%d\n",
           expected, actual, success);

    free(all);
    return success;
}

int test2_loadAllRecords_empty_file() {
    remove("transactions.dat");

    int count = 0;
    void *ptr = loadAllRecords(TRAN_FILE, sizeof(StockTransaction), &count);

    int actual   = (ptr == NULL && count == 0);
    int expected = 1;
    int success  = (actual == expected);

    printf("test2_loadAllRecords_empty_file -> expected:%d actual:%d success:%d\n",
           expected, actual, success);
    return success;
}

int test3_addCategory() {
    resetData();

    /* manual input: type 'Electronics' when asked */
    printf("\n[test3_addCategory] Type: Electronics⏎\n");
    addCategory();

    int actual   = (categoryCount == 1 &&
                    categories[0].id == 1 &&
                    strcmp(categories[0].name, "Electronics") == 0);
    int expected = 1;
    int success  = (actual == expected);

    printf("test3_addCategory -> expected:%d actual:%d success:%d\n",
           expected, actual, success);
    return success;
}

int test4_findItemIndexById() {
    resetData();
    items[0].id = 1; strcpy(items[0].name, "Keyboard");
    items[1].id = 2; strcpy(items[1].name, "Mouse");
    itemCount = 2;

    int i1 = findItemIndexById(1);
    int i2 = findItemIndexById(2);
    int i3 = findItemIndexById(99);

    int actual   = (i1 == 0 && i2 == 1 && i3 == -1);
    int expected = 1;
    int success  = (actual == expected);

    printf("test4_findItemIndexById -> expected:%d actual:%d success:%d\n",
           expected, actual, success);
    return success;
}

int test5_deleteItem_shifts() {
    resetData();
    items[0].id = 1; strcpy(items[0].name, "A");
    items[1].id = 2; strcpy(items[1].name, "B");
    items[2].id = 3; strcpy(items[2].name, "C");
    itemCount = 3;

    int idx = findItemIndexById(2);
    for (int i = idx; i < itemCount-1; i++)
        items[i] = items[i+1];
    itemCount--;

    int actual   = (itemCount == 2 &&
                    items[0].id == 1 &&
                    items[1].id == 3);
    int expected = 1;
    int success  = (actual == expected);

    printf("test5_deleteItem_shifts -> expected:%d actual:%d success:%d\n",
           expected, actual, success);
    return success;
}

int test6_validateStock_not_enough() {
    Item it;
    it.quantity = 5;
    StockTransaction tx;
    tx.type   = STOCK_OUT;
    tx.amount = 10;

    int valid   = validateStock(&it, &tx);
    int actual  = (valid == 0);
    int expected= 1;
    int success = (actual == expected);

    printf("test6_validateStock_not_enough -> expected:%d actual:%d success:%d\n",
           expected, actual, success);
    return success;
}

int test7_updateStock_in() {
    Item it;
    it.quantity = 10;
    StockTransaction tx;
    tx.type   = STOCK_IN;
    tx.amount = 5;

    updateStock(&it, &tx);

    int actual   = (it.quantity == 15);
    int expected = 1;
    int success  = (actual == expected);

    printf("test7_updateStock_in -> expected:%d actual:%d success:%d\n",
           expected, actual, success);
    return success;
}

int test8_recordTransaction_ids() {
    remove("transactions.dat");

    StockTransaction tx1 = {0, 1, STOCK_IN, 5,  "u1", "staff"};
    StockTransaction tx2 = {0, 1, STOCK_OUT, 3, "u1", "staff"};

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

    printf("test8_recordTransaction_ids -> expected:%d actual:%d success:%d\n",
           expected, actual, success);
    free(all);
    return success;
}

int test9_reportLowStock_flag_logic() {
    resetData();
    items[0].id = 1; strcpy(items[0].name, "A");
    items[0].quantity = 2; items[0].reorder_level = 5;
    items[1].id = 2; strcpy(items[1].name, "B");
    items[1].quantity = 10; items[1].reorder_level = 5;
    itemCount = 2;

    printf("\n[test9_reportLowStock] Visual: should show item A only.\n");
    reportLowStock();

    int actual   = (items[0].quantity <= items[0].reorder_level &&
                    items[1].quantity >  items[1].reorder_level);
    int expected = 1;
    int success  = (actual == expected);

    printf("test9_reportLowStock_flag_logic -> expected:%d actual:%d success:%d\n",
           expected, actual, success);
    return success;
}

int test10_reportTransactionsByItem_file_exists() {
    remove("transactions.dat");
    StockTransaction tx1 = {1, 5, STOCK_IN, 10, "u1", "staff"};
    StockTransaction tx2 = {2, 7, STOCK_OUT, 3, "u2", "admin"};
    appendRecord(TRAN_FILE, &tx1, sizeof(StockTransaction));
    appendRecord(TRAN_FILE, &tx2, sizeof(StockTransaction));

    printf("\n[test10_reportTransactionsByItem] Visual: enter 5 when asked.\n");
    reportTransactionsByItem(); 

    int count = 0;
    StockTransaction *all =
        (StockTransaction *)loadAllRecords(TRAN_FILE, sizeof(StockTransaction), &count);

    int actual   = (count == 2);
    int expected = 1;
    int success  = (actual == expected);

    printf("test10_reportTransactionsByItem_file_exists -> expected:%d actual:%d success:%d\n",
           expected, actual, success);
    free(all);
    return success;
}

int test11_reportTransactionsByUser_file_exists() {
    remove("transactions.dat");
    StockTransaction tx1 = {1, 1, STOCK_IN,  5, "alice", "staff"};
    StockTransaction tx2 = {2, 1, STOCK_OUT, 2, "bob",   "admin"};
    appendRecord(TRAN_FILE, &tx1, sizeof(StockTransaction));
    appendRecord(TRAN_FILE, &tx2, sizeof(StockTransaction));

    printf("\n[test11_reportTransactionsByUser] Visual: enter alice when asked.\n");
    reportTransactionsByUser(); 

    int count = 0;
    StockTransaction *all =
        (StockTransaction *)loadAllRecords(TRAN_FILE, sizeof(StockTransaction), &count);

    int actual   = (count == 2);
    int expected = 1;
    int success  = (actual == expected);

    printf("test11_reportTransactionsByUser_file_exists -> expected:%d actual:%d success:%d\n",
           expected, actual, success);
    free(all);
    return success;
}

int test12_userExistsInFile_empty() {
    remove(USER_TXT);

    int actual   = (userExistsInFile(USER_TXT, "x", "y") == 0);
    int expected = 1;
    int success  = (actual == expected);

    printf("test12_userExistsInFile_empty -> expected:%d actual:%d success:%d\n",
           expected, actual, success);
    return success;
}

int test13_signupUser_and_loginUser() {
    remove(USER_TXT);

    printf("\n[test13_signupUser] Type username: testuser, password: testpass\n");
    signupUser();

    char out[100];
    printf("[test13_loginUser] Type same username/password again.\n");
    int ok = loginUser(out);

    int actual   = (ok == 1 && strcmp(out, "testuser") == 0);
    int expected = 1;
    int success  = (actual == expected);

    printf("test13_signupUser_and_loginUser -> expected:%d actual:%d success:%d\n",
           expected, actual, success);
    return success;
}

int test14_signupAdmin_and_loginAdmin() {
    remove(ADMIN_TXT);

    printf("\n[test14_signupAdmin] Type admin username: admin1, password: apass\n");
    signupAdmin();

    char out[100];
    printf("[test14_loginAdmin] Type same admin username/password again.\n");
    int ok = loginAdmin(out);

    int actual   = (ok == 1 && strcmp(out, "admin1") == 0);
    int expected = 1;
    int success  = (actual == expected);

    printf("test14_signupAdmin_and_loginAdmin -> expected:%d actual:%d success:%d\n",
           expected, actual, success);
    return success;
}

int main() {
    printf("Running Custode tests...\n");

    /* core */
    test1_append_and_load_transaction();
    test2_loadAllRecords_empty_file();

    /* CRUD */
    test3_addCategory();
    test4_findItemIndexById();
    test5_deleteItem_shifts();

    /* stock/transactions */
    test6_validateStock_not_enough();
    test7_updateStock_in();
    test8_recordTransaction_ids();

    /* reports */
    test9_reportLowStock_flag_logic();
    test10_reportTransactionsByItem_file_exists();
    test11_reportTransactionsByUser_file_exists();

    /* login */
    test12_userExistsInFile_empty();
    test13_signupUser_and_loginUser();
    test14_signupAdmin_and_loginAdmin();

    return 0;
}
