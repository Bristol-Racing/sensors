

#ifndef throttle_H
#define throttle_H

#include "sensor.hpp"

namespace Sensor {
    typedef void (* ThrottleCallback)(float); 

    class Throttle : public Sensor {
    private:
        uint8_t vin;

        long totalReading;
        int readings;

        ThrottleCallback throttleCallback;
    public:
        Throttle(uint8_t pin, ThrottleCallback callback);
        ~Throttle();

        void tick();
        float read();
    };

    Throttle::Throttle(uint8_t pin, ThrottleCallback callback) {
        vin = pin;
        pinMode(pin, INPUT);

        totalReading = 0;
        readings = 0;

        throttleCallback = callback;
    }

    Throttle::~Throttle() {

    }

    void Throttle::tick() {
        totalReading += analogRead(vin);
        readings++;
    }

    float Throttle::read() {
        float average = (float)totalReading / (float)readings;
        float voltage = (5.0 / 1023.0) * (float)average;// Read the voltage from sensor

        totalReading = 0;
        readings = 0;

        throttleCallback(voltage);

        return voltage;
    }
}

#endif
