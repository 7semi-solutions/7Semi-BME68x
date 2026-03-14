#ifndef _7SEMI_BME68X_H_
#define _7SEMI_BME68X_H_

#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>

extern "C"
{
#include "bme68x.h"
#include "bme68x_defs.h"
}

/**
 * Maximum number of heater profiles
 *
 * Used when storing multiple heater configurations.
 */
#define MAX_HEATER_PROFILES 10

/**
 * Oversampling configuration
 *
 * Controls measurement resolution
 * and sensor measurement time.
 *
 * Higher values increase accuracy but
 * also increase measurement duration.
 */
enum BME68x_sampling : uint8_t
{
    SKIPPED = BME68X_OS_NONE,
    OS1 = BME68X_OS_1X,
    OS2 = BME68X_OS_2X,
    OS4 = BME68X_OS_4X,
    OS8 = BME68X_OS_8X,
    OS16 = BME68X_OS_16X
};

/**
 * IIR filter configuration
 *
 * Used to smooth pressure measurements
 * and reduce noise in readings.
 */
enum BME68x_filter : uint8_t
{
    FILTER_OFF = BME68X_FILTER_OFF,
    FILTER_1 = BME68X_FILTER_SIZE_1,
    FILTER_3 = BME68X_FILTER_SIZE_3,
    FILTER_7 = BME68X_FILTER_SIZE_7,
    FILTER_15 = BME68X_FILTER_SIZE_15,
    FILTER_31 = BME68X_FILTER_SIZE_31,
    FILTER_63 = BME68X_FILTER_SIZE_63,
    FILTER_127 = BME68X_FILTER_SIZE_127
};

/**
 * Output Data Rate configuration
 *
 * Defines the measurement interval when
 * operating in continuous modes.
 */
enum class BME68x_odr : uint8_t
{
    MS_0_59 = BME68X_ODR_0_59_MS,
    MS_10 = BME68X_ODR_10_MS,
    MS_20 = BME68X_ODR_20_MS,
    MS_62_5 = BME68X_ODR_62_5_MS,
    MS_125 = BME68X_ODR_125_MS,
    MS_250 = BME68X_ODR_250_MS,
    MS_500 = BME68X_ODR_500_MS,
    MS_1000 = BME68X_ODR_1000_MS,
    NONE = BME68X_ODR_NONE
};

/**
 * I2C communication handle
 *
 * Stores information required by the
 * internal communication functions.
 *
 * Members
 * - i2c     : pointer to I2C interface
 * - address : sensor I2C address
 */
struct i2c_handle
{
    TwoWire *i2c;
    uint8_t address;
};

class BME68x_7Semi
{
public:
    BME68x_7Semi();

    /**
     * Initialize sensor using I2C interface
     *
     * Parameters
     * - address  : sensor I2C address (default 0x77)
     * - i2cPort  : I2C interface instance (default Wire)
     * - i2cSpeed : I2C bus speed in Hz (default 100000)
     * - sda      : custom SDA pin (ESP32 only, default uses board pin)
     * - scl      : custom SCL pin (ESP32 only, default uses board pin)
     *
     * Returns
     * - true  : initialization successful
     * - false : sensor not detected or initialization failed
     */
    bool begin(uint8_t address = 0x77,
               TwoWire &i2cPort = Wire,
               uint32_t i2cSpeed = 100000,
               uint8_t sda = 255,
               uint8_t scl = 255);

    /**
     * Initialize sensor using SPI interface
     *
     * Parameters
     * - csPin   : chip select pin
     * - spiPort : SPI interface instance (default SPI)
     * - spiFreq : SPI clock frequency in Hz (default 1 MHz)
     * - sck     : custom SCK pin (ESP32 only)
     * - miso    : custom MISO pin (ESP32 only)
     * - mosi    : custom MOSI pin (ESP32 only)
     *
     * Returns
     * - true  : initialization successful
     * - false : sensor not detected or initialization failed
     */
    bool beginSPI(uint8_t csPin,
                  SPIClass &spiPort = SPI,
                  uint32_t spiFreq = 1000000,
                  uint8_t sck = 255,
                  uint8_t miso = 255,
                  uint8_t mosi = 255);

    /**
     * Read sensor chip ID
     *
     * Parameters
     * - id : variable where chip ID will be stored
     *
     * Returns
     * - true  : chip ID read successfully
     * - false : communication error
     */
    bool readChipID(uint8_t &id);

    /**
     * Perform sensor soft reset
     *
     * Returns
     * - true  : reset successful
     * - false : reset failed
     */
    bool softReset();

    /**
     * Set sensor operating mode
     *
     * Parameters
     * - op_mode : desired operating mode
     *
     * Returns
     * - true  : mode set successfully
     * - false : operation failed
     */
    bool setOperationMode(uint8_t op_mode);

    /**
     * Read current sensor operating mode
     *
     * Parameters
     * - op_mode : variable where mode will be stored
     *
     * Returns
     * - true  : mode read successfully
     * - false : operation failed
     */
    bool getOperationMode(uint8_t &op_mode);

    /**
     * Configure heater settings
     *
     * Parameters
     * - enable     : enable or disable heater
     * - heaterTemp : heater temperature in °C
     * - heaterDur  : heater duration in milliseconds
     *
     * Returns
     * - true  : configuration successful
     * - false : configuration failed
     */
    bool setHeater(bool enable, uint16_t heaterTemp, uint16_t heaterDur);

    /**
     * Read heater configuration
     *
     * Parameters
     * - enable     : heater enable state
     * - heaterTemp : heater temperature
     * - heaterDur  : heater duration
     *
     * Returns
     * - true  : configuration read successfully
     * - false : read failed
     */
    bool getHeater(bool &enable, uint16_t &heaterTemp, uint16_t &heaterDur);

    /**
     * Configure heater profile by index
     *
     * Parameters
     * - profile    : profile index
     * - heaterTemp : heater temperature
     * - heaterDur  : heater duration
     * - enable     : heater enable state
     *
     * Returns
     * - true  : configuration successful
     * - false : invalid profile or configuration failed
     */
    bool setHeaterProfile(uint8_t profile,
                          uint16_t heaterTemp,
                          uint16_t heaterDur,
                          bool enable = true);

    /**
     * Read stored heater profile
     *
     * Parameters
     * - profile    : profile index
     * - heaterTemp : stored heater temperature
     * - heaterDur  : stored heater duration
     * - enable     : stored heater enable state
     *
     * Returns
     * - true  : profile exists
     * - false : invalid profile index
     */
    bool getHeaterProfile(uint8_t profile,
                          uint16_t &heaterTemp,
                          uint16_t &heaterDur,
                          bool &enable);

    /**
     * Apply heater configuration using structure
     */
    int8_t setHeater(const struct bme68x_heatr_conf *conf);

    /**
     * Read heater configuration into structure
     */
    int8_t getHeater(struct bme68x_heatr_conf *conf);

    /**
     * Configure oversampling and filter settings
     *
     * Parameters
     * - temperatureSamples : temperature oversampling
     * - humiditySamples    : humidity oversampling
     * - pressureSamples    : pressure oversampling
     * - filterSetting      : filter size
     *
     * Returns
     * - true  : configuration successful
     * - false : configuration failed
     */
    bool setSampling(BME68x_sampling temperatureSamples,
                     BME68x_sampling humiditySamples,
                     BME68x_sampling pressureSamples,
                     BME68x_filter filterSetting);

    /**
     * Read oversampling and filter settings
     *
     * Returns
     * - true  : configuration read successfully
     * - false : read failed
     */
    bool getSampling(BME68x_sampling &temperatureSamples,
                     BME68x_sampling &humiditySamples,
                     BME68x_sampling &pressureSamples,
                     BME68x_filter &filterSetting);

    /**
     * Set measurement update interval
     */
    bool setODR(BME68x_odr odr);

    /**
     * Read measurement update interval
     */
    bool getODR(BME68x_odr &odr) const;

    /**
     * Calculate measurement duration
     *
     * Parameters
     * - op_mode  : operating mode
     * - duration : returned measurement time
     */
    bool getMeasurementDuration(uint8_t op_mode, uint32_t &duration);

    /**
     * Trigger forced measurement mode
     */
    bool setForcedMode();

    /**
     * Perform measurement and return raw data structure
     */
    bool getData(bme68x_data &outData);

    /**
     * Perform measurement and update internal data
     */
    bool getData();

    /**
     * Perform measurement and return values as floats
     *
     * Outputs
     * - temperature
     * - humidity
     * - pressure
     * - gasResistance
     */
    bool getData(float &temperature,
                 float &humidity,
                 float &pressure,
                 float &gasResistance);

    /**
     * Read temperature value
     */
    float getTemperature();

    /**
     * Read pressure value
     */
    float getPressure();

    /**
     * Read humidity value
     */
    float getHumidity();

    /**
     * Read gas resistance value
     */
    float getGasResistance();

    /**
     * Perform forced mode measurement
     */
    bool readForced(bme68x_data &outData);

    /**
     * Perform parallel mode measurement
     */
    bool readParallel(bme68x_data *dataArray, uint8_t &fields);

private:
    /**
     * Low-level communication callbacks
     *
     * These functions are used internally for
     * reading and writing sensor registers.
     */

    static int8_t i2cRead(uint8_t reg, uint8_t *data, uint32_t len, void *intf_ptr);
    static int8_t i2cWrite(uint8_t reg, const uint8_t *data, uint32_t len, void *intf_ptr);

    static int8_t spiRead(uint8_t reg, uint8_t *data, uint32_t len, void *intf_ptr);
    static int8_t spiWrite(uint8_t reg, const uint8_t *data, uint32_t len, void *intf_ptr);

    /**
     * Delay helper
     *
     * Provides microsecond delay required
     * during sensor communication.
     */
    static void delayUs(uint32_t period, void *intf_ptr);

private:
    /**
     * Active communication interface
     *
     * Indicates whether the sensor is
     * currently using I2C or SPI.
     */
    // Interface iface;

    /**
     * Communication interface pointers
     *
     * - i2c : pointer to Wire interface
     * - spi : pointer to SPI interface
     */

    TwoWire *i2c = nullptr;
    SPIClass *spi = nullptr;

    /**
     * I2C communication handle
     *
     * Stores I2C interface pointer and
     * sensor address for register access.
     */
    struct i2c_handle
    {
        TwoWire *i2c;
        uint8_t address;
    } i2c_h;

    /**
     * SPI communication handle
     *
     * Stores SPI interface pointer and
     * configuration required for transfers.
     */
    struct spi_handle
    {
        SPIClass *spi;
        uint8_t csPin;
        uint32_t frequency;
    } spi_h;

    /**
     * Sensor internal configuration structures
     *
     * These store sensor state and settings.
     */

    struct bme68x_dev BME68x_dev{};
    struct bme68x_conf conf{};
    struct bme68x_heatr_conf heatr{};
    struct bme68x_data BME68x_Data{};

    /**
     * Heater profile storage
     *
     * Stores multiple heater configurations
     * that can be applied dynamically.
     */
    struct bme68x_heatr_conf heaterProfiles[MAX_HEATER_PROFILES];

    /**
     * Measurement result tracking
     *
     * - data_size : number of data fields returned
     * - status   : last operation status
     */
    uint8_t data_size{0};
    int8_t status{0};
};

#endif

