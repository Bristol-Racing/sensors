// Code that registers a push button on the driving steering wheel, for diagnostics

#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H
//#include <Arduino.h> check if this line is required---------------
#include "sensor.hpp"

namespace Sensor {
    class PushButton : public Sensor{
    private:
        uint8_t buttonPin;     // Pin number where the button is connected
    public:
        PushButton(uint8_t pin);  // Constructor
        ~PushButton();      // Destructor
        void setup();
        void tick();        // Sets up the button pin
        double report();    // Reads the button state with debounce logic
    };

    PushButton::PushButton(uint8_t pin) {
        buttonPin = pin;
    }

    PushButton::~PushButton() {
        // Nothing to do when the object is destroyed
    }

    void PushButton::setup() {
        pinMode(buttonPin, INPUT);
    }

    void PushButton::tick() {
        // No averaging needed for push button
    }

    double PushButton::report() {
        bool reading = digitalRead(buttonPin);  // read pit confirm HIGH/LOW state
        double output = reading ? 0.0 : 1.0;    // Converts to double and flips polarity of reading
        return output;
    }
}

#endif
