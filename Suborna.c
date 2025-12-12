#include <stdio.h>
#include <string.h>

#define MAX_CATEGORIES 50
#define MAX_ITEMS 200

// -------------------------
// Category Structure
// -------------------------
typedef struct {
    int id;
    char name[50];
} Category;

// -------------------------
// Item Structure
// -------------------------
typedef struct {
    int id;
    char name[50];
    int category_id;
    int quantity;
    int reorder_level;
    float price;
} Item;

// -------------------------
// Global Storage
// -------------------------
Category categories[MAX_CATEGORIES];
Item items[MAX_ITEMS];

int categoryCount = 0;
int itemCount = 0;

// -------------------------
// Helper Input Function
// -------------------------
void inputString(char *s, int size) {
    fflush(stdin);
    fgets(s, size, stdin);
    s[strcspn(s, "\n")] = 0;
}

// ===========================================================
// CATEGORY CRUD
// ===========================================================

// Add Category
void addCategory() {
    if (categoryCount >= MAX_CATEGORIES) {
        printf("Category limit reached!\n");
        return;
    }

    Category c;
    c.id = categoryCount + 1;

    printf("Enter Category Name: ");
    inputString(c.name, 50);

    categories[categoryCount++] = c;
    printf("Category Added Successfully!\n");
}

// List Categories
void listCategories() {
    printf("\n--- Category List ---\n");
    for (int i = 0; i < categoryCount; i++) {
        printf("%d. %s\n", categories[i].id, categories[i].name);
    }
    printf("----------------------\n");
}

// Search Category
int findCategoryIndexById(int id) {
    for (int i = 0; i < categoryCount; i++) {
        if (categories[i].id == id)
            return i;
    }
    return -1;
}

// Edit Category
void editCategory() {
    int id;
    printf("Enter Category ID to Edit: ");
    scanf("%d", &id);
    fflush(stdin);

    int index = findCategoryIndexById(id);
    if (index == -1) {
        printf("Category Not Found!\n");
        return;
    }

    printf("Enter New Name: ");
    inputString(categories[index].name, 50);

    printf("Category Updated.\n");
}

// Delete Category
void deleteCategory() {
    int id;
    printf("Enter Category ID to Delete: ");
    scanf("%d", &id);
    fflush(stdin);

    int index = findCategoryIndexById(id);
    if (index == -1) {
        printf("Category Not Found!\n");
        return;
    }

    for (int i = index; i < categoryCount - 1; i++) {
        categories[i] = categories[i + 1];
    }
    categoryCount--;

    printf("Category Deleted.\n");
}

// ===========================================================
// ITEM CRUD
// ===========================================================

// Add Item
void addItem() {
    if (itemCount >= MAX_ITEMS) {
        printf("Item limit reached!\n");
        return;
    }

    Item it;
    it.id = itemCount + 1;

    printf("Enter Item Name: ");
    inputString(it.name, 50);

    listCategories();
    printf("Choose Category ID: ");
    scanf("%d", &it.category_id);

    if (findCategoryIndexById(it.category_id) == -1) {
        printf("Invalid Category! Item not added.\n");
        return;
    }

    printf("Enter Quantity: ");
    scanf("%d", &it.quantity);

    printf("Enter Reorder Level: ");
    scanf("%d", &it.reorder_level);

    printf("Enter Price: ");
    scanf("%f", &it.price);
    fflush(stdin);

    items[itemCount++] = it;
    printf("Item Added Successfully!\n");
}

// Find Item by ID
int findItemIndexById(int id) {
    for (int i = 0; i < itemCount; i++) {
        if (items[i].id == id)
            return i;
    }
    return -1;
}

// Edit Item
void editItem() {
    int id;
    printf("Enter Item ID to Edit: ");
    scanf("%d", &id);
    fflush(stdin);

    int index = findItemIndexById(id);
    if (index == -1) {
        printf("Item Not Found!\n");
        return;
    }

    printf("Enter New Item Name: ");
    inputString(items[index].name, 50);

    listCategories();
    printf("New Category ID: ");
    scanf("%d", &items[index].category_id);

    printf("New Quantity: ");
    scanf("%d", &items[index].quantity);

    printf("New Reorder Level: ");
    scanf("%d", &items[index].reorder_level);

    printf("New Price: ");
    scanf("%f", &items[index].price);

    printf("Item Updated.\n");
}

// Delete Item
void deleteItem() {
    int id;
    printf("Enter Item ID to Delete: ");
    scanf("%d", &id);
    fflush(stdin);

    int index = findItemIndexById(id);
    if (index == -1) {
        printf("Item Not Found!\n");
        return;
    }

    for (int i = index; i < itemCount - 1; i++) {
        items[i] = items[i + 1];
    }
    itemCount--;

    printf("Item Deleted.\n");
}

// List All Items
void listItems() {
    printf("\n--- Item List ---\n");
    for (int i = 0; i < itemCount; i++) {
        printf("ID: %d | %s | Cat: %d | Qty: %d | Reorder: %d | Price: %.2f\n",
               items[i].id,
               items[i].name,
               items[i].category_id,
               items[i].quantity,
               items[i].reorder_level,
               items[i].price);
    }
    printf("------------------\n");
}

// Search Item by Name
void searchItem() {
    char name[50];
    printf("Enter Item Name to Search: ");
    inputString(name, 50);

    printf("\nSearch Results:\n");
    for (int i = 0; i < itemCount; i++) {
        if (strstr(items[i].name, name)) {
            printf("ID: %d | %s | Qty: %d\n",
                items[i].id,
                items[i].name,
                items[i].quantity);
        }
    }
}

