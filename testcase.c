#include "custode.c"   /* or a proper header if you split files */

/* Helper: reset in-memory data for test */
void resetData() {
    categoryCount = 0;
    itemCount = 0;
}

/* 1) Test adding a category */
int test_add_category() {
    resetData();
    addCategory(); /* when running, type: Electronics */
    int actual = (categoryCount == 1 && findCategoryIndexById(1) != -1);
    int expected = 1;
    int success = actual == expected;
    printf("test_add_category -> expected:%d actual:%d success:%d\n",
           expected, actual, success);
    return success;
}

/* 2) Test adding an item with existing category */
int test_add_item_valid_category() {
    resetData();
    /* prepare one category */
    categories[0].id = 1;
    strcpy(categories[0].name, "Electronics");
    categoryCount = 1;

    addItem(); /* when running, choose name e.g. Keyboard, category 1, qty 10, reorder 5, price 100 */

    int idx = findItemIndexById(1);
    int actual = (itemCount == 1 && idx != -1);
    int expected = 1;
    int success = actual == expected;
    printf("test_add_item_valid_category -> expected:%d actual:%d success:%d\n",
           expected, actual, success);
    return success;
}

/* 3) Test findItemIndexById for invalid id */
int test_find_item_invalid_id() {
    resetData();
    /* one item with id 1 */
    items[0].id = 1;
    strcpy(items[0].name, "Mouse");
    items[0].category_id = 1;
    items[0].quantity = 10;
    items[0].reorder_level = 3;
    items[0].price = 500;
    itemCount = 1;

    int idx = findItemIndexById(99);
    int actual = (idx == -1);
    int expected = 1;
    int success = actual == expected;
    printf("test_find_item_invalid_id -> expected:%d actual:%d success:%d\n",
           expected, actual, success);
    return success;
}

/* 4) Test stock-out validation (not enough stock) */
int test_stock_out_not_enough() {
    resetData();
    items[0].id = 1;
    strcpy(items[0].name, "Keyboard");
    items[0].category_id = 1;
    items[0].quantity = 5;
    items[0].reorder_level = 2;
    items[0].price = 1000;
    itemCount = 1;

    StockTransaction tx;
    tx.itemId = 1;
    tx.type   = STOCK_OUT;
    tx.amount = 10;               /* more than available */
    strcpy(tx.username, "admin1");
    strcpy(tx.role, "admin");

    int valid = validateStock(&items[0], &tx);  /* should be 0 */
    int actual = (valid == 0);
    int expected = 1;
    int success = actual == expected;
    printf("test_stock_out_not_enough -> expected:%d actual:%d success:%d\n",
           expected, actual, success);
    return success;
}

/* 5) Test stock-in updates quantity correctly */
int test_stock_in_updates_quantity() {
    resetData();
    items[0].id = 1;
    strcpy(items[0].name, "Keyboard");
    items[0].category_id = 1;
    items[0].quantity = 10;
    items[0].reorder_level = 2;
    items[0].price = 1000;
    itemCount = 1;

    StockTransaction tx;
    tx.itemId = 1;
    tx.type   = STOCK_IN;
    tx.amount = 5;
    strcpy(tx.username, "admin1");
    strcpy(tx.role, "admin");

    int before = items[0].quantity;
    int valid  = validateStock(&items[0], &tx);
    if (valid) updateStock(&items[0], &tx);
    int after  = items[0].quantity;

    int actual = (before == 10 && after == 15);
    int expected = 1;
    int success = actual == expected;
    printf("test_stock_in_updates_quantity -> expected:%d actual:%d success:%d\n",
           expected, actual, success);
    return success;
}

int main() {
    test_add_category();
    test_add_item_valid_category();
    test_find_item_invalid_id();
    test_stock_out_not_enough();
    test_stock_in_updates_quantity();
    return 0;
}
