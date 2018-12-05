/*^^^^^^^^^^^^^^^^^^^^^^^^^^^^^Display Updater^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
Rev. 0.4
Author:
      Miguel A Naranjo 
Date: 
      08/25/2014
****************************************************************************************************************************            
      Description:  
            When called all the I/O pins that conect to the arduinot go to low state
      Syntax: 
            void lowFunction(AllPinVar *data); 
      Variables: 
            AllPinVar *data[]     Structure contains all the variables that are used on this functions 
      Returns: 
           Nothing       
****************************************************************************************************************************    
      Description:  
            Display shows 50 empty spaces the very firt time system boots
      Syntax: 
            set50Function(&data);   
      Variables: 
            AllPinVar *data[]     Structure contains all the variables that are used on this functions
      Rrturns: 
            Nothing             
****************************************************************************************************************************            
      Description:  
            Function Updates all four seven segment displays to show the number of available spaces
            If the lot is full the displays will show "FULL"
      Syntax: 
            void LCD_DisplayFunction(AllPinVar *data);  
      Variables:
            AllPinVar *data[]     Structure contains all the variables that are used on this functions
       Returns: 
            Nothing 
****************************************************************************************************************************            
      Description:  
             function to clear variables after 7seg display has changed  
      Syntax: 
            void clearFunction(AllPinVar *data)  
      Variables:
            AllPinVar *data[]     Structure contains all the variables that are used on this functions
      Returns: 
            Nothing 
****************************************************************************************************************************            
*/

#include "elapsedMillis.h"
#include "ToStruct.h"
#include "internetVars.h"
#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>


#define NumberOfCarsParkings 50    // number of available parking spaces 
#define noOfPins 28    // number of I/O pins used to display the available number of spaces on the four 7-seg displays 
#define holdBootTime 2000  // delay the startup of the Omega 2560 arduino board in for the other two boards 
                           //to be up and runnig when this board's setup void() function runs the first time
                           // this deprives noise spikes from the other two boards to trigger the Omega 2560 (2000mS "or" 2S)

#define sizeOftimeElapsedInMin 2100    // minumum time it takes to this board to loop around the codeonce (mS)
#define sizeOftimeElapsedInMax 7002575  // add seven second to give time for the other aparato to pick up the voltage (mS)
#define sizeOftimeElapsedOutMin 2100    // minumum time it takes to this board to loop around the codeonce (mS)
#define sizeOftimeElapsedOutMax 7002575  // add seven second to give time for the other aparato to pick up the voltage (mS)

#define HTMLTextSize 6  // size of the HTML text printed to application 

//#define sizeOftimeElapsedInMin 2100
//#define sizeOftimeElapsedInMax 1002575
//#define sizeOftimeElapsedOutMin 2100
//#define sizeOftimeElapsedOutMax 1002575


int clock_in = 2;  // clock input from entrance
int clock_out = 3; // clock input from exit

// time test 
//int five = 5;

unsigned long time;
unsigned long timd;

elapsedMicros timeElapsedIn;  // variable of type elapsedMicros Class()
elapsedMicros timeElapsedOut;  // variable of type elapsedMicros Class()


int LCD_1_A_out22 = 22; // LCD_digit # 1 (left to right)
int LCD_1_B_out23 = 23; // LCD_digit # 1 (left to right)
int LCD_1_C_out24 = 24; // LCD_digit # 1 (left to right)
int LCD_1_D_out25 = 25; // LCD_digit # 1 (left to right)
int LCD_1_E_out26 = 26; // LCD_digit # 1 (left to right)
int LCD_1_F_out27 = 27; // LCD_digit # 1 (left to right)
int LCD_1_G_out28 = 28; // LCD_digit # 1 (left to right)

int LCD_2_A_out29 = 29; // LCD_digit # 2 (left to right)
int LCD_2_B_out30 = 30; // LCD_digit # 2 (left to right)
int LCD_2_C_out31 = 31; // LCD_digit # 2 (left to right)
int LCD_2_D_out32 = 32; // LCD_digit # 2 (left to right)
int LCD_2_E_out33 = 33; // LCD_digit # 2 (left to right)
int LCD_2_F_out34 = 34; // LCD_digit # 2 (left to right)
int LCD_2_G_out35 = 35; // LCD_digit # 2 (left to right)

int LCD_3_A_out36 = 36; // LCD_digit # 3 (left to right)
int LCD_3_B_out37 = 37; // LCD_digit # 3 (left to right)
int LCD_3_C_out38 = 38; // LCD_digit # 3 (left to right)
int LCD_3_D_out39 = 39; // LCD_digit # 3 (left to right)
int LCD_3_E_out40 = 40; // LCD_digit # 3 (left to right)
int LCD_3_F_out41 = 41; // LCD_digit # 3 (left to right)
int LCD_3_G_out42 = 42; // LCD_digit # 3 (left to right)

int LCD_4_A_out43 = 43; // LCD_digit # 4 (left to right)
int LCD_4_B_out44 = 44; // LCD_digit # 4 (left to right)
int LCD_4_C_out45 = 45; // LCD_digit # 4 (left to right)
int LCD_4_D_out46 = 46; // LCD_digit # 4 (left to right)
int LCD_4_E_out47 = 47; // LCD_digit # 4 (left to right)
int LCD_4_F_out48 = 48; // LCD_digit # 4 (left to right)
int LCD_4_G_out49 = 49; // LCD_digit # 4 (left to right)

     
    AllPinVar data;  // variable on whicl all data is stored 
     
    int val_2 = 0;
    int val_3 = 0;
    int count = NumberOfCarsParkings;
    int internerCarCount = NumberOfCarsParkings;
    
    //Internet Setup 90 A2 DA 0F 8E 04
    byte mac[] = {0x90, 0xA2, 0xDA, 0x0F, 0x8E, 0x04 };
    IPAddress ip(131,247,8,6);
//    IPAddress ip(192,168,1,177);
    EthernetServer server(80);


void setup()
{
    void lowFunction(AllPinVar *data);  // bring all LCD OUTPUT pins to a LOW
    void set50Function(AllPinVar *data); // used the very first time so that trhe LCd 7-seg displays 50 50 parking spaces available 
    int countingFunction(AllPinVar *data); // count up or down to provide for the OMEGA to light on the correct number on LCD
    void LCD_DisplayFunction(AllPinVar *data); // takes the values of count(data.noOfSpaces) and displays them on to the LCD
    void clearFunction(AllPinVar *data);  // function to clear variables after 7seg display has changed
    void internetFunction(AllPinVar *data); // Fintion that sets up the web server wit the numbe rof parking spaces 
       
    Serial.begin(115200);
    pinMode(clock_in, INPUT); // Pin 2, looks for a pulse from pin 8 on the Arduino_Uno boards (entrance)
    pinMode(clock_out, INPUT);// Pin 3, looks for a pulse from pin 9 on the Arduino_Uno boards (exit)
    
    while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
    }
    
    //test
//    pinMode(five, OUTPUT);
    // setting all unsed pins to an INPUT or an OUTPUT 
    
    pinMode(LCD_1_A_out22, OUTPUT);
    pinMode(LCD_1_B_out23, OUTPUT);
    pinMode(LCD_1_C_out24, OUTPUT);
    pinMode(LCD_1_D_out25, OUTPUT);
    pinMode(LCD_1_E_out26, OUTPUT);
    pinMode(LCD_1_F_out27, OUTPUT);
    pinMode(LCD_1_G_out28, OUTPUT);
    
    pinMode(LCD_2_A_out29, OUTPUT);
    pinMode(LCD_2_B_out30, OUTPUT);
    pinMode(LCD_2_C_out31, OUTPUT);
    pinMode(LCD_2_D_out32, OUTPUT);
    pinMode(LCD_2_E_out33, OUTPUT);
    pinMode(LCD_2_F_out34, OUTPUT);
    pinMode(LCD_2_G_out35, OUTPUT);
    
    pinMode(LCD_3_A_out36, OUTPUT);
    pinMode(LCD_3_B_out37, OUTPUT);
    pinMode(LCD_3_C_out38, OUTPUT);
    pinMode(LCD_3_D_out39, OUTPUT);
    pinMode(LCD_3_E_out40, OUTPUT);
    pinMode(LCD_3_F_out41, OUTPUT);
    pinMode(LCD_3_G_out42, OUTPUT);
    
    pinMode(LCD_4_A_out43, OUTPUT);
    pinMode(LCD_4_B_out44, OUTPUT);
    pinMode(LCD_4_C_out45, OUTPUT);
    pinMode(LCD_4_D_out46, OUTPUT);
    pinMode(LCD_4_E_out47, OUTPUT);
    pinMode(LCD_4_F_out48, OUTPUT);
    pinMode(LCD_4_G_out49, OUTPUT);
    
    // Making all pins part of an array for easy accesss 
    
    data.pinsArray[0] = LCD_1_A_out22;
    data.pinsArray[1] = LCD_1_B_out23; 
    data.pinsArray[2] = LCD_1_C_out24; 
    data.pinsArray[3] = LCD_1_D_out25; 
    data.pinsArray[4] = LCD_1_E_out26; 
    data.pinsArray[5] = LCD_1_F_out27; 
    data.pinsArray[6] = LCD_1_G_out28; 
   
    data.pinsArray[7] = LCD_2_A_out29; 
    data.pinsArray[8] = LCD_2_B_out30; 
    data.pinsArray[9] = LCD_2_C_out31; 
    data.pinsArray[10] = LCD_2_D_out32; 
    data.pinsArray[11] = LCD_2_E_out33; 
    data.pinsArray[12] = LCD_2_F_out34; 
    data.pinsArray[13] = LCD_2_G_out35; 
    
    data.pinsArray[14] = LCD_3_A_out36; 
    data.pinsArray[15] = LCD_3_B_out37; 
    data.pinsArray[16] = LCD_3_C_out38; 
    data.pinsArray[17] = LCD_3_D_out39; 
    data.pinsArray[18] = LCD_3_E_out40; 
    data.pinsArray[19] = LCD_3_F_out41; 
    data.pinsArray[20] = LCD_3_G_out42; 
    
    data.pinsArray[21] = LCD_4_A_out43; 
    data.pinsArray[22] = LCD_4_B_out44; 
    data.pinsArray[23] = LCD_4_C_out45; 
    data.pinsArray[24] = LCD_4_D_out46; 
    data.pinsArray[25] = LCD_4_E_out47; 
    data.pinsArray[26] = LCD_4_F_out48; 
    data.pinsArray[27] = LCD_4_G_out49; 

//delay(holdBootTime); // delay two seconds, so when the main switch is turned on, the OMEGA2560 is the last controller to turn on
lowFunction(&data); // clears array
//delay(10);
set50Function(&data); // Display shows 50 empty spaces the very firt time system boots

data.noOfspaces = NumberOfCarsParkings; // 

// start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

}    
    
  
     
void loop() // loop take aproximatelly 170.129 uS
{
  
    val_2 = digitalRead(clock_in);  // read whether clock is high ort low at entrance 
    val_3 = digitalRead(clock_out);  // read whether clock is high ort low at exit
  
//***************************************************************************************************************************** // detecting if the entrance clock has gone HIGH or LOW

    if(val_2 == 1)  // car detected at the entrance 
    {
       data.EntrancePulseFlag = true;
       Serial.print("-"); // Print character to dealy the loop 1uS
    }
    			
    else if(val_2 == 0)  // nothing detected at the enctrance 
    {
        data.EntrancePulseFlag = false;
        //Serial.println("INSIDE DATA.ENTRANCEFLAG=FALSE");
    } 
    else  // ERROR code, in case a bugous reading appears 
    {
        return;
    }
  	

	
    if (data.EntrancePulseFlag == true)  // the rising edge of a pulse has been detected 
    {
        data.prevEntrancePulseFlag = true;
        Serial.print("-");  // Print character to dealy the loop 1uS
    }
    
    else if((data.EntrancePulseFlag == false) && (data.prevEntrancePulseFlag == false))   // the pulse has not gone high  and there has not been a a rising edge yet  
    {
        timeElapsedIn = 0;  // set microseconds counter = 0 uS
        data.up = false;
    }

    else if((data.EntrancePulseFlag == false) && (data.prevEntrancePulseFlag == true))  // after having detected a rising edge, a fallling adge 
    {                                                                                   // has been detected
        //Serial.println(timeElapsedIn);
        if ((timeElapsedIn >= sizeOftimeElapsedInMin) && (timeElapsedIn <= sizeOftimeElapsedInMax))  // time from when timer resets (rising edge)and when it gets checked (falling edge ) 
        {
            data.up = true;
            data.prevEntrancePulseFlag = false;
            timeElapsedIn = 0;
            //Serial.println("time reset");
        }
        else
        {
            data.up = false;
            timeElapsedIn = 0;
        }
    }
//****************************************************************************************************************************************************************        
        


//***************************************************************************************************************************** // detecting if the ERXIT clock has gone HIGH or LOW

    if(val_3 == 1)  // car detected at the exit 
    {
       data.ExitPulseFlag = true;
       Serial.print("-");  // Print character to dealy the loop 1uS

    }
    			
    else if(val_3 == 0)  // nothing detected at the enctrance 
    {
        data.ExitPulseFlag = false;
        //Serial.println("INSIDE DATA.ENTRANCEFLAG=FALSE");
    } 
    else  // ERROR code, in case a bugous reading appears 
    {
        return;
    }
  	

	
    if (data.ExitPulseFlag == true)  // the rising edge of a pulse has been detected 
    {
        data.prevExitPulseFlag = true;
        Serial.print("-");  // Print character to dealy the loop 1uS
    }
    
    else if((data.ExitPulseFlag == false) && (data.prevExitPulseFlag == false))   // the pulse has not gone high  and there has not been a a rising edge yet  
    {
        timeElapsedOut = 0;  // set microseconds counter = 0 uS
        data.down = false;
    }

    else if((data.ExitPulseFlag == false) && (data.prevExitPulseFlag == true))  // after having detected a rising edge, a fallling adge 
    {                                                                                   // has been detected
        //Serial.println(timeElapsedOut);
        if ((timeElapsedOut >= sizeOftimeElapsedOutMin) && (timeElapsedOut <= sizeOftimeElapsedOutMax))  // time from when timer resets (rising edge)and when it gets checked (falling edge ) 
        {
            data.down = true;
            data.prevExitPulseFlag = false;
            timeElapsedOut = 0;
                            
            //clearFunction(&data);
            //delay(1000);
            //Serial.println("time reset");
        }
        else
        {
            data.down = false;
            timeElapsedOut = 0; 
        }
    }
//****************************************************************************************************************************************************************  
 
 
    count = countingFunction(&data);  // call countingFunction, that will return count
    data.noOfspaces = count; // store count onto a variable (data.noOfSpaces) that belong to structure 
    LCD_DisplayFunction(&data);  // call function that will display the count valie onto LCD Display 
    internetFunction(&data);
    

}

//####################################################################################################################################################################################################################  
//####################################################################################################################################################################################################################
//####################################################################################################################################################################################################################

void lowFunction(AllPinVar *data) // bring all LCD OUTPUT pins to a LOW
{
    for(int i = 0; i < noOfPins; i++)
    {
        digitalWrite(data->pinsArray[i], LOW);
    }
}

//####################################################################################################################################################################################################################  
//####################################################################################################################################################################################################################
//####################################################################################################################################################################################################################


void clearFunction(AllPinVar *data)  // functyion to clear variables after 7seg display has changed 
{
//    data->up = false;
//    data->down = false;
    data->EntrancePulseFlag = false;
    //data->prevEntrancePulseFlag = false;
}

//####################################################################################################################################################################################################################  
//####################################################################################################################################################################################################################
//####################################################################################################################################################################################################################

void set50Function(AllPinVar *data)  // at startup (very first time sistem comes on) this finction sets LCD 7seg to count = 50
{
    digitalWrite(data->pinsArray[0], HIGH);
    digitalWrite(data->pinsArray[1], HIGH);
    digitalWrite(data->pinsArray[2], HIGH);
    digitalWrite(data->pinsArray[3], HIGH);
    digitalWrite(data->pinsArray[4], HIGH);
    digitalWrite(data->pinsArray[5], HIGH);
    digitalWrite(data->pinsArray[6], LOW);
    
    digitalWrite(data->pinsArray[7], HIGH);
    digitalWrite(data->pinsArray[8], LOW);
    digitalWrite(data->pinsArray[9], HIGH);
    digitalWrite(data->pinsArray[10], HIGH);
    digitalWrite(data->pinsArray[11], LOW);
    digitalWrite(data->pinsArray[12], HIGH);
    digitalWrite(data->pinsArray[13], HIGH);
    
    digitalWrite(data->pinsArray[14], LOW);
    digitalWrite(data->pinsArray[15], LOW);
    digitalWrite(data->pinsArray[16], LOW);
    digitalWrite(data->pinsArray[17], LOW);
    digitalWrite(data->pinsArray[18], LOW);
    digitalWrite(data->pinsArray[19], LOW);
    digitalWrite(data->pinsArray[20], LOW);
    
    digitalWrite(data->pinsArray[21], LOW);
    digitalWrite(data->pinsArray[22], LOW);
    digitalWrite(data->pinsArray[23], LOW);
    digitalWrite(data->pinsArray[24], LOW);
    digitalWrite(data->pinsArray[25], LOW);
    digitalWrite(data->pinsArray[26], LOW);
    digitalWrite(data->pinsArray[27], LOW);
}

//####################################################################################################################################################################################################################  
//####################################################################################################################################################################################################################
//####################################################################################################################################################################################################################

int countingFunction(AllPinVar *data)
{
     if((data->up == true)&&(data->down == false)) // decrease the number of available spaces when (clock_in ) is triggered 
     {
         count -= 1; // count down
         //Serial.println(count);
     }
     
     else if((data->down == true)&&(data->up == false)) // increase the number of available spaces when (clock_out ) is triggered
     {
         count += 1; // count up
        //Serial.println(count);
     }
     
     else if((data->down == true)&&(data->up == true)) // clock_in and clock_out are thriggered at the same time, do nothing 
     {
         count = count; // keep count 
         //Serial.println(count);
     }
     
     else if((data->down == false)&&(data->up == false)) // neither clock_in or clock_out are triggered, do noyhing 
     {
         count = count; // keep count 
         //Serial.println(count);
     }
     
     else  // error, keep the same count 
     {
         count = count;  // keep count 
     }
     
     internerCarCount = count;
     return count; // the  value of count is returened to be displayed in the four 7-seg displays
    
}


//####################################################################################################################################################################################################################  
//####################################################################################################################################################################################################################
//####################################################################################################################################################################################################################

void internetFunction(AllPinVar *data)
{
     EthernetClient client = server.available();
  if (client) {
    //Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        //Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
	  client.println("Refresh: 0.5");  // refresh the page automatically every 5 sec
          client.println();
          client.println("<!DOCTYPE HTML>");
          client.println("<html>");
          // output the value of each analog input pin
          
          // Saving number of cars to server 
//            client.print("<H1>");
            
            client.print("<font size=30>");
            client.print("<b>");
            client.print("<center>");
            client.print(internerCarCount);
            client.println("<br />");  
            client.print("</center>");
            client.print("</font>");
            client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } 
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    //delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }



return;
}

//####################################################################################################################################################################################################################  
//####################################################################################################################################################################################################################
//####################################################################################################################################################################################################################

void LCD_DisplayFunction(AllPinVar *data)  // function that updates the display 
{
  
    switch(data->noOfspaces) // switch between the number that should get displayed in LCD 7 segment 
    {

      case 50:
        {                      
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], HIGH);
                digitalWrite(data->pinsArray[5], HIGH);
                digitalWrite(data->pinsArray[6], LOW);
                
                digitalWrite(data->pinsArray[7], HIGH);
                digitalWrite(data->pinsArray[8], LOW);
                digitalWrite(data->pinsArray[9], HIGH);
                digitalWrite(data->pinsArray[10], HIGH);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], HIGH);
                digitalWrite(data->pinsArray[13], HIGH);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 49:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], LOW);
                digitalWrite(data->pinsArray[5], HIGH);
                digitalWrite(data->pinsArray[6], HIGH);
                
                digitalWrite(data->pinsArray[7], LOW);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], HIGH);
                digitalWrite(data->pinsArray[10], LOW);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], HIGH);
                digitalWrite(data->pinsArray[13], HIGH);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
        
        case 48:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], HIGH);
                digitalWrite(data->pinsArray[5], HIGH);
                digitalWrite(data->pinsArray[6], HIGH);
                
                digitalWrite(data->pinsArray[7], LOW);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], HIGH);
                digitalWrite(data->pinsArray[10], LOW);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], HIGH);
                digitalWrite(data->pinsArray[13], HIGH);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
        
        case 47:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], LOW);
                digitalWrite(data->pinsArray[4], LOW);
                digitalWrite(data->pinsArray[5], LOW);
                digitalWrite(data->pinsArray[6], LOW);
                
                digitalWrite(data->pinsArray[7], LOW);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], HIGH);
                digitalWrite(data->pinsArray[10], LOW);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], HIGH);
                digitalWrite(data->pinsArray[13], HIGH);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
        
        case 46:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], LOW);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], HIGH);
                digitalWrite(data->pinsArray[5], HIGH);
                digitalWrite(data->pinsArray[6], HIGH);
                
                digitalWrite(data->pinsArray[7], LOW);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], HIGH);
                digitalWrite(data->pinsArray[10], LOW);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], HIGH);
                digitalWrite(data->pinsArray[13], HIGH);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
        
        case 45:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], LOW);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], LOW);
                digitalWrite(data->pinsArray[5], HIGH);
                digitalWrite(data->pinsArray[6], HIGH);
                
                digitalWrite(data->pinsArray[7], LOW);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], HIGH);
                digitalWrite(data->pinsArray[10], LOW);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], HIGH);
                digitalWrite(data->pinsArray[13], HIGH);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;
      }
        
        case 44:
        {
                        
                digitalWrite(data->pinsArray[0], LOW);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], LOW);
                digitalWrite(data->pinsArray[4], LOW);
                digitalWrite(data->pinsArray[5], HIGH);
                digitalWrite(data->pinsArray[6], HIGH);
                
                digitalWrite(data->pinsArray[7], LOW);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], HIGH);
                digitalWrite(data->pinsArray[10], LOW);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], HIGH);
                digitalWrite(data->pinsArray[13], HIGH);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 43:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], LOW);
                digitalWrite(data->pinsArray[5], LOW);
                digitalWrite(data->pinsArray[6], HIGH);
                
                digitalWrite(data->pinsArray[7], LOW);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], HIGH);
                digitalWrite(data->pinsArray[10], LOW);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], HIGH);
                digitalWrite(data->pinsArray[13], HIGH);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 42:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], LOW);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], HIGH);
                digitalWrite(data->pinsArray[5], LOW);
                digitalWrite(data->pinsArray[6], HIGH);
                
                digitalWrite(data->pinsArray[7], LOW);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], HIGH);
                digitalWrite(data->pinsArray[10], LOW);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], HIGH);
                digitalWrite(data->pinsArray[13], HIGH);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 41:
        {
                        
                digitalWrite(data->pinsArray[0], LOW);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], LOW);
                digitalWrite(data->pinsArray[4], LOW);
                digitalWrite(data->pinsArray[5], LOW);
                digitalWrite(data->pinsArray[6], LOW);
                
                digitalWrite(data->pinsArray[7], LOW);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], HIGH);
                digitalWrite(data->pinsArray[10], LOW);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], HIGH);
                digitalWrite(data->pinsArray[13], HIGH);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 40:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], HIGH);
                digitalWrite(data->pinsArray[5], HIGH);
                digitalWrite(data->pinsArray[6], LOW);
                
                digitalWrite(data->pinsArray[7], LOW);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], HIGH);
                digitalWrite(data->pinsArray[10], LOW);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], HIGH);
                digitalWrite(data->pinsArray[13], HIGH);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 39:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], LOW);
                digitalWrite(data->pinsArray[5], HIGH);
                digitalWrite(data->pinsArray[6], HIGH);
                
                digitalWrite(data->pinsArray[7], HIGH);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], HIGH);
                digitalWrite(data->pinsArray[10], HIGH);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], HIGH);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 38:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], HIGH);
                digitalWrite(data->pinsArray[5], HIGH);
                digitalWrite(data->pinsArray[6], HIGH);
                
                digitalWrite(data->pinsArray[7], HIGH);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], HIGH);
                digitalWrite(data->pinsArray[10], HIGH);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], HIGH);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 37:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], LOW);
                digitalWrite(data->pinsArray[4], LOW);
                digitalWrite(data->pinsArray[5], LOW);
                digitalWrite(data->pinsArray[6], LOW);
                
                digitalWrite(data->pinsArray[7], HIGH);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], HIGH);
                digitalWrite(data->pinsArray[10], HIGH);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], HIGH);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 36:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], LOW);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], HIGH);
                digitalWrite(data->pinsArray[5], HIGH);
                digitalWrite(data->pinsArray[6], HIGH);
                
                digitalWrite(data->pinsArray[7], HIGH);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], HIGH);
                digitalWrite(data->pinsArray[10], HIGH);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], HIGH);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 35:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], LOW);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], LOW);
                digitalWrite(data->pinsArray[5], HIGH);
                digitalWrite(data->pinsArray[6], HIGH);
                
                digitalWrite(data->pinsArray[7], HIGH);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], HIGH);
                digitalWrite(data->pinsArray[10], HIGH);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], HIGH);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 34:
        {
                        
                digitalWrite(data->pinsArray[0], LOW);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], LOW);
                digitalWrite(data->pinsArray[4], LOW);
                digitalWrite(data->pinsArray[5], HIGH);
                digitalWrite(data->pinsArray[6], HIGH);
                
                digitalWrite(data->pinsArray[7], HIGH);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], HIGH);
                digitalWrite(data->pinsArray[10], HIGH);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], HIGH);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 33:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], LOW);
                digitalWrite(data->pinsArray[5], LOW);
                digitalWrite(data->pinsArray[6], HIGH);
                
                digitalWrite(data->pinsArray[7], HIGH);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], HIGH);
                digitalWrite(data->pinsArray[10], HIGH);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], HIGH);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 32:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], LOW);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], HIGH);
                digitalWrite(data->pinsArray[5], LOW);
                digitalWrite(data->pinsArray[6], HIGH);
                
                digitalWrite(data->pinsArray[7], HIGH);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], HIGH);
                digitalWrite(data->pinsArray[10], HIGH);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], HIGH);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 31:
        {
                        
                digitalWrite(data->pinsArray[0], LOW);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], LOW);
                digitalWrite(data->pinsArray[4], LOW);
                digitalWrite(data->pinsArray[5], LOW);
                digitalWrite(data->pinsArray[6], LOW);
                
                digitalWrite(data->pinsArray[7], HIGH);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], HIGH);
                digitalWrite(data->pinsArray[10], HIGH);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], HIGH);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 30:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], HIGH);
                digitalWrite(data->pinsArray[5], HIGH);
                digitalWrite(data->pinsArray[6], LOW);
                
                digitalWrite(data->pinsArray[7], HIGH);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], HIGH);
                digitalWrite(data->pinsArray[10], HIGH);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], HIGH);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 29:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], LOW);
                digitalWrite(data->pinsArray[5], HIGH);
                digitalWrite(data->pinsArray[6], HIGH);
                
                digitalWrite(data->pinsArray[7], HIGH);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], LOW);
                digitalWrite(data->pinsArray[10], HIGH);
                digitalWrite(data->pinsArray[11], HIGH);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], HIGH);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 28:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], HIGH);
                digitalWrite(data->pinsArray[5], HIGH);
                digitalWrite(data->pinsArray[6], HIGH);
                
                digitalWrite(data->pinsArray[7], HIGH);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], LOW);
                digitalWrite(data->pinsArray[10], HIGH);
                digitalWrite(data->pinsArray[11], HIGH);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], HIGH);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 27:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], LOW);
                digitalWrite(data->pinsArray[4], LOW);
                digitalWrite(data->pinsArray[5], LOW);
                digitalWrite(data->pinsArray[6], LOW);
                
                digitalWrite(data->pinsArray[7], HIGH);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], LOW);
                digitalWrite(data->pinsArray[10], HIGH);
                digitalWrite(data->pinsArray[11], HIGH);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], HIGH);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 26:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], LOW);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], HIGH);
                digitalWrite(data->pinsArray[5], HIGH);
                digitalWrite(data->pinsArray[6], HIGH);
                
                digitalWrite(data->pinsArray[7], HIGH);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], LOW);
                digitalWrite(data->pinsArray[10], HIGH);
                digitalWrite(data->pinsArray[11], HIGH);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], HIGH);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 25:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], LOW);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], LOW);
                digitalWrite(data->pinsArray[5], HIGH);
                digitalWrite(data->pinsArray[6], HIGH);
                
                digitalWrite(data->pinsArray[7], HIGH);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], LOW);
                digitalWrite(data->pinsArray[10], HIGH);
                digitalWrite(data->pinsArray[11], HIGH);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], HIGH);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 24:
        {
                        
                digitalWrite(data->pinsArray[0], LOW);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], LOW);
                digitalWrite(data->pinsArray[4], LOW);
                digitalWrite(data->pinsArray[5], HIGH);
                digitalWrite(data->pinsArray[6], HIGH);
                
                digitalWrite(data->pinsArray[7], HIGH);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], LOW);
                digitalWrite(data->pinsArray[10], HIGH);
                digitalWrite(data->pinsArray[11], HIGH);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], HIGH);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 23:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], LOW);
                digitalWrite(data->pinsArray[5], LOW);
                digitalWrite(data->pinsArray[6], HIGH);
                
                digitalWrite(data->pinsArray[7], HIGH);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], LOW);
                digitalWrite(data->pinsArray[10], HIGH);
                digitalWrite(data->pinsArray[11], HIGH);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], HIGH);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 22:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], LOW);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], HIGH);
                digitalWrite(data->pinsArray[5], LOW);
                digitalWrite(data->pinsArray[6], HIGH);
                
                digitalWrite(data->pinsArray[7], HIGH);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], LOW);
                digitalWrite(data->pinsArray[10], HIGH);
                digitalWrite(data->pinsArray[11], HIGH);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], HIGH);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 21:
        {
                        
                digitalWrite(data->pinsArray[0], LOW);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], LOW);
                digitalWrite(data->pinsArray[4], LOW);
                digitalWrite(data->pinsArray[5], LOW);
                digitalWrite(data->pinsArray[6], LOW);
                
                digitalWrite(data->pinsArray[7], HIGH);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], LOW);
                digitalWrite(data->pinsArray[10], HIGH);
                digitalWrite(data->pinsArray[11], HIGH);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], HIGH);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 20:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], HIGH);
                digitalWrite(data->pinsArray[5], HIGH);
                digitalWrite(data->pinsArray[6], LOW);
                
                digitalWrite(data->pinsArray[7], HIGH);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], LOW);
                digitalWrite(data->pinsArray[10], HIGH);
                digitalWrite(data->pinsArray[11], HIGH);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], HIGH);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 19:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], LOW);
                digitalWrite(data->pinsArray[5], HIGH);
                digitalWrite(data->pinsArray[6], HIGH);
                
                digitalWrite(data->pinsArray[7], LOW);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], HIGH);
                digitalWrite(data->pinsArray[10], LOW);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], LOW);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 18:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], HIGH);
                digitalWrite(data->pinsArray[5], HIGH);
                digitalWrite(data->pinsArray[6], HIGH);
                
                digitalWrite(data->pinsArray[7], LOW);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], HIGH);
                digitalWrite(data->pinsArray[10], LOW);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], LOW);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 17:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], LOW);
                digitalWrite(data->pinsArray[4], LOW);
                digitalWrite(data->pinsArray[5], LOW);
                digitalWrite(data->pinsArray[6], LOW);
                
                digitalWrite(data->pinsArray[7], LOW);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], HIGH);
                digitalWrite(data->pinsArray[10], LOW);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], LOW);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 16:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], LOW);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], HIGH);
                digitalWrite(data->pinsArray[5], HIGH);
                digitalWrite(data->pinsArray[6], HIGH);
                
                digitalWrite(data->pinsArray[7], LOW);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], HIGH);
                digitalWrite(data->pinsArray[10], LOW);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], LOW);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 15:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], LOW);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], LOW);
                digitalWrite(data->pinsArray[5], HIGH);
                digitalWrite(data->pinsArray[6], HIGH);
                
                digitalWrite(data->pinsArray[7], LOW);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], HIGH);
                digitalWrite(data->pinsArray[10], LOW);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], LOW);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 14:
        {
                        
                digitalWrite(data->pinsArray[0], LOW);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], LOW);
                digitalWrite(data->pinsArray[4], LOW);
                digitalWrite(data->pinsArray[5], HIGH);
                digitalWrite(data->pinsArray[6], HIGH);
                
                digitalWrite(data->pinsArray[7], LOW);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], HIGH);
                digitalWrite(data->pinsArray[10], LOW);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], LOW);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 13:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], LOW);
                digitalWrite(data->pinsArray[5], LOW);
                digitalWrite(data->pinsArray[6], HIGH);
                
                digitalWrite(data->pinsArray[7], LOW);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], HIGH);
                digitalWrite(data->pinsArray[10], LOW);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], LOW);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 12:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], LOW);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], HIGH);
                digitalWrite(data->pinsArray[5], LOW);
                digitalWrite(data->pinsArray[6], HIGH);
                
                digitalWrite(data->pinsArray[7], LOW);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], HIGH);
                digitalWrite(data->pinsArray[10], LOW);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], LOW);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 11:
        {
                        
                digitalWrite(data->pinsArray[0], LOW);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], LOW);
                digitalWrite(data->pinsArray[4], LOW);
                digitalWrite(data->pinsArray[5], LOW);
                digitalWrite(data->pinsArray[6], LOW);
                
                digitalWrite(data->pinsArray[7], LOW);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], HIGH);
                digitalWrite(data->pinsArray[10], LOW);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], LOW);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 10:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], HIGH);
                digitalWrite(data->pinsArray[5], HIGH);
                digitalWrite(data->pinsArray[6], LOW);
                
                digitalWrite(data->pinsArray[7], LOW);
                digitalWrite(data->pinsArray[8], HIGH);
                digitalWrite(data->pinsArray[9], HIGH);
                digitalWrite(data->pinsArray[10], LOW);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], LOW);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 9:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], LOW);
                digitalWrite(data->pinsArray[5], HIGH);
                digitalWrite(data->pinsArray[6], HIGH);
                
                digitalWrite(data->pinsArray[7], LOW);
                digitalWrite(data->pinsArray[8], LOW);
                digitalWrite(data->pinsArray[9], LOW);
                digitalWrite(data->pinsArray[10], LOW);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], LOW);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 8:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], HIGH);
                digitalWrite(data->pinsArray[5], HIGH);
                digitalWrite(data->pinsArray[6], HIGH);
                
                digitalWrite(data->pinsArray[7], LOW);
                digitalWrite(data->pinsArray[8], LOW);
                digitalWrite(data->pinsArray[9], LOW);
                digitalWrite(data->pinsArray[10], LOW);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], LOW);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 7:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], LOW);
                digitalWrite(data->pinsArray[4], LOW);
                digitalWrite(data->pinsArray[5], LOW);
                digitalWrite(data->pinsArray[6], LOW);
                
                digitalWrite(data->pinsArray[7], LOW);
                digitalWrite(data->pinsArray[8], LOW);
                digitalWrite(data->pinsArray[9], LOW);
                digitalWrite(data->pinsArray[10], LOW);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], LOW);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 6:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], LOW);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], HIGH);
                digitalWrite(data->pinsArray[5], HIGH);
                digitalWrite(data->pinsArray[6], HIGH);
                
                digitalWrite(data->pinsArray[7], LOW);
                digitalWrite(data->pinsArray[8], LOW);
                digitalWrite(data->pinsArray[9], LOW);
                digitalWrite(data->pinsArray[10], LOW);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], LOW);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 5:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], LOW);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], LOW);
                digitalWrite(data->pinsArray[5], HIGH);
                digitalWrite(data->pinsArray[6], HIGH);
                
                digitalWrite(data->pinsArray[7], LOW);
                digitalWrite(data->pinsArray[8], LOW);
                digitalWrite(data->pinsArray[9], LOW);
                digitalWrite(data->pinsArray[10], LOW);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], LOW);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 4:
        {
                        
                digitalWrite(data->pinsArray[0], LOW);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], LOW);
                digitalWrite(data->pinsArray[4], LOW);
                digitalWrite(data->pinsArray[5], HIGH);
                digitalWrite(data->pinsArray[6], HIGH);
                
                digitalWrite(data->pinsArray[7], LOW);
                digitalWrite(data->pinsArray[8], LOW);
                digitalWrite(data->pinsArray[9], LOW);
                digitalWrite(data->pinsArray[10], LOW);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], LOW);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 3:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], LOW);
                digitalWrite(data->pinsArray[5], LOW);
                digitalWrite(data->pinsArray[6], HIGH);
                
                digitalWrite(data->pinsArray[7], LOW);
                digitalWrite(data->pinsArray[8], LOW);
                digitalWrite(data->pinsArray[9], LOW);
                digitalWrite(data->pinsArray[10], LOW);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], LOW);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 2:
        {
                        
                digitalWrite(data->pinsArray[0], HIGH);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], LOW);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], HIGH);
                digitalWrite(data->pinsArray[5], LOW);
                digitalWrite(data->pinsArray[6], HIGH);
                
                digitalWrite(data->pinsArray[7], LOW);
                digitalWrite(data->pinsArray[8], LOW);
                digitalWrite(data->pinsArray[9], LOW);
                digitalWrite(data->pinsArray[10], LOW);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], LOW);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 1:
        {
                        
                digitalWrite(data->pinsArray[0], LOW);
                digitalWrite(data->pinsArray[1], HIGH);
                digitalWrite(data->pinsArray[2], HIGH);
                digitalWrite(data->pinsArray[3], LOW);
                digitalWrite(data->pinsArray[4], LOW);
                digitalWrite(data->pinsArray[5], LOW);
                digitalWrite(data->pinsArray[6], LOW);
                
                digitalWrite(data->pinsArray[7], LOW);
                digitalWrite(data->pinsArray[8], LOW);
                digitalWrite(data->pinsArray[9], LOW);
                digitalWrite(data->pinsArray[10], LOW);
                digitalWrite(data->pinsArray[11], LOW);
                digitalWrite(data->pinsArray[12], LOW);
                digitalWrite(data->pinsArray[13], LOW);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], LOW);
                digitalWrite(data->pinsArray[16], LOW);
                digitalWrite(data->pinsArray[17], LOW);
                digitalWrite(data->pinsArray[18], LOW);
                digitalWrite(data->pinsArray[19], LOW);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], LOW);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], LOW);
                digitalWrite(data->pinsArray[26], LOW);
                digitalWrite(data->pinsArray[27], LOW);
      break;  
      }
      
      case 0:  // parking lot full, no available spaces 
        {
                        
                digitalWrite(data->pinsArray[0], LOW);
                digitalWrite(data->pinsArray[1], LOW);
                digitalWrite(data->pinsArray[2], LOW);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], HIGH);
                digitalWrite(data->pinsArray[5], HIGH);
                digitalWrite(data->pinsArray[6], LOW);
                
                digitalWrite(data->pinsArray[7], LOW);
                digitalWrite(data->pinsArray[8], LOW);
                digitalWrite(data->pinsArray[9], LOW);
                digitalWrite(data->pinsArray[10], HIGH);
                digitalWrite(data->pinsArray[11], HIGH);
                digitalWrite(data->pinsArray[12], HIGH);
                digitalWrite(data->pinsArray[13], LOW);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], HIGH);
                digitalWrite(data->pinsArray[16], HIGH);
                digitalWrite(data->pinsArray[17], HIGH);
                digitalWrite(data->pinsArray[18], HIGH);
                digitalWrite(data->pinsArray[19], HIGH);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], HIGH);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], HIGH);
                digitalWrite(data->pinsArray[26], HIGH);
                digitalWrite(data->pinsArray[27], HIGH);
      break;  
      }
      
      default:  // more cars exited the garage than the number of cars that entered 
         {
                digitalWrite(data->pinsArray[0], LOW);
                digitalWrite(data->pinsArray[1], LOW);
                digitalWrite(data->pinsArray[2], LOW);
                digitalWrite(data->pinsArray[3], HIGH);
                digitalWrite(data->pinsArray[4], HIGH);
                digitalWrite(data->pinsArray[5], HIGH);
                digitalWrite(data->pinsArray[6], LOW);
                
                digitalWrite(data->pinsArray[7], LOW);
                digitalWrite(data->pinsArray[8], LOW);
                digitalWrite(data->pinsArray[9], LOW);
                digitalWrite(data->pinsArray[10], HIGH);
                digitalWrite(data->pinsArray[11], HIGH);
                digitalWrite(data->pinsArray[12], HIGH);
                digitalWrite(data->pinsArray[13], LOW);
                
                digitalWrite(data->pinsArray[14], LOW);
                digitalWrite(data->pinsArray[15], HIGH);
                digitalWrite(data->pinsArray[16], HIGH);
                digitalWrite(data->pinsArray[17], HIGH);
                digitalWrite(data->pinsArray[18], HIGH);
                digitalWrite(data->pinsArray[19], HIGH);
                digitalWrite(data->pinsArray[20], LOW);
                
                digitalWrite(data->pinsArray[21], HIGH);
                digitalWrite(data->pinsArray[22], LOW);
                digitalWrite(data->pinsArray[23], LOW);
                digitalWrite(data->pinsArray[24], LOW);
                digitalWrite(data->pinsArray[25], HIGH);
                digitalWrite(data->pinsArray[26], HIGH);
                digitalWrite(data->pinsArray[27], HIGH);
         } 
    } 
}



