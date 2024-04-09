#if !defined(A_SENSOR_ADAPTER_HPP_)
#define A_SENSOR_ADAPTER_HPP_

#include "i_sensor_adapter.hpp"

class ASensorAdapter : public ISensorAdapter
{
private:
    int start_up_time;
    int enable_pin;
    unsigned long time_last_measurement;
    unsigned long frequency;

protected:
    size_t measurement_amount;
    const RemoteTestSite_MeasurementInfo *measurement_types;

public:
    ASensorAdapter();
    ASensorAdapter(const int enable_pin, const RemoteTestSite_MeasurementInfo *measurement_types, const size_t measurement_amount = 0, const int start_up_time = 0);
    ~ASensorAdapter();

    void Enable() override;
    void Disable() override;
    int GetStartupTime() override;
    void SetTimeLastMeasurement(unsigned long time) override;
    unsigned long GetTimeLastMeasurement() override;
    size_t GetMeasurementAmount() override;
    bool GetMeasurementTypes(RemoteTestSite_MeasurementInfo *measurement_types) override;

    unsigned long GetFrequency() override;
    void SetFrequency(unsigned long frequency) override;

    /**
     * @brief Overwrite this function
     *
     * @return true
     * @return false
     */
    virtual bool StartMeasurement() override;

    /**
     * @brief Overwrite this function
     *
     * @return true
     * @return false
     */
    virtual bool IsMeasurementFinnished() override;

    /**
     * @brief Overwrite this function
     *
     * @param measurements
     * @return true
     * @return false
     */
    virtual bool GetMeasurements(uint32_t *measurements) override;
};

#endif // A_SENSOR_ADAPTER_HPP_
