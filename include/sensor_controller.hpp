#if !defined(A_SENSOR_CONTROLLLER_HPP_)
#define A_SENSOR_CONTROLLLER_HPP_

#include "sensors/a_sensor_adapter.hpp"
#include "measurement_types.hpp"
#include <stddef.h>

class SensorController
{
private:
    ASensorAdapter **sensors;
    size_t amount_of_sensors = 0;
    size_t current_sensor_index = -1;
    unsigned long *frequencies;

    static bool IsDuplicate(ASensorAdapter *new_sensor, ASensorAdapter *sensors[], int array_size);

public:
    SensorController();
    ~SensorController();

    void AddSensor(ASensorAdapter *new_sensors[], size_t size);

    /**
     * @brief Function used to determine if measurements need to be taken and if not when they do need to be taken.
     * Function automatically sets current sensor to the sensor that needs to measure.
     *
     * @return int
     */
    int TimeUntillNextMeasurement();

    int GetSensorStartupTime();
    void EnableSensor();
    void StartMeasurement();
    bool IsMeasurementFinnished();
    float GetMeasurement();
    MeasurementType GetMeasurementType();
    void DisableSensor();

    unsigned long GetFrequency(MeasurementType measurement_type);
    void SetFrequency(MeasurementType measurement_type, unsigned long frequency);
};

#endif // A_SENSOR_CONTROLLLER_HPP_