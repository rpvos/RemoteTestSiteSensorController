#include "sensors/a_sensor_adapter.hpp"
#include <Arduino.h>

ASensorAdapter::ASensorAdapter()
{
    this->time_last_measurement = 0;
    this->measurement_types = nullptr;
    this->measurement_amount = 0;
    this->enable_pin = 0;
    this->start_up_time = 0;
    Disable();
}

ASensorAdapter::ASensorAdapter(const int enable_pin, const MeasurementType *measurement_types, const size_t measurement_amount, const int start_up_time)
{
    this->time_last_measurement = 0;

    this->enable_pin = enable_pin;
    this->measurement_types = measurement_types;
    this->measurement_amount = measurement_amount;
    this->start_up_time = start_up_time;
    Disable();
}

ASensorAdapter::~ASensorAdapter()
{
}

unsigned long ASensorAdapter::GetTimeLastMeasurement()
{
    return this->time_last_measurement;
}

bool ASensorAdapter::GetMeasurementTypes(MeasurementType *measurement_types)
{
    if (measurement_types != nullptr)
    {
        for (size_t i = 0; i < this->measurement_amount; i++)
        {
            measurement_types[i] = this->measurement_types[i];
        }
        return true;
    }

    return false;
}

size_t ASensorAdapter::GetMeasurementAmount()
{
    return this->measurement_amount;
}

bool GetMeasurements(float *measurements)
{
    return false;
}

int ASensorAdapter::GetStartupTime()
{
    return start_up_time;
}

void ASensorAdapter::SetTimeLastMeasurement(unsigned long time)
{
    this->time_last_measurement = time;
}

void ASensorAdapter::Enable()
{
    digitalWrite(enable_pin, HIGH);
}

void ASensorAdapter::Disable()
{
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

bool ASensorAdapter::GetMeasurements(float *measurements)
{
    return false;
}
