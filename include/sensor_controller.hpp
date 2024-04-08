#if !defined(A_SENSOR_CONTROLLLER_HPP_)
#define A_SENSOR_CONTROLLLER_HPP_

#include "sensors/a_sensor_adapter.hpp"
#include <proto/measurement_info.pb.h>
#include "measurement_types.hpp"
#include <stddef.h>

class SensorController
{
private:
    ASensorAdapter **sensors;
    size_t amount_of_sensors = 0;
    size_t current_sensor_index = -1;
    static bool IsDuplicate(ASensorAdapter *new_sensor, ASensorAdapter *sensors[], int array_size);

public:
    SensorController();
    ~SensorController();

    // void AddSensor(ASensorAdapter *new_sensors[], size_t size);
    // void AddSensor(ASensorAdapter *new_sensor);

    /**
     * @brief Function used to determine if measurements need to be taken and if not when they do need to be taken.
     * Function automatically sets current sensor to the sensor that needs to measure.
     *
     * @return unsigned long in milliseconds
     */
    unsigned long TimeUntillNextMeasurement();

    int GetSensorStartupTime();
    void EnableSensor();
    bool StartMeasurement();
    bool IsMeasurementFinnished();

    size_t GetMeasurementAmount();
    bool GetMeasurements(uint32_t *measurements);
    bool GetMeasurementTypes(RemoteTestSite_MeasurementInfo *measurement_types);
    void DisableSensor();

    void SetFrequency(RemoteTestSite_MeasurementInfo measurement_type, unsigned long frequency);

    void UpdateTimeLastMeasurement();
};

#endif // A_SENSOR_CONTROLLLER_HPP_
