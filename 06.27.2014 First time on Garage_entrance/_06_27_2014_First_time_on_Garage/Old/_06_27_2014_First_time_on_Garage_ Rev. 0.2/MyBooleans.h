#ifndef MyBooleans_h
#define MyBooleans_h

#include <WString.h>

#define NO_Of_Sensors 4
#define No_Of_Inputs 4
#define No_Of_Outputs 5

struct booleans_Variables
{
    bool check_1;  // this is to check if a car is at the entrance
    bool check_2;  //this is to check if a car is leaving 
    bool check_3;    
};

typedef booleans_Variables true_false;
#endif



