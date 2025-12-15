int validateStock(Item *item, StockTransaction *tx) {
    if (tx->type == STOCK_OUT && item->quantity < tx->amount) {
        printf("Not enough stock! Available:%d, Required:%d\n",
               item->quantity, tx->amount);
        return 0;
    }
    return 1;
}

void updateStock(Item *item, StockTransaction *tx) {
    if (tx->type == STOCK_IN) {
        item->quantity += tx->amount;
        printf("Stock-In OK. New Qty: %d\n", item->quantity);
    } else {
        item->quantity -= tx->amount;
        printf("Stock-Out OK. New Qty: %d\n", item->quantity);
    }
}

void recordTransaction(StockTransaction *tx) {
    int count = 0;
    StockTransaction *all =
        (StockTransaction *)loadAllRecords(TRAN_FILE, sizeof(StockTransaction), &count);
    tx->transactionId = (all && count > 0) ? all[count-1].transactionId + 1 : 1;
    free(all);
    if (appendRecord(TRAN_FILE, tx, sizeof(StockTransaction)))
        printf("Transaction recorded.\n");
    else
        printf("Error recording transaction.\n");
}

void doStock(const char *username, const char *role, StockType type) {
    int id, qty;
    listItems(); /* Member 2’s function */
    printf("Item ID: ");
    scanf("%d", &id);
    printf("Quantity: ");
    scanf("%d", &qty);

    int idx = findItemIndexById(id);
    if (idx == -1) { printf("Item not found.\n"); return; }

    StockTransaction tx;
    tx.itemId = id;
    tx.type   = type;
    tx.amount = qty;
    strncpy(tx.username, username, MAX_NAME);
    strncpy(tx.role, role, MAX_ROLE);
    tx.username[MAX_NAME-1] = 0;
    tx.role[MAX_ROLE-1]     = 0;

    if (!validateStock(&items[idx], &tx)) return;
    updateStock(&items[idx], &tx);
    recordTransaction(&tx);
}