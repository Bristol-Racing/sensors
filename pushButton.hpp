#ifndef PUSHBUTTON_H
#define PUSHBUTTON_H
#include <Arduino.h>
#include "sensor.hpp"

namespace Sensor {
    class PushButton : public Sensor{

    private:
        uint8_t buttonPin;     // Pin number where the button is connected
        bool buttonState;      // Current state of the button
        bool lastButtonState;  // Previous state of the button
        unsigned long lastDebounceTime;  // Last time the button state changed
        unsigned long debounceDelay;     // Debounce time delay

    public:
        PushButton(uint8_t pin, unsigned long debounceTime = 50);  // Constructor
        ~PushButton();  // Destructor
        void setup();
        void tick();   // Sets up the button pin
        double report();    // Reads the button state with debounce logic
    };

    PushButton::PushButton(uint8_t pin, unsigned long debounceTime) {
        buttonPin = pin;
        debounceDelay = debounceTime;
        buttonState = LOW;
        lastButtonState = LOW;
        lastDebounceTime = 0;
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
        bool reading = digitalRead(buttonPin); // read pit confirm HIGH/LOW state
        double output = reading ? 1.0 : 0.0; //change boolean to double
        return output;
    }
}

#endif
