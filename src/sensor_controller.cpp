#include "sensor_controller.hpp"
#include <Arduino.h>

static const unsigned long kMaxValueUnsignedLong = ~0ul;

SensorController::SensorController(ISensorAdapter **sensors, size_t amount_of_sensors)
{
    this->sensors = sensors;
    this->amount_of_sensors = amount_of_sensors;
}

SensorController::~SensorController()
{
}

void SensorController::SetSensors(ISensorAdapter **sensors, size_t amount_of_sensors)
{
    this->sensors = sensors;
    this->amount_of_sensors = amount_of_sensors;
}

// void SensorController::AddSensor(ISensorAdapter *new_sensors[], size_t size)
// {
//     // Count the number of unique transitions
//     int unique_count = 0;
//     for (size_t i = 0; i < size; ++i)
//     {
//         if (!IsDuplicate(new_sensors[i], this->sensors, amount_of_sensors) &&
//             !IsDuplicate(new_sensors[i], new_sensors, i))
//         {
//             unique_count++;
//         }
//     }

//     // No new unique sensors
//     if (!unique_count)
//     {
//         return;
//     }

//     // Allocate or expand storage for transitions with exact size
//     ISensorAdapter **temp = new ISensorAdapter *[amount_of_sensors + unique_count];
//     if (sensors != nullptr)
//     {
//         memcpy(temp, sensors, amount_of_sensors * sizeof(ISensorAdapter *));
//         delete[] sensors;
//     }
//     sensors = temp;
//     // Check if memory allocation was successful
//     if (sensors == nullptr)
//     {
//         // Serial.print("Out of storage");
//         abort();
//     }
//     // Add new sensors, avoiding duplicates
//     for (size_t i = 0; i < size; ++i)
//     {
//         if (!IsDuplicate(new_sensors[i], sensors, amount_of_sensors) &&
//             !IsDuplicate(new_sensors[i], new_sensors, i))
//         {
//             sensors[amount_of_sensors] = new_sensors[i];
//             amount_of_sensors++;
//         }
//     }
// }

// void SensorController::AddSensor(ISensorAdapter *new_sensor)
// {
//     if (IsDuplicate(new_sensor, this->sensors, amount_of_sensors))
//     {
//         return;
//     }

//     // Allocate or expand storage for transitions with exact size
//     ISensorAdapter **temp = new ISensorAdapter *[amount_of_sensors + 1];
//     if (this->sensors != nullptr)
//     {
//         memcpy(temp, new_sensor, 1 * sizeof(ISensorAdapter *));
//         delete[] sensors;
//     }
//     this->sensors = temp;
//     // Check if memory allocation was successful
//     if (sensors == nullptr)
//     {
//         // Serial.print("Out of storage");
//         abort();
//     }
//     // Add new Sensor

//     sensors[amount_of_sensors] = new_sensor;
//     amount_of_sensors++;
// }

unsigned long SensorController::TimeUntillNextMeasurement()
{
    // Initiate with bigest value of unsigned long
    unsigned long earliest_next_measurement = kMaxValueUnsignedLong;
    const unsigned long now = millis();

    for (size_t i = 0; i < this->amount_of_sensors; i++)
    {
        // Assign shortest measurement frequency of sensors
        unsigned long frequency = sensors[i]->GetFrequency();

        unsigned long time_last_measurement = this->sensors[i]->GetTimeLastMeasurement();
        // If never measured, measure now
        if (time_last_measurement == 0)
        {
            current_sensor_index = i;

            return 0;
        }

        // Calculate time between now and time_last_measurement
        unsigned long time_since_measurement = now - time_last_measurement;

        // Check for value overflow
        if (now < time_last_measurement)
        {
            time_since_measurement = now + (kMaxValueUnsignedLong - time_last_measurement);
        }

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
    if (sensors != nullptr)
    {
        return sensors[current_sensor_index]->GetStartupTime();
    }

    return -1;
}

void SensorController::EnableSensor()
{
    if (sensors != nullptr)
    {
        sensors[current_sensor_index]->Enable();
    }
}

bool SensorController::StartMeasurement()
{
    if (sensors != nullptr)
    {
        return sensors[current_sensor_index]->StartMeasurement();
    }
    return false;
}

bool SensorController::IsMeasurementFinnished()
{
    if (sensors != nullptr)
    {
        return sensors[current_sensor_index]->IsMeasurementFinnished();
    }
    return false;
}

size_t SensorController::GetMeasurementAmount()
{
    if (sensors != nullptr)
    {
        return sensors[current_sensor_index]->GetMeasurementAmount();
    }
    return 0;
}

bool SensorController::GetMeasurements(uint32_t *measurements)
{
    if (sensors != nullptr)
    {
        return sensors[current_sensor_index]->GetMeasurements(measurements);
    }
    return false;
}

bool SensorController::GetMeasurementTypes(RemoteTestSite_MeasurementInfo *measurement_types)
{
    if (sensors != nullptr)
    {
        return sensors[current_sensor_index]->GetMeasurementTypes(measurement_types);
    }
    return false;
}

void SensorController::DisableSensor()
{
    if (sensors != nullptr)
    {
        sensors[current_sensor_index]->Disable();
    }
}

void SensorController::SetFrequency(RemoteTestSite_MeasurementInfo measurement_type, unsigned long frequency)
{
    for (size_t i = 0; i < amount_of_sensors; i++)
    {
        const size_t amount_of_measurements = sensors[i]->GetMeasurementAmount();
        RemoteTestSite_MeasurementInfo measurement_types[amount_of_measurements];
        bool succes = sensors[i]->GetMeasurementTypes(measurement_types);
        if (succes)
        {
            for (size_t j = 0; j < amount_of_measurements; j++)
            {
                if (measurement_types[j] == measurement_type)
                {
                    sensors[i]->SetFrequency(frequency);
                }
            }
        }
    }
}

void SensorController::UpdateTimeLastMeasurement()
{
    if (sensors != nullptr)
    {
        this->sensors[current_sensor_index]->SetTimeLastMeasurement(millis());
    }
}

bool SensorController::IsDuplicate(ISensorAdapter *new_sensor, ISensorAdapter *sensors, int array_size)
{
    for (int i = 0; i < array_size; ++i)
    {
        const size_t measurement_amount = sensors[i].GetMeasurementAmount();
        const size_t new_measurement_amount = new_sensor->GetMeasurementAmount();
        // Check basic atributes
        if (
            measurement_amount == new_measurement_amount &&
            sensors[i].GetStartupTime() == new_sensor->GetStartupTime() &&
            sensors[i].GetTimeLastMeasurement() == new_sensor->GetTimeLastMeasurement())
        {
            // Check measurement types
            RemoteTestSite_MeasurementInfo measurement_types[measurement_amount];
            RemoteTestSite_MeasurementInfo new_measurement_types[new_measurement_amount];
            sensors[i].GetMeasurementTypes(measurement_types);
            new_sensor->GetMeasurementTypes(new_measurement_types);
            for (size_t j = 0;
                 j < measurement_amount &&
                 j < new_measurement_amount;
                 j++)
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