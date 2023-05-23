
#ifndef LOADCELL_H
#define LOADCELL_H

#include "sensor.hpp"

#include <HX711.h>

namespace Sensor {
    class LoadCell : public Sensor {
    private:
        HX711 scale;

        long totalReading;
        int readings;

        float length;

    public:
        LoadCell(int dout, int sck, float armLength);
        ~LoadCell();

        void tick();
        float read();
    };

    LoadCell::LoadCell(int dout, int sck, float armLength) {
        totalReading = 0;
        readings = 0;
        scale.begin(dout, sck);

        length = armLength;
    }

    LoadCell::~LoadCell() {

    }

    void LoadCell::tick() {
        if (scale.is_ready()) {
            totalReading += scale.read();
            readings++;
        }
    }

    float LoadCell::read() {
        long average = totalReading / readings;
        float weight = 0.0022 * (float)average + 147.39;  //  grams
        float force = (weight / 1000.0) * 9.8066500286389; //  newtons
        float torque = force * length;                     //  newton-meters
        
        totalReading = 0;
        readings = 0;

        return torque;
    }
}

#endif
