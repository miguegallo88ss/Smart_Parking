#ifndef MyTypes_h
#define MyTypes_h

#include <WString.h>

#define NO_Of_Sensors 4
#define No_Of_Inputs 4
#define No_Of_Outputs 5

typedef struct 
{
    float distance [NO_Of_Sensors];  
    float time[NO_Of_Sensors];
    float tempDistance[NO_Of_Sensors];
    
    int inputs [No_Of_Inputs];
    int outputs[No_Of_Outputs];
    int errorCode;
     
} readings;
#endif
