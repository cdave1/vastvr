#ifndef _VAST_CORE_LOG_H_
#define _VAST_CORE_LOG_H_

#include <string>
#include <functional>

namespace hd {
    class LogCallback {
    public:
        LogCallback () {}

        LogCallback (std::function<void(const std::string &log)> f) : m_callback(f) {}

        void execute(const std::string &log);

    private:
        std::function<void(const std::string &log)> m_callback;
        
    };
}

namespace hd {

    enum class LogLevel {
        ALL,
        INFO,
        INFO_AND_WARNINGS,
        INFO_AND_WARNINGS_AND_ERRORS,
        ERRORS_ONLY,
    };

    class log {
    public:
        static void SetLogCallback(std::function<void(const std::string &log)> callback);

        static void i(const char *fmt, ...);
        
        static void w(const char *fmt, ...);
        
        static void e(const char *fmt, ...);
        
    };
}

#endif