#ifndef _TMAP_
#define _TMAP_

#if ARDUINO >= 100
 #include "Arduino.h"
 #include "Print.h"
#else
 #include "WProgram.h"
#endif

#include <Adafruit_Sensor.h>
#include <TMAP.h>

class TMAP : public Adafruit_Sensor {

 public: 
  /* Constructor */
  TMAP(int32_t);
  void  getTemperature(float *temp);
  void  getPressure(float *pressure);
  void getEvent(sensors_event_t*);
  void getSensor(sensor_t*);
  
 private:
   int32_t _sensorID;
};

#endifs