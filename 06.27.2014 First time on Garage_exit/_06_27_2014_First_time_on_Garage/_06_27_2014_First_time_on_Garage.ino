/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^EXIT^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Rev. 0.4
Author:
      Miguel A Naranjo 
Date: 
      08/25/2014
****************************************************************************************************************************            
      Description:  
            Uses time read from sensors and calculates distances (total of 4 distances)
      Syntax: 
            void distanceCalc(readings *data, true_false *carPresent); 
      Variables: 
            data.time[]         time it takes the ultrasonic sound to go from the emitter to the receiver
            data.distance[]     distance at which the object is from the sensor
      Returns: 
           Nothing       
****************************************************************************************************************************    
      Description:  
            This is only done when it has been detected a car i within the entrance/exit
            Uses previous destances and stores them into a previous reading array
      Syntax: 
            void storePrevious(readings *prev_Distance, readings *data);  
      Variables: 
            prev_Distance[]     after a car is detected, the data.distcane[] reading are stored for lates use
            data.distance[]     distance at which the object is from the sensor
      Rrturns: 
            Nothing             
****************************************************************************************************************************            
      Description:  
            Detecting that a cars is leaving. 
            Car leaving is detected only if all previous prev_Distance.distance[] = 0, and data catual (data.distance[1,2] > 0.75 entrance width)& (data.distance[3,4] < 0.75 entrance width) is true  
      Syntax: 
            void carLeaving(readings *data, readings *prev_Distance);  
      Variables:
            prev_Distance[]     after a car is detected, the data.distcane[] reading are stored for lates use
            data.distance[]     distance at which the object is from the sensor
       Returns: 
            Nothing 
****************************************************************************************************************************            
      Description:  
             Clear all the previous distances (prev_Distancec []) when called
             If a variable is set to true or false by the contructor, when finction called, variable is reset to it original state
      Syntax: 
            void clearFunction(readings *prev_Distance);  
      Variables:
            prev_Distance->distance[]
      Returns: 
            Nothing 
****************************************************************************************************************************            
*/

#include "MyTypes.h"
#include "MyBooleans.h"
#include <Arduino.h>


# define NO_Of_Sensors 4  // number of sensors that are being sed at the EXIT 
#define No_Of_Inputs 4  //number of PINS that are used as inputs (digital)
#define No_Of_Outputs 5  //number of outputs that are usded as outputs (4-analog, 1-digital)
#define width_Of_Entrance_Exit 20 // Width of the entrance, measured by the sensors (cm) 
#define distance_Tolerance 1.3  // allowable deviation from the distance reading 
#define pulseTime 2   // when a car is detected a pilse of this lenght of tme is emmited (mS)
#define minDetectableDistance 2  // munimum distance any of the sensors ca read because of their capabiities
#define noSoundTime 2 // time from when one distance is checked to when the next distance is checked
#define distanceaDetectionTolerance 0.375  // multiplied by the width of the netrance, this will give the 
                                           //maximum distance there can be between a car a a senson in order for a car to be detected (cm)
#define prev_istanceaDetectionTolerance 0.8 // multiplied by the width of the entrance, this is 
                                            // maximimum reading that can be stored. (cm)
                                            // readings larger than this will be discarded 

// Digital I/O Pins 
//int buttonPin_1_Input = 1;
int buttonPin_2_Input = 2;
int buttonPin_3_Input = 3;
int buttonPin_4_Input = 4;
int buttonPin_5_Input = 5;
//int buttonPin_6_Input = 6;
//int buttonPin_7_Output = 7;
int buttonPin_8_Output = 8;
//int buttonPin_9_Output = 9;

// Analog I/O Pins
int buttonPin_A0_Output = A0;
int buttonPin_A1_Output = A1;
int buttonPin_A2_Output = A2;
int buttonPin_A3_Output = A3;
//int buttonPin_A4_Output = A4;
//int buttonPin_A5_Output = A5;

    
    readings data;                 // This is a global variable
    readings prev_Distance;        // This is a global variable
    readings buttonPin;            // This is a global variable
    readings buttonPin_No_Input;   // This is a global variable
    readings error;

    true_false carPresent;         // This is a global variable
    true_false doneWaiting;        // This is a global variable
    true_false carWaitIn;          // This is a global variable

//**********************************************************// End of Structure
void setup()
{
  void distanceCalc(readings *data, true_false *carPresent);  // uses time read fron sensors and calculates distances
  void storePrevious(readings *prev_Distance, readings *data);  //uses previous destances and stores them into a previous reading array 
  void carLeaving(readings *data, readings *prev_Distance);  // detecting that a cars is leaving 
  void clearFunction(readings *prev_Distance);  // Clear all the previous distances (prev_Distancec []) when called 
  
  Serial.begin(9600);
  pinMode(buttonPin_A0_Output, OUTPUT); // Analog I/O Pins
  pinMode(buttonPin_A1_Output, OUTPUT); // Analog I/O Pins
  pinMode(buttonPin_A2_Output, OUTPUT); // Analog I/O Pins
  pinMode(buttonPin_A3_Output, OUTPUT); // Analog I/O Pins

  pinMode(buttonPin_2_Input, INPUT); // Digital I/O Pins
  pinMode(buttonPin_3_Input, INPUT); // Digital I/O Pins
  pinMode(buttonPin_4_Input, INPUT); // Digital I/O Pins
  pinMode(buttonPin_5_Input, INPUT); // Digital I/O Pins

  pinMode(buttonPin_8_Output, OUTPUT); // Digital I/O Pins
  
   // *******************making all variables part of the array *************************
    buttonPin.outputs[0] = buttonPin_A0_Output; // LOWER RIGHT+++++++
    buttonPin.outputs[1] = buttonPin_A1_Output; // UPPER LEFT
    buttonPin.outputs[2] = buttonPin_A2_Output; // UPPER RIGHT 
    buttonPin.outputs[3] = buttonPin_A3_Output; // LOWER LEFT 
    buttonPin.outputs[4] = buttonPin_8_Output;
    
    buttonPin_No_Input.inputs[0] = buttonPin_2_Input;
    buttonPin_No_Input.inputs[1] = buttonPin_3_Input;
    buttonPin_No_Input.inputs[2] = buttonPin_4_Input;
    buttonPin_No_Input.inputs[3] = buttonPin_5_Input;
    // *******************making all variables part of the array ************************* Enf Of
    digitalWrite(buttonPin_8_Output, LOW);
}

void loop()
{
              //digitalWrite(buttonPin_8_Output, LOW);

              distanceCalc(&data, &carPresent);  // getting previous distances
         //**************************************************************** Setting previous Distances              
              if (carPresent.check_1 == true) // car detected in entrance
              {
                  storePrevious(&prev_Distance, &data);
                  //Serial.print(carPresent.check_1);
              }
              
              else if (carPresent.check_1 == false)  // no car detected in entrance
              {
                 
                 //Serial.print(carPresent.check_1);
                 carLeaving(&data, &prev_Distance);
                 
                 
                     if ((doneWaiting.check_2 == true)&&   // It has been detected that a car is entering the garage
                          (prev_Distance.distance[0] > minDetectableDistance)&&  // previous distances are ZERO only if sensors 1 & 2 are off (=width of entrance)
                          (prev_Distance.distance[1] > minDetectableDistance)&&  // and sensor 3 & 4 are being triggered, after if is know a car was present
                          (prev_Distance.distance[2] > minDetectableDistance)&&
                          (prev_Distance.distance[3] > minDetectableDistance)&&
                          (prev_Distance.distance[0] < prev_istanceaDetectionTolerance*width_Of_Entrance_Exit)&&
                          (prev_Distance.distance[1] < prev_istanceaDetectionTolerance*width_Of_Entrance_Exit)&&
                          (prev_Distance.distance[2] < prev_istanceaDetectionTolerance*width_Of_Entrance_Exit)&&
                          (prev_Distance.distance[3] < prev_istanceaDetectionTolerance*width_Of_Entrance_Exit))
                     {
//                         Serial.print("Done Waiting =  ");
//                         Serial.println(doneWaiting.check_2);
                         digitalWrite(buttonPin_8_Output, HIGH);  // clock pulse
//                       delay(pulseTime);  // pulse for 2mS **************************************************************************************************** 
                         delay(pulseTime + 7000); // add seven second to give time for the other aparato to pick up the voltage
                         digitalWrite(buttonPin_8_Output, LOW);
//                         Serial.println(data.distance[0]);
//                         Serial.println(data.distance[1]);
//                         Serial.println(data.distance[2]); 
//                         Serial.println(data.distance[3]);
                         //delay(10);
//                         Serial.println ("clock Dn");
                         
                         clearFunction(&prev_Distance);
                     }
                          
              }
               
              else   // ERROR, no car detected || something else trigered one or nore (not all four) of the sensors 
                  return;
                  //                  Serial.print("ERROR");

         //**************************************************************** End of Setting previous Distances                  
}
//####################################################################################################################################################################################################################  
//####################################################################################################################################################################################################################
//####################################################################################################################################################################################################################

void distanceCalc(readings *data, true_false *carPresent)  // function that detemines if a car is within the entrance (all 4 sensors triggered) 
{ 
        label:
        
         //**************************************************************** calculate distances
         for (int t = 0; t < NO_Of_Sensors; t++)
             {
                digitalWrite(buttonPin.outputs[t], HIGH); // emitte sound to every sensor to calculate distances
                delay(pulseTime);
                digitalWrite(buttonPin.outputs[t], LOW);
                data->time[t] = pulseIn(buttonPin_No_Input.inputs[t], HIGH);
                
                data->tempDistance[t] =(340.29*(data->time[t]/20000) + distance_Tolerance); // store distances in temporary array
                
                if((data->tempDistance[t] >= minDetectableDistance) &&   // if distances within range, tore them in regula distance array[]
                   (data->tempDistance[t] <= width_Of_Entrance_Exit + 2))
                    {
                        data->distance[t] = data->tempDistance[t];
                    }
                    
                else
                    {
                        goto label; // if bugous distance, discard and take 4 new readings 
        
                    }
                        
             }
//                      Serial.println(t);
//                    Serial.println(data->tempDistance[0]);  // oXhere
//                    Serial.println(data->tempDistance[1]);  // oXhere
//                    Serial.println(data->tempDistance[2]);  // oXhere
//                    Serial.println(data->tempDistance[3]);  // oXhere
                   
          //**************************************************************** En of calculate distances 

//              }
//               Serial.println("***********************************************************\n");
                       // Serial.println(data->distance[0]);
         //*************************************************************** check stoed distances to decide whether to clock or not 
                   if( (data->distance[0] < distanceaDetectionTolerance*width_Of_Entrance_Exit)&&  // it has been detected that a car is within the entrance opening
                       (data->distance[1] < distanceaDetectionTolerance*width_Of_Entrance_Exit)&&
                       (data->distance[2] < distanceaDetectionTolerance*width_Of_Entrance_Exit)&&
                       (data->distance[3] < distanceaDetectionTolerance*width_Of_Entrance_Exit)&&
                       (data->distance[0] != 0)&&
                       (data->distance[1] != 0)&&
                       (data->distance[2] != 0)&&
                       (data->distance[3] != 0) )
                      {                            
                            carPresent->check_1 = true;
                      }
                   else
                            carPresent->check_1 = false;  
                            
         //**************************************************************** Check previous distance to see if a car is in or stopped inside 
}  //end of function that detemines if a car is within the entrance (all 4 sensors triggered)//####################################################################################################################################################################################################################
//####################################################################################################################################################################################################################
//####################################################################################################################################################################################################################
        
        
void storePrevious(readings *prev_Distance, readings *data) //**************************************************************** Set up previous distences 
{            
  
     for (int i = 0; i < NO_Of_Sensors; i ++)
          {
             prev_Distance->distance[i] = data->distance[i];
             //delay (2);
          }     
}  //**************************************************************** End of Set up previous distences  

//####################################################################################################################################################################################################################
//####################################################################################################################################################################################################################
//####################################################################################################################################################################################################################

void carLeaving(readings *data, readings *prev_Distance)
{
         if ((data->distance[2] > (distanceaDetectionTolerance*width_Of_Entrance_Exit))&&  // Car triggering only 2 sensoras, after all 4 have alredy been triggered
             (data->distance[3] > (distanceaDetectionTolerance*width_Of_Entrance_Exit))&&  // Car triggering only 2 sensoras, after all 4 have alredy been triggered
             (data->distance[0] <= width_Of_Entrance_Exit/2)&&
             (data->distance[1] <= width_Of_Entrance_Exit/2)&&
             (prev_Distance->distance[0] < (width_Of_Entrance_Exit)) &&
             (prev_Distance->distance[1] < (width_Of_Entrance_Exit)) &&
             (prev_Distance->distance[2] < (width_Of_Entrance_Exit)) &&
             (prev_Distance->distance[3] < (width_Of_Entrance_Exit)))
         {
             doneWaiting.check_2 = true;          
         }
         
         else
         {
             doneWaiting.check_2 = false;
         }     
}
//####################################################################################################################################################################################################################
//####################################################################################################################################################################################################################
//####################################################################################################################################################################################################################

void clearFunction(readings *prev_Distance)
{
    for (int clean = 0; clean < NO_Of_Sensors; clean++)
    {
        prev_Distance->distance[clean] = 0;
    }    
}




