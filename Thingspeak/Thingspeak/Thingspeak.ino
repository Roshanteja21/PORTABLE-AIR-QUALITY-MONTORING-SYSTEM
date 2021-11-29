#include "Adafruit_PM25AQI.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
Adafruit_PM25AQI aqi = Adafruit_PM25AQI();

#include <DHT.h>

#include <SoftwareSerial.h>
char ssid[] = "Vijaya venu"; //You can replace the wifi name to your wifi 
char pass[] = "vijayavenu2022";  //Type password of your wifi.
SoftwareSerial EspSerial(2, 3);
String HOST = "api.thingspeak.com";
String PORT = "80";

String apiKey = "I4FRSZMLSIGBH48L";  //Change this key to your "Write API key"

int gas_sensor = A0; //Sensor pin 
float m = -0.3376; //Slope 
float b = 0.7165; //Y-Intercept 
float R0 = 51.21; //Sensor Resistance in fresh air from previous code

#define CO_sensor A1 //Sensor pin 
float m1 = -0.6527; //Slope 
float b1 = 1.30; //Y-Intercept 
float R01 = 42.51; //Sensor Resistance


DHT dht(9, DHT11);
#define DHT11_PIN 9
void setup() {
  Serial.begin(9600);      // PC to Arduino Serial Monitor
  Serial1.begin(115200);   // Arduino to ESP01 Communication
  pinMode(gas_sensor, INPUT);
  pinMode(CO_sensor,INPUT);
  pinMode(9,INPUT);  //For DHT Sensor
  if (! aqi.begin_UART(&pmSerial)) {
   Serial.println("Could not find PM 2.5 sensor!");
    while (1) delay(10);
  }
 Serial.println("PM25 found!");
} 

void loop() { 
  // put your main code here, to run repeatedly:
  float sensor_volt; //Define variable for sensor voltage 
  float RS_gas; //Define variable for sensor resistance  
  float ratio; //Define variable for ratio
  float sensorValue = analogRead(gas_sensor); //Read analog values of sensor  
  sensor_volt = sensorValue*(5.0/1023.0); //Convert analog values to voltage 
    RS_gas = ((5.0*10.0)/sensor_volt)-10.0; //Get value of RS in a gas
  ratio = RS_gas/R0;  // Get ratio RS_gas/RS_air
  double ppm_log = (log10(ratio)-b)/m; //Get ppm value in linear scale according to the the ratio value  
  double ppm = pow(10, ppm_log); //Convert ppm value to log scale 
  Serial.print("Our desired PPM = ");
  Serial.println(ppm);
  

  float sensor_volt1; //Define variable for sensor voltage 
  float RS_gas1; //Define variable for sensor resistance  
  float ratio1; //Define variable for ratio
  float sensorValue1 = analogRead(CO_sensor); //Read analog values of sensor  
  sensor_volt1 = sensorValue1*(5.0/1023.0); //Convert analog values to voltage 
  RS_gas1 = ((5.0*10.0)/sensor_volt1)-10.0; //Get value of RS in a gas
  ratio1 = RS_gas1/R01;  // Get ratio RS_gas/RS_air
  double ppm_log1 = (log10(ratio1)-b1)/m1; //Get ppm value in linear scale according to the the ratio value  
  double ppm1 = pow(10, ppm_log1); //Convert ppm value to log scale 
  Serial.print("CO PPM = ");
  Serial.println(ppm1);


  int chk = DHT.read11(DHT11_PIN);
  Serial.print("Temperature = ");
  float temp = DHT.temperature;
  Serial.println(temp);
  float hum = DHT.humidity;
  Serial.print("Humidity = ");
  Serial.println(hum);

  PM25_AQI_Data data;
   if (! aqi.read(&data)) {
    Serial.println("Could not read from AQI");
    delay(500);  // try again in a bit!
    return;
  }
  Serial.print(F("Particles > 1.0um / 0.1L air:")); 
  Serial.println(data.particles_10um);



  Serial1.println("AT+CIPMUX=0\r\n");      // To Set MUX = 0
  delay(2000);                             // Wait for 2 sec

  // TCP connection 
  String cmd = "AT+CIPSTART=\"TCP\",\"";   // TCP connection with https://thingspeak.com server
  cmd += "184.106.153.149";                // IP addr of api.thingspeak.com
  cmd += "\",80\r\n\r\n";                  // Port No. = 80

  Serial1.println(cmd);                    // Display above Command on PC
  Serial.println(cmd);                     // Send above command to Rx1, Tx1

  delay(1000);                         

  if(Serial1.find("ERROR"))                // If returns error in TCP connection
  { 
    Serial.println("AT+CIPSTART error");   // Display error msg to PC
    //return; 
  }

  // prepare GET string 
  String getStr = "GET /update?api_key=";   
  getStr += apiKey;
  getStr +="&field1=";
  getStr += ppm;
  getStr +="&field2=";
  getStr += ppm1; 
  getStr +="&field3=";
  getStr += temp;
  getStr +="&field4=";
  getStr += hum;
  getstr +="&field5=";
  getstr +=data.particles_10um;
  getStr += "\r\n\r\n"; 

  Serial.println(getStr);                 // Display GET String on PC

  cmd = "AT+CIPSEND=";                    // send data length 
  cmd += String(getStr.length());
  cmd+="\r\n";

  Serial.println(cmd);                   // Display Data length on PC
  Serial1.println(cmd);                  // Send Data length command to Tx1, Rx1
  if(Serial1.find(">"))                    // If prompt opens //verify connection with cloud
  {
    Serial.println("Pushed whole data TO CLOUD");  // Display confirmation msg to PC
    Serial1.print(getStr);                 // Send GET String to Rx1, Tx1
  }
  else
  { 
    Serial1.println("AT+CIPCLOSE\r\n");    // Send Close Connection command to Rx1, Tx1
    Serial.println("AT+CIPCLOSE");         // Display Connection closed command on PC
  } 
  // thingspeak free version needs 15-20 sec delay between every push
  delay(15000);                            // wait for 16sec
 }
