#include <stdio.h>
#include <string.h>
#define TEST_BUILD
#include "custode.c"   /* your full code above, but wrap its main/start in #ifndef TEST_BUILD */

/* Helper to reset in-memory data for tests */
void resetData() {
    categoryCount = 0;
    itemCount = 0;
}
int test13_userExistsInFile_false_when_empty() {
    remove(USER_TXT);
    int actual   = (userExistsInFile(USER_TXT, "x", "y") == 0);
    int expected = 1;
    int success  = (actual == expected);

    printf("Member5_test13_userExistsInFile_empty -> expected:%d actual:%d success:%d\n",
           expected, actual, success);
    return success;
}

int test14_signupUser_and_loginUser() {
    remove(USER_TXT);

    printf("\n[Member5] For signupUser type: testuser⏎ then testpass⏎\n");
    signupUser();

    char out[100];
    printf("[Member5] For loginUser type: testuser⏎ then testpass⏎\n");
    int ok = loginUser(out);

    int actual   = (ok == 1 && strcmp(out, "testuser") == 0);
    int expected = 1;
    int success  = (actual == expected);

    printf("Member5_test14_signupUser_and_loginUser -> expected:%d actual:%d success:%d\n",
           expected, actual, success);
    return success;
}

int test15_signupAdmin_and_loginAdmin() {
    remove(ADMIN_TXT);

    printf("\n[Member5] For signupAdmin type: admin1⏎ then apass⏎\n");
    signupAdmin();

    char out[100];
    printf("[Member5] For loginAdmin type: admin1⏎ then apass⏎\n");
    int ok = loginAdmin(out);

    int actual   = (ok == 1 && strcmp(out, "admin1") == 0);
    int expected = 1;
    int success  = (actual == expected);

    printf("Member5_test15_signupAdmin_and_loginAdmin -> expected:%d actual:%d success:%d\n",
           expected, actual, success);
    return success;
}
int main(){
test13_userExistsInFile_false_when_empty();
    test14_signupUser_and_loginUser();
    test15_signupAdmin_and_loginAdmin();
    return 0;
}