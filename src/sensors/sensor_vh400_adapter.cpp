#include "sensors/sensor_vh400_adapter.hpp"

SensorVh400Adapter::SensorVh400Adapter() : ASensorAdapter(VH400_ENABLE_PIN)
{
    this->measurement_type = MeasurementType::kMeasurementTypeVwc;
}

SensorVh400Adapter::~SensorVh400Adapter()
{
}

int SensorVh400Adapter::GetStartupTime()
{
    return 4000;
}

bool SensorVh400Adapter::StartMeasurement()
{
    // Start taking average over periode
    return true;
}

bool SensorVh400Adapter::IsMeasurementFinnished()
{
    // Return true when x amount of measurements have been taken
    return true;
}

float SensorVh400Adapter::GetMeasurement()
{
    // return 2.0f;
    return analogRead(VH400_ADC_PIN);
}