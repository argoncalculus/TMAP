#include <Adafruit_Sensor.h>
#include <TMAP.h>
#include <OSHClient.h>
#include <ESP8266WiFi.h>
#include <Wire.h>

using namespace osh;

const char ssid[]     = "WiFi SSID";
const char password[] = "WiFi Password";
WiFiClient client;

Sensor s1;
SOSClient* sos;

static const char BPRES_URI[] PROGMEM = "http://sweet.jpl.nasa.gov/2.3/relaSpace.owl#BarometricPressure";
static const char TEMP_URI[] PROGMEM = "http://sweet.jpl.nasa.gov/2.2/quanTemperature.owl#Temperature";
static const char ALT_URI[] PROGMEM = "http://sweet.jpl.nasa.gov/2.3/propSpaceDistance.owl#Altitude";

Adafruit_BMP085_Unified bmp = Adafruit_BMP085_Unified(10085);
int delayMS = 1000;

void setup() {
    Serial.begin(9600);

    // set sensor metadata
    s1.setUniqueID("urn:osh:djl0012:tmap:001");
    s1.setName("TMAP 001");
    s1.addTimeStampOBC("ms");
    s1.addMeasurement("bpres", BPRES_URI, "hPa", "Barometric Pressure");
    s1.addMeasurement("temp", TEMP_URI, "Cel", "Air Temperature");
    s1.addMeasurement("alt", ALT_URI, "m", "Altitude");

    // connect to WiFi
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    // print IP address 
    Serial.println();
    Serial.println("WiFi connected");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    // register to OSH node using SOS-T protocol
    sos = new SOSClient(client, "192.168.0.25", 8181, "/sensorhub/sos");
    sos->registerDevice(&s1);

/* Legacy Initialization; BMP has most of the functions I would like, but seems to be largely self contained, the Bosch TMAPs on the other hand are analog sensors fed into a microcontroller, and would require additional hardware & software in order to behave identically with respect to OSH -DJL

    //Initialise the sensor
    if(!bmp.begin())
    {
      //There was a problem detecting the BMP085 ... check your connections
      Serial.print("Ooops, no BMP085/180 detected ... Check your wiring or I2C ADDR!");
      while(1);
    }
*/   
 
    Serial.println("TMAP Unified Sensor Test");
    // Print TMAP sensor details.
    sensor_t sensor;
    TMAP.getSensor(&sensor);
    Serial.println("------------------------------------");
    Serial.print  ("Sensor:       "); Serial.println(sensor.name);
    Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
    Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
    Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" *C");
    Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" *C");
    Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" *C");  
    Serial.println("------------------------------------");
}

void loop() {
    // Delay between measurements.
    delay(delayMS);

    sos->startMeasurement(&s1);
    sos->pushInt(millis());
    
    // Get pressure event and print its value.
    sensors_event_t event;  
    TMAP.getEvent(&event);
    Serial.print("Barometric Pressure: ");
    Serial.print(event.pressure);
    Serial.println(" hPa");

    sos->pushFloat(event.pressure);
    
    // Get temperature event and print its value.
    float temperature;
    bmp.getTemperature(&temperature);
    Serial.print("Temperature: ");
    Serial.print(temperature);
    Serial.println(" C");

    sos->pushFloat(temperature);

    sos->sendMeasurement();
}