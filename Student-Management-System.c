#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>


typedef enum {
    ADD_STUDENT = 1,
    VIEW_STUDENT,
    DELETE_STUDENT,
    EDIT_STUDENT_INFO,
    LIST_STUDENTS,
    RANK_STUDENTS,
    READ_FILE,
    EXPORT_FILE,
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
void askDetailsToEditMenu();
void initSetup(StudentDatabase *db);

void printStudent(Student student);
void initDatabase(StudentDatabase *db, int initialCapacity);
void resizeDatabase(StudentDatabase *db);
void addStudent(StudentDatabase *db);
void viewStudent(StudentDatabase *db);
void removeStudent(StudentDatabase *db);
void editStudentInfo(StudentDatabase *db);

void trimWhitespace(char *str);
bool endSession();
void timeDelay(double seconds);

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
                while (endSession() == false) {
                   addStudent(&myDB);
                }
                break;
            case VIEW_STUDENT:
                viewStudent(&myDB);
                while (endSession() == false) {
                   viewStudent(&myDB);
                }
                break;
            case DELETE_STUDENT:
                removeStudent(&myDB);
                while(endSession() == false) {
                    removeStudent(&myDB);
                }
                break;
            case EDIT_STUDENT_INFO:
                editStudentInfo(&myDB);
                while (endSession() == false) {
                   editStudentInfo(&myDB);
                   }
                break;
            case LIST_STUDENTS:
                printf("Viewing ng Student List\n");
                break;
            case RANK_STUDENTS:
                printf("Viewing Student Ranking\n");
                break;
            case READ_FILE:
                printf("Reading File\n");
                break;
            case EXPORT_FILE:
                printf("Generating File\n");
                break;
            case EXIT:
                printf("\nExiting...");
                exit(0);
            default:
                printf("\nInvalid Choice Selected! Try again.\n\n");    
        }
    } while(choice > 0 || choice <= 9);
    
    free(myDB.students);
    return 0;
}

void timeDelay(double seconds) {
    struct timespec ts;
    ts.tv_sec = (time_t)seconds;
    ts.tv_nsec = (long)((seconds - ts.tv_sec) * 1000000000L);
    nanosleep(&ts, NULL);
}

void menu() {
    system("clear");
    printf("Student Management System\n");
    printf("1. Add Student\n");
    printf("2. View Student Info\n");
    printf("3. Remove Student\n");
    printf("4. Edit Student Info\n");
    printf("5. View Student List\n");
    printf("6. Student Ranking\n");
    printf("7. Read Data From File\n");
    printf("8. Export Data To File\n");
    printf("9. Exit\n");
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
            timeDelay(0.5);
            return;
        }
    }
    printf("Student not found.\n");
}

void editStudentInfo(StudentDatabase *db) {
    system("clear");
    char name[50];
    char input[20];
    int input_number;
    int choice;
    
    
    printf("Enter the student name to edit: ");
    while (getchar() != '\n');
    fgets(name, sizeof(name), stdin);
    trimWhitespace(name);
    
    for (int i=0; i< db->count; i++) {
        if(strcmp(name, db->students[i].name) == 0) {
            do {
            askDetailsToEditMenu();
            scanf("%d", &choice);
            
            switch (choice) {
                case 1:
                    printf("Enter new name: ");
                    while (getchar() != '\n');
                    fgets(name, sizeof(name), stdin);
                    trimWhitespace(name);
                    
                    strcpy(db->students[i].name, name);
                    system("clear");
                    printf("\nName changed to: %s", name);
                    timeDelay(0.5);
                    break;
                    
                case 2:
                    printf("Enter new age: ");
                    scanf("%d", &input_number);
                    
                    db->students[i].age = input_number;
                    system("clear");
                    printf("Age changed to: %d", input_number);
                    timeDelay(0.5);
                    break;
                    
                case 3:
                    printf("Enter new year level: ");
                    while (getchar() != '\n');
                    fgets(input, sizeof(input), stdin);
                    trimWhitespace(input);
                    
                    strcpy(db->students[i].yearLevel, input);
                    system("clear");
                    printf("Year Level changed to: %s", input);
                    timeDelay(0.5);
                    break;
                    
                case 4:
                    printf("Enter new grade: ");
                    scanf("%f", &input_number);
                    
                    db->students[i].grade = input_number;
                    system("clear");
                    printf("Grade changed to: %.2f", input_number);
                    timeDelay(0.5);
                    break;
                
                case 5:
                    printf("Update student status. Enrolled(1) or Not Enrolled (0)\nNew Status: ");
                    scanf("%d", &input_number);
                    
                    if (input_number == 1) {
                        db->students[i].isEnrolled = true;
                        system("clear");
                        printf("Changed student status to Enrolled.\n");
                        timeDelay(0.5);
                    } else if(input_number == 2) {
                        db->students[i].isEnrolled = false;
                        system("clear");
                        printf("Student is now currently Not Enrolled\n");
                        timeDelay(0.5);
                    } else {
                        printf("That choice is invalid!\nExiting...\n");
                        timeDelay(0.5);
                        system("clear");
                        return;
                    }
                    break;
                    
                case 6:
                    printf("Exiting...");
                    timeDelay(0.5);
                    return;
                    
                default:
                    printf("Invalid choice! Try again\n");    
                    break;    
                }
            } while (choice > 0 || choice <= 6);
        }
    }
    printf("Student not found.\n");
    timeDelay(.5);
    system("clear");
}

void askDetailsToEditMenu() {
    system("clear");
    printf("\nChoose among which you want to edit.\n");
    printf("1. Name\n");
    printf("2. Age\n");
    printf("3. Year Level\n");
    printf("4. Grade\n");
    printf("5. Status\n");
    printf("6. Exit\n");  // ✅ add this
}