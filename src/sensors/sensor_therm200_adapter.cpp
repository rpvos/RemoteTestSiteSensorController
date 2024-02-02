#include "sensors/sensor_therm200_adapter.hpp"

const int kStartUpTime = 2000;

SensorTherm200Adapter::SensorTherm200Adapter(uint8_t enable_pin, uint8_t adc_pin) : ASensorAdapter(enable_pin)
{
    this->start_up_time = kStartUpTime;
    this->measurement_type = MeasurementType::kMeasurementTypeTemperature;

    this->therm200 = Therm200(adc_pin);
}

SensorTherm200Adapter::~SensorTherm200Adapter()
{
}

int SensorTherm200Adapter::GetStartupTime()
{
    return this->start_up_time;
}

bool SensorTherm200Adapter::StartMeasurement()
{
    return true;
}

bool SensorTherm200Adapter::IsMeasurementFinnished()
{
    return true;
}

float SensorTherm200Adapter::GetMeasurement()
{
    return therm200.Measure();
}