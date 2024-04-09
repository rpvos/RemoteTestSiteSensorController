#include "sensors/sensor_therm200_adapter.hpp"
#include <therm200_helper.hpp>

const int kStartUpTime = 1000;

const RemoteTestSite_MeasurementInfo kMeasurementTypes[] = {
    RemoteTestSite_MeasurementInfo::RemoteTestSite_MeasurementInfo_MEASUREMENT_INFO_THERM200_TEMPERATURE};

SensorTherm200Adapter::SensorTherm200Adapter(uint8_t enable_pin, uint8_t adc_pin, uint8_t adc_resolution, uint8_t reference_voltage) : ASensorAdapter(enable_pin, kMeasurementTypes, sizeof(kMeasurementTypes) / sizeof(RemoteTestSite_MeasurementInfo), kStartUpTime)
{
    this->therm200 = Therm200(adc_pin);
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

bool SensorTherm200Adapter::GetMeasurements(uint32_t *measurements)
{
    if (measurements != nullptr)
    {
        uint16_t measurement = therm200.Measure();
        measurements[0] = measurement;
        return true;
    }

    return false;
}

float SensorTherm200Adapter::ConvertToUnit(const uint32_t &value)
{
    return Therm200Helper::CalculateTemperature(value);
}