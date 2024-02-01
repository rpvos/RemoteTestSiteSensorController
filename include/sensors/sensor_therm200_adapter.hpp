#if !defined(SENSOR_THERM200_ADAPTER_HPP_)
#define SENSOR_THERM200_ADAPTER_HPP_

#include "a_sensor_adapter.hpp"
#include <therm200.hpp>

#define THERM200_ADC_PIN A0
#define THERM200_ENABLE_PIN 23

class SensorTherm200Adapter : public ASensorAdapter
{
private:
    Therm200 therm200 = Therm200(THERM200_ADC_PIN);

public:
    SensorTherm200Adapter();
    ~SensorTherm200Adapter();

    int GetStartupTime() override;
    bool StartMeasurement() override;
    bool IsMeasurementFinnished() override;
    float GetMeasurement() override;
};

#endif // SENSOR_THERM200_ADAPTER_HPP_
