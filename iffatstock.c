 // ANALYTICS 

void reportLowStock() {
    printf("\n--- Low Stock Items (Qty <= Reorder Level) ---\n");
    int found = 0;
    for (int i = 0; i < itemCount; i++) {
        if (items[i].quantity <= items[i].reorder_level) {
            printf("ID:%d | %s | Qty:%d | Reorder:%d\n",
                   items[i].id,
                   items[i].name,
                   items[i].quantity,
                   items[i].reorder_level);
            found = 1;
        }
    }
    if (!found) printf("No low stock items.\n");
}

void reportTransactionsByItem() {
    int itemId;
    printf("Enter Item ID for transaction report: ");
    scanf("%d", &itemId);

    int count = 0;
    StockTransaction *txs =
        (StockTransaction *)loadAllRecords(TRAN_FILE, sizeof(StockTransaction), &count);
    if (!txs || count == 0) {
        printf("No transactions found.\n");
        free(txs);
        return;
    }

    printf("\n--- Transactions for Item ID %d ---\n", itemId);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (txs[i].itemId == itemId) {
            printf("#%d | %s | User:%s | Qty:%d\n",
                   txs[i].transactionId,
                   txs[i].type == STOCK_IN ? "IN " : "OUT",
                   txs[i].username,
                   txs[i].amount);
            found = 1;
        }
    }
    if (!found) printf("No transactions for this item.\n");
    free(txs);
}

void reportTransactionsByUser() {
    char user[50];
    printf("Enter username for transaction report: ");
    scanf("%s", user);

    int count = 0;
    StockTransaction *txs =
        (StockTransaction *)loadAllRecords(TRAN_FILE, sizeof(StockTransaction), &count);
    if (!txs || count == 0) {
        printf("No transactions found.\n");
        free(txs);
        return;
    }

    printf("\n--- Transactions by User '%s' ---\n", user);
    int found = 0;
    for (int i = 0; i < count; i++) {
        if (strcmp(txs[i].username, user) == 0) {
            printf("#%d | ItemID:%d | %s | Qty:%d\n",
                   txs[i].transactionId,
                   txs[i].itemId,
                   txs[i].type == STOCK_IN ? "IN " : "OUT",
                   txs[i].amount);
            found = 1;
        }
    }
    if (!found) printf("No transactions by this user.\n");
    free(txs);
}

void reportMenu() {
    int ch;
    do {
        printf("\n--- Reports Menu ---\n");
        printf("1. Low Stock Items\n");
        printf("2. Transactions by Item\n");
        printf("3. Transactions by User\n");
        printf("4. Back\nChoice: ");
        scanf("%d", &ch);
        if (ch == 1) reportLowStock();
        else if (ch == 2) reportTransactionsByItem();
        else if (ch == 3) reportTransactionsByUser();
    } while (ch != 4);
}