#if !defined(A_SENSOR_ADAPTER_HPP_)
#define A_SENSOR_ADAPTER_HPP_

#include "measurement_types.hpp"

class ASensorAdapter
{
private:
protected:
    MeasurementType measurement_type;
    unsigned long time_last_measurement;
    int enable_pin;
    int start_up_time;

public:
    ASensorAdapter();
    ASensorAdapter(const int enable_pin);
    ~ASensorAdapter();
    void Enable();
    void Disable();

    unsigned long GetTimeLastMeasurement();
    MeasurementType GetMeasurementType();

    virtual int GetStartupTime();
    virtual bool StartMeasurement();
    virtual bool IsMeasurementFinnished();
    virtual float GetMeasurement();
};

#endif // A_SENSOR_ADAPTER_HPP_
