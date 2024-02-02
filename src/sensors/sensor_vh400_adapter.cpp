#include "sensors/sensor_vh400_adapter.hpp"

const int kStartUpTime = 4000;

SensorVh400Adapter::SensorVh400Adapter(const uint8_t enable_pin, uint8_t adc_pin) : ASensorAdapter(enable_pin)
{
    this->start_up_time = kStartUpTime;
    this->measurement_type = MeasurementType::kMeasurementTypeVwc;

    this->vh400 = Vh400(adc_pin);
}

SensorVh400Adapter::~SensorVh400Adapter()
{
}

int SensorVh400Adapter::GetStartupTime()
{
    return this->start_up_time;
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
    return vh400.Measure();
}