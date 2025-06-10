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


int main(){
    
}
