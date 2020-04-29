#include<DallasTemperature.h>
#include<OneWire.h>
#define ONE_WIRE_BUS 4  // D2 Pin in NodeMCU
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>

// WiFi network info.
char ssid[] = "Red Skull";
char pass[] = "nopassword";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "45289030-5d29-11ea-ba7c-716e7f5ba423";  // Got in cayenne-My device
char password[] = "b08a75f26c4a4a9b85210be0e5ffc49e1122cd23"; 
char clientID[] = "3ed67e10-5d2c-11ea-8221-599f77add412";

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire); // passing the refernece to dallas Temp
int Pulse=A0; //analog input pin number
int threshold=550; //default pulse rate in pulse sensor 

void setup()
{
     Serial.println("Connecting to ");
       Serial.println(ssid); 
 
       WiFi.begin(ssid, pass); 
       while (WiFi.status() != WL_CONNECTED) 
          {
            delay(500);
            Serial.print(".");
          }
      Serial.println("");
      Serial.println("WiFi connected"); 
    pinMode(Pulse,INPUT);//making A0 pin as input pin
    Serial.begin(9600); //setting baud rate
    sensors.begin(); // starting sensors executing
    Cayenne.begin(username, password, clientID); //Connection establishing to cayenne
    
}

int New_Pulse=0; // intial value
float temp=0;
void loop()
{
    sensors.requestTemperatures();                // Send the command to get temperatures  
    Serial.println("Temperature is: ");
    temp= sensors.getTempCByIndex(0);  // Why "byIndex"? You can have more than one IC on the same bus. 0 refers to the first IC on the wire  
    Serial.print(temp);  //31 C is normal temp in wrist
    Serial.println("\u2103");  
  
   New_Pulse=analogRead(Pulse); //Reading input from A0 pin
   
      if(New_Pulse>threshold){ //ignore noise value
        New_Pulse=(1.0/New_Pulse) * 60.0 * 1000;

          if(New_Pulse>100 && New_Pulse !=104)
          {
            Serial.print("Please Take Action Heart Beat is: \n");
            Serial.println(New_Pulse);
            
          }
          else if(New_Pulse==104) // Default noice value 
                  Serial.println("No Heart Beat is Detected ");
               else{
                Serial.print("Your BPM is: ");
                Serial.println(New_Pulse);  //60 to 100 is normal BPM
                
             }
              
        }
    Cayenne.loop();
    Cayenne.virtualWrite(0, New_Pulse);;// Sending pulse rate to cayenne ( 0 is virtual channel number , New_Pulse is BPM)
    Cayenne.virtualWrite(1, temp);;
    //Serial.print("Data Sending to cayenne\n");
    Serial.println("Wait 15 sec Thank You!");
    Serial.println("--------------------------------------\n");
 
 delay(15000);
}

CAYENNE_OUT_DEFAULT()
{
  //Cayenne.virtualWrite(0, New_Pulse);;// Sending pulse rate to cayenne ( 0 is virtual channel number , New_Pulse is BPM)
  //Cayenne.virtualWrite(1, temp);;
  Serial.print("Data Sending to cayenne\n");
  

}

CAYENNE_IN_DEFAULT()
{
  CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
  //Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}
