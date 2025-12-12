#include <stdio.h>
#include <string.h>

/* ---------------------------------------------------------
   ENUM : Defines whether a transaction is Stock-In or Out
   --------------------------------------------------------- */
typedef enum {
    STOCK_IN = 1,
    STOCK_OUT = 2
} StockType;

/* ---------------------------------------------------------
   STRUCT : Represents an item stored in inventory
   --------------------------------------------------------- */
typedef struct {
    int itemId;
    char name[50];
    int quantity;       // current available stock
} Item;

/* ---------------------------------------------------------
   STRUCT : Represents a stock transaction (IN/OUT movement)
   --------------------------------------------------------- */
typedef struct {
    int transactionId;
    int itemId;
    StockType type;
    int amount;         // amount added or removed
} StockTransaction;

/* ---------------------------------------------------------
   FUNCTION : Validate stock availability before OUT
   --------------------------------------------------------- */
int validateStock(Item *item, StockTransaction *tx) {
    if (tx->type == STOCK_OUT) {
        if (item->quantity < tx->amount) {
            printf("\n❌ Transaction Failed: Not enough stock!\n");
            printf("   Available: %d, Required: %d\n",
                   item->quantity, tx->amount);
            return 0;
        }
    }
    return 1; // valid
}

/* ---------------------------------------------------------
   FUNCTION : Apply the stock change (IN or OUT)
   --------------------------------------------------------- */
void updateStock(Item *item, StockTransaction *tx) {
    if (tx->type == STOCK_IN) {
        item->quantity += tx->amount;
        printf("✅ Stock-In Successful! New Quantity: %d\n", item->quantity);
    } else {
        item->quantity -= tx->amount;
        printf("✅ Stock-Out Successful! New Quantity: %d\n", item->quantity);
    }
}

/* ---------------------------------------------------------
   FUNCTION : Main flow to process a transaction
   --------------------------------------------------------- */
void processStockTransaction(Item *item, StockTransaction *tx) {
    printf("\n--- Processing Transaction #%d ---\n", tx->transactionId);
    printf("Item: %s (ID: %d)\n", item->name, item->itemId);

    // Step 1: Validation
    if (!validateStock(item, tx)) {
        return; // stop if invalid
    }

    // Step 2: Update
    updateStock(item, tx);

    // Step 3: Confirmation
    printf("📦 Transaction Recorded Successfully.\n");
}

/* ---------------------------------------------------------
   MAIN : Example run
   --------------------------------------------------------- */
int main() {

    // Sample item
    Item item1;
    item1.itemId = 1;
    strcpy(item1.name, "Keyboard");
    item1.quantity = 50;

    // Sample transactions
    StockTransaction t1 = {1001, 1, STOCK_IN, 20};
    StockTransaction t2 = {1002, 1, STOCK_OUT, 30};
    StockTransaction t3 = {1003, 1, STOCK_OUT, 100}; // will fail

    // Process each transaction
    processStockTransaction(&item1, &t1);
    processStockTransaction(&item1, &t2);
    processStockTransaction(&item1, &t3);

    return 0;
}
