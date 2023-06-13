

#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include "sensor.hpp"

#define MINTEMP 15.0
#define MINMV 1939.0
#define MAXTEMP 100.0
#define MAXMV 997.0
#define TEMP_GRADIENT ((MAXMV - MINMV) / (MAXTEMP - MINTEMP))

namespace Sensor {
    class TemperatureSensor : public Sensor {
    private:
        uint8_t vin;
    public:
        TemperatureSensor(uint8_t pin);
        ~TemperatureSensor();

        void tick();
        double read();
    };

    TemperatureSensor::TemperatureSensor(uint8_t pin) {
        vin = pin;
    }

    TemperatureSensor::~TemperatureSensor() {

    }

    void TemperatureSensor::tick() {

    }

    double TemperatureSensor::read() {
        int reading = analogRead(vin);
        double voltage = (5000.0 / 1023.0) * (double)reading;
        double temperature = (voltage - MINMV) / TEMP_GRADIENT + MINTEMP;

        return temperature;
    }
}

#endif
