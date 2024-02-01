#if !defined(SENSOR_VH400_ADAPTER_HPP_)
#define SENSOR_VH400_ADAPTER_HPP_

#include "a_sensor_adapter.hpp"
#include <Arduino.h>

#define VH400_ADC_PIN A3
#define VH400_ENABLE_PIN 22

class SensorVh400Adapter : public ASensorAdapter
{
private:
public:
    SensorVh400Adapter();
    ~SensorVh400Adapter();

    int GetStartupTime() override;
    bool StartMeasurement() override;
    bool IsMeasurementFinnished() override;
    float GetMeasurement() override;
};

#endif // SENSOR_VH400_ADAPTER_HPP_
