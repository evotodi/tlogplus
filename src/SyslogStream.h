
#ifndef _H_SYSLOG_STREAM_PLUS
#define _H_SYSLOG_STREAM_PLUS

#include <TLogPlus.h>
#include <Print.h>

#  if defined(ESP32)
#    include <WiFiUdp.h>
#  elif defined(ESP8266)
#    include <WiFiUdp.h>
#  else
#    error "Must be ESP32 or ESP8266"
#  endif
namespace TLogPlusStream {
    class SyslogStream : public TLogPlus::TLog {
    public:
        SyslogStream(const uint16_t syslogPort = 514) : _syslogPort(syslogPort) {};

        void setPort(uint16_t port) { _syslogPort = port; }

        void setDestination(const char *dest) { _dest = dest; }

        void setRaw(bool raw) { _raw = raw; }

        virtual size_t write(uint8_t c);

        virtual void begin() { _logging = true; }

        virtual void end() { _logging = false; }

    private:
        const char *_dest;
        uint16_t _syslogPort;
        char logbuff[512]; // 1024 seems to be to0 large for some syslogd's.
        size_t at = 0;
        bool _raw;
        bool _logging = false;
    protected:
    };
}
#endif