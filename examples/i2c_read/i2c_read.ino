/**
 * 7Semi BME68x Library Example
 *
 * Basic Sensor Reading
 *
 * Demonstrates how to initialize the sensor and read
 * environmental measurements.
 *
 * Measured Values
 * - Temperature
 * - Humidity
 * - Pressure
 * - Gas resistance
 *
 * Sensor Connection (I2C)
 * - VCC  -> 3.3V
 * - GND  -> GND
 * - SDA  -> MCU SDA pin
 * - SCL  -> MCU SCL pin
 */

#include <7Semi_BME68x.h>

BME68x_7Semi sensor;

void setup()
{
  Serial.begin(115200);

  /**
   * Initialize Sensor
   *
   * Uses default I2C address 0x77
   */
  if (!sensor.begin(0x77))
  {
    Serial.println("BME68x sensor not detected");
    while (1);
  }

  Serial.println("Sensor initialized");
}

void loop()
{
  float temperature, humidity, pressure, gas;

  /**
   * Read sensor measurements
   */
  if (sensor.getData(temperature, humidity, pressure, gas))
  {
    Serial.print("Temperature: ");
    Serial.println(temperature);

    Serial.print("Humidity: ");
    Serial.println(humidity);

    Serial.print("Pressure: ");
    Serial.println(pressure);

    Serial.print("Gas Resistance: ");
    Serial.println(gas);

    Serial.println();
  }

  delay(2000);
}