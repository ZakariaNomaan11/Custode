/* Category + Item CRUD */

int test4_addCategory_increments_count() {
    resetData();

    /* For manual run, type: Electronics */
    printf("\n[Member2] Type: Electronics⏎ when asked\n");
    addCategory();

    int actual   = (categoryCount == 1 &&
                    categories[0].id == 1 &&
                    strcmp(categories[0].name, "Electronics") == 0);
    int expected = 1;
    int success  = (actual == expected);

    printf("Member2_test4_addCategory -> expected:%d actual:%d success:%d\n",
           expected, actual, success);
    return success;
}

int test5_findItemIndexById() {
    resetData();
    items[0].id = 1;
    strcpy(items[0].name, "Keyboard");
    items[1].id = 2;
    strcpy(items[1].name, "Mouse");
    itemCount = 2;

    int idx1 = findItemIndexById(1);
    int idx2 = findItemIndexById(2);
    int idx3 = findItemIndexById(99);

    int actual   = (idx1 == 0 && idx2 == 1 && idx3 == -1);
    int expected = 1;
    int success  = (actual == expected);

    printf("Member2_test5_findItemIndexById -> expected:%d actual:%d success:%d\n",
           expected, actual, success);
    return success;
}

int test6_deleteItem_shifts_array() {
    resetData();
    items[0].id = 1; strcpy(items[0].name, "A");
    items[1].id = 2; strcpy(items[1].name, "B");
    items[2].id = 3; strcpy(items[2].name, "C");
    itemCount = 3;

    /* simulate delete ID=2 using your deleteItem logic core */
    int idx = findItemIndexById(2);
    for (int i = idx; i < itemCount-1; i++)
        items[i] = items[i+1];
    itemCount--;

    int actual   = (itemCount == 2 &&
                    items[0].id == 1 &&
                    items[1].id == 3);
    int expected = 1;
    int success  = (actual == expected);
printf("Member2_test6_deleteItem_shifts_array -> expected:%d actual:%d success:%d\n",
           expected, actual, success);
    return success;
}

/* ==================== MEMBER 3 TESTS ==================== */
/* Stock + Transaction logic */

int test7_validateStock_not_enough() {
    Item it = {1, "Keyboard", 5, 1000.0f, 0, 0}; /* only qty field matters */
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
