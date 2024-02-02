#include "sensor_controller.hpp"
// #include <memory.h>
#include <Arduino.h>

SensorController::SensorController()
{
    this->frequencies = new unsigned long[MeasurementTypeHelper::as_integer(MeasurementType::kCount)];
    for (size_t i = 0; i < MeasurementTypeHelper::as_integer(MeasurementType::kCount); i++)
    {
        this->frequencies[i] = 0;
    }

    this->amount_of_sensors = amount_of_sensors;
}

SensorController::~SensorController()
{
    delete[] this->sensors;
    delete[] this->frequencies;
}

void SensorController::AddSensor(ASensorAdapter *new_sensors[], size_t size)
{
    // Count the number of unique transitions
    int unique_count = 0;
    for (size_t i = 0; i < size; ++i)
    {
        if (!IsDuplicate(new_sensors[i], this->sensors, amount_of_sensors) &&
            !IsDuplicate(new_sensors[i], new_sensors, i))
        {
            unique_count++;
        }
    }

    // No new unique sensors
    if (!unique_count)
    {
        return;
    }

    // Allocate or expand storage for transitions with exact size
    ASensorAdapter **temp = new ASensorAdapter *[amount_of_sensors + unique_count];
    if (sensors != NULL)
    {
        memcpy(temp, sensors, amount_of_sensors * sizeof(ASensorAdapter *));
        delete[] sensors;
    }
    sensors = temp;
    // Check if memory allocation was successful
    if (sensors == NULL)
    {
        Serial.print("Out of storage");
        abort();
    }
    // Add new transitions, avoiding duplicates
    for (int i = 0; i < size; ++i)
    {
        if (!IsDuplicate(new_sensors[i], sensors, amount_of_sensors) &&
            !IsDuplicate(new_sensors[i], new_sensors, i))
        {
            sensors[amount_of_sensors] = new_sensors[i];
            amount_of_sensors++;
        }
    }
}

int SensorController::TimeUntillNextMeasurement()
{
    unsigned long earliest_next_measurement = 0;
    const unsigned long now = millis();

    for (size_t i = 0; i < this->amount_of_sensors; i++)
    {
        unsigned long frequency = GetFrequency(this->sensors[i]->GetMeasurementType());
        // Check if this type of sensor needs to be measured
        if (frequency == 0)
        {
            continue;
        }

        unsigned long time_last_measurement = this->sensors[i]->GetTimeLastMeasurement();
        // If never measured, measure now
        if (time_last_measurement == 0)
        {
            current_sensor_index = i;
            return 0;
        }

        unsigned long time_since_measurement = now - time_last_measurement;
        // Time exeeded time frequency
        if (time_since_measurement > frequency)
        {
            current_sensor_index = i;
            return 0;
        }

        // Initialise value if not initialised
        if (!earliest_next_measurement)
        {
            earliest_next_measurement = frequency - time_since_measurement;
            current_sensor_index = i;
        }
        // Check if earliest next measurement is bigger then this sensor next predicted measurement
        else if (frequency - time_since_measurement < earliest_next_measurement)
        {
            // Assign this value as new earliest measurement
            earliest_next_measurement = frequency - time_since_measurement;
            current_sensor_index = i;
        }
    }

    return earliest_next_measurement;
}

int SensorController::GetSensorStartupTime()
{
    return sensors[current_sensor_index]->GetStartupTime();
}

void SensorController::EnableSensor()
{
    sensors[current_sensor_index]->Enable();
}

void SensorController::StartMeasurement()
{
    sensors[current_sensor_index]->StartMeasurement();
}

bool SensorController::IsMeasurementFinnished()
{
    return sensors[current_sensor_index]->IsMeasurementFinnished();
}

float SensorController::GetMeasurement()
{
    return sensors[current_sensor_index]->GetMeasurement();
}

MeasurementType SensorController::GetMeasurementType()
{
    return sensors[current_sensor_index]->GetMeasurementType();
}

void SensorController::DisableSensor()
{
    sensors[current_sensor_index]->Disable();
}

unsigned long SensorController::GetFrequency(MeasurementType measurement_type)
{
    switch (measurement_type)
    {
    case MeasurementType::kMeasurementTypeTemperature:
    case MeasurementType::kMeasurementTypeVwc:
    case MeasurementType::kMeasurementTypeEcBulk:
    case MeasurementType::kMeasurementTypeEcPore:
        return frequencies[MeasurementTypeHelper::as_integer(measurement_type)];
        break;
    default:
        return 0;
        break;
    }
}

void SensorController::SetFrequency(MeasurementType measurement_type, unsigned long frequency)
{
    frequencies[MeasurementTypeHelper::as_integer(measurement_type)] = frequency;
}

bool SensorController::IsDuplicate(ASensorAdapter *new_sensor, ASensorAdapter *sensors[], int array_size)
{
    for (int i = 0; i < array_size; ++i)
    {
        if (sensors[i]->GetMeasurementType() == new_sensor->GetMeasurementType() &&
            sensors[i]->GetStartupTime() == new_sensor->GetStartupTime() &&
            sensors[i]->GetTimeLastMeasurement() == new_sensor->GetTimeLastMeasurement())
        {
            return true;
        }
    }
    return false;
}