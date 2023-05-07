#ifndef CHECK_H
#define CHECK_H

#define CHECK(cond, message)    \
    {                           \
        if (!(cond)) {          \
            RAISE(message);     \
        }                       \
    }

#define RAISE(message)                                                      \
    {                                                                       \
        char m[128];                                                        \
        sprintf(m, "Error at %s line %d: %s", __FILE__, __LINE__, message); \
        Serial.println(m);                                                  \
        while (true) {delay(1000);}                                         \
    }                                                                       \

#endif