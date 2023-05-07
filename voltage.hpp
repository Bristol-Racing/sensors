

#ifndef VOLTAGE_H
#define VOLTAGE_H

#include "sensor.hpp"

namespace Sensor {
    class VoltageSensor : public Sensor {
    private:
        uint8_t vin;
        float rPlus;
        float rMinus;
    public:
        VoltageSensor(uint8_t pin, float rPositive, float rNegative);
        ~VoltageSensor();

        void tick();
        double read();
    };

    VoltageSensor::VoltageSensor(uint8_t pin, float rPositive, float rNegative) {
        vin = pin;
        rPlus = rPositive;
        rMinus = rNegative;
        pinMode(pin, INPUT);
    }

    VoltageSensor::~VoltageSensor() {

    }

    void VoltageSensor::tick() {

    }

    double VoltageSensor::read() {
        float readVoltage = (5.0 / 1023.0) * analogRead(vin);// Read the voltage from sensor
        float voltage = ((rPlus + rMinus) / rMinus) * readVoltage;

        return voltage;
    }
}

#endif
