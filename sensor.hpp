
//  A header guard prevents the file from being included twice
#ifndef SENSOR_H
#define SENSOR_H

#include "check.hpp"

namespace Sensor {
    class Sensor {
    private:
        //  The rates in milliseconds at which the tick and report functions should be called
        int tickRate = -1;
        int reportRate = -1;
    public:
        Sensor() {}
        ~Sensor() {}

        //  Tick and report are virtual, so they must be implemented by classes that inherit from sensor
        virtual void tick() = 0;
        virtual double report() = 0;

        //  Get/set tick/report rate should be self explanitory 
        void setTickRate(int tickRate);
        int getTickRate();

        void setReportRate(int reportRate);
        int getReportRate();
    };

    void Sensor::setTickRate(int newTickRate) {
        CHECK(newTickRate >= 1, "Tick rate must be at least 1 millisecond")
        CHECK(tickRate == -1, "Tick rate already set")

        tickRate = newTickRate;
    }

    int Sensor::getTickRate() {
        return tickRate;
    }

    void Sensor::setReportRate(int newReportRate) {
        CHECK(newReportRate >= 1, "Read rate must be at least 1 millisecond")
        CHECK(reportRate == -1, "Read rate already set")

        reportRate = newReportRate;
    }

    int Sensor::getReportRate() {
        return reportRate;
    }
}

#endif
