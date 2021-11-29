#include <ArduinoJson.h>
#include <SoftwareSerial.h>


//D6 = Rx & D5 = Tx
SoftwareSerial nodemcu(D6,D5);

//Timer to run Arduino code every 5 seconds
unsigned long previousMillis = 0;
unsigned long currentMillis;
const unsigned long period = 10000;  

void setup() {
  // Initialize Serial port
  Serial.begin(9600);
  nodemcu.begin(9600);
  while (!Serial) continue;
}

void loop() {
  //Get current time
  currentMillis = millis();

  if ((currentMillis - previousMillis >= period)) {

  StaticJsonBuffer<3000> jsonBuffer;
  JsonObject& data = jsonBuffer.parseObject(nodemcu);

  // Test parsing
  if (data == JsonObject::invalid()) {
    Serial.println("INVALID JSON OBJECT");
    jsonBuffer.clear();
    return;
  }

  Serial.println("JSON Object Recieved");
  Serial.print("Recieved Air quality (CO2ppm)");
  float ppm = data["Air quality (CO2ppm)"];
  Serial.println(ppm);
  Serial.print("Recieved CO ppm");
  float ppm1 = data["CO ppm"];
  Serial.println(ppm1);
  Serial.print("Recieved Humidity:  ");
  float hum = data["humidity"];
  Serial.println(hum);
  Serial.print("Recieved Temperature:  ");
  float temp = data["temperature"];
  Serial.println(temp);
  Serial.println("-----------------------------------------");

previousMillis = previousMillis + period;
  }
}
