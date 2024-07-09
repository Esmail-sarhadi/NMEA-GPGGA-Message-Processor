#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 100

void processGPGGALine(char *line) {
    char *token, *delim = ",*";
    int hour, minute;
    double second;
    char latStr[10], longStr[11], latDir[2], longDir[2];
    int satCount;

    token = strtok(line, delim); 
    if (token == NULL || strcmp(token, "$GPGGA") != 0) {
        printf("Error: Invalid message type\n");
        return;
    }

    token = strtok(NULL, delim);  
    if (token != NULL) {
        sscanf(token, "%2d%2d%lf", &hour, &minute, &second);

        // Check if the time is valid
        if (hour < 0 || hour >= 24) {
            printf("Error: Invalid hour value\n");
            return;
        }
        if (minute < 0 || minute >= 60) {
            printf("Error: Invalid minute value\n");
            return;
        }
        if (second < 0.0 || second >= 60.0) {
            printf("Error: Invalid second value\n");
            return;
        }
    } else {
        printf("Error parsing time\n");
        return;
    }

    token = strtok(NULL, delim);  
    if (token != NULL) {
        strcpy(latStr, token);
    } else {
        printf("Error parsing latitude\n");
        return;
    }
    token = strtok(NULL, delim);  
    if (token != NULL) {
        strncpy(latDir, token, sizeof(latDir) - 1);
        latDir[sizeof(latDir) - 1] = '\0';
    } else {
        printf("Error parsing latitude direction\n");
        return;
    }

    token = strtok(NULL, delim);  
    if (token != NULL) {
        strcpy(longStr, token);
    } else {
        printf("Error parsing longitude\n");
        return;
    }
    token = strtok(NULL, delim); 
    if (token != NULL) {
        strncpy(longDir, token, sizeof(longDir) - 1);
        longDir[sizeof(longDir) - 1] = '\0';
    } else {
        printf("Error parsing longitude direction\n");
        return;
    }

    token = strtok(NULL, delim); 
    token = strtok(NULL, delim);  
    if (token != NULL) {
        satCount = atoi(token);
    } else {
        printf("Error parsing satellite count\n");
        return;
    }

    printf("Time: %02d:%02d:%06.3lf, Latitude: %s%s, Longitude: %s%s, Satellites: %d\n",
           hour, minute, second, latStr, latDir, longStr, longDir, satCount);
}

int main() {
    FILE *file = fopen("nmea_messages.txt", "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    int index = 0;
    int ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n' || ch == '\r') {
            if (index > 0) {
                line[index] = '\0';
                if (strncmp(line, "$GPGGA", 6) == 0) {
                    processGPGGALine(line);
                }
                index = 0;  // reset for next line
            }
        } else {
            if (index < MAX_LINE_LENGTH - 1) {
                line[index++] = ch;
            }
        }
    }

    // Process last line if file does not end with a newline
    if (index > 0) {
        line[index] = '\0';
        if (strncmp(line, "$GPGGA", 6) == 0) {
            processGPGGALine(line);
        }
    }

    fclose(file);
    return 0;
}
