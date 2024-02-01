#include "sensors/sensor_therm200_adapter.hpp"

SensorTherm200Adapter::SensorTherm200Adapter() : ASensorAdapter(THERM200_ENABLE_PIN)
{
    this->measurement_type = MeasurementType::kMeasurementTypeTemperature;
}

SensorTherm200Adapter::~SensorTherm200Adapter()
{
}

int SensorTherm200Adapter::GetStartupTime()
{
    return 4000;
}

bool SensorTherm200Adapter::StartMeasurement()
{
    // Start taking average over periode
    return true;
}

bool SensorTherm200Adapter::IsMeasurementFinnished()
{
    // Return true when x amount of measurements have been taken
    return true;
}

float SensorTherm200Adapter::GetMeasurement()
{
    // return 1.0f;
    return analogRead(THERM200_ADC_PIN);
}