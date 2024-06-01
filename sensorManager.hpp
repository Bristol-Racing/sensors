
//  A header guard prevents the file from being included twice
#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include "sensor.hpp"

namespace Sensor {
    //  Defines a report callback function type
    typedef void (* ReportCallback)(double*);
    typedef void (* SendLEDCommand) (int, int);

    //  A sensor that keeps track of the CPU usage of the arduino
    class CPUMonitor : public Sensor {
    private:
        long totalWaitTime;     //  The total wait time since the last report
        unsigned long prevTime; //  The time of the last report
    public:
        CPUMonitor();   //  Called when a new sensor object is created
        ~CPUMonitor();  //  Called when a sensor object is destroyed

        void tick();        //  Both called by the sensor manager
        double report();

        void addWait(int time); //  Adds wait time, for CPU usage calcs
    };

    CPUMonitor::CPUMonitor() {
        //  Resets the wait time
        totalWaitTime = 0;

        //  Stores the current time
        prevTime = millis();
    }

    CPUMonitor::~CPUMonitor() {
        //  Don't need to do anything when the object is destroyed
    }

    void CPUMonitor::tick() {
        //  Don't need to do anything for ticks
    }

    double CPUMonitor::report() {
        //  Called by the sensor manager whenever CPU usage should be reported

        //  Calculate the time since the last report
        unsigned long time = millis();
        long timeDelta = time - prevTime;

        //  Update the time
        prevTime = time;

        //  Calculate the CPU utilisation (the amount of time not spent waiting)
        double utilisation = (double)(timeDelta - totalWaitTime) / timeDelta;

        //  Reset the wait time
        totalWaitTime = 0;

        return utilisation;
    }

    void CPUMonitor::addWait(int time) {
        totalWaitTime += time;
    }


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

        int spinTime;   //  The time to run the sensor manager for

        int diagTimer = 0;
        bool diagMode = 0;
        int faultMode = sensorCount + 2;
        int faultTimer = 0;

        ReportCallback reportCallback;  //  The callback function that passes sensor readings back to the program
        SendLEDCommand sendLEDCommand;

        CPUMonitor monitor;     //  The CPU monitor sensor

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

        CPUMonitor* getMonitor();
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

        //  Stores the callback rate
        callbackRate = rate;
        //  Resets the next callback time
        nextCallback = callbackRate;

        //  Stores the current time
        prevTime = millis();

        spinTime = 0;
        reportCallback = NULL;

        //  Sets the CPU monitor report rate to be the same as the callback rate
        monitor.setReportRate(callbackRate);
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

        //  Subtract the elapsed time from the next callback time
        nextCallback -= timeDelta;

        //  Subtract the elapsed time from the spin time
        spinTime -= timeDelta;
    }

    void SensorManager::processTicks() {
        //  Go through each sensor
        for (int i = 0; i < sensorCount; i++) {
            //  If the next tick time has elapsed
            if (nextTicks[i] <= 0) {
                //  Call the tick method
                sensors[i]->tick();
                //  And reset the next tick time
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

    void SensorManager::setSendLEDCommand(SendLEDCommand command) {
        sendLEDCommand = command;
    }

    void SensorManager::processCallbacks() {
        //  If the next callback time has elapsed
        if (nextCallback <= 0) {
            sendLEDCommand(1,0);
            //  reset the next callback time
            nextCallback = callbackRate;
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


            if (diagTimer >= 15) {
                diagMode = diagMode ? 0 : 1;
                faultMode = 2;
                diagTimer = 0;
            }

    }

    void SensorManager::faultInject() {

        //Serial.println(faultTimer);
        //Serial.println(faultMode);

        readings[faultMode] = 999; // Inject a fault value of 999
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

    void SensorManager::addSensor(Sensor* sensor) {
        //  Adds the sensor to the sensors array
        sensors[sensorCount] = sensor;
        //  And resets the next tick/report times for it
        nextTicks[sensorCount] = sensor->getTickRate();
        nextReports[sensorCount] = sensor->getReportRate();

        //  Increments the sensor count
        sensorCount++;
    }

    void SensorManager::spin(int maxTime = -1) {
        //  Updates the next everything times
        updateTimes();
        //  Sets the spin time to the max time passed in to the spin function
        //  spin time is ignored if -1 is passed in
        spinTime = maxTime;

        //  Repeats as long as there is spin time remaining
        //  or max time is -1
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
                //  wait
                delay(minTime);

                //  Register the wait with the CPU monitor
                monitor.addWait(minTime);

                //  Update the next everything times
                updateTimes();
            }

            //  Process any ticks, reports, and callbacks that have happened
            processTicks();
            processReports();
            processCallbacks();
        }

    }

    int SensorManager::timeToNextTick() {
        //  time is 1 sec by default
        int minTime = 1000;

        //  Always set min time if this is the first sensor
        bool found = false;

        //  Go through all the sensors
        for (int i = 0; i < sensorCount; i++) {
            //  If the sensor has a tick rate
            if (sensors[i]->getTickRate() > 0) {
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

        //  min time is time to first sensor report
        int minTime = nextReports[0];

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

    CPUMonitor* SensorManager::getMonitor() {
        //  Returns the CPU monitor sensor
        return &monitor;
    }
}

#endif
