#include "sensors/sensor_vh400_adapter.hpp"

const int kStartUpTime = 1000;
const MeasurementType kMeasurementTypes[] = {MeasurementType::kMeasurementTypeVwc};

SensorVh400Adapter::SensorVh400Adapter(const uint8_t enable_pin, uint8_t adc_pin, uint8_t adc_resolution, uint8_t reference_voltage) : ASensorAdapter(enable_pin, kMeasurementTypes, sizeof(kMeasurementTypes) / sizeof(MeasurementType), kStartUpTime)
{
    this->vh400 = Vh400(adc_pin, reference_voltage, adc_resolution);
}

SensorVh400Adapter::~SensorVh400Adapter()
{
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

bool SensorVh400Adapter::GetMeasurements(float *measurements)
{
    float measurement = vh400.Measure();
    if (measurements != nullptr)
    {
        measurements[0] = measurement;
        return true;
    }

    return false;
}
