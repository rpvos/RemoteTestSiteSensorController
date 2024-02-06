#include "sensor_controller.hpp"
// #include <memory.h>
#include <Arduino.h>

const unsigned long kMaxValueUnsignedLong = ~0ul;

SensorController::SensorController()
{
    this->frequencies = new unsigned long[MeasurementTypeHelper::as_integer(MeasurementType::kCount)];
    for (size_t i = 0; i < MeasurementTypeHelper::as_integer(MeasurementType::kCount); i++)
    {
        this->frequencies[i] = 0;
    }

    this->amount_of_sensors = 0;
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
        // Serial.print("Out of storage");
        abort();
    }
    // Add new transitions, avoiding duplicates
    for (size_t i = 0; i < size; ++i)
    {
        if (!IsDuplicate(new_sensors[i], sensors, amount_of_sensors) &&
            !IsDuplicate(new_sensors[i], new_sensors, i))
        {
            sensors[amount_of_sensors] = new_sensors[i];
            amount_of_sensors++;
        }
    }
}

unsigned long SensorController::TimeUntillNextMeasurement()
{
    // Initiate with bigest value of unsigned long
    unsigned long earliest_next_measurement = kMaxValueUnsignedLong;
    const unsigned long now = millis();

    for (size_t i = 0; i < this->amount_of_sensors; i++)
    {
        // Get all frequencies by measurement type
        size_t measurement_amount = this->sensors[i]->GetMeasurementAmount();
        MeasurementType types[measurement_amount];
        if (!this->sensors[i]->GetMeasurementTypes(types))
        {
            continue;
        }

        // Assign shortest measurement frequency of sensor types
        unsigned long frequency = 0;
        for (size_t j = 0; j < measurement_amount; j++)
        {
            unsigned long temp = GetFrequency(types[j]);
            // If temp is 0, this type doesnt need te be measured
            // If frequency is 0, this is the first found freqeuncy to which we can compare it
            if ((frequency > temp || frequency == 0) && temp != 0)
            {
                frequency = temp;
            }
        }

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

        // Check for value overflow
        if (now < time_last_measurement)
        {
        }

        // Calculate time between now and time_last_measurement
        unsigned long time_since_measurement = now + (kMaxValueUnsignedLong - time_last_measurement) + 1;
        // Time exeeded time frequency
        if (time_since_measurement > frequency)
        {
            current_sensor_index = i;

            return 0;
        }

        // Check if earliest next measurement is bigger then this sensor next predicted measurement
        if (frequency - time_since_measurement < earliest_next_measurement)
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

bool SensorController::StartMeasurement()
{
    return sensors[current_sensor_index]->StartMeasurement();
}

bool SensorController::IsMeasurementFinnished()
{
    return sensors[current_sensor_index]->IsMeasurementFinnished();
}

size_t SensorController::GetMeasurementAmount()
{
    return sensors[current_sensor_index]->GetMeasurementAmount();
}

bool SensorController::GetMeasurements(float *measurements)
{
    return sensors[current_sensor_index]->GetMeasurements(measurements);
}

bool SensorController::GetMeasurementTypes(MeasurementType *measurement_types)
{
    return sensors[current_sensor_index]->GetMeasurementTypes(measurement_types);
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

void SensorController::UpdateTimeLastMeasurement()
{
    this->sensors[current_sensor_index]->SetTimeLastMeasurement(millis());
}

bool SensorController::IsDuplicate(ASensorAdapter *new_sensor, ASensorAdapter *sensors[], int array_size)
{
    for (int i = 0; i < array_size; ++i)
    {
        size_t measurement_amount = sensors[i]->GetMeasurementAmount();
        size_t new_measurement_amount = new_sensor->GetMeasurementAmount();
        // Check basic atributes
        if (
            measurement_amount == new_measurement_amount &&
            sensors[i]->GetStartupTime() == new_sensor->GetStartupTime() &&
            sensors[i]->GetTimeLastMeasurement() == new_sensor->GetTimeLastMeasurement())
        {
            // Check measurement types
            MeasurementType measurement_types[measurement_amount];
            MeasurementType new_measurement_types[new_measurement_amount];
            sensors[i]->GetMeasurementTypes(measurement_types);
            new_sensor->GetMeasurementTypes(new_measurement_types);
            for (size_t j = 0; j < measurement_amount && j < new_measurement_amount; j++)
            {
                // If measurement type is not the same the objects are different
                if (measurement_types[j] != new_measurement_types[j])
                {
                    return false;
                }
            }

            return true;
        }
    }
    return false;
}