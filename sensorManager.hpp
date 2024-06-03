// Sensor manager is a class 
// that takes sensor objects, keeps track of report rate and
// every tick gets each sensor value and outputs to main code

//  A header guard prevents the file from being included twice
#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H
#include "sensor.hpp"

namespace Sensor {
    //  Defines a report callback function type
    typedef void (* ReportCallback)(double*);
    typedef void (* SendLEDCommand) (int, int);

    class SensorManager {
    private:
        int sensorCount;    //  The number of sensors in use
        int maxSensorCount; //  The max number of sensors in use

        Sensor** sensors;   //  An array of sensor objects
        int* nextTicks;     //  The times for the next tick call on each sensor object
        int* nextReports;   //  The times for the next report call on each sensor object
        double* readings;   //  The last reading from each sensor

        int callbackRate;   //  The rate at which the callback function should pass sensor readings back to the program
        int nextCallback;   //  The time for the next callback
        unsigned long prevTime; //  The time of the last thing (tick, report, callback)
        int spinTime;       //  The time to run the sensor manager for

        int diagTimer = 0;
        bool diagMode = 0;
        int faultMode = sensorCount + 2;
        int faultTimer = 0;

        ReportCallback reportCallback;  //  The callback function that passes sensor readings back to the program
        SendLEDCommand sendLEDCommand;

        void tempCheck(double*);
        void diagCheck();
        void faultInject();
        void updateTimes();
        void processTicks();
        void processReports();
        void processCallbacks();

    public:
        SensorManager(int maxSensors, int rate);
        ~SensorManager();
        void setReportCallback(ReportCallback callback);
        void setSendLEDCommand(SendLEDCommand command);
        void addSensor(Sensor* sensor);
        void spin(int maxTime = -1);
        int timeToNextTick();
        int timeToNextReport();
        double getLastReport(int sensorIndex);
        double getLastReport(Sensor* sensor);
    };


    SensorManager::SensorManager(int maxSensors, int rate) {
        //  Sensor count starts at 0
        sensorCount = 0;
        maxSensorCount = maxSensors;

        //  Allocates memory for arrays for the sensors
        //  as well as the times and readings
        sensors = (Sensor**)malloc(sizeof(Sensor*) * maxSensors);
        nextTicks = (int*)malloc(sizeof(int) * maxSensors);
        nextReports = (int*)malloc(sizeof(int) * maxSensors);
        readings = (double*)malloc(sizeof(double) * maxSensors);

        callbackRate = rate;            //  Stores the callback rate
        nextCallback = callbackRate;    //  Resets the next callback time

        prevTime = millis();            //  Stores the current time

        spinTime = 0;
        reportCallback = NULL;
    }


    SensorManager::~SensorManager() {
        //  Free the memory for the arrays when the sensor manager is destroyed
        free(sensors);
        free(nextTicks);
        free(nextReports);
        free(readings);
    }

    void SensorManager::updateTimes() {
        //  Calculate the time since the last update
        unsigned long time = millis();
        int timeDelta = time - prevTime;

        //  Update the time
        prevTime = time;

        //  Subtract the elapsed time from all of the next tick/report times
        for (int i = 0; i < sensorCount; i++) {
            nextTicks[i] -= timeDelta;
            nextReports[i] -= timeDelta;
        }
        nextCallback -= timeDelta; //  Subtract the elapsed time from the next callback time
        spinTime -= timeDelta; //  Subtract the elapsed time from the spin time
    }

    void SensorManager::processTicks() {
        //  Go through each sensor
        for (int i = 0; i < sensorCount; i++) {
            //  If the next tick time has elapsed
            if (nextTicks[i] <= 0) {
                //  Call the tick method
                sensors[i]->tick();
                //  Reset the next tick time
                nextTicks[i] = sensors[i]->getTickRate();
            }
        }
    }

    void SensorManager::processReports() {
        //  Go through each sensor
        for (int i = 0; i < sensorCount; i++) {
            //  If the next report time has elapsed
            if (nextReports[i] <= 0) {
                //  Call the report method and store the returned reading
                readings[i] = sensors[i]->report();
                //  And reset the next report time
                nextReports[i] = sensors[i]->getReportRate();
            }
        }
    }

    void SensorManager::processCallbacks() {
        //  If the next callback time has elapsed
        if (nextCallback <= 0) {
            //  reset the next callback time
            nextCallback = callbackRate;
            tempCheck(readings);
            diagCheck();

            //  And call the callback function with the array of sensor readings
            //  to pass the readings back to the main program
            if (diagMode) {
                faultInject();
                sendLEDCommand(1,2);
            } else {
                sendLEDCommand(1,1);
            }
            reportCallback(readings);
            delay(50);
            sendLEDCommand(1,0);
        }
    }

    void SensorManager::tempCheck(double* readings) {
        double motTemp = readings[4];
        if (motTemp < 60.0) {
            sendLEDCommand(2,1);
        } else if (motTemp < 90.0) {
            sendLEDCommand(2,2);
        } else {
            sendLEDCommand(2,3);
        }
    }

    void SensorManager::diagCheck() {
            double butOn = sensors[1]->report();
            if (butOn >= 0.5) {
                diagTimer++;
                sendLEDCommand(3,1);
            } else {
                diagTimer = 0;
                sendLEDCommand(3,0);
            }
            if (diagTimer >= 10) {
                diagMode = diagMode ? 0 : 1;
                faultMode = 2;
                diagTimer = 0;
            }
    }

    void SensorManager::faultInject() {
        readings[faultMode] = 999; // Inject a fault value of 666.6
        faultTimer++;
        if (faultTimer >= 5) {
            if (faultMode >= sensorCount-1) {
                faultMode = 2;
            } else {
                faultMode++;
            }
            faultTimer = 0;
        }
    }

    void SensorManager::setReportCallback(ReportCallback callback) {
        CHECK(reportCallback == NULL, "Report callback already set")
        reportCallback = callback;
    }

    void SensorManager::setSendLEDCommand(SendLEDCommand command) {
        sendLEDCommand = command;
    }

    void SensorManager::addSensor(Sensor* sensor) {
        //  Adds the sensor to the sensors array
        sensors[sensorCount] = sensor;
        //  And resets the next tick/report times for it
        nextTicks[sensorCount] = sensor->getTickRate();
        nextReports[sensorCount] = sensor->getReportRate();

        sensorCount++; //  Increments the sensor count
    }

    void SensorManager::spin(int maxTime = -1) {
        //  Updates the next everything times
        updateTimes();

        //  Sets the spin time to the max time passed in to the spin function
        //  spin time is ignored if -1 is passed in
        spinTime = maxTime;

        //  Repeats as long as there is spin time remaining or max time is -1
        while (spinTime > 0 || maxTime == -1) {
            //  Finds the minimum time to the next tick/report/callback
            int minTime = timeToNextTick();
            int nextReport = timeToNextReport();

            if (nextReport < minTime) {
                minTime = nextReport;
            }

            if (nextCallback < minTime) {
                minTime = nextCallback;
            }

            if (maxTime >= 0 && spinTime < minTime) {
                minTime = spinTime;
            }

            //  If the minimum time is greater than 0
            if (minTime >= 1) {
                delay(minTime); //  wait
                //monitor.addWait(minTime); //  Register the wait with the CPU monitor
                updateTimes(); //  Update the next everything times
            }

            //  Process any ticks, reports, and callbacks that have happened
            processTicks();
            processReports();
            processCallbacks();
        }


    }

    int SensorManager::timeToNextTick() {
        int minTime = 1000; //  time is 1 sec by default
        bool found = false; //  Always set min time if this is the first sensor

        //  Go through all the sensors
        for (int i = 0; i < sensorCount; i++) {
            if (sensors[i]->getTickRate() > 0) { //  If the sensor has a tick rate
                //  Set the min time if it is the first sensor or has a lower next tick time
                if (!found || nextTicks[i] < minTime) {
                    minTime = nextTicks[i];
                }
            }
        }
        return minTime;
    }

    int SensorManager::timeToNextReport() {
        //  time is 1 sec by default
        if (sensorCount == 0) return 1000;
        int minTime = nextReports[0]; //  min time is time to first sensor report

        //  Go through all the other sensors
        for (int i = 1; i < sensorCount; i++) {
            //  If the sensor has a report rate
            if (sensors[i]->getReportRate() > 0) {
                //  Set the min time if it has a lower next report time
                if (nextReports[i] < minTime) {
                    minTime = nextReports[i];
                }
            }
        }
        return minTime;
    }

    double SensorManager::getLastReport(int sensorIndex) {
        //  Returns the last reading from the sensor
        return readings[sensorIndex];
    }

    double SensorManager::getLastReport(Sensor* sensor) {
        //  Finds the sensor and returns its last reading
        for (int i = 0; i < sensorCount; i++) {
            if (sensors[i] == sensor) {
                return readings[i];
            }
        }
        RAISE("Sensor not found.");
    }
}

#endif