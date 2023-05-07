
#ifndef SENSOR_H
#define SENSOR_H

#include "../check.hpp"

namespace Sensor {
    class Sensor {
    private:
        int tickRate = -1;
        int readRate = -1;
    public:
        Sensor() {}
        ~Sensor() {}

        virtual void tick() = 0;
        virtual double read() = 0;

        void setTickRate(int tickRate);
        int getTickRate();

        void setReadRate(int readRate);
        int getReadRate();
    };

    void Sensor::setTickRate(int newTickRate) {
        CHECK(newTickRate >= 1, "Tick rate must be at least 1 millisecond")
        CHECK(tickRate == -1, "Tick rate already set")

        tickRate = newTickRate;
    }

    int Sensor::getTickRate() {
        return tickRate;
    }

    void Sensor::setReadRate(int newReadRate) {
        CHECK(newReadRate >= 1, "Read rate must be at least 1 millisecond")
        CHECK(readRate == -1, "Read rate already set")

        readRate = newReadRate;
    }

    int Sensor::getReadRate() {
        return readRate;
    }
}

#endif
