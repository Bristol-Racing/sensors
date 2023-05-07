
#ifndef SENSOR_MANAGER_H
#define SENSOR_MANAGER_H

#include "sensor.hpp"

namespace Sensor {
    typedef void (* ReadCallback)(double*); 

    class SensorManager {
    private:
        int sensorCount;
        int maxSensorCount;

        Sensor** sensors;
        int* nextTicks;
        int nextRead;
        double* readings;

        int readRate;

        unsigned long prevTime;

        int spinTime;

        ReadCallback readCallback;

        void updateTimes();
        void processTicks();
        void processReads();

    public:
        SensorManager(int maxSensors, int rate = -1);
        ~SensorManager();

        void setReadCallback(ReadCallback callback);

        void addSensor(Sensor* sensor);
        void spin(int maxTime = -1);
        int timeToNextTick();
        int timeToNextRead();
    };
    
    SensorManager::SensorManager(int maxSensors, int rate = -1) {
        sensorCount = 0;
        maxSensorCount = maxSensors;

        sensors = (Sensor**)malloc(sizeof(Sensor*) * maxSensors);
        nextTicks = (int*)malloc(sizeof(int) * maxSensors);
        readings = (double*)malloc(sizeof(double) * maxSensors);

        readRate = rate;
        nextRead = readRate;

        prevTime = millis();

        spinTime = 0;

        readCallback = NULL;
    }
    
    SensorManager::~SensorManager() {
        free(sensors);
        free(nextTicks);
    }

    void SensorManager::updateTimes() {
        unsigned long time = millis();
        int timeDelta = time - prevTime;

        prevTime = time;

        for (int i = 0; i < sensorCount; i++) {
            nextTicks[i] -= timeDelta;
        }

        nextRead -= timeDelta;

        spinTime -= timeDelta;
    }

    void SensorManager::processTicks() {
        for (int i = 0; i < sensorCount; i++) {
            while (nextTicks[i] <= 0) {
                sensors[i]->tick();
                nextTicks[i] += sensors[i]->getTickRate();
            }
        }
    }

    void SensorManager::processReads() {
        while (nextRead <= 0) {
            for (int i = 0; i < sensorCount; i++) {
                readings[i] = sensors[i]->read();
            }
            nextRead += readRate;

            readCallback(readings);
        }
    }
    
    void SensorManager::setReadCallback(ReadCallback callback) {
        CHECK(readCallback == NULL, "Read callback already set")

        readCallback = callback;
    }

    void SensorManager::addSensor(Sensor* sensor) {
        if (readRate == -1) {
            readRate = sensor->getReadRate();
            nextRead = readRate;
        }
        else {
            if (sensor->getReadRate() >= 1) {
                CHECK(sensor->getReadRate() == readRate, "Sensor read rate doens't match manager read rate.")
            }
            else {
                sensor->setReadRate(readRate);
            }
        }

        sensors[sensorCount] = sensor;
        nextTicks[sensorCount] = sensor->getTickRate();

        sensorCount++;
    }

    void SensorManager::spin(int maxTime = -1) {
        updateTimes();
        spinTime = maxTime;

        while (spinTime > 0 || maxTime == -1) {
            int minTime = timeToNextTick();
            if (nextRead < minTime) {
                minTime = nextRead;
            }
            if (maxTime >= 0 && spinTime < minTime) {
                minTime = spinTime;
            }

            if (minTime >= 1) {
                delay(minTime);
                updateTimes();
            }

            processTicks();
            processReads();
        }
        
    }

    int SensorManager::timeToNextTick() {
        if (sensorCount == 0) return 1000;

        int minTime = nextTicks[0];

        for (int i = 1; i < sensorCount; i++) {
            if (sensors[i]->getTickRate() > 0) {
                if (nextTicks[i] < minTime) {
                    minTime = nextTicks[i];
                }
            }
        }

        return minTime;
    }

    int SensorManager::timeToNextRead() {
        return nextRead;
    }
}

#endif