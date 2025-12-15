#include <stdio.h>
#include <string.h>
#include "subu.c"

/*
----------------------------------
Test Case 1:
Valid Item ID exists
----------------------------------
*/
int test_valid_item_search()
{
    // Arrange (mock data)
    itemCount = 1;
    items[0].itemId = 1;
    strcpy(items[0].itemName, "Pen");

    int actual = findItemById(1);
    int expected = 0;   // index 0 means item found

    int success = (actual == expected);
    printf("expected: %d, actual: %d, success: %d\n",
           expected, actual, success);

    return success;
}

/*
----------------------------------
Test Case 2:
Invalid Category ID check
----------------------------------
*/
int test_invalid_category_check()
{
    // Arrange (mock data)
    categoryCount = 1;
    categories[0].categoryId = 1;
    strcpy(categories[0].categoryName, "Stationery");

    int actual = findCategoryById(99); // invalid ID
    int expected = -1;                 // -1 means not found

    int success = (actual == expected);
    printf("expected: %d, actual: %d, success: %d\n",
           expected, actual, success);

    return success;
}

/*
----------------------------------
Main Test Runner
----------------------------------
*/
int main()
{
    test_valid_item_search();
    test_invalid_category_check();
    return 0;
}
