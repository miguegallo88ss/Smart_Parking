#ifndef ToStruct_h
#define ToStruct_h

#include <WString.h>
#define noOfPins 28

typedef struct 
    {
        int pinsArray[noOfPins]; // array that contain all pins and its numbers
        int count;
        int internerCarCount;
        int noOfspaces;
        bool up;    // (car at entrance)
        bool down;  // (car at Exit)
        
        bool EntrancePulseFlag;  // set entrancePulseFlag true (car detected)or false        
        bool ExitPulseFlag;  // set exitPulseFlag true (car detected) or false 
        
        bool prevEntrancePulseFlag;  // variable where the previous entrance is stored for comparison
        bool prevExitPulseFlag;  // variable where the previous exit is stored for comparison
        
        bool both;
        
    }AllPinVar;

#endif

