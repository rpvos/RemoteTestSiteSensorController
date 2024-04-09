#if !defined(I_SENSOR_ADAPTER_HPP_)
#define I_SENSOR_ADAPTER_HPP_

#include <stdint.h>
#include <proto/measurement_info.pb.h>

class ISensorAdapter
{
private:
public:
    virtual void Enable() = 0;
    virtual void Disable() = 0;
    virtual int GetStartupTime() = 0;
    virtual void SetTimeLastMeasurement(unsigned long time) = 0;
    virtual unsigned long GetTimeLastMeasurement() = 0;
    virtual size_t GetMeasurementAmount() = 0;
    virtual bool GetMeasurementTypes(RemoteTestSite_MeasurementInfo *measurement_types) = 0;
    virtual unsigned long GetFrequency() = 0;
    virtual void SetFrequency(unsigned long frequency) = 0;
    virtual bool StartMeasurement() = 0;
    virtual bool IsMeasurementFinnished() = 0;
    virtual bool GetMeasurements(uint32_t *measurements) = 0;
};

#endif // I_SENSOR_ADAPTER_HPP_
