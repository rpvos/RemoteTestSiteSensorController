#include "sensors/sensor_murata_soil_sensor_adapter.hpp"

const int kStartUpTime = 10000;
const MeasurementType kMeasurementTypes[] = {MeasurementType::kMeasurementTypeTemperature, MeasurementType::kMeasurementTypeVwc, MeasurementType::kMeasurementTypeEcBulk, MeasurementType::kMeasurementTypeEcPore};

SensorMurataSoilSensorAdapter::SensorMurataSoilSensorAdapter(RS485 *rs485, uint8_t enable_pin) : ASensorAdapter(enable_pin, kMeasurementTypes, sizeof(kMeasurementTypes) / sizeof(MeasurementType), kStartUpTime)
{
    this->murata_soil_sensor = new MurataSoilSensor::MurataSoilSensor(rs485, enable_pin);
}

SensorMurataSoilSensorAdapter::~SensorMurataSoilSensorAdapter()
{
    delete this->murata_soil_sensor;
}

bool SensorMurataSoilSensorAdapter::StartMeasurement()
{
    return (this->murata_soil_sensor->StartMeasurement() == MurataSoilSensorError::kOk);
}

bool SensorMurataSoilSensorAdapter::IsMeasurementFinnished()
{
    bool is_finished = false;
    if (this->murata_soil_sensor->IsMeasurementFinished(is_finished) == MurataSoilSensorError::kOk)
    {
        // Serial.print("Code: ");
        // Serial.println(static_cast<int>(this->murata_soil_sensor->IsMeasurementFinished(is_finished)));
        return is_finished;
    }
    return false;
}

bool SensorMurataSoilSensorAdapter::GetMeasurements(float *measurements)
{
    MurataSoilSensorHelper::MeasurementData data;

    if (this->murata_soil_sensor->ReadMeasurementData(data) == MurataSoilSensorError::kOk)
    {
        measurements[0] = MurataSoilSensorHelper::CalculateTemperature(data.temperature);
        measurements[1] = MurataSoilSensorHelper::CalculateVWC(data.vwc);
        measurements[2] = MurataSoilSensorHelper::CalculateECBulk(data.ec_bulk);
        measurements[3] = MurataSoilSensorHelper::CalculateECPore(data.ec_pore);
        return true;
    }
    return false;
}
