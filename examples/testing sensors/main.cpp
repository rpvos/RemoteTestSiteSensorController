#include <Arduino.h>
#include "sensor_controller.hpp"
#include "sensors/sensor_therm200_adapter.hpp"
#include "sensors/sensor_vh400_adapter.hpp"
// #include <esp_sleep.h>

const uint8_t kTherm200Adcpin = A0;
const uint8_t kVh400Adcpin = A3;
const uint8_t kTherm200Enablepin = 23;
const uint8_t kVh400Enablepin = 22;

SensorTherm200Adapter temp_sensor = SensorTherm200Adapter(kTherm200Enablepin, kTherm200Adcpin);
SensorVh400Adapter vwc_sensor = SensorVh400Adapter(kVh400Enablepin, kVh400Adcpin);
const size_t amount_of_sensors = 2;

SensorController controller = SensorController();

void setup()
{
  pinMode(kTherm200Adcpin, INPUT);
  pinMode(kVh400Adcpin, INPUT);
  pinMode(kTherm200Enablepin, OUTPUT);
  pinMode(kVh400Enablepin, OUTPUT);

  Serial.begin(9600);
  Serial.println("Controller starting up");

  ASensorAdapter *sensors[] = {&temp_sensor, &vwc_sensor};

  controller.AddSensor(sensors, amount_of_sensors);

  if (controller.TimeUntillNextMeasurement() != 0)
  {
    Serial.println("Initilisation failed");
  }

  controller.SetFrequency(MeasurementType::kMeasurementTypeTemperature, 60000);
  controller.SetFrequency(MeasurementType::kMeasurementTypeVwc, 60000);

  Serial.println("Type,waarde");
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

    if (MeasurementTypeHelper::as_integer(controller.GetMeasurementType()) == 1)
    {
      Serial.print("Temperature (C),");
    }
    else if (MeasurementTypeHelper::as_integer(controller.GetMeasurementType()) == 2)
    {
      Serial.print("Vwc (%),");
    }
    Serial.println(value);
    Serial.flush();
  }

  // esp_sleep_enable_timer_wakeup(controller.TimeUntillNextMeasurement() * 1000);
  // esp_light_sleep_start();
  delay(1000);
}
