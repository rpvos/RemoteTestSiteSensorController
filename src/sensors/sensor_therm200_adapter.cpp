#include "sensors/sensor_therm200_adapter.hpp"

const int kStartUpTime = 1000;

const MeasurementType kMeasurementTypes[] = {MeasurementType::kMeasurementTypeTemperature};

SensorTherm200Adapter::SensorTherm200Adapter(uint8_t enable_pin, uint8_t adc_pin, uint8_t adc_resolution, uint8_t reference_voltage) : ASensorAdapter(enable_pin, kMeasurementTypes, sizeof(kMeasurementTypes) / sizeof(MeasurementType), kStartUpTime)
{
    this->therm200 = Therm200(adc_pin, reference_voltage, adc_resolution);
}

SensorTherm200Adapter::~SensorTherm200Adapter()
{
}

bool SensorTherm200Adapter::StartMeasurement()
{
    return true;
}

bool SensorTherm200Adapter::IsMeasurementFinnished()
{
    return true;
}

bool SensorTherm200Adapter::GetMeasurements(float *measurements)
{
    float measurement = therm200.Measure();
    if (measurements != nullptr)
    {
        measurements[0] = measurement;
        return true;
    }

    return false;
}
