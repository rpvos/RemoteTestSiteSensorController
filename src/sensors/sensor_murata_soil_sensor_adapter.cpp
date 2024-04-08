#include "sensors/sensor_murata_soil_sensor_adapter.hpp"

const int kStartUpTime = 10000;
const RemoteTestSite_MeasurementInfo kMeasurementTypes[] = {
    RemoteTestSite_MeasurementInfo::RemoteTestSite_MeasurementInfo_MEASUREMENT_INFO_MURATA_TEMPERATURE,
    RemoteTestSite_MeasurementInfo::RemoteTestSite_MeasurementInfo_MEASUREMENT_INFO_MURATA_VWC,
    RemoteTestSite_MeasurementInfo::RemoteTestSite_MeasurementInfo_MEASUREMENT_INFO_MURATA_EC_BULK,
    RemoteTestSite_MeasurementInfo::RemoteTestSite_MeasurementInfo_MEASUREMENT_INFO_MURATA_EC_PORE};

SensorMurataSoilSensorAdapter::SensorMurataSoilSensorAdapter(RS485 *rs485, const uint8_t enable_pin) : ASensorAdapter(enable_pin, kMeasurementTypes, sizeof(kMeasurementTypes) / sizeof(RemoteTestSite_MeasurementInfo), kStartUpTime)
{
    this->murata_soil_sensor = MurataSoilSensor::MurataSoilSensor(rs485, enable_pin);
}

SensorMurataSoilSensorAdapter::~SensorMurataSoilSensorAdapter()
{
}

bool SensorMurataSoilSensorAdapter::StartMeasurement()
{
    return (this->murata_soil_sensor.StartMeasurement() == MurataSoilSensorError::kOk);
}

bool SensorMurataSoilSensorAdapter::IsMeasurementFinnished()
{
    bool is_finished = false;
    if (this->murata_soil_sensor.IsMeasurementFinished(is_finished) == MurataSoilSensorError::kOk)
    {
        return is_finished;
    }
    return false;
}

bool SensorMurataSoilSensorAdapter::GetMeasurements(uint32_t *measurements)
{
    MurataSoilSensorHelper::MeasurementData data;

    if (this->murata_soil_sensor.ReadMeasurementData(data) == MurataSoilSensorError::kOk)
    {
        measurements[0] = data.temperature;
        measurements[1] = data.vwc;
        measurements[2] = data.ec_bulk;
        measurements[3] = data.ec_pore;
        return true;
    }
    return false;
}
