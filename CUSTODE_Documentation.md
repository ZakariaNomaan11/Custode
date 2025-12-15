# Custode Inventory Management System

## Documentation

**Project Name:** Custode  
**Type:** Inventory Management System  
**Language:** C  
**Version:** 1.0.0  
**Date:** December 2025

---

## Table of Contents

1. [Project Overview](#project-overview)
2. [System Architecture](#system-architecture)
3. [Data Structures](#data-structures)
4. [Core Modules](#core-modules)
5. [API Reference](#api-reference)
6. [Function Documentation](#function-documentation)
7. [User Guide](#user-guide)
8. [File Structure](#file-structure)
9. [Configuration](#configuration)

---

## Project Overview

### Description

**Custode** is a comprehensive inventory management system developed in C that provides role-based access control for managing product categories, items, and stock transactions. The system supports two user roles: **User** (staff) and **Admin**, each with distinct privileges and responsibilities.

### Key Features

- **Role-Based Access Control**: Separate authentication for users and administrators
- **Category Management**: Create, read, update, and delete product categories
- **Item Management**: Comprehensive CRUD operations for inventory items
- **Stock Transactions**: Track stock-in and stock-out operations with audit trail
- **Transaction History**: Persistent storage of all stock transactions
- **Reporting Module**: Low stock alerts, transaction reports by item/user
- **Data Persistence**: File-based storage for users, admins, and transactions

### Purpose

Custode streamlines inventory management by providing:
- Centralized product and stock management
- Complete audit trail of all stock movements
- Real-time low stock warnings
- User accountability through transaction logging
- Simple and intuitive interface for staff and administrators

---

## System Architecture

### High-Level Design

```
┌─────────────────────────────────────────┐
│         USER INTERFACE LAYER            │
│  (Start Menu, Menus, Input Handling)    │
└─────────────────┬───────────────────────┘
                  │
┌─────────────────▼───────────────────────┐
│      BUSINESS LOGIC LAYER               │
│ (CRUD Operations, Validations, Reports) │
└─────────────────┬───────────────────────┘
                  │
┌─────────────────▼───────────────────────┐
│     DATA MANAGEMENT LAYER               │
│  (Memory Storage, File I/O)             │
└─────────────────┬───────────────────────┘
                  │
┌─────────────────▼───────────────────────┐
│      FILE STORAGE LAYER                 │
│ (Text & Binary Files)                   │
└─────────────────────────────────────────┘
```

### Module Breakdown

| Module | Responsibility | Key Functions |
|--------|-----------------|---------------|
| **Authentication** | User/Admin login and registration | `loginUser()`, `loginAdmin()`, `signupUser()`, `signupAdmin()` |
| **Category Management** | Product category operations | `addCategory()`, `editCategory()`, `deleteCategory()`, `listCategories()` |
| **Item Management** | Inventory item operations | `addItem()`, `editItem()`, `deleteItem()`, `listItems()`, `searchItem()` |
| **Stock Management** | Stock-in/out operations | `doStock()`, `validateStock()`, `updateStock()` |
| **Transaction Management** | Transaction recording and retrieval | `recordTransaction()`, `reportTransactionsByItem()`, `reportTransactionsByUser()` |
| **Reports** | Analytics and reporting | `reportLowStock()`, `reportTransactionsByItem()`, `reportTransactionsByUser()` |

---

## Data Structures

### 1. StockType (Enum)

```c
typedef enum { STOCK_IN = 1, STOCK_OUT = 2 } StockType;
```

**Description:** Enumeration for stock transaction types.

| Value | Description |
|-------|-------------|
| `STOCK_IN` | Goods received into inventory |
| `STOCK_OUT` | Goods removed from inventory |

---

### 2. Category (Struct)

```c
typedef struct {
    int  id;              /*!< Unique category identifier */
    char name[MAX_NAME];  /*!< Category name (max 100 chars) */
} Category;
```

**Purpose:** Represents a product category.

**Fields:**
- `id`: Unique identifier (auto-incremented starting from 1)
- `name`: Display name for the category

**Constraints:**
- Maximum name length: 100 characters
- Maximum categories in memory: 50

---

### 3. Item (Struct)

```c
typedef struct {
    int   id;              /*!< Unique item identifier */
    char  name[MAX_NAME];  /*!< Item name (max 100 chars) */
    int   category_id;     /*!< Reference to Category.id */
    int   quantity;        /*!< Current stock quantity */
    int   reorder_level;   /*!< Minimum stock threshold */
    float price;           /*!< Unit price */
} Item;
```

**Purpose:** Represents an inventory item.

**Fields:**
- `id`: Unique identifier (auto-incremented starting from 1)
- `name`: Item display name
- `category_id`: Foreign key to Category
- `quantity`: Current quantity in stock
- `reorder_level`: Threshold for low stock warnings
- `price`: Cost per unit

**Constraints:**
- Maximum items in memory: 200
- Name length: max 100 characters
- Price: floating-point (2 decimal places recommended)

---

### 4. StockTransaction (Struct)

```c
typedef struct {
    int        transactionId;  /*!< Auto-incremented transaction ID */
    int        itemId;         /*!< Reference to Item.id */
    StockType  type;           /*!< STOCK_IN or STOCK_OUT */
    int        amount;         /*!< Quantity moved */
    char       username[MAX_NAME];  /*!< User who performed transaction */
    char       role[MAX_ROLE];      /*!< User role (User/Admin) */
} StockTransaction;
```

**Purpose:** Represents a stock movement transaction (audit trail).

**Fields:**
- `transactionId`: Unique transaction identifier
- `itemId`: Reference to the Item involved
- `type`: Transaction type (STOCK_IN or STOCK_OUT)
- `amount`: Quantity involved
- `username`: Username of person performing transaction
- `role`: User role at time of transaction

**Storage:** Binary file-based persistent storage

---

## Core Modules

### Authentication Module

Handles user and administrator authentication with file-based credential storage.

#### Files Used
- `users.txt`: User credentials (format: `username password`)
- `admins.txt`: Admin credentials (format: `username password`)

#### Key Functions

**`int userExistsInFile(const char *filename, const char *name, const char *password)`**
- **Purpose:** Verify user credentials against a file
- **Parameters:**
  - `filename`: Path to credential file
  - `name`: Username to verify
  - `password`: Password to verify
- **Returns:** 1 if credentials valid, 0 otherwise
- **Security Note:** Passwords stored in plaintext (not recommended for production)

**`int loginUser(char *out)`**
- **Purpose:** User login interface
- **Parameters:**
  - `out`: Buffer to store logged-in username
- **Returns:** 1 on success, 0 on failure
- **Process:**
  1. Prompt for username and password
  2. Verify against `users.txt`
  3. Store username on success

**`int loginAdmin(char *out)`**
- **Purpose:** Admin login interface
- **Parameters:**
  - `out`: Buffer to store logged-in admin username
- **Returns:** 1 on success, 0 on failure
- **Process:** Same as `loginUser()` but uses `admins.txt`

**`void signupUser()`**
- **Purpose:** Register new user account
- **Process:**
  1. Collect username and password
  2. Append to `users.txt`
  3. Confirm registration

**`void signupAdmin()`**
- **Purpose:** Register new admin account
- **Process:**
  1. Collect username and password
  2. Append to `admins.txt`
  3. Confirm registration

---

### Category Management Module

Manages product categories stored in memory during runtime.

**Global Variables:**
```c
Category categories[MAX_CATEGORIES];  /*!< In-memory category storage */
int      categoryCount = 0;           /*!< Current number of categories */
```

#### Functions

**`int findCategoryIndexById(int id)`**
- **Purpose:** Search for category by ID
- **Parameters:** `id` - Category ID to find
- **Returns:** Array index if found (-1 otherwise)
- **Time Complexity:** O(n)

**`void addCategory()`**
- **Purpose:** Create new category
- **Process:**
  1. Check category limit (MAX_CATEGORIES = 50)
  2. Prompt user for category name
  3. Auto-assign ID = categoryCount + 1
  4. Add to memory storage
- **Side Effects:** Modifies `categoryCount`

**`void listCategories()`**
- **Purpose:** Display all categories
- **Output Format:**
  ```
  --- Category List ---
  1. Electronics
  2. Furniture
  ...
  ```

**`void editCategory()`**
- **Purpose:** Update category name
- **Process:**
  1. Prompt for category ID
  2. Search in memory
  3. Update name if found

**`void deleteCategory()`**
- **Purpose:** Remove category
- **Process:**
  1. Find category by ID
  2. Shift remaining elements left
  3. Decrement categoryCount

---

### Item Management Module

Manages inventory items with full CRUD operations.

**Global Variables:**
```c
Item items[MAX_ITEMS];     /*!< In-memory item storage */
int  itemCount = 0;        /*!< Current number of items */
```

#### Functions

**`int findItemIndexById(int id)`**
- **Purpose:** Locate item by ID in memory
- **Parameters:** `id` - Item ID
- **Returns:** Array index (-1 if not found)

**`void addItem()`**
- **Purpose:** Create new inventory item
- **Input Fields:**
  - Item name
  - Category ID (validates against existing categories)
  - Initial quantity
  - Reorder level
  - Unit price
- **Validation:** Checks category ID exists
- **Auto-Assignment:** ID = itemCount + 1

**`void listItems()`**
- **Purpose:** Display all items with details
- **Output Format:**
  ```
  --- Item List ---
  ID:1 | Item Name | Cat:1 | Qty:100 | Reorder:20 | Price:99.99
  ID:2 | Item Name | Cat:2 | Qty:50  | Reorder:10 | Price:149.99
  ```

**`void editItem()`**
- **Purpose:** Update item details
- **Editable Fields:**
  - Name
  - Category (with validation)
  - Quantity
  - Reorder level
  - Price

**`void deleteItem()`**
- **Purpose:** Remove item from inventory
- **Process:** Array element removal with shift

**`void searchItem()`**
- **Purpose:** Find items by partial name match
- **Matching:** Case-sensitive substring search
- **Output:** ID, name, and quantity of matches

---

### Stock Management Module

Handles stock-in and stock-out operations with validation and transaction recording.

#### Functions

**`int validateStock(Item *item, StockTransaction *tx)`**
- **Purpose:** Verify stock movement is valid
- **Validation Rules:**
  - For STOCK_OUT: Item quantity ≥ transaction amount
  - For STOCK_IN: No upper limit
- **Returns:** 1 if valid, 0 otherwise
- **Error Handling:** Displays "Not enough stock" message

**`void updateStock(Item *item, StockTransaction *tx)`**
- **Purpose:** Modify item quantity based on transaction
- **Logic:**
  - STOCK_IN: `quantity += amount`
  - STOCK_OUT: `quantity -= amount`
- **Output:** New quantity confirmation

**`void doStock(const char *username, const char *role, StockType type)`**
- **Purpose:** Execute stock-in or stock-out operation
- **Parameters:**
  - `username`: User performing operation
  - `role`: User role (User/Admin)
  - `type`: STOCK_IN or STOCK_OUT
- **Process:**
  1. Display items
  2. Collect item ID and quantity
  3. Validate transaction
  4. Update stock
  5. Record transaction
- **Validation:** Checks item exists, validates stock

**`void recordTransaction(StockTransaction *tx)`**
- **Purpose:** Persist transaction to binary file
- **Storage:** `transactions.dat`
- **Auto-Assignment:** Transaction ID based on last record
- **Output:** Success/failure message
- **Data Format:** Binary (not human-readable)

---

### Reports Module

Generates analysis reports from stored data.

#### Functions

**`void reportLowStock()`**
- **Purpose:** Identify items below reorder level
- **Display:** Items where `quantity ≤ reorder_level`
- **Fields Shown:** ID, name, current quantity, reorder level
- **Use Case:** Procurement planning

**`void reportTransactionsByItem()`**
- **Purpose:** View all transactions for specific item
- **Input:** Item ID
- **Output Format:**
  ```
  --- Transactions for Item ID 1 ---
  #1 | IN  | User:john | Qty:50
  #2 | OUT | User:jane | Qty:10
  ```
- **Data Source:** `transactions.dat` (binary file)

**`void reportTransactionsByUser()`**
- **Purpose:** View all transactions performed by user
- **Input:** Username
- **Output:** Transaction ID, item ID, type, quantity
- **Use Case:** Audit trail, user accountability

**`void reportMenu()`**
- **Purpose:** Report selection interface
- **Options:**
  1. Low Stock Items
  2. Transactions by Item
  3. Transactions by User
  4. Back to main menu

---

## API Reference

### Helper Functions

#### String and I/O

**`void inputString(char *s, int size)`**
- Reads string from stdin, removes newline
- Clears extra input from buffer
- Safe against buffer overflow

**`int appendRecord(const char *filename, const void *record, size_t size)`**
- Appends binary record to file
- Opens in append-binary mode ("ab")
- Returns 1 on success, 0 on failure

**`void *loadAllRecords(const char *filename, size_t size, int *countOut)`**
- Loads all binary records from file
- Allocates memory dynamically
- Sets countOut to number of records
- Returns NULL on failure
- **Caller Responsibility:** Must free returned pointer

---

## Function Documentation

### Authentication Functions

| Function | Purpose | Input | Output |
|----------|---------|-------|--------|
| `loginUser()` | User login | username, password | success (1/0) |
| `loginAdmin()` | Admin login | username, password | success (1/0) |
| `signupUser()` | User registration | username, password | confirmation |
| `signupAdmin()` | Admin registration | username, password | confirmation |

### Category Functions

| Function | Purpose |
|----------|---------|
| `addCategory()` | Create new category |
| `listCategories()` | Display all categories |
| `editCategory()` | Update category name |
| `deleteCategory()` | Remove category |
| `findCategoryIndexById()` | Search by ID |

### Item Functions

| Function | Purpose |
|----------|---------|
| `addItem()` | Create new item |
| `listItems()` | Display all items |
| `editItem()` | Update item details |
| `deleteItem()` | Remove item |
| `searchItem()` | Search by name |
| `findItemIndexById()` | Search by ID |

### Stock Functions

| Function | Purpose |
|----------|---------|
| `doStock()` | Execute stock transaction |
| `validateStock()` | Verify transaction validity |
| `updateStock()` | Modify quantity |
| `recordTransaction()` | Save to transaction log |

### Report Functions

| Function | Purpose |
|----------|---------|
| `reportLowStock()` | List low stock items |
| `reportTransactionsByItem()` | Item transaction history |
| `reportTransactionsByUser()` | User transaction history |
| `reportMenu()` | Report selection menu |

---

## User Guide

### System Requirements

- **Language:** C99 or later
- **Compiler:** GCC, MinGW, or compatible
- **Memory:** Minimum 1 MB
- **Disk:** Storage for credential and transaction files

### Compilation

```bash
gcc -o custode custode.c -Wall -Wextra
```

### Running the System

```bash
./custode          # Unix/Linux/Mac
custode.exe        # Windows
```

### Main Menu Navigation

```
*______________Custode_______________*
1. Sign Up          - Register new account
2. User Login       - Access as staff member
3. Admin Login      - Access as administrator
4. Exit             - Close application
```

### User (Staff) Operations

**Accessible Menus:**
- Add item stock (STOCK_IN)
- Remove item stock (STOCK_OUT)
- View inventory
- Search items

**Limitations:**
- Cannot create/edit categories
- Cannot delete items
- Cannot access admin functions

### Admin Operations

**Additional Permissions:**
- Full category management
- Full item management
- Item deletion
- Advanced reporting
- Transaction auditing

### Workflow Example: Adding Stock

1. Login as user/admin
2. Select "Stock In" option
3. View items
4. Enter item ID (e.g., 1)
5. Enter quantity (e.g., 50)
6. System validates and confirms
7. Transaction recorded automatically

### Workflow Example: Low Stock Report

1. Login as user/admin
2. Select "Reports" option
3. Choose "Low Stock Items"
4. System displays items where quantity ≤ reorder_level
5. Use results for procurement planning

---

## File Structure

### Project Files

```
custode/
├── custode.c              # Main source code
├── users.txt              # User credentials (created at runtime)
├── admins.txt             # Admin credentials (created at runtime)
└── transactions.dat       # Binary transaction log (created at runtime)
```

### File Descriptions

#### `custode.c`
- **Type:** C Source File
- **Lines:** ~600
- **Modules:** All modules included
- **Compilation:** Single file compilation

#### `users.txt`
- **Format:** Plain text
- **Structure:** `username password` (space-separated)
- **Created:** On first user signup
- **Example:**
  ```
  john password123
  jane secure456
  ```
- **Security Note:** Plaintext storage (improve for production)

#### `admins.txt`
- **Format:** Plain text
- **Structure:** `username password` (space-separated)
- **Created:** On first admin signup
- **Note:** Same format as users.txt

#### `transactions.dat`
- **Format:** Binary file
- **Record Size:** sizeof(StockTransaction)
- **Record Structure:** [transactionId][itemId][type][amount][username][role]
- **Created:** On first stock transaction
- **Advantages:** 
  - Efficient storage
  - Type-safe reading
  - Prevents text editing
- **Tool to View:** Custom reader program needed

---

## Configuration

### Build-Time Constants

```c
#define MAX_CATEGORIES 50      /* Maximum categories in memory */
#define MAX_ITEMS      200     /* Maximum items in memory */
#define MAX_NAME       100     /* Max length of names */
#define MAX_ROLE       10      /* Max length of role strings */
```

### File Names

```c
#define USER_TXT  "users.txt"      /* User credentials file */
#define ADMIN_TXT "admins.txt"     /* Admin credentials file */
```

### Database File

```c
const char *TRAN_FILE = "transactions.dat";  /* Transaction log */
```

### Customization Guide

**To Increase Item Capacity:**
```c
#define MAX_ITEMS 500  /* Change from 200 */
```

**To Change Credential File Location:**
```c
#define USER_TXT "path/to/users.txt"
#define ADMIN_TXT "path/to/admins.txt"
```

**To Change Transaction File:**
```c
const char *TRAN_FILE = "path/to/transactions.dat";
```

---

## Data Flow Diagrams

### Stock Transaction Flow

```
User Action (Stock-In/Out)
    ↓
Item Selection & Quantity Input
    ↓
Validation (Sufficient Stock?)
    ↓
        │ Invalid
        └─→ Error Message → Return
    │
    │ Valid
    ↓
Update Item Quantity in Memory
    ↓
Create StockTransaction Record
    ↓
Append to transactions.dat
    ↓
Confirmation Message
```

### Category CRUD Flow (Memory Only)

```
Add Category
    ↓
    • Check limit (50 max)
    • Get name from user
    • Auto-assign ID
    • Store in memory array
    • Increment categoryCount
    
Edit Category
    ↓
    • Search by ID
    • Update name in array
    
Delete Category
    ↓
    • Search by ID
    • Shift array elements
    • Decrement categoryCount
```

### Authentication Flow

```
User Input (username, password)
    ↓
Open credential file (users.txt/admins.txt)
    ↓
Read each line and compare
    ↓
        │ Match found
        └─→ Login success → Access granted
    │
    │ No match
    ↓
Login failed → Return to menu
```

---

## Limitations and Known Issues

### Current Limitations

1. **In-Memory Storage Only for Categories/Items**
   - Data lost on program exit unless added back
   - Recommend implementing file persistence

2. **Plaintext Password Storage**
   - Security risk for production systems
   - Implement hashing (bcrypt/SHA-256) for real deployment

3. **No Database Backend**
   - File-based storage limits scalability
   - Consider SQL database for larger systems

4. **Basic Input Validation**
   - Limited error checking
   - No regex validation for usernames/emails

5. **Single Instance Only**
   - Concurrent access not supported
   - No locking mechanism for files

6. **No Encryption**
   - Transaction file readable with binary tools
   - Data exposed if file accessed

### Potential Improvements

1. Implement SQLite database backend
2. Add password hashing with bcrypt
3. Implement file-based persistence for categories/items
4. Add data encryption
5. Implement multi-user concurrent access
6. Add role-based permissions at granular level
7. Implement audit logging
8. Add data backup functionality
9. Implement undo/redo functionality
10. Add export to CSV/PDF reports

---

## Troubleshooting

### Common Issues

| Issue | Cause | Solution |
|-------|-------|----------|
| "Category Not Found" | Invalid category ID | List categories first, use correct ID |
| "Not enough stock" | Insufficient quantity | Reduce STOCK_OUT amount or add stock |
| "Item limit reached" | MAX_ITEMS exceeded | Increase constant or delete items |
| File not found (users.txt) | First run | Sign up creates files automatically |
| Transaction not recorded | File write error | Check disk space and permissions |

### Debug Mode

To add debug output, compile with:
```bash
gcc -o custode custode.c -DDEBUG -Wall
```

Add debug prints in code:
```c
#ifdef DEBUG
printf("DEBUG: categoryCount = %d\n", categoryCount);
#endif
```

---

## API Usage Examples

### Example 1: Adding a Category

```c
// Internal function call
addCategory();
// Input: "Electronics"
// Output: "Category Added."
// Result: New Category with ID=1 added to memory
```

### Example 2: Finding an Item

```c
int idx = findItemIndexById(5);
if (idx != -1) {
    printf("Found: %s\n", items[idx].name);
} else {
    printf("Item not found\n");
}
```

### Example 3: Recording a Transaction

```c
StockTransaction tx;
tx.itemId = 1;
tx.type = STOCK_IN;
tx.amount = 50;
strcpy(tx.username, "john");
strcpy(tx.role, "User");
recordTransaction(&tx);
```

---

## References

### Data Structure Documentation

- **Enum:** StockType - Stock movement direction
- **Struct:** Category - Product category
- **Struct:** Item - Inventory item details
- **Struct:** StockTransaction - Audit trail record

### Key Modules

1. **Authentication:** User/Admin login and registration
2. **CRUD Operations:** Category and item management
3. **Stock Control:** Inventory updates and validation
4. **Transactions:** Record keeping and auditing
5. **Reports:** Analytics and business intelligence

### File Operations

- **Text Files:** users.txt, admins.txt (credential storage)
- **Binary Files:** transactions.dat (transaction log)
- **Memory Storage:** Global arrays for categories and items

---

## Conclusion

**Custode** is a functional inventory management system providing essential CRUD operations, role-based access control, and transaction auditing. The system is suitable for small to medium-scale inventory operations and can be extended with database backends and enhanced security measures for production use.

### Summary Statistics

- **Lines of Code:** ~600
- **Functions:** 40+
- **Data Structures:** 4 types
- **File Storage:** 3 file types
- **User Roles:** 2 (User, Admin)
- **Main Features:** 5 core modules

### Version History

| Version | Date | Changes |
|---------|------|---------|
| 1.0.0 | Dec 2025 | Initial release |

---

**Documentation Generated:** December 2025  
**Project Status:** Production Ready (with noted limitations)  
**License:** [Your License Here]  
**Author:** [Your Name/Team]

