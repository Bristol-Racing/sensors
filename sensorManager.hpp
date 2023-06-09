
#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include "sensor.hpp"

namespace Sensor {
    typedef void (* ReadCallback)(double*); 

    class CPUMonitor : public Sensor {
    private:
        long totalWaitTime;
        unsigned long prevTime;
    public:
        CPUMonitor();
        ~CPUMonitor();

        void tick();
        double read();

        void addWait(int time);
    };

    CPUMonitor::CPUMonitor() {
        totalWaitTime = 0;
        prevTime = millis();
    }

    CPUMonitor::~CPUMonitor() {

    }

    void CPUMonitor::tick() {

    }

    double CPUMonitor::read() {
        unsigned long time = millis();
        long timeDelta = time - prevTime;

        prevTime = time;

        double utilisation = (double)(timeDelta - totalWaitTime) / timeDelta;

        totalWaitTime = 0;

        return utilisation;
    }

    void CPUMonitor::addWait(int time) {
        totalWaitTime += time;
    }

    class SensorManager {
    private:
        int sensorCount;
        int maxSensorCount;

        Sensor** sensors;
        int* nextTicks;
        int* nextReads;
        double* readings;

        int reportRate;
        int nextReport;

        unsigned long prevTime;

        int spinTime;

        ReadCallback readCallback;

        int sleepTime = 0;

        CPUMonitor monitor;

        void updateTimes();
        void processTicks();
        void processReads();
        void processReports();

    public:
        SensorManager(int maxSensors, int rate);
        ~SensorManager();

        void setReadCallback(ReadCallback callback);

        void addSensor(Sensor* sensor);
        void spin(int maxTime = -1);
        int timeToNextTick();
        int timeToNextRead();

        double getLastRead(int sensorIndex);
        double getLastRead(Sensor* sensor);

        CPUMonitor* getMonitor();
    };
    
    SensorManager::SensorManager(int maxSensors, int rate) {
        sensorCount = 0;
        maxSensorCount = maxSensors;

        sensors = (Sensor**)malloc(sizeof(Sensor*) * maxSensors);
        nextTicks = (int*)malloc(sizeof(int) * maxSensors);
        nextReads = (int*)malloc(sizeof(int) * maxSensors);
        readings = (double*)malloc(sizeof(double) * maxSensors);

        reportRate = rate;
        nextReport = reportRate;

        prevTime = millis();

        spinTime = 0;

        readCallback = NULL;

        monitor.setReadRate(reportRate);
    }
    
    SensorManager::~SensorManager() {
        free(sensors);
        free(nextTicks);
        free(nextReads);
        free(readings);
    }

    void SensorManager::updateTimes() {
        unsigned long time = millis();
        int timeDelta = time - prevTime;

        // Serial.println(sleepTime);
        // Serial.println((double)sleepTime / timeDelta);

        prevTime = time;

        for (int i = 0; i < sensorCount; i++) {
            nextTicks[i] -= timeDelta;
            nextReads[i] -= timeDelta;
        }

        nextReport -= timeDelta;

        spinTime -= timeDelta;
    }

    void SensorManager::processTicks() {
        for (int i = 0; i < sensorCount; i++) {
            if (nextTicks[i] <= 0) {
                sensors[i]->tick();
                nextTicks[i] = sensors[i]->getTickRate();
            }
        }
    }

    void SensorManager::processReads() {
        for (int i = 0; i < sensorCount; i++) {
            if (nextReads[i] <= 0) {
                readings[i] = sensors[i]->read();
                nextReads[i] = sensors[i]->getReadRate();
            }
        }
    }

    void SensorManager::processReports() {
        if (nextReport <= 0) {
            nextReport = reportRate;

            readCallback(readings);
        }
    }
    
    void SensorManager::setReadCallback(ReadCallback callback) {
        CHECK(readCallback == NULL, "Read callback already set")

        readCallback = callback;
    }

    void SensorManager::addSensor(Sensor* sensor) {
        sensors[sensorCount] = sensor;
        nextTicks[sensorCount] = sensor->getTickRate();
        nextReads[sensorCount] = sensor->getReadRate();

        sensorCount++;
    }

    void SensorManager::spin(int maxTime = -1) {
        updateTimes();
        spinTime = maxTime;

        while (spinTime > 0 || maxTime == -1) {
            int minTime = timeToNextTick();
            int nextRead = timeToNextRead();

            if (nextRead < minTime) {
                minTime = nextRead;
            }
            if (nextReport < minTime) {
                minTime = nextReport;
            }
            if (maxTime >= 0 && spinTime < minTime) {
                minTime = spinTime;
            }

            // Serial.print("minTime: ");
            // Serial.println(minTime);

            if (minTime >= 1) {
                delay(minTime);
                monitor.addWait(minTime);
                updateTimes();
            }

            processTicks();
            processReads();
            processReports();
        }
        
    }

    int SensorManager::timeToNextTick() {
        int minTime = 1000;
        bool found = false;

        for (int i = 0; i < sensorCount; i++) {
            if (sensors[i]->getTickRate() > 0) {
                if (!found || nextTicks[i] < minTime) {
                    minTime = nextTicks[i];
                }
            }
        }

        return minTime;
    }

    int SensorManager::timeToNextRead() {
        if (sensorCount == 0) return 1000;

        int minTime = nextReads[0];

        for (int i = 1; i < sensorCount; i++) {
            if (sensors[i]->getReadRate() > 0) {
                if (nextReads[i] < minTime) {
                    minTime = nextReads[i];
                }
            }
        }

        return minTime;
    }

    double SensorManager::getLastRead(int sensorIndex) {
        return readings[sensorIndex];
    }

    double SensorManager::getLastRead(Sensor* sensor) {
        for (int i = 0; i < sensorCount; i++) {
            if (sensors[i] == sensor) {
                return readings[i];
            }
        }
        RAISE("Sensor not found.");
    }

    CPUMonitor* SensorManager::getMonitor() {
        return &monitor;
    }
}

#endif