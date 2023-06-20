

#ifndef CHARGE_H
#define CHARGE_H

#include "sensor.hpp"
#include "sensorManager.hpp"
#include "ADC_Current.hpp"

namespace Sensor {
    class ChargeMonitor : public Sensor {
    private:
        SensorManager* m;
        CurrentSensor* c;

        double ah;
    public:
        ChargeMonitor(SensorManager* manager, CurrentSensor* current, double startingAmpHours);
        ~ChargeMonitor();

        void tick();
        double read();
    };

    ChargeMonitor::ChargeMonitor(SensorManager* manager, CurrentSensor* current, double startingAmpHours) {
        m = manager;
        c = current;
        ah = startingAmpHours;
    }

    ChargeMonitor::~ChargeMonitor() {

    }

    void ChargeMonitor::tick() {

    }

    double ChargeMonitor::read() {
        double current = m->getLastRead(c);
        ah -= current * (1.0 / 60.0 / 60.0);

        return ah;
    }
}

#endif
