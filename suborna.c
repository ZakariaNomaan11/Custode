int findCategoryIndexById(int id) {
    for (int i = 0; i < categoryCount; i++)
        if (categories[i].id == id) return i;
    return -1;
}

void addCategory() {
    if (categoryCount >= MAX_CATEGORIES) { printf("Category limit reached!\n"); return; }
    Category c;
    c.id = categoryCount + 1;
    printf("Enter Category Name: ");
    inputString(c.name, MAX_NAME);
    categories[categoryCount++] = c;
    printf("Category Added.\n");
}

void listCategories() {
    printf("\n--- Category List ---\n");
    for (int i = 0; i < categoryCount; i++)
        printf("%d. %s\n", categories[i].id, categories[i].name);
    if (!categoryCount) printf("(none)\n");
    printf("---------------------\n");
}

void editCategory() {
    int id;
    printf("Enter Category ID to Edit: ");
    scanf("%d", &id); getchar();
    int idx = findCategoryIndexById(id);
    if (idx == -1) { printf("Category Not Found!\n"); return; }
    printf("New Name: ");
    inputString(categories[idx].name, MAX_NAME);
    printf("Category Updated.\n");
}

void deleteCategory() {
    int id;
    printf("Enter Category ID to Delete: ");
    scanf("%d", &id); getchar();
    int idx = findCategoryIndexById(id);
    if (idx == -1) { printf("Category Not Found!\n"); return; }
    for (int i = idx; i < categoryCount-1; i++)
        categories[i] = categories[i+1];
    categoryCount--;
    printf("Category Deleted.\n");
}

/* item side */

int findItemIndexById(int id) {
    for (int i = 0; i < itemCount; i++)
        if (items[i].id == id) return i;
    return -1;
}

void addItem() {
    if (itemCount >= MAX_ITEMS) { printf("Item limit reached!\n"); return; }
    Item it;
    it.id = itemCount + 1;
    printf("Enter Item Name: ");
    getchar();
    inputString(it.name, MAX_NAME);

    listCategories();
    printf("Category ID: ");
    scanf("%d", &it.category_id);
    if (findCategoryIndexById(it.category_id) == -1) {
        printf("Invalid Category.\n");
        return;
    }

    printf("Quantity: ");
    scanf("%d", &it.quantity);
    printf("Reorder Level: ");
    scanf("%d", &it.reorder_level);
    printf("Price: ");
    scanf("%f", &it.price);

    items[itemCount++] = it;
    printf("Item Added.\n");
}

void listItems() {
    printf("\n--- Item List ---\n");
    for (int i = 0; i < itemCount; i++) {
        printf("ID:%d | %s | Cat:%d | Qty:%d | Reorder:%d | Price:%.2f\n",
            items[i].id, items[i].name, items[i].category_id,
            items[i].quantity, items[i].reorder_level, items[i].price);
    }
    if (!itemCount) printf("(none)\n");
    printf("------------------\n");
}

void editItem() {
    int id;
    printf("Enter Item ID to Edit: ");
    scanf("%d", &id); getchar();
    int idx = findItemIndexById(id);
    if (idx == -1) { printf("Item Not Found!\n"); return; }

    printf("New Item Name: ");
    inputString(items[idx].name, MAX_NAME);
    listCategories();
    printf("New Category ID: ");
    scanf("%d", &items[idx].category_id);
    printf("New Qty: ");
    scanf("%d", &items[idx].quantity);
    printf("New Reorder Level: ");
    scanf("%d", &items[idx].reorder_level);
    printf("New Price: ");
    scanf("%f", &items[idx].price);
    printf("Item Updated.\n");
}

void deleteItem() {
    int id;
    printf("Enter Item ID to Delete: ");
    scanf("%d", &id); getchar();
    int idx = findItemIndexById(id);
    if (idx == -1) { printf("Item Not Found!\n"); return; }
    for (int i = idx; i < itemCount-1; i++)
        items[i] = items[i+1];
    itemCount--;
    printf("Item Deleted.\n");
}

void searchItem() {
    char name[MAX_NAME];
    printf("Search Item Name: ");
    getchar();
    inputString(name, MAX_NAME);
    printf("\nSearch Results:\n");
    for (int i = 0; i < itemCount; i++) {
        if (strstr(items[i].name, name)) {
            printf("ID:%d | %s | Qty:%d\n",
                   items[i].id, items[i].name, items[i].quantity);
        }
    }
}
