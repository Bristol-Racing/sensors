

#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include "sensor.hpp"

namespace Sensor {
    class Potentiometer : public Sensor {
    private:
        uint8_t vin;

        long totalReading;
        int readings;

        bool rev;
    public:
        Potentiometer(uint8_t pin, bool reverse = false);
        ~Potentiometer();

        void tick();
        double read();
    };

    Potentiometer::Potentiometer(uint8_t pin, bool reverse = false) {
        vin = pin;
        pinMode(pin, INPUT);

        totalReading = 0;
        readings = 0;

        rev = reverse;
    }

    Potentiometer::~Potentiometer() {

    }

    void Potentiometer::tick() {
        totalReading += analogRead(vin);
        readings++;
    }

    double Potentiometer::read() {
        double average = (double)totalReading / (double)readings;
        double voltage = (5.0 / 1023.0) * (double)average;// Read the voltage from sensor

        if (rev) {
            voltage = 5.0 - voltage;
        }

        totalReading = 0;
        readings = 0;

        return voltage;
    }
}

#endif
