#include "sensors/a_sensor_adapter.hpp"
#include <Arduino.h>
#include "sensors/a_sensor_adapter.hpp"

ASensorAdapter::ASensorAdapter()
{
    this->measurement_type = MeasurementType::kMeasurementTypeUndefined;
    this->time_last_measurement = 0;
    this->enable_pin = -1;
    this->start_up_time = 0;
    Disable();
}

ASensorAdapter::ASensorAdapter(const int enable_pin)
{
    this->measurement_type = MeasurementType::kMeasurementTypeUndefined;
    this->time_last_measurement = 0;
    this->enable_pin = enable_pin;
    this->start_up_time = 0;
    Disable();
}

ASensorAdapter::~ASensorAdapter()
{
}

unsigned long ASensorAdapter::GetTimeLastMeasurement()
{
    return this->time_last_measurement;
}

MeasurementType ASensorAdapter::GetMeasurementType()
{
    return this->measurement_type;
}

int ASensorAdapter::GetStartupTime()
{
    return 0;
}

void ASensorAdapter::Enable()
{
    this->time_last_measurement = millis();
    digitalWrite(enable_pin, HIGH);
}

bool ASensorAdapter::StartMeasurement()
{
    return false;
}

bool ASensorAdapter::IsMeasurementFinnished()
{
    return false;
}

float ASensorAdapter::GetMeasurement()
{
    return 0;
}

void ASensorAdapter::Disable()
{
    digitalWrite(enable_pin, HIGH);
}
