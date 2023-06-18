#ifndef CHECK_H
#define CHECK_H


class ErrorLog {
private:
    int max;
    int messageCount;
    char** messages;
public:
    ErrorLog(int maxMessages = 8);
    ~ErrorLog();

    void raise(char* message);
    int getMessageCount();
    char* getMessage(int i);
};

ErrorLog::ErrorLog(int maxMessages = 8) {
    max = maxMessages;
    messages = (char**)malloc(max * sizeof(char*));

    messageCount = 0;
}

ErrorLog::~ErrorLog() {

}

void ErrorLog::raise(char* message) {
    int len = strlen(message);
    char* m = (char*)malloc((len + 1) * sizeof(char));
    strcpy(m, message);

    messages[messageCount] = m;

    messageCount++;
}

int ErrorLog::getMessageCount() {
    return messageCount;
}

char* ErrorLog::getMessage(int i) {
    return messages[i];
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
        sprintf(m, "Error at %s line %d: %s\0", __FILE__, __LINE__, message); \
        errorLog.raise(m);                                                  \
    }

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