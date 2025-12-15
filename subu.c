#include <stdio.h>
#include <string.h>

#define MAX_CATEGORIES 20
#define MAX_ITEMS 100


typedef struct {
    int categoryId;
    char categoryName[50];
} Category;

typedef struct {
    int itemId;
    char itemName[50];
    int categoryId;
    int quantity;
    int reorderLevel;
    float price;
} Item;



Category categories[MAX_CATEGORIES];
Item items[MAX_ITEMS];

int categoryCount = 0;
int itemCount = 0;




void addCategory()
{
    Category c;
    c.categoryId = categoryCount + 1;

    printf("Enter Category Name: ");
    scanf(" %[^\n]", c.categoryName);

    categories[categoryCount++] = c;
    printf("Category Added Successfully!\n");
}

// List Categories
void listCategories()
{
    printf("\n--- CATEGORY LIST ---\n");
    for (int i = 0; i < categoryCount; i++) {
        printf("ID: %d | Name: %s\n",
               categories[i].categoryId,
               categories[i].categoryName);
    }
}

// Search Category by ID
int findCategoryById(int id)
{
    for (int i = 0; i < categoryCount; i++) {
        if (categories[i].categoryId == id)
            return i;
    }
    return -1;
}

// Edit Category
void editCategory()
{
    int id;
    printf("Enter Category ID to Edit: ");
    scanf("%d", &id);

    int index = findCategoryById(id);
    if (index == -1) {
        printf("Category Not Found!\n");
        return;
    }

    printf("Enter New Category Name: ");
    scanf(" %[^\n]", categories[index].categoryName);

    printf("Category Updated!\n");
}

// Delete Category
void deleteCategory()
{
    int id;
    printf("Enter Category ID to Delete: ");
    scanf("%d", &id);

    int index = findCategoryById(id);
    if (index == -1) {
        printf("Category Not Found!\n");
        return;
    }

    for (int i = index; i < categoryCount - 1; i++) {
        categories[i] = categories[i + 1];
    }
    categoryCount--;

    printf("Category Deleted!\n");
}

/* =========================
   ITEM FUNCTIONS
========================= */

// Add Item
void addItem()
{
    Item it;
    it.itemId = itemCount + 1;

    printf("Enter Item Name: ");
    scanf(" %[^\n]", it.itemName);

    listCategories();
    printf("Enter Category ID: ");
    scanf("%d", &it.categoryId);

    if (findCategoryById(it.categoryId) == -1) {
        printf("Invalid Category!\n");
        return;
    }

    printf("Enter Quantity: ");
    scanf("%d", &it.quantity);

    printf("Enter Reorder Level: ");
    scanf("%d", &it.reorderLevel);

    printf("Enter Price: ");
    scanf("%f", &it.price);

    items[itemCount++] = it;
    printf("Item Added Successfully!\n");
}

// List Items
void listItems()
{
    printf("\n--- ITEM LIST ---\n");
    for (int i = 0; i < itemCount; i++) {
        printf("ID:%d | Name:%s | Cat:%d | Qty:%d | Reorder:%d | Price:%.2f\n",
               items[i].itemId,
               items[i].itemName,
               items[i].categoryId,
               items[i].quantity,
               items[i].reorderLevel,
               items[i].price);
    }
}

// Search Item by ID
int findItemById(int id)
{
    for (int i = 0; i < itemCount; i++) {
        if (items[i].itemId == id)
            return i;
    }
    return -1;
}

// Edit Item
void editItem()
{
    int id;
    printf("Enter Item ID to Edit: ");
    scanf("%d", &id);

    int index = findItemById(id);
    if (index == -1) {
        printf("Item Not Found!\n");
        return;
    }

    printf("Enter New Item Name: ");
    scanf(" %[^\n]", items[index].itemName);

    printf("Enter New Quantity: ");
    scanf("%d", &items[index].quantity);

    printf("Enter New Reorder Level: ");
    scanf("%d", &items[index].reorderLevel);

    printf("Enter New Price: ");
    scanf("%f", &items[index].price);

    printf("Item Updated!\n");
}

// Delete Item
void deleteItem()
{
    int id;
    printf("Enter Item ID to Delete: ");
    scanf("%d", &id);

    int index = findItemById(id);
    if (index == -1) {
        printf("Item Not Found!\n");
        return;
    }

    for (int i = index; i < itemCount - 1; i++) {
        items[i] = items[i + 1];
    }
    itemCount--;

    printf("Item Deleted!\n");
}

// Search Item by Name
void searchItem()
{
    char name[50];
    printf("Enter Item Name to Search: ");
    scanf(" %[^\n]", name);

    printf("\nSearch Result:\n");
    for (int i = 0; i < itemCount; i++) {
        if (strstr(items[i].itemName, name)) {
            printf("ID:%d | Name:%s | Qty:%d\n",
                   items[i].itemId,
                   items[i].itemName,
                   items[i].quantity);
        }
    }
}
