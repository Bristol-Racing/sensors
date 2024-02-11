
//  A header guard prevents the file from being included twice
#ifndef LOADCELL_H
#define LOADCELL_H

//  Include the parent sensor
#include "sensor.hpp"

//  Include the load cell library
#include <HX711.h>

namespace Sensor {
    class LoadCell : public Sensor {
    private:
        HX711 scale;    //  The load cell object

        long totalReading;  //  Stores the sum of the readings taken since the last report
        int readings;   //  Stores the number of readings taken since the last report

        double length;  //  Stores the length of the arm pressing on the load cell

    public:
        LoadCell(int dout, int sck, double armLength);   //  Called when a new sensor object is created
        ~LoadCell();  //  Called when a sensor object is destroyed

        void tick();        //  Both called by the sensor manager
        double report();
    };

    LoadCell::LoadCell(int dout, int sck, double armLength) {
        //  Resets running totals
        totalReading = 0;
        readings = 0;

        //  Starts the load cell
        scale.begin(dout, sck);

        //  Stores the arm length
        length = armLength;
    }

    LoadCell::~LoadCell() {
        //  Don't need to do anything when the object is destroyed
    }

    void LoadCell::tick() {
        //  Called by the sensor manager whenever a new reading needs adding to the average

        //  If the scale is ready to read
        if (scale.is_ready()) {
            //  Add the reading to the running total
            totalReading += scale.read();
            readings++;
        }
    }

    double LoadCell::report() {
        //  Called by the sensor manager whenever an average reading should be reported

        //  Compute the average from the running total
        long average = totalReading / readings;

        //  Convert the average to a torque
        double weight = 0.0022 * (double)average + 147.39;  //  grams
        double force = (weight / 1000.0) * 9.8066500286389; //  newtons
        double torque = force * length;                     //  newton-meters
        
        //  Reset the running total
        totalReading = 0;
        readings = 0;

        return torque;
    }
}

#endif
