#include <stdio.h>
#include <string.h>
#define TEST_BUILD
#include "Sristy.c"  


void resetData() {
    int categoryNum = 0;
    int num = 0;
}
int userfile_false_when_empty() {
    remove(USER_TXT);
    int actual   = (userfile(USER_TXT, "x", "y") == 0);
    int expected = 1;
    int success  = (actual == expected);

    printf("Member5_userfile_empty -> expected:%d actual:%d success:%d\n",
           expected, actual, success);
    return success;
}

int test14_userSignup_and_userLogin() {
    remove(USER_TXT);

    printf("\n[Member5] For userSignup type: testuser then testpass\n");
    userSignup();

    char out[100];
    printf("[Member5] For userLogin type: testuser then testpass\n");
    int ok = userLogin(out);

    int actual   = (ok == 1 && strcmp(out, "testuser") == 0);
    int expected = 1;
    int success  = (actual == expected);

    printf("Member5_test14_userSignup_and_userLogin -> expected:%d actual:%d success:%d\n",
           expected, actual, success);
    return success;
}

int test15_signupAdmin_and_adminLogin() {
    remove(ADMIN_TXT);

    printf("\n[Member5] For signupAdmin type: admin1 then apass\n");
    signupAdmin();

    char out[100];
    printf("[Member5] For adminLogin type: admin1 then apass\n");
    int ok = adminLogin(out);

    int actual   = (ok == 1 && strcmp(out, "admin1") == 0);
    int expected = 1;
    int success  = (actual == expected);

    printf("Member5_test15_signupAdmin_and_adminLogin -> expected:%d actual:%d success:%d\n",
           expected, actual, success);
    return success;
}
int main(){
userfile_false_when_empty();
    test14_userSignup_and_userLogin();
    test15_signupAdmin_and_adminLogin();
    return 0;
}