

#ifndef VOLTAGE_H
#define VOLTAGE_H

#include "sensor.hpp"

namespace Sensor {
    class VoltageSensor : public Sensor {
    private:
        uint8_t vin;
        double rPlus;
        double rMinus;
        double divFactor;
    public:
        VoltageSensor(uint8_t pin, double div);
        ~VoltageSensor();

        void tick();
        double read();
    };

    VoltageSensor::VoltageSensor(uint8_t pin, double div) {
        vin = pin;
        divFactor = div;
        pinMode(pin, INPUT);
    }

    VoltageSensor::~VoltageSensor() {

    }

    void VoltageSensor::tick() {

    }

    double VoltageSensor::read() {
        // double readVoltage = (5.0 / 1023.0) * analogRead(vin);// Read the voltage from sensor
        // double voltage = ((rPlus + rMinus) / rMinus) * readVoltage;

        double voltage = analogRead(vin) / divFactor;

        return voltage;
    }
}

#endif
