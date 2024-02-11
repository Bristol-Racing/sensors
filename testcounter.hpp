

#ifndef COUNTER_H
#define COUNTER_H

#include "sensor.hpp"

namespace Sensor {
    class CounterSensor : public Sensor {
    private:
        double count;
    public:
        CounterSensor();
        ~CounterSensor();

        void tick();
        double report();
    };

    CounterSensor::CounterSensor() {
        count = 0;
    }

    CounterSensor::~CounterSensor() {

    }

    void CounterSensor::tick() {

    }

    double CounterSensor::report() {
        //  Just counts up for every report

        count++;
        return count;
    }
}

#endif
