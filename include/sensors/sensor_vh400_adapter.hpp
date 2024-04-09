#if !defined(SENSOR_VH400_ADAPTER_HPP_)
#define SENSOR_VH400_ADAPTER_HPP_

#include "a_sensor_adapter.hpp"
#include <Arduino.h>

#include <vh400.hpp>

class SensorVh400Adapter : public ASensorAdapter
{
private:
    Vh400 vh400 = 0;

public:
    SensorVh400Adapter(const uint8_t enable_pin, uint8_t adc_pin, uint8_t adc_resolution, uint8_t reference_voltage);
    ~SensorVh400Adapter();

    bool StartMeasurement() override;
    bool IsMeasurementFinnished() override;
    bool GetMeasurements(uint32_t *measurements) override;
    float ConvertToUnit(const uint32_t &value) override;
};

#endif // SENSOR_VH400_ADAPTER_HPP_
