// This example shows how to connect to Cayenne using an ESP8266 and send/receive sample data.
// Make sure you install the ESP8266 Board Package via the Arduino IDE Board Manager and select the correct ESP8266 board before compiling. 

//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>

// WiFi network info.
char ssid[] = "Red Skull";
char wifiPassword[] = "nopassword";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "45289030-5d29-11ea-ba7c-716e7f5ba423";
char password[] = "b08a75f26c4a4a9b85210be0e5ffc49e1122cd23";
char clientID[] = "3ed67e10-5d2c-11ea-8221-599f77add412";

unsigned long lastMillis = 0;

void setup() {
  Serial.begin(9600);
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);
}

void loop() {
  Cayenne.loop();
}

// Default function for sending sensor data at intervals to Cayenne.
// You can also use functions for specific channels, e.g CAYENNE_OUT(1) for sending channel 1 data.
CAYENNE_OUT_DEFAULT()
{
  // Write data to Cayenne here. This example just sends the current uptime in milliseconds on virtual channel 0.
  Cayenne.virtualWrite(0, millis());
  // Some examples of other functions you can use to send data.
  //Cayenne.celsiusWrite(1, 22.0);
  //Cayenne.luxWrite(2, 700);
  //Cayenne.virtualWrite(3, 50, TYPE_PROXIMITY, UNIT_CENTIMETER);
}

// Default function for processing actuator commands from the Cayenne Dashboard.
// You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT()
{
  CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
  //Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}
