#include <Arduino.h>
#include "sensor_controller.hpp"
#include "sensors/sensor_therm200_adapter.hpp"
#include "sensors/sensor_vh400_adapter.hpp"
#include <esp_sleep.h>

const size_t amount_of_sensors = 2;
SensorTherm200Adapter temp_sensor = SensorTherm200Adapter();
SensorVh400Adapter vwc_sensor = SensorVh400Adapter();

SensorController controller = SensorController();

void setup()
{
  pinMode(THERM200_ADC_PIN, INPUT);
  pinMode(THERM200_ENABLE_PIN, OUTPUT);
  pinMode(VH400_ADC_PIN, INPUT);
  pinMode(VH400_ENABLE_PIN, OUTPUT);

  Serial.begin(9600);
  Serial.println("Controller starting up");

  ASensorAdapter *sensors[] = {&temp_sensor, &vwc_sensor};

  controller.AddSensor(sensors, amount_of_sensors);

  if (controller.TimeUntillNextMeasurement() != 0)
  {
    Serial.println("Initilisation failed");
  }

  controller.SetFrequency(MeasurementType::kMeasurementTypeTemperature, 5000);
  controller.SetFrequency(MeasurementType::kMeasurementTypeVwc, 10000);
}

void loop()
{
  while (!controller.TimeUntillNextMeasurement())
  {
    controller.EnableSensor();

    int startup_time = controller.GetSensorStartupTime();
    delay(startup_time);

    controller.StartMeasurement();
    while (!controller.IsMeasurementFinnished())
    {
      delay(100);
    }

    float value = controller.GetMeasurement();

    Serial.print("Sensor type: ");
    Serial.println(MeasurementTypeHelper::as_integer(controller.GetMeasurementType()));
    Serial.print("Sensor value: ");
    Serial.println(value);
  }

  esp_sleep_enable_timer_wakeup(controller.TimeUntillNextMeasurement() * 1000);
  esp_light_sleep_start();
}
