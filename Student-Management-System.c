#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


typedef enum {
    ADD_STUDENT = 1,
    VIEW_STUDENT,
    DELETE_STUDENT,
    EDIT_STUDENT_INFO,
    LIST_STUDENTS,
    RANK_STUDENTS,
    EXIT
} OPTIONS;

typedef struct {
    char name[50];
    int age;
    int id;
    float grade;
    char yearLevel[20];
    bool isEnrolled;
} Student;

typedef struct {
    Student *students;
    int count;
    int capacity;
    int lastID;
} StudentDatabase;

void menu();
void initSetup(StudentDatabase *db);
void printStudent(Student student);
void initDatabase(StudentDatabase *db, int initialCapacity);
void resizeDatabase(StudentDatabase *db);
void addStudent(StudentDatabase *db);
void viewStudent(StudentDatabase *db);
void trimWhitespace(char *str);
void removeStudent(StudentDatabase *db);
bool endSession();

int main() {

    StudentDatabase myDB;
    
    int choice;
    
    initSetup(&myDB);
    
    do{
        menu();
        printf("Enter: ");
        scanf("%d", &choice);
        
        switch (choice) {
            case ADD_STUDENT:
                addStudent(&myDB);
                if (endSession() == false) {
                   addStudent(&myDB);
                }
                break;
            case VIEW_STUDENT:
                viewStudent(&myDB);
                if (endSession() == false) {
                   viewStudent(&myDB);
                }
                break;
            case DELETE_STUDENT:
                printf("Removing Student\n");
                break;
            case EDIT_STUDENT_INFO:
                printf("Editing Student Info\n");
                break;
            case LIST_STUDENTS:
                printf("Viewing ng Student List\n");
                break;
            case RANK_STUDENTS:
                printf("Viewing Student Ranking\n");
                break;
            case EXIT:
                printf("\nExiting...");
                exit(0);
            default:
                printf("\nInvalid Choice Selected! Try again.\n\n");    
        }
    } while(choice > 0 || choice <= 7);
    
    free(myDB.students);
    return 0;
}

void menu() {
    printf("\nStudent Management System\n");
    printf("1. Add Student\n");
    printf("2. View Student Info\n");
    printf("3. Remove Student\n");
    printf("4. Edit Student Info\n");
    printf("5. View Student List\n");
    printf("6. Student Ranking\n");
    printf("7. Exit\n");
    printf("\n");
}

bool endSession() {
        
        char confirm[10];
        
        printf("\nGo back to menu? (Y) Do another task(any key): ");
        scanf("%s", confirm);

        if (strcasecmp(confirm, "Y") == 0 || strcasecmp(confirm, "YES") == 0) {
            system("clear");  // use "cls" if on Windows
            return true;
        }  
        return false;
}

void printStudent(Student student) {
    system("clear");
    printf("Name: %s\n", student.name);
    printf("ID: %d\n", student.id);
    printf("Age: %d\n", student.age);
    printf("Year Level: %s\n", student.yearLevel);
    printf("Grade: %.2f\n", student.grade);
    printf("Status: %s\n", student.isEnrolled ? "Enrolled" : "Not Enrolled");
    printf("\n");
}

void initDatabase(StudentDatabase *db, int initialCapacity) {
    db->count = 0;
    db->capacity = initialCapacity;
    db->lastID = 100;
    
    db->students = (Student *)malloc(initialCapacity * sizeof(Student));
    
    if (db->students == NULL) {
        printf("Memory Allocation Failed!\n");
        exit(1);
    }
}

void resizeDatabase(StudentDatabase *db) {
    system("clear");
    
    db->capacity *=2;
    printf("Resizing database capacity to: %d\n", db->capacity);
    
    Student *temp = (Student *)realloc(db->students, db->capacity * sizeof(Student));
    
    if (temp == NULL) {
        printf("Memory Allocation Failed!\n");
        return;
    }
    db->students = temp;
}

void addStudent(StudentDatabase *db) {
    system("clear");
    
    if (db->count >= db->capacity) {
        resizeDatabase(db);
    }
    
    Student student;
    char answer[5];
    
    db->lastID++;
    student.id = db->lastID;
    printf("Generated Student ID: %d\n\n", student.id);
    
    printf("Enter student name: ");
    while (getchar() != '\n');
    fgets(student.name, sizeof(student.name), stdin);
    trimWhitespace(student.name);;
    
    printf("Enter student age: ");
    scanf("%d", &student.age);
    
    printf("Enter student Year Level: ");
    while (getchar() != '\n');
    fgets(student.yearLevel, sizeof(student.yearLevel), stdin);
    trimWhitespace(student.yearLevel);
    
    printf("Enter student grade: ");
    scanf("%f", &student.grade);
    
    printf("Is the student enrolled (Y/N): ");
    
    scanf("%s", &answer);
    
    student.isEnrolled = (answer[0] == 'Y' || answer[0] == 'y');
    
    db->students[db->count++] = student;
    printf("\nStudent added successfully.\n");
    
}

void initSetup(StudentDatabase *db) {
    int amount;
    
    printf("How many students are expected: ");
    scanf("%d", &amount);
    
    if (amount < 1) {
        printf("Invalid amount. Defaulting to 1.\n");
        amount = 1;
    }
    
    initDatabase(db, amount);
}

void viewStudent(StudentDatabase *db) {
    system("clear");
    char lookupName[50];
    
    printf("Enter student name: ");
    while (getchar() != '\n');
    fgets(lookupName, sizeof(lookupName), stdin);
    trimWhitespace(lookupName);
    
    for (int i=0; i< db->count; i++) {
        if(strcmp(lookupName, db->students[i].name) == 0) {
            printStudent(db->students[i]);
            return;
        }
    }
    printf("Student not found.\n");
}

void trimWhitespace(char *str) {
    // Trim trailing whitespace
    int len = strlen(str);
    while (len > 0 && (str[len - 1] == ' ' || str[len - 1] == '\n' || str[len - 1] == '\t')) {
        str[len - 1] = '\0';
        len--;
    }

    // Trim leading whitespace
    int start = 0;
    while (str[start] == ' ' || str[start] == '\t') {
        start++;
    }

    if (start > 0) {
        memmove(str, str + start, strlen(str) - start + 1);
    }
}

void removeStudent(StudentDatabase *db) {
    system("clear");
    char lookupName[50];

    printf("Enter student name to remove: ");
    while (getchar() != '\n');
    fgets(lookupName, sizeof(lookupName), stdin);
    trimWhitespace(lookupName);

    for (int i = 0; i < db->count; i++) {
        if (strcmp(lookupName, db->students[i].name) == 0) {
            for (int j = i; j < db->count - 1; j++) {
                db->students[j] = db->students[j + 1];
            }
            db->count--;
            printf("Student removed successfully.\n");
            return;
        }
    }
    printf("Student not found.\n");
}