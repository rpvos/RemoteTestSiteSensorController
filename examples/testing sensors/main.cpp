#include <Arduino.h>
#include "sensor_controller.hpp"
#include "sensors/sensor_therm200_adapter.hpp"
#include "sensors/sensor_vh400_adapter.hpp"
#include <max485ttl.hpp>
#include "sensors/sensor_murata_soil_sensor_adapter.hpp"

const uint8_t kAdcResolution = 10;
const uint8_t kReferenceVoltage = 5.0f;

const uint8_t kTherm200Adcpin = A0;
const uint8_t kVh400Adcpin = A1;
const uint8_t kTherm200Enablepin = 5;
const uint8_t kVh400Enablepin = 4;
const uint8_t kMurataEnablepin = 3;
const uint8_t kRS485Enablepin = 2;

RS485 rs = RS485(kRS485Enablepin, kRS485Enablepin, &Serial1);

SensorTherm200Adapter temp_sensor = SensorTherm200Adapter(kTherm200Enablepin, kTherm200Adcpin, kAdcResolution, kReferenceVoltage);
SensorVh400Adapter vwc_sensor = SensorVh400Adapter(kVh400Enablepin, kVh400Adcpin, kAdcResolution, kReferenceVoltage);
SensorMurataSoilSensorAdapter murata_sensor = SensorMurataSoilSensorAdapter(&rs, kMurataEnablepin);

SensorController controller = SensorController();

void setup()
{
  // Pins being used
  pinMode(kTherm200Adcpin, INPUT);
  pinMode(kVh400Adcpin, INPUT);
  pinMode(kTherm200Enablepin, OUTPUT);
  pinMode(kVh400Enablepin, OUTPUT);
  pinMode(kRS485Enablepin, OUTPUT);
  pinMode(kMurataEnablepin, OUTPUT);
  // RS485 serial
  Serial1.begin(9600);

  // Information channel
  Serial.begin(9600);
  Serial.println("Controller starting up");

  // Sensors being used currently
  ASensorAdapter *sensors[] = {&temp_sensor, &vwc_sensor, &murata_sensor};

  size_t amount_of_sensors = sizeof(sensors) / sizeof(ASensorAdapter *);
  controller.AddSensor(sensors, amount_of_sensors);

  controller.SetFrequency(MeasurementType::kMeasurementTypeTemperature, 60000);
  controller.SetFrequency(MeasurementType::kMeasurementTypeVwc, 60000);

  Serial.println("Type,Value");
}

void loop()
{
  while (!controller.TimeUntillNextMeasurement())
  {
    controller.EnableSensor();

    int startup_time = controller.GetSensorStartupTime();
    delay(startup_time);

    while (!controller.StartMeasurement())
    {
      Serial.println("Measurement start did not start");
      delay(1000);
    }
    while (!controller.IsMeasurementFinnished())
    {
      Serial.println("Measurement has not finished");
      delay(1000);
    }

    size_t measurement_amount = controller.GetMeasurementAmount();
    float measurements[measurement_amount];
    while (!controller.GetMeasurements(measurements))
    {
      Serial.println("Error on getting measurements");
      delay(1000);
    }

    MeasurementType measurement_types[measurement_amount];
    while (!controller.GetMeasurementTypes(measurement_types))
    {
      Serial.println("Measurement types could not be gotten");
      delay(1000);
    }

    for (size_t i = 0; i < measurement_amount; i++)
    {
      if (MeasurementTypeHelper::as_integer(measurement_types[i]) ==
          MeasurementTypeHelper::as_integer(MeasurementType::kMeasurementTypeTemperature))
      {
        Serial.print("Temperature (C),");
      }
      else if (MeasurementTypeHelper::as_integer(measurement_types[i]) ==
               MeasurementTypeHelper::as_integer(MeasurementType::kMeasurementTypeVwc))
      {
        Serial.print("Vwc (%),");
      }
      else if (MeasurementTypeHelper::as_integer(measurement_types[i]) ==
               MeasurementTypeHelper::as_integer(MeasurementType::kMeasurementTypeEcBulk))
      {
        Serial.print("EC bulk (%),");
      }
      else if (MeasurementTypeHelper::as_integer(measurement_types[i]) ==
               MeasurementTypeHelper::as_integer(MeasurementType::kMeasurementTypeEcPore))
      {
        Serial.print("EC Pore (%),");
      }
      Serial.println(measurements[i]);
      Serial.flush();
    }

    controller.UpdateTimeLastMeasurement();
    controller.DisableSensor();
  }

  delay(1000);
}
