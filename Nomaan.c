#include <stdio.h>
#include <stdlib.h>

int main(){
    printf("_________________________________\n");
    printf("            Bookers              \n");
    printf("_________________________________\n");
    
    int choice = 0;
    printf("Your Choice:\n");
    printf("1. Sign Up\n");
    printf("2. User Login\n");
    printf("3. Admin Login\n");
    scanf("%d", &choice);

    if(choice == 1){
        printf("1. As User\n");
        printf("2. As Admin\n");
        int signup_choice = 0;
        scanf("%d", &signup_choice);
        
        if(signup_choice == 1){
            char username[1000];
            char password[100];
            printf("User Sign Up Selected\n");
            printf("Choose Username: ");
            scanf("%s", username);
            printf("Choose Password: ");
            scanf("%s", password);
            
            FILE *f = fopen("users.txt", "a");
            if(f == NULL){
                printf("Error opening file!\n");
                return 1;
            }
            fprintf(f, "%s %s\n", username, password);
            fclose(f);
            printf("User registered successfully!\n");
        }
        else if(signup_choice == 2){
            char admin[1000];
            char password[100];
            printf("Admin Sign Up Selected\n");
            printf("Choose admin: ");
            scanf("%s", admin);
            printf("Choose Password: ");
            scanf("%s", password);
            
            FILE *f = fopen("admins.txt", "a");
            if(f == NULL){
                printf("Error opening file!\n");
                return 1;
            }
            fprintf(f, "%s %s\n", admin, password);
            fclose(f);
            printf("Admin registered successfully!\n");
        }
        else{
            printf("Invalid Sign Up Choice\n");
        }
    }
    else if(choice == 2){
        char username[1000];
        char password[100];
        printf("User Login Selected\n");
        printf("Please enter your Username: ");
        scanf("%s", username);
        printf("Please enter your Password: ");
        scanf("%s", password);
    }
    else if(choice == 3){
        char admin[1000];
        char password[100];
        printf("Admin Login Selected\n");
        printf("Please enter your admin username: ");
        scanf("%s", admin);
        printf("Please enter your Password: ");
        scanf("%s", password);
    }
    else{
        printf("Invalid Choice\n");
    }
    
    return 0;
}
