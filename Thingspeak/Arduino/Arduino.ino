#include <ArduinoJson.h>
#include <DHT.h>

//Arduino to NodeMCU Lib
#include <SoftwareSerial.h>

//Initialise Arduino to NodeMCU (5=Rx & 6=Tx)
SoftwareSerial nodemcu(5, 6);

//Initialisation of DHT11 Sensor
#define DHTPIN 9
DHT dht(DHTPIN, DHT11);
float temp;
float hum;
double ppm;
double ppm1;

//Initializing Airquality sensor
int gas_sensor = A0; //Sensor pin 
float m = -0.3376; //Slope 
float b = 0.7165; //Y-Intercept 
float R0 = 2.82; //Sensor Resistance in fresh air from previous code


int CO_sensor = A1; //Sensor pin 
float m1 = -0.6527; //Slope 
float b1 = 1.30; //Y-Intercept 
float R01 = 7.22; //Sensor Resistance


void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(gas_sensor, INPUT);
  pinMode(CO_sensor,INPUT);
  nodemcu.begin(9600);
  delay(1000);

  Serial.println("Program started");
}

void loop() {

  StaticJsonBuffer<2500> jsonBuffer;
  JsonObject& data = jsonBuffer.createObject();
  //Obtain Air qualitity (C02 ppm)
  AQ_func();
  data["Air quality (CO2ppm)"] = ppm;

  //Obtain CO Ppm
  CO_func();
  data["CO ppm"] =  ppm1;

   //Obtain Temp and Hum data
  dht11_func();


  //Assign collected data to JSON Object
  data["humidity"] = hum;
  data["temperature"] = temp; 


  //Send data to NodeMCU
  data.printTo(nodemcu);
  jsonBuffer.clear();
  
  delay(2000);
}

void AQ_func(){
  float sensor_volt; //Define variable for sensor voltage 
  float RS_gas; //Define variable for sensor resistance  
  float ratio; //Define variable for ratio
  float sensorValue = analogRead(gas_sensor); //Read analog values of sensor  
  sensor_volt = sensorValue*(5.0/1023.0); //Convert analog values to voltage 
  RS_gas = ((5.0*10.0)/sensor_volt)-10.0; //Get value of RS in a gas
  ratio = RS_gas/R0;  // Get ratio RS_gas/RS_air
  double ppm_log = (log10(ratio)-b)/m; //Get ppm value in linear scale according to the the ratio value  
  double ppm = pow(10, ppm_log);
  Serial.print("Our desired PPM = ");
  Serial.println(ppm);
}

void CO_func() {
  float sensor_volt1; //Define variable for sensor voltage 
  float RS_gas1; //Define variable for sensor resistance  
  float ratio1; //Define variable for ratio
  float sensorValue1 = analogRead(CO_sensor); //Read analog values of sensor  
  sensor_volt1 = sensorValue1*(5.0/1023.0); //Convert analog values to voltage 
  RS_gas1 = ((5.0*10.0)/sensor_volt1)-10.0; //Get value of RS in a gas
  ratio1 = RS_gas1/R01;  // Get ratio RS_gas/RS_air
  double ppm_log1 = (log10(ratio1)-b1)/m1; //Get ppm value in linear scale according to the the ratio value  
  double ppm1 = pow(10, ppm_log1);
  Serial.print("CO PPM = ");
  Serial.println(ppm1);
  
}

void dht11_func() {

  hum = dht.readHumidity();
  temp = dht.readTemperature();
  Serial.print("Humidity: ");
  Serial.println(hum);
  Serial.print("Temperature: ");
  Serial.println(temp);

}
