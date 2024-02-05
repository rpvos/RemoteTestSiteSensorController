#if !defined(SENSOR_MURATA_SOIL_SENSOR_ADAPTER_HPP_)
#define SENSOR_MURATA_SOIL_SENSOR_ADAPTER_HPP_

#include "a_sensor_adapter.hpp"
#include <murata_soil_sensor.h>

class SensorMurataSoilSensorAdapter : public ASensorAdapter
{
private:
    MurataSoilSensor::MurataSoilSensor *murata_soil_sensor;

public:
    SensorMurataSoilSensorAdapter(RS485 *rs485, const uint8_t enable_pin_murata);
    ~SensorMurataSoilSensorAdapter();

    bool StartMeasurement() override;
    bool IsMeasurementFinnished() override;
    bool GetMeasurements(float *measurements) override;
};

#endif // SENSOR_MURATA_SOIL_SENSOR_ADAPTER_HPP_
