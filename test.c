#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_USERS 100

typedef struct {
    char name[50];
    float weight;   // in kg
    float height;   // in meters
    int age;        // in years
    char gender;    // 'M' for male, 'F' for female
    float BMI;
    float BFP;
    float BMR;
    float RMR;
} User;

float calculate_BMI(float weight, float height) {
    return weight / (height * height);
}

float calculate_BFP(float BMI, int age, char gender) {
    if (gender == 'M' || gender == 'm') {
        return 1.20 * BMI + (0.23 * age) - 16.2;
    } else if (gender == 'F' || gender == 'f') {
        return 1.20 * BMI + (0.23 * age) - 5.4;
    }
    return -1; // Error case
}

float calculate_BMR(float weight, float height_cm, int age, char gender) {
    if (gender == 'M' || gender == 'm') {
        return 88.362 + (13.397 * weight) + (4.799 * height_cm) - (5.677 * age);
    } else if (gender == 'F' || gender == 'f') {
        return 447.593 + (9.247 * weight) + (3.098 * height_cm) - (4.330 * age);
    }
    return -1; // Error case
}

float calculate_RMR(float weight, float height_cm, int age, char gender) {
    return calculate_BMR(weight, height_cm, age, gender);
}

void categorize_health(User *user) {
    int normal_count = 0;

    if (user->BMI >= 18.5 && user->BMI <= 25)
        normal_count++;
    if ((user->gender == 'M' && user->BFP >= 14 && user->BFP <= 17) ||
        (user->gender == 'F' && user->BFP >= 21 && user->BFP <= 24))
        normal_count++;
    if (user->BMR >= 1000 && user->BMR <= 2000)
        normal_count++;
    if ((user->gender == 'M' && user->RMR >= 1200 && user->RMR <= 1600) ||
        (user->gender == 'F' && user->RMR >= 1000 && user->RMR <= 1200))
        normal_count++;

    switch (normal_count) {
        case 4: printf("Category: Green (All metrics normal)\n"); break;
        case 3: printf("Category: Yellow (Three metrics normal)\n"); break;
        case 2: printf("Category: Orange (Two metrics normal)\n"); break;
        case 1: printf("Category: Red (One metric normal)\n"); break;
        default: printf("Category: Critical (No metrics normal)\n"); break;
    }
}

void save_users(User users[], int user_count, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        printf("Error opening file for writing.\n");
        return;
    }
    for (int i = 0; i < user_count; i++) {
        fprintf(file, "%s %.2f %.2f %d %c %.2f %.2f %.2f %.2f\n",
                users[i].name, users[i].weight, users[i].height, users[i].age,
                users[i].gender, users[i].BMI, users[i].BFP, users[i].BMR, users[i].RMR);
    }
    fclose(file);
}

int load_users(User users[], const char *filename) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        printf("Error opening file for reading.\n");
        return 0;
    }
    int user_count = 0;
    while (fscanf(file, "%s %f %f %d %c %f %f %f %f",
                  users[user_count].name, &users[user_count].weight,
                  &users[user_count].height, &users[user_count].age,
                  &users[user_count].gender, &users[user_count].BMI,
                  &users[user_count].BFP, &users[user_count].BMR,
                  &users[user_count].RMR) == 9) {
        user_count++;
    }
    fclose(file);
    return user_count;
}

int main() {
    User users[MAX_USERS];
    int user_count = 0;

    user_count = load_users(users, "users.txt");

    printf("Welcome to the Health Management System\n");

    while (1) {
        printf("\nMenu:\n1. Add User\n2. Show Users\n3. Save and Exit\n");
        printf("Enter your choice: ");
        int choice;
        scanf("%d", &choice);

        if (choice == 1) {
            if (user_count >= MAX_USERS) {
                printf("User limit reached.\n");
                continue;
            }

            User new_user;
            printf("Enter Name: ");
            scanf("%s", new_user.name);
            printf("Enter Weight (kg): ");
            scanf("%f", &new_user.weight);
            printf("Enter Height (m): ");
            scanf("%f", &new_user.height);
            printf("Enter Age (years): ");
            scanf("%d", &new_user.age);
            printf("Enter Gender (M/F): ");
            scanf(" %c", &new_user.gender);

            // Validate data
            if (new_user.weight <= 0 || new_user.height <= 0 || new_user.age <= 0) {
                printf("Invalid input. Please try again.\n");
                continue;
            }

            // Calculate metrics
            new_user.BMI = calculate_BMI(new_user.weight, new_user.height);
            new_user.BFP = calculate_BFP(new_user.BMI, new_user.age, new_user.gender);
            new_user.BMR = calculate_BMR(new_user.weight, new_user.height * 100, new_user.age, new_user.gender);
            new_user.RMR = calculate_RMR(new_user.weight, new_user.height * 100, new_user.age, new_user.gender);

            users[user_count++] = new_user;

            printf("User added successfully!\n");
            categorize_health(&new_user);

        } else if (choice == 2) {
            for (int i = 0; i < user_count; i++) {
                printf("Name: %s, BMI: %.2f, BFP: %.2f, BMR: %.2f, RMR: %.2f\n",
                       users[i].name, users[i].BMI, users[i].BFP, users[i].BMR, users[i].RMR);
            }

        } else if (choice == 3) {
            save_users(users, user_count, "users.txt");
            printf("Data saved. Exiting...\n");
            break;
        } else {
            printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}

