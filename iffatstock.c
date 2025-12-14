#include <stdio.h>
#include <string.h>

int itemId[100], itemQty[100], reorderLevel[100];
char itemName[100][50];
float itemPrice[100];
int itemCount = 0;

int transItem[500], transQty[500];
char transType[500][5];
char transDate[500][12];
int transCount = 0;

void addItem() {
    printf("Enter item name: "); scanf("%s", itemName[itemCount]);
    itemId[itemCount] = itemCount + 1;
    printf("Enter quantity: "); scanf("%d", &itemQty[itemCount]);
    printf("Enter price: "); scanf("%f", &itemPrice[itemCount]);
    printf("Enter reorder level: "); scanf("%d", &reorderLevel[itemCount]);
    itemCount++;
    printf("Item added!\n");
}

void stockIn() {
    int id, qty;
    char date[12];
    printf("Item ID: "); scanf("%d", &id);
    printf("Quantity: "); scanf("%d", &qty);
    printf("Date (YYYY-MM-DD): "); scanf("%s", date);
    itemQty[id-1] += qty;

    transItem[transCount] = id;
    transQty[transCount] = qty;
    strcpy(transType[transCount], "IN");
    strcpy(transDate[transCount], date);
    transCount++;

    printf("Stock in done!\n");
}

void stockOut() {
    int id, qty;
    char date[12];
    printf("Item ID: "); scanf("%d", &id);
    printf("Quantity: "); scanf("%d", &qty);
    if(qty > itemQty[id-1]) { printf("Not enough stock!\n"); return; }
    printf("Date (YYYY-MM-DD): "); scanf("%s", date);
    itemQty[id-1] -= qty;

    transItem[transCount] = id;
    transQty[transCount] = qty;
    strcpy(transType[transCount], "OUT");
    strcpy(transDate[transCount], date);
    transCount++;

    printf("Stock out done!\n");
}

void reportStock() {
    printf("\nID\tName\tQty\tPrice\n");
    for(int i=0;i<itemCount;i++)
        printf("%d\t%s\t%d\t%.2f\n", itemId[i], itemName[i], itemQty[i], itemPrice[i]);
}

void reportLowStock() {
    printf("\n--- Low Stock ---\n");
    for(int i=0;i<itemCount;i++)
        if(itemQty[i] <= reorderLevel[i])
            printf("%s Qty: %d ReorderLevel: %d\n", itemName[i], itemQty[i], reorderLevel[i]);
}

int main() {
    int choice;
    while(1) {
        printf("\n1.Add Item 2.Stock In 3.Stock Out 4.Stock Report 5.Low Stock 6.Exit\nChoice: ");
        scanf("%d",&choice);
        if(choice==1) addItem();
        else if(choice==2) stockIn();
        else if(choice==3) stockOut();
        else if(choice==4) reportStock();
        else if(choice==5) reportLowStock();
        else break;
    }
    printf("Exiting...\n");
    return 0;
}