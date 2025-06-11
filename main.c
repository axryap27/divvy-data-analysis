/*main.c*/

//
// Loops indefinitely and takes user input to output 
// various statistics on DIVVY bikes 
//
// Author:
// Aarya Patel
// 
// Northwestern University
// DIVVY Data analysis
//

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dist.h"


// station struct
struct Station {
    char* stationID;
    int capacity;
    double latitude;
    double longitude;
    char* name;
};


// trip struct
struct Trip {
    char* tripID;
    char* bikeID;
    char* startStationID;
    char* endStationID;
    int duration;
    char* startTime;
};



/////////////////////
//
// PRIVATE FUNCTIONS
//
/////////////////////


//
// array helper functions
//


//
// doubleCharArray()
//
// returns a pointer to a char array double the size of the inputted char array pointer 
//
static char* doubleCharArray(char* array, int* capacity) {
    *capacity *= 2;
    return realloc(array, *capacity * sizeof(char));
}

//
// doubleStationArray()
//
// doubles array of Station structs, returning pointer to new larger array
//
static struct Station* doubleStationArray(struct Station* array, int* capacity) {
    *capacity *= 2;
    return realloc(array, *capacity * sizeof(struct Station));
}

//
// doubleTripArray()
//
// doubles array of Trip structs, returning pointer to new larger array
//
static struct Trip* doubleTripArray(struct Trip* array, int* capacity) {
    *capacity *= 2;
    return realloc(array, *capacity * sizeof(struct Trip));
}

//
// countTripsForStation()
//
// counts number of trips given a station ID and start/end station ID of a given trip
// using a for loop and count variable which is returned at the end.
//
static int countTripsForStation(char* stationID, struct Trip* trips, int tripCount) {
    int count = 0;
    for (int i = 0; i < tripCount; i++) {
        if (strcmp(trips[i].startStationID, stationID) == 0 || 
            strcmp(trips[i].endStationID, stationID) == 0) {
            count++;
        }
    }
    return count;
}

//
// printStats
//
// given a station, trip and their respective counts outputs the number 
static void printStats(struct Station* stations, int stationCount, struct Trip* trips, int tripCount) {
    // Calculate total bike capacity across all stations
    int totalCapacity = 0;
    for (int i = 0; i < stationCount; i++) {
        totalCapacity += stations[i].capacity;
    }
    
    // Print the three required statistics
    printf(" stations: %d\n", stationCount);
    printf(" trips: %d\n", tripCount);
    printf(" total bike capacity: %d\n", totalCapacity);
}


//
// readStringInput()
//
// given a string, reads the file that matches with the string 
// by dynamically allocating an array of chars. Checks to see if the pointer
// is not a '\n' or EOF, adds said character to the array at index length.
// Also saves the length of the array and uses length as the index to traversal via a pointer
//
static char* readStringInput(const char* prompt) {
    printf("%s", prompt);
    
    int capacity = 10;
    char* input = malloc(capacity * sizeof(char));
    int length = 0;
    int c;
    
    // Read character by character until newline
    while ((c = getchar()) != '\n' && c != EOF) {
        if (length >= capacity - 1) {
            input = doubleCharArray(input, &capacity);
        }
        input[length] = c;
        length++;
    }
    
    input[length] = '\0';
    return input;
}


//
// memory management
//

//
// freeStations
//
// frees the memory allocated for the stations array 
// by traversing through the array for "count" number of times
// and each time freeing memory for each station struct
// and then at the end frees the array data structure it self
//
static void freeStations(struct Station* stations, int count) {
    for (int i = 0; i < count; i++) {
        free(stations[i].stationID);
        free(stations[i].name);
    }
    free(stations);
}

//
// freeTrips
//
// frees the memory allocated for the trips array 
// by traversing through the array for "count" number of times
// and each time freeing memory for each trip struct
// and then at the end frees the array data structure it self
//
static void freeTrips(struct Trip* trips, int count) {
    for (int i = 0; i < count; i++) {
        free(trips[i].tripID);
        free(trips[i].bikeID);
        free(trips[i].startStationID);
        free(trips[i].endStationID);
        free(trips[i].startTime);
    }
    free(trips);
}


//
// Helper function to extract the next word from a line
//
static char* extractWord(char** current) {
    // Skip whitespace
    while (**current == ' ' || **current == '\t') (*current)++;
    
    if (**current == '\0' || **current == '\n') {
        return NULL;  // No more words
    }
    
    char* start = *current;
    // Find end of word
    while (**current && **current != ' ' && **current != '\t' && **current != '\n') {
        (*current)++;
    }
    
    int len = *current - start;
    char* word = malloc(len + 1);
    strncpy(word, start, len);
    word[len] = '\0';
    
    return word;
}

//
// Helper function to extract rest of line (for station names)
//
static char* extractRestOfLine(char* current) {
    // Skip whitespace
    while (*current == ' ' || *current == '\t') current++;
    
    // Remove newline if present
    char* end = current + strlen(current) - 1;
    if (*end == '\n') *end = '\0';
    
    char* result = malloc(strlen(current) + 1);
    strcpy(result, current);
    return result;
}

//
// readStations()
//
// reads station data from file into dynamically allocated array
// parses: StationID Capacity Latitude Longitude Name
//
struct Station* readStations(char* filename, int* count) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: unable to open file \"%s\"\n", filename);
        return NULL;
    }
    
    int capacity = 10;
    struct Station* stations = malloc(capacity * sizeof(struct Station));
    *count = 0;
    
    char* line = NULL;
    size_t len = 0;
    
    while (getline(&line, &len, file) != -1) {
        if (*count >= capacity) {
            stations = doubleStationArray(stations, &capacity);
        }
        
        char* current = line;
        
        // Extract each field
        stations[*count].stationID = extractWord(&current);
        char* capacityStr = extractWord(&current);
        char* latStr = extractWord(&current);
        char* lonStr = extractWord(&current);
        
        if (capacityStr && latStr && lonStr) {
            stations[*count].capacity = atoi(capacityStr);
            stations[*count].latitude = atof(latStr);
            stations[*count].longitude = atof(lonStr);
            stations[*count].name = extractRestOfLine(current);
            
            (*count)++;
        }
        
        free(capacityStr);
        free(latStr);
        free(lonStr);
    }
    
    free(line);
    fclose(file);
    return stations;
}

//
// readTrips()
//
// reads trip data from file into dynamically allocated array
// parses: TripID BikeID StartStationID EndStationID Duration StartTime
//
struct Trip* readTrips(char* filename, int* count) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error: unable to open file \"%s\"\n", filename);
        return NULL;
    }
    
    int capacity = 10;
    struct Trip* trips = malloc(capacity * sizeof(struct Trip));
    *count = 0;
    
    char* line = NULL;
    size_t len = 0;
    
    while (getline(&line, &len, file) != -1) {
        if (*count >= capacity) {
            trips = doubleTripArray(trips, &capacity);
        }
        
        char* current = line;
        
        // Extract each field
        trips[*count].tripID = extractWord(&current);
        trips[*count].bikeID = extractWord(&current);
        trips[*count].startStationID = extractWord(&current);
        trips[*count].endStationID = extractWord(&current);
        char* durationStr = extractWord(&current);
        trips[*count].startTime = extractWord(&current);
        
        if (durationStr) {
            trips[*count].duration = atoi(durationStr);
            (*count)++;
        }
        
        free(durationStr);
    }
    
    free(line);
    fclose(file);
    return trips;
}

//
// printDurations()
//
// categorizes trips by duration and prints counts
//
static void printDurations(struct Trip* trips, int tripCount) {
    int counts[5] = {0}; // Initialize all counts to 0
    
    for (int i = 0; i < tripCount; i++) {
        int duration = trips[i].duration; // duration is in seconds
        
        if (duration <= 1800) {                    // <= 30 minutes (1800 seconds)
            counts[0]++;
        } else if (duration <= 3600) {             // 30-60 minutes (3600 seconds)
            counts[1]++;
        } else if (duration <= 7200) {             // 1-2 hours (7200 seconds)
            counts[2]++;
        } else if (duration <= 18000) {            // 2-5 hours (18000 seconds)
            counts[3]++;
        } else {                                   // > 5 hours
            counts[4]++;
        }
    }
    
    // Print results in the exact format required
    printf(" trips <= 30 mins: %d\n", counts[0]);
    printf(" trips 30..60 mins: %d\n", counts[1]);
    printf(" trips 1-2 hrs: %d\n", counts[2]);
    printf(" trips 2-5 hrs: %d\n", counts[3]);
    printf(" trips > 5 hrs: %d\n", counts[4]);
}

//
// printStartingTimes
//
// outputs a histogram of trip starting times by hour (0-23)
//
static void printStartingTimes(struct Trip* trips, int tripCount) {
    int hourCounts[24] = {0}; // Fixed-size array for 24 hours
    
    for (int i = 0; i < tripCount; i++) {
        // Parse hour from startTime (format: "H:MM" or "HH:MM")
        int hour = atoi(trips[i].startTime);
        hourCounts[hour]++;
    }
    
    // Print histogram for all 24 hours (0-23)
    for (int i = 0; i < 24; i++) {
        printf(" %d: %d\n", i, hourCounts[i]);
    }
}


/////////////////////////////////////////////////////////


//
// processCommands()
//
//
// Main command that processes user inputted commands via a while loop.
// Manages which helpers to use when and controls overall program flow
//
static void processCommands(struct Station* stations, int stationCount, struct Trip* trips, int tripCount){
    int commandCapacity = 10;
    char* command = malloc(commandCapacity * sizeof(char));
    
    while (1) {
        // Read command dynamically
        printf("Enter command (# to stop)> ");
        int length = 0;
        int c;
        
        while ((c = getchar()) != '\n' && c != EOF) {
            if (length >= commandCapacity - 1) {
                command = doubleCharArray(command, &commandCapacity);
            }
            command[length] = c;
            length++;
        }
        command[length] = '\0';
        
        // Process commands
        if (strcmp(command, "#") == 0) {
            break;
        }
        else if (strcmp(command, "stats") == 0) {
            printStats(stations, stationCount, trips, tripCount);
        }
        else if (strcmp(command, "durations") == 0) {
            printDurations(trips, tripCount);
        }
        else if (strcmp(command, "starting") == 0) {
            printStartingTimes(trips, tripCount);
        }
        else if (strcmp(command, "nearme") == 0) {
            //handleNearMe(stations, stationCount, trips, tripCount);
        }
        else if (strcmp(command, "stations") == 0) {
            //printAllStations(stations, stationCount, trips, tripCount);
        }
        else if (strcmp(command, "find") == 0) {
            //findStations(stations, stationCount, trips, tripCount);
        }
    }
    
    free(command);

}


//
// main()
//
// handles file input and program execution between all helpers
//
int main(){
    char* stationsFile = readStringInput("Please enter name of stations file> ");
    char* tripsFile = readStringInput("Please enter name of bike trips file> ");
    
    int stationCount = 0, tripCount = 0;
    struct Station* stations = readStations(stationsFile, &stationCount);
    if (stations == NULL) {
        free(stationsFile);
        free(tripsFile);
        return 1;
    }
    
    struct Trip* trips = readTrips(tripsFile, &tripCount);
    if (trips == NULL) {
        freeStations(stations, stationCount);
        free(stationsFile);
        free(tripsFile);
        return 1;
    }
    
    processCommands(stations, stationCount, trips, tripCount);
    
    freeStations(stations, stationCount);
    freeTrips(trips, tripCount);
    free(stationsFile);
    free(tripsFile);
    return 0;
}

