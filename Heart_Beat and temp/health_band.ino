#include<DallasTemperature.h>
#include<OneWire.h>
#define ONE_WIRE_BUS 15  // D2 Pin of NodeMCU
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>
#include <MPU6050.h>
#include <ESP8266WiFi.h>
#include "Gsender.h"


MPU6050 mpu; 

boolean freefallDetected = false;


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
    Cayenne.begin(username, password, clientID); //Connection establishing to 

  //Fall Detection
  Serial.println("Initialize MPU6050");

  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_16G))
  {
    Serial.println("Could not find a valid MPU6050 sensor, check wiring!");
    delay(500);
  }

  mpu.setAccelPowerOnDelay(MPU6050_DELAY_3MS);
  
  mpu.setIntFreeFallEnabled(true);
  mpu.setIntZeroMotionEnabled(false);
  mpu.setIntMotionEnabled(false);
  
  mpu.setDHPFMode(MPU6050_DHPF_5HZ);

  mpu.setFreeFallDetectionThreshold(17);
  mpu.setFreeFallDetectionDuration(2);  
  
  checkSettings();
  
  attachInterrupt(12, doInt, RISING);
    
}

void doInt()
{
  freefallDetected = true;  
}

void checkSettings()
{
  Serial.println();
  
  Serial.print(" * Free Fall Interrupt:       ");
  Serial.println(mpu.getIntFreeFallEnabled() ? "Enabled" : "Disabled");

  Serial.print(" * Free Fal Threshold:          ");
  Serial.println(mpu.getFreeFallDetectionThreshold());

  Serial.print(" * Free FallDuration:           ");
  Serial.println(mpu.getFreeFallDetectionDuration());
  
  Serial.print(" * Clock Source:              ");
  switch(mpu.getClockSource())
  {
    case MPU6050_CLOCK_KEEP_RESET:     Serial.println("Stops the clock and keeps the timing generator in reset"); break;
    case MPU6050_CLOCK_EXTERNAL_19MHZ: Serial.println("PLL with external 19.2MHz reference"); break;
    case MPU6050_CLOCK_EXTERNAL_32KHZ: Serial.println("PLL with external 32.768kHz reference"); break;
    case MPU6050_CLOCK_PLL_ZGYRO:      Serial.println("PLL with Z axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_YGYRO:      Serial.println("PLL with Y axis gyroscope reference"); break;
    case MPU6050_CLOCK_PLL_XGYRO:      Serial.println("PLL with X axis gyroscope reference"); break;
    case MPU6050_CLOCK_INTERNAL_8MHZ:  Serial.println("Internal 8MHz oscillator"); break;
  }
  
  Serial.print(" * Accelerometer:             ");
  switch(mpu.getRange())
  {
    case MPU6050_RANGE_16G:            Serial.println("+/- 16 g"); break;
    case MPU6050_RANGE_8G:             Serial.println("+/- 8 g"); break;
    case MPU6050_RANGE_4G:             Serial.println("+/- 4 g"); break;
    case MPU6050_RANGE_2G:             Serial.println("+/- 2 g"); break;
  }  

  Serial.print(" * Accelerometer offsets:     ");
  Serial.print(mpu.getAccelOffsetX());
  Serial.print(" / ");
  Serial.print(mpu.getAccelOffsetY());
  Serial.print(" / ");
  Serial.println(mpu.getAccelOffsetZ());

  Serial.print(" * Accelerometer power delay: ");
  switch(mpu.getAccelPowerOnDelay())
  {
    case MPU6050_DELAY_3MS:            Serial.println("3ms"); break;
    case MPU6050_DELAY_2MS:            Serial.println("2ms"); break;
    case MPU6050_DELAY_1MS:            Serial.println("1ms"); break;
    case MPU6050_NO_DELAY:             Serial.println("0ms"); break;
  }  
  
  Serial.println();
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
    //Email Service 
    if(New_Pulse > 75 )
    {
        Gsender *gsender = Gsender::Instance();    // Getting pointer to class instance
        String subject = "Secure System Notification ( Heart Beat )";
        if(gsender->Subject(subject)->Send("chethan1996m@gmail.com", "Heart Beat Reaches Threshold value of <br> ",New_Pulse)) {
          Serial.println("Message send.");
        }
        else {
          Serial.print("Error sending message: ");
          Serial.println(gsender->getError());
        }
    }

    if (temp > 28) //if 31 then it's fever
    {
        Gsender *gsender = Gsender::Instance();    // Getting pointer to class instance
        String subject = "Secure System Notification ( Body Temp )";
        if(gsender->Subject(subject)->Send("chethan1996m@gmail.com", "Body Temprature is High Please Take Action <br> Current Body Temp is:  ",temp)) {
            Serial.println("Message send.");
        } else {
            Serial.print("Error sending message: ");
            Serial.println(gsender->getError());
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
  Vector rawAccel = mpu.readRawAccel();
  Activites act = mpu.readActivites();
  Serial.println("Free Fall Value is :");
  Serial.println(act.isFreeFall);

    if (freefallDetected)
    {
      Cayenne.virtualWrite(3, act.isFreeFall); //virtual pin
      Gsender *gsender = Gsender::Instance();    // Getting pointer to class instance
      String subject = "Secure System Notification ( Fall Detection )";
      if(gsender->Subject(subject)->Send("chethan1996m@gmail.com", "Fall Detection Detected please take Action ASAP <br> ",1)) {
          Serial.println("Message send.");
      } else {
          Serial.print("Error sending message: ");
          Serial.println(gsender->getError());
      }
      delay(100);
    }
  

}

CAYENNE_IN_DEFAULT()
{
  CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
  //Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}
