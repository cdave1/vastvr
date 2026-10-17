#include "Log.h"

std::function<void(const std::string &log)> logCallback;

void hd::log::SetLogCallback(std::function<void(const std::string &log)> callback) {
    logCallback = callback;
}


void hd::log::i(const char *fmt, ...) {
    static char output[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(output, sizeof(output), fmt, args);
    va_end(args);
    vinfo("%s", output);

    output[sizeof(output) - 1] = '\0';

    if (logCallback) {
        logCallback(std::string(output));
    }

#ifdef WIN32
	OutputDebugStringA(output);
#endif
}


void hd::log::w(const char *fmt, ...) {
    static char output[1024];
    va_list args;
    va_start(args, fmt);
    vsnprintf(output, sizeof(output), fmt, args);
    va_end(args);
    vwarn("%s", output);
    output[sizeof(output) - 1] = '\0';

    if (logCallback) {
        logCallback(std::string(output));
    }

#ifdef WIN32
	OutputDebugStringA(output);
#endif
}


void hd::log::e(const char *fmt, ...) {
    static char output[1024];

    va_list args;
    va_start(args, fmt);
    vsnprintf(output, sizeof(output), fmt, args);
    va_end(args);
    verr("%s", output);
    output[sizeof(output) - 1] = '\0';

    if (logCallback) {
        logCallback(std::string(output));
    }

#ifdef WIN32
	OutputDebugStringA(output);
#endif
}
