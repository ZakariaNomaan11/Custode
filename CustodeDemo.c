#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_USERS     100
#define MAX_ADMINS    50
#define MAX_ROUTES    50
#define MAX_VEHICLES  50
#define MAX_TRIPS     100
#define MAX_BOOKINGS  500

/* ---------- DATA STRUCTURES ---------- */

typedef struct {
    int id;
    char username[50];
    char password[50];
} User;

typedef struct {
    int id;
    char username[50];
    char password[50];
} Admin;

typedef struct {
    int id;
    char from[50];
    char to[50];
    float distance;
} Route;

typedef struct {
    int id;
    char regNumber[20];
    int capacity;
    char type[20];
} Vehicle;

typedef struct {
    int id;
    int routeId;
    int vehicleId;
    char departure[20];
    float fare;
    int availableSeats;
} Trip;

typedef struct {
    int id;
    int userId;
    int tripId;
    int seats;
    char status[20];
} Booking;

/* ---------- GLOBAL DATABASE ---------- */

User users[MAX_USERS];
int numUsers = 0;

Admin admins[MAX_ADMINS];
int numAdmins = 0;

Route routes[MAX_ROUTES];
int numRoutes = 0;

Vehicle vehicles[MAX_VEHICLES];
int numVehicles = 0;

Trip trips[MAX_TRIPS];
int numTrips = 0;

Booking bookings[MAX_BOOKINGS];
int numBookings = 0;

/* ---------- LOADING / SAVING (DB) ---------- */
/* You already have users/admins load/save – extend similarly for trips/bookings if needed */

void loadAllData(void) {
    FILE *fp;

    if ((fp = fopen("users.txt", "r"))) {
        while (numUsers < MAX_USERS &&
               fscanf(fp, "%d %49s %49s",
                      &users[numUsers].id,
                      users[numUsers].username,
                      users[numUsers].password) == 3) {
            numUsers++;
        }
        fclose(fp);
    }

    if ((fp = fopen("admins.txt", "r"))) {
        while (numAdmins < MAX_ADMINS &&
               fscanf(fp, "%d %49s %49s",
                      &admins[numAdmins].id,
                      admins[numAdmins].username,
                      admins[numAdmins].password) == 3) {
            numAdmins++;
        }
        fclose(fp);
    }

    /* For demo: initialize some trips in memory (like your first code) */
    trips[0] = (Trip){1, 0, 0, "10:00", 500, 40};
    trips[1] = (Trip){2, 0, 0, "12:00", 600, 30};
    trips[2] = (Trip){3, 0, 0, "14:00", 300, 20};
    trips[3] = (Trip){4, 0, 0, "16:00", 450, 50};
    trips[4] = (Trip){5, 0, 0, "18:00", 550, 25};
    numTrips = 5;
}

void saveAllData(void) {
    FILE *fp = fopen("users.txt", "w");
    if (fp) {
        for (int i = 0; i < numUsers; i++) {
            fprintf(fp, "%d %s %s\n",
                    users[i].id,
                    users[i].username,
                    users[i].password);
        }
        fclose(fp);
    }

    fp = fopen("admins.txt", "w");
    if (fp) {
        for (int i = 0; i < numAdmins; i++) {
            fprintf(fp, "%d %s %s\n",
                    admins[i].id,
                    admins[i].username,
                    admins[i].password);
        }
        fclose(fp);
    }

    /* Extend to save trips/bookings if you want persistent bookings */
}

/* ---------- VALIDATION HELPERS (DB CHECKS) ---------- */

int findUserByUsername(const char *name) {
    for (int i = 0; i < numUsers; i++) {
        if (strcmp(users[i].username, name) == 0)
            return i;
    }
    return -1;
}

int findAdminByUsername(const char *name) {
    for (int i = 0; i < numAdmins; i++) {
        if (strcmp(admins[i].username, name) == 0)
            return i;
    }
    return -1;
}

int findTripById(int id) {
    for (int i = 0; i < numTrips; i++) {
        if (trips[i].id == id)
            return i;
    }
    return -1;
}

int findBookingById(int id) {
    for (int i = 0; i < numBookings; i++) {
        if (bookings[i].id == id)
            return i;
    }
    return -1;
}

/* ---------- SIGN UP / LOGIN (VALIDATED) ---------- */

void userSignUp(void) {
    char username[50], password[50];

    printf("\n--- User Sign Up ---\n");
    printf("Choose Username: ");
    scanf("%49s", username);
    printf("Choose Password: ");
    scanf("%49s", password);

    if (findUserByUsername(username) != -1) {
        printf("Error: Username already exists!\n");
        return;
    }

    if (numUsers >= MAX_USERS) {
        printf("Error: User database full!\n");
        return;
    }

    users[numUsers].id = numUsers + 1;
    strcpy(users[numUsers].username, username);
    strcpy(users[numUsers].password, password);
    numUsers++;

    saveAllData();
    printf("User registered successfully!\n");
}

void adminSignUp(void) {
    char username[50], password[50];

    printf("\n--- Admin Sign Up ---\n");
    printf("Choose Admin Username: ");
    scanf("%49s", username);
    printf("Choose Password: ");
    scanf("%49s", password);

    if (findAdminByUsername(username) != -1) {
        printf("Error: Admin username already exists!\n");
        return;
    }

    if (numAdmins >= MAX_ADMINS) {
        printf("Error: Admin database full!\n");
        return;
    }

    admins[numAdmins].id = numAdmins + 1;
    strcpy(admins[numAdmins].username, username);
    strcpy(admins[numAdmins].password, password);
    numAdmins++;

    saveAllData();
    printf("Admin registered successfully!\n");
}

int userLogin(void) {
    char username[50], password[50];

    printf("\n--- User Login ---\n");
    printf("Enter Username: ");
    scanf("%49s", username);
    printf("Enter Password: ");
    scanf("%49s", password);

    int idx = findUserByUsername(username);
    if (idx != -1 && strcmp(users[idx].password, password) == 0) {
        printf("Login successful! Welcome %s\n", username);
        return users[idx].id;
    }

    printf("Error: Invalid username or password!\n");
    return -1;
}

int adminLogin(void) {
    char username[50], password[50];

    printf("\n--- Admin Login ---\n");
    printf("Enter Admin Username: ");
    scanf("%49s", username);
    printf("Enter Password: ");
    scanf("%49s", password);

    int idx = findAdminByUsername(username);
    if (idx != -1 && strcmp(admins[idx].password, password) == 0) {
        printf("Admin login successful! Welcome %s\n", username);
        return admins[idx].id;
    }

    printf("Error: Invalid admin credentials!\n");
    return -1;
}

/* ---------- BOOKING LOGIC (VALIDATED) ---------- */

int makeBooking(int userId, int tripId, int seats) {
    if (numBookings >= MAX_BOOKINGS) {
        printf("Error: Booking database full!\n");
        return 0;
    }

    int tIndex = findTripById(tripId);
    if (tIndex == -1) {
        printf("Error: Trip not found!\n");
        return 0;
    }

    if (seats <= 0) {
        printf("Error: Seats must be positive!\n");
        return 0;
    }

    if (trips[tIndex].availableSeats < seats) {
        printf("Error: Not enough seats available!\n");
        return 0;
    }

    trips[tIndex].availableSeats -= seats;

    bookings[numBookings].id = numBookings + 1;
    bookings[numBookings].userId = userId;
    bookings[numBookings].tripId = tripId;
    bookings[numBookings].seats = seats;
    strcpy(bookings[numBookings].status, "confirmed");
    numBookings++;

    printf("Booking confirmed! ID: %d\n", numBookings);
    return 1;
}

void cancelBooking(int userId) {
    int id;
    printf("Enter Booking ID to cancel (0 to go back): ");
    scanf("%d", &id);
    if (id == 0) return; /* back */

    int bIndex = findBookingById(id);
    if (bIndex == -1 || bookings[bIndex].userId != userId) {
        printf("Error: Booking not found for this user!\n");
        return;
    }

    int tIndex = findTripById(bookings[bIndex].tripId);
    if (tIndex != -1) {
        trips[tIndex].availableSeats += bookings[bIndex].seats;
    }

    /* shift bookings */
    for (int i = bIndex; i < numBookings - 1; i++) {
        bookings[i] = bookings[i + 1];
    }
    numBookings--;

    printf("Booking %d cancelled.\n", id);
}

/* ---------- VIEW FUNCTIONS ---------- */

void showTrips(void) {
    printf("\n--- Available Trips ---\n");
    for (int i = 0; i < numTrips; i++) {
        printf("TripID:%d Fare:%.2f Seats:%d\n",
               trips[i].id, trips[i].fare, trips[i].availableSeats);
    }
}

void showUserBookings(int userId) {
    printf("\n--- Your Bookings ---\n");
    int count = 0;
    for (int i = 0; i < numBookings; i++) {
        if (bookings[i].userId == userId) {
            printf("BookingID:%d TripID:%d Seats:%d Status:%s\n",
                   bookings[i].id, bookings[i].tripId,
                   bookings[i].seats, bookings[i].status);
            count++;
        }
    }
    if (count == 0) printf("No bookings.\n");
}

/* ---------- ADMIN REPORT ---------- */

void adminReport(void) {
    float total = 0;
    printf("\n--- Admin Report ---\n");
    for (int i = 0; i < numTrips; i++) {
        int booked = 0;
        for (int j = 0; j < numBookings; j++) {
            if (bookings[j].tripId == trips[i].id &&
                strcmp(bookings[j].status, "confirmed") == 0) {
                booked += bookings[j].seats;
            }
        }
        float revenue = booked * trips[i].fare;
        total += revenue;
        printf("TripID:%d Passengers:%d Revenue:%.2f\n",
               trips[i].id, booked, revenue);
    }
    printf("Total Revenue: %.2f\n", total);
}

/* ---------- MENUS WITH BACK OPTION ---------- */

void userMenu(int userId) {
    int choice;
    while (1) {
        printf("\n--- User Menu ---\n");
        printf("1. View Trips\n");
        printf("2. Make Booking\n");
        printf("3. View My Bookings\n");
        printf("4. Cancel Booking\n");
        printf("0. Back\n");
        printf("Your Choice: ");
        scanf("%d", &choice);

        if (choice == 0) return; /* back to main */

        if (choice == 1) {
            showTrips();
        } else if (choice == 2) {
            int tripId, seats;
            showTrips();
            printf("Enter Trip ID (0 to back): ");
            scanf("%d", &tripId);
            if (tripId == 0) continue;
            printf("Enter Seats: ");
            scanf("%d", &seats);
            makeBooking(userId, tripId, seats);
        } else if (choice == 3) {
            showUserBookings(userId);
        } else if (choice == 4) {
            cancelBooking(userId);
        } else {
            printf("Invalid choice.\n");
        }
    }
}

void adminMenu(int adminId) {
    int choice;
    while (1) {
        printf("\n--- Admin Menu ---\n");
        printf("1. View Trips\n");
        printf("2. Admin Report\n");
        printf("0. Back\n");
        printf("Your Choice: ");
        scanf("%d", &choice);

        if (choice == 0) return;

        if (choice == 1) {
            showTrips();
        } else if (choice == 2) {
            adminReport();
        } else {
            printf("Invalid choice.\n");
        }
    }
}

/* ---------- MAIN ---------- */

int main(void) {
    loadAllData();

    int mainChoice;

    while (1) {
        printf("\n===============================\n");
        printf("      Transport Booking\n");
        printf("===============================\n");
        printf("1. Sign Up\n");
        printf("2. Login\n");
        printf("3. Exit\n");
        printf("Your Choice: ");
        scanf("%d", &mainChoice);

        if (mainChoice == 1) {
            int signupChoice;
            printf("\n1. Sign Up as User\n");
            printf("2. Sign Up as Admin\n");
            printf("0. Back\n");
            printf("Your Choice: ");
            scanf("%d", &signupChoice);

            if (signupChoice == 1) userSignUp();
            else if (signupChoice == 2) adminSignUp();
            else if (signupChoice == 0) continue;
            else printf("Invalid choice.\n");
        } else if (mainChoice == 2) {
            int loginChoice;
            printf("\n1. User Login\n");
            printf("2. Admin Login\n");
            printf("0. Back\n");
            printf("Your Choice: ");
            scanf("%d", &loginChoice);

            if (loginChoice == 1) {
                int userId = userLogin();
                if (userId != -1) userMenu(userId);
            } else if (loginChoice == 2) {
                int adminId = adminLogin();
                if (adminId != -1) adminMenu(adminId);
            } else if (loginChoice == 0) {
                continue;
            } else {
                printf("Invalid choice.\n");
            }
        } else if (mainChoice == 3) {
            printf("Thank you for using our system!\n");
            break;
        } else {
            printf("Invalid choice.\n");
        }
    }

    saveAllData();
    return 0;
}
