#include "TMAP.h"
 
#include <avr/pgmspace.h>
#include <limits.h>
#include "pins_arduino.h"
#include "wiring_private.h"
 



/***************************************************************************
 CONSTRUCTOR
 ***************************************************************************/
 
/**************************************************************************/
/*!
    @brief  Instantiates a new TMAP class
*/
/**************************************************************************/
TMAP::TMAP(int32_t sensorID) {
  _sensorID = sensorID;
}

/***************************************************************************
 PUBLIC FUNCTIONS
 ***************************************************************************/

// TODO: Add function for I2C, explore modularization of the hardware.
// For references, see Adafruit BMP180 sensor driver

/**************************************************************************/
/*!
    @brief  Gets the pressure level in kPa
*/
/**************************************************************************/
void TMAP::getPressure(float *pressure)
{
  int32_t  rawpressure
  int analogPin = 3;     // Output signal to analog pin 3 (placeholder pin)

  rawpressure = analogRead(analogPin);    // read the input pin
  *pressure = rawpressure;

}


/**************************************************************************/
/*!
    @brief  Reads the temperatures in degrees Celsius
*/
/**************************************************************************/
void TMAP::getTemperature(float *temp)
{
  int32_t UT, B5;     // following ds convention
  float t;

  readRawTemperature(&UT);

  #if BMP085_USE_DATASHEET_VALS
    // use datasheet numbers!
    UT = 27898;
    _bmp085_coeffs.ac6 = 23153;
    _bmp085_coeffs.ac5 = 32757;
    _bmp085_coeffs.mc = -8711;
    _bmp085_coeffs.md = 2868;
  #endif

// TODO: Replace this to read relevant DS values; also check for NTC -DJL

  B5 = computeB5(UT);
  t = (B5+8) >> 4;
  t /= 10;

  *temp = t;
}

/**************************************************************************/
/*!
    @brief  Provides the sensor_t data for this sensor
*/
/**************************************************************************/
 
void TMAP::getSensor(sensor_t *sensor) {
  /* Clear the sensor_t object */
  memset(sensor, 0, sizeof(sensor_t));
 
  /* Insert the sensor name in the fixed length char array */
  strncpy (sensor->name, "TMAPS", sizeof(sensor->name) - 1);
  sensor->name[sizeof(sensor->name)- 1] = 0;
  sensor->version     = 1;
  sensor->sensor_id   = _sensorID;
  sensor->type        = SENSOR_TYPE_PRESSURE;
  sensor->min_delay   = 0;
  sensor->max_value   = 3000.0F;               // 300..1100 hPa
  sensor->min_value   = 200.0F;
  sensor->resolution  = 0.01F;                // 0.01 hPa resolution
 
  /* Clear the reserved section */
  memset(sensor->reserved, 0, sizeof(sensor->reserved));
}

/**************************************************************************/
/*!
    @brief  Reads the sensor and returns the data as a sensors_event_t
*/
/**************************************************************************/

TMAP::TMAP(int32_t sensorID) {
  _sensorID = sensorID;
}
 
void TMAP::getEvent(sensors_event_t *event) {
  /* Clear the event */
  memset(event, 0, sizeof(sensors_event_t));
 
  event->version   = sizeof(sensors_event_t);
  event->sensor_id = _sensorID;
  event->type      = SENSOR_TYPE_PRESSURE;
  event->timestamp = 0;
  event->pressure = 123.0F;
}