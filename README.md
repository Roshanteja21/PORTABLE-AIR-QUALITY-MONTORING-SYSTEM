# PORTABLE AIR QUALITY MONITORING SYSTEM  

![LOGO](https://user-images.githubusercontent.com/76561059/143826245-7f4b3226-e7cb-4728-bc78-b6ddfbfaf88b.png)

This project deals with measuring Air Quality using MQ135 sensor along with Carbon Monoxide CO using MQ7 sensor. Measuring Air Quality is an important element for bringing lot of awareness in the people to take care of the future generations a healthier life. Based on this, Government of India has already taken certain measures to ban ‘Single Stroke’ and ‘Two Stroke’ Engine based motorcycles which are emitting high pollution comparatively. We are trying to implement the same system using IoT platforms like Thingspeak or Cayenne, we can bring awareness to every individual about the harm we are doing to our environment. Already, New Delhi is remarked as the most polluted city in the world recording Air Quality above 300 PPM. We have used the easiest platform like Thingspeak and set the dashboard to the public such that everyone can come to know the Air Quality at the location where the system is installed. Also, we have reduced the cost of components used on comparing with the papers referred. 
Index Terms—IoT, MQ135, MQ7, PM2.5 Thingspeak. 




## Features

- Can sense the PPM of AIR Quality
- Can sense the specific CO in PPMs in Air
- Can sense DUST within atleast 10um
- Can sense Temperature and Humidity
- Can analyise the data into graphs in real time


## Tech Stack

**Server:** Thinkspeak or Cayene

**Components:** MQ135, MQ7, PM2.5, DHT11, ARDUINO UNO, ESP01

**EDITOR:** Arduino IDE

## Flow Chart

![Component 1](https://user-images.githubusercontent.com/76561059/143827508-d8a2a84b-5046-4881-993a-04e3d59b94c3.png)

## Circuit Diagram 
![PROJECT_bb](https://user-images.githubusercontent.com/76561059/143827604-7587c553-4ef9-4be5-a15e-8a4a9a8826f0.png)

## Calibration setup.
MQ7 and MQ135 being calibraised with [Calibration](https://github.com/Roshanteja21/PORTABLE-AIR-QUALITY-MONTORING-SYSTEM/tree/main/Calibration) some of the formulas mentioned below

Calculations,

From the Graph showed in the [DATASHEET](https://www.olimex.com/Products/Components/Sensors/Gas/SNS-MQ135/resources/SNS-MQ135.pdf), by the line of CO2


```bash
  y = mx + b
```
For a log-log scale, the formula looks like this:
```bash
  log10y = m ∗ log10x + b
```
Let’s find the slope. To do so, we need to choose 2 points
from the graph. In our case, we chose the points (200,2.6)
and (10000,0.75) from the LPG (Liquified Petroleum gas)line
from fig 3. The LPG line is a result of sensor under testing
with various level of LPG as input. The formula to calculate
slope m(here) is the following:
```bash
  m = (logy − log(y0))/(logx − log(x0))
```
If we apply the logarithmic quotient rule we get the following:
```bash
  m = log(0.75/2.6)/log(1000/200)
  m = -0.318
```
Now that we have m, we can calculate the y intercept. To do
so, we need to choose one point from the graph (once again
from the CO2 line). In our case, we chose (5000,0.9)
```bash
  log(y) = m ∗ log(x) + b
  b =  = log(0.9) − (−0.318) ∗ log(5000)
  b = 1.13
```
By these Calculations we consider the values of Slope and Y intercept(b) in [Execution code](https://github.com/Roshanteja21/PORTABLE-AIR-QUALITY-MONTORING-SYSTEM/tree/main/Execution%20code)
 
