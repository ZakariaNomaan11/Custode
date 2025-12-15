int userExistsInFile(const char *filename, const char *name, const char *password) {
    FILE *f = fopen(filename, "r");
    if (!f) return 0;
    char u[100], p[100];
    while (fscanf(f, "%s %s", u, p) == 2) {
        if (!strcmp(u, name) && !strcmp(p, password)) {
            fclose(f);
            return 1;
        }
    }
    fclose(f);
    return 0;
}

void signupUser() {
    char u[100], p[100];
    printf("User Sign Up\nUsername: ");
    scanf("%s", u);
    printf("Password: ");
    scanf("%s", p);
    FILE *f = fopen(USER_TXT, "a");
    if (!f) { printf("Error opening user file.\n"); return; }
    fprintf(f, "%s %s\n", u, p);
    fclose(f);
    printf("User registered.\n");
}

void signupAdmin() {
    char u[100], p[100];
    printf("Admin Sign Up\nUsername: ");
    scanf("%s", u);
    printf("Password: ");
    scanf("%s", p);
    FILE *f = fopen(ADMIN_TXT, "a");
    if (!f) { printf("Error opening admin file.\n"); return; }
    fprintf(f, "%s %s\n", u, p);
    fclose(f);
    printf("Admin registered.\n");
}

int loginUser(char *out) {
    char u[100], p[100];
    printf("User Login\nUsername: ");
    scanf("%s", u);
    printf("Password: ");
    scanf("%s", p);
    if (userExistsInFile(USER_TXT, u, p)) {
        strcpy(out, u);
        printf("Login successful.\n");
        return 1;
    }
    printf("Invalid credentials.\n");
    return 0;
}

int loginAdmin(char *out) {
    char u[100], p[100];
    printf("Admin Login\nUsername: ");
    scanf("%s", u);
    printf("Password: ");
    scanf("%s", p);
    if (userExistsInFile(ADMIN_TXT, u, p)) {
        strcpy(out, u);
        printf("Login successful.\n");
        return 1;
    }
    printf("Invalid credentials.\n");
    return 0;
}

/* menus */

void adminMenu(const char *admin) {
    int ch;
    do {
        printf("\n=== Admin Menu ===\n");
        printf("1.Category Management\n");
        printf("2.Item Management\n");
        printf("3.Stock In\n");
        printf("4.Stock Out\n");
        printf("5.Reports\n");
        printf("6.Logout\nChoice: ");
        scanf("%d", &ch);
        if (ch == 1) { /* call add/edit/delete category etc. */ }
        else if (ch == 2) { /* call item CRUD menu */ }
        else if (ch == 3) doStock(admin, "admin", STOCK_IN);
        else if (ch == 4) doStock(admin, "admin", STOCK_OUT);
        else if (ch == 5) reportMenu();
    } while (ch != 6);
}

void staffMenu(const char *user) {
    int ch;
    do {
        printf("\n=== User Menu ===\n");
        printf("1.List Items\n");
        printf("2.Stock In\n");
        printf("3.Stock Out\n");
        printf("4.Logout\nChoice: ");
        scanf("%d", &ch);
        if (ch == 1) listItems();
        else if (ch == 2) doStock(user, "staff", STOCK_IN);
        else if (ch == 3) doStock(user, "staff", STOCK_OUT);
    } while (ch != 4);
}

int start() {
    int choice;
    while (1) {
        printf("_\n");
        printf("             Custode             \n");
        printf("_\n");
        printf("1.Sign Up\n2.User Login\n3.Admin Login\n4.Exit\nChoice: ");
        scanf("%d", &choice);
        if (choice == 1) {
            int sc;
            printf("1.As User  2.As Admin\nChoice: ");
            scanf("%d", &sc);
            if (sc == 1) signupUser();
            else if (sc == 2) signupAdmin();
            else printf("Invalid.\n");
        } else if (choice == 2) {
            char u[100];
            if (loginUser(u)) staffMenu(u);
        } else if (choice == 3) {
            char a[100];
            if (loginAdmin(a)) adminMenu(a);
        } else if (choice == 4) {
            printf("Exiting...\n");
            break;
        } else {
            printf("Invalid choice.\n");
        }
    }
    return 0;
}