#if !defined(SENSOR_THERM200_ADAPTER_HPP_)
#define SENSOR_THERM200_ADAPTER_HPP_

#include "a_sensor_adapter.hpp"
#include <therm200.hpp>

class SensorTherm200Adapter : public ASensorAdapter
{
private:
    Therm200 therm200 = 0;

public:
    SensorTherm200Adapter(uint8_t enable_pin, uint8_t adc_pin);
    ~SensorTherm200Adapter();

    int GetStartupTime() override;
    bool StartMeasurement() override;
    bool IsMeasurementFinnished() override;
    float GetMeasurement() override;
};

#endif // SENSOR_THERM200_ADAPTER_HPP_
