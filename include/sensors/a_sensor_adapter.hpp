#if !defined(A_SENSOR_ADAPTER_HPP_)
#define A_SENSOR_ADAPTER_HPP_

#include "measurement_types.hpp"

class ASensorAdapter
{
private:
    int start_up_time;
    int enable_pin;
    unsigned long time_last_measurement;

protected:
    size_t measurement_amount;
    const MeasurementType *measurement_types;

public:
    ASensorAdapter();
    ASensorAdapter(const int enable_pin, const MeasurementType *measurement_types, const size_t measurement_amount = 0, const int start_up_time = 0);
    ~ASensorAdapter();
    void Enable();
    void Disable();
    int GetStartupTime();
    void SetTimeLastMeasurement(unsigned long time = millis());
    unsigned long GetTimeLastMeasurement();
    size_t GetMeasurementAmount();
    bool GetMeasurementTypes(MeasurementType *measurement_types);

    /**
     * @brief Overwrite this function
     *
     * @return true
     * @return false
     */
    virtual bool StartMeasurement();

    /**
     * @brief Overwrite this function
     *
     * @return true
     * @return false
     */
    virtual bool IsMeasurementFinnished();

    /**
     * @brief Overwrite this function
     *
     * @param measurements
     * @return true
     * @return false
     */
    virtual bool GetMeasurements(float *measurements);
};

#endif // A_SENSOR_ADAPTER_HPP_
