#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ATTENDEES 500
#define MAX_NAME_LENGTH 100
#define MAX_EMAIL_LENGTH 100

typedef struct {
    char name[MAX_NAME_LENGTH];
    char surname[MAX_NAME_LENGTH];
    double duration;
} Attendee;

int compareAttendees(const void* a, const void* b) {
    Attendee* attendeeA = (Attendee*)a;
    Attendee* attendeeB = (Attendee*)b;

    int result = strcmp(attendeeA->surname, attendeeB->surname);
    if (result == 0) {
        result = strcmp(attendeeA->name, attendeeB->name);
    }

    return result;
}

int main() {
    char filename[100];
    char sortedAnswer[4];
    Attendee attendees[MAX_ATTENDEES];
    int numAttendees = 0;

    printf("Enter the input file name: ");
    scanf("%s", filename);

    printf("Sort the results? (yes/no): ");
    scanf("%s", sortedAnswer);

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Failed to open the file.\n");
        return 1;
    }

    // Skip the header line
    char line[500];
    fgets(line, sizeof(line), file);

    // Read and process attendance records
    while (fgets(line, sizeof(line), file)) {
        char* token;
        token = strtok(line, ",");

        // Extract join time, leave time, and duration
        token = strtok(NULL, ",");
        token = strtok(NULL, ",");
        double duration = atof(token);

        // Extract name and email
        token = strtok(NULL, ",");
        char* name = token;
        token = strtok(NULL, ",");
        char* email = token;

        // Extract surname from the name
        char* surname = strrchr(name, ' ');
        if (surname == NULL) {
            printf("Invalid name format: %s\n", name);
            continue;
        }
        *surname = '\0';
        surname++;

        // Check if the attendee already exists
        int i;
        for (i = 0; i < numAttendees; i++) {
            if (strcasecmp(attendees[i].name, name) == 0 && strcasecmp(attendees[i].surname, surname) == 0) {
                attendees[i].duration += duration;
                break;
            }
        }

        // Add a new attendee if not found
        if (i == numAttendees) {
            strncpy(attendees[numAttendees].name, name, MAX_NAME_LENGTH);
            strncpy(attendees[numAttendees].surname, surname, MAX_NAME_LENGTH);
            attendees[numAttendees].duration = duration;
            numAttendees++;
        }
    }

    fclose(file);

    // Sort attendees if required
    if (strcasecmp(sortedAnswer, "yes") == 0) {
        qsort(attendees, numAttendees, sizeof(Attendee), compareAttendees);
    }

    // Display attendance records
    for (int i = 0; i < numAttendees; i++) {
        printf("%s %s %.2f\n", attendees[i].surname, attendees[i].name, attendees[i].duration);
    }

    return 0;
}
