
//  A header guard prevents the file from being included twice
#ifndef CHECK_H
#define CHECK_H

//  The error log keeps track of errors produced by the sensors
class ErrorLog {
private:
    int max;    //  The max number of error messages that can be produced
    int messageCount;   //  The number of error messages produced
    char** messages;    //  The error messages
public:
    ErrorLog(int maxMessages = 8);
    ~ErrorLog();
    void raise(char* message);
    int getMessageCount();
    char* getMessage(int i);
};

ErrorLog::ErrorLog(int maxMessages = 8) {
    max = maxMessages;
    messages = (char**)malloc(max * sizeof(char*)); //  Initialises an array to store the messages
    messageCount = 0; //  No messages have been produced to begin with
}
ErrorLog::~ErrorLog() {
}

void ErrorLog::raise(char* message) {
    //  When an error message is produced
    int len = strlen(message); //  Initialise memory to store the message
    char* m = (char*)malloc((len + 1) * sizeof(char)); //  Initialise memory to store the message
    strcpy(m, message); //  Copy the message into memory
    messages[messageCount] = m; //  Store the message in the array of messages
    messageCount++; //  Add one to the message count
}

int ErrorLog::getMessageCount() {
    return messageCount;
}

char* ErrorLog::getMessage(int i) {
    return messages[i];
}

ErrorLog errorLog;

//  Shorthand for checking if a condition is met, and raising an error if it isn't
#define CHECK(cond, message)    \
    {                           \
        if (!(cond)) {          \
            RAISE(message);     \
        }                       \
    }

//  Shorthand for raising an error and passing it to the log
#define RAISE(message)                                                      \
    {                                                                       \
        char m[128];                                                        \
        sprintf(m, "Error at %s line %d: %s\0", __FILE__, __LINE__, message); \
        errorLog.raise(m);                                                  \
    }

//  Shorthand for reporting all the errors to an error handling function
//  and stopping the program
#define HANDLE_ERRS(handler)                                        \
    {                                                               \
        if (errorLog.getMessageCount() > 0) {                       \
            for (int i = 0; i < errorLog.getMessageCount(); i++) {  \
                char* message = errorLog.getMessage(i);             \
                handler(message);                                   \
            }                                                       \
            while (true) {delay(1000);}                             \
        }                                                           \
    }
#endif