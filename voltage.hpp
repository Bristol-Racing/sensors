

#ifndef VOLTAGE_H
#define VOLTAGE_H

#include "sensor.hpp"

namespace Sensor {
    class VoltageSensor : public Sensor {
    private:
        uint8_t vin;
        double rPlus;
        double rMinus;
    public:
        VoltageSensor(uint8_t pin, double rPositive, double rNegative);
        ~VoltageSensor();

        void tick();
        double read();
    };

    VoltageSensor::VoltageSensor(uint8_t pin, double rPositive, double rNegative) {
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
        double readVoltage = (5.0 / 1023.0) * analogRead(vin);// Read the voltage from sensor
        double voltage = ((rPlus + rMinus) / rMinus) * readVoltage;

        return voltage;
    }
}

#endif
