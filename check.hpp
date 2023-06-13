#ifndef CHECK_H
#define CHECK_H


class ErrorLog {
private:
    char ** messages;
public:
    ErrorLog();
    ~ErrorLog();

    void raise(char* message);
};

ErrorLog::ErrorLog() {

}

ErrorLog::~ErrorLog() {

}

void ErrorLog::raise(char* message) {

}

ErrorLog errorLog;

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