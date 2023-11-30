#ifndef _H_PLUS_LOG_TEE
#define _H_PLUS_LOG_TEE

#include <Arduino.h>
#include <Print.h>
#include <stddef.h>
#include <memory>
#include <vector>
#include <functional>
#include "Priority.h"

#include <sstream>

#if defined(ESP32)

#  include <WiFi.h>

#elif defined(ESP8266)
#  include <ESP8266WiFi.h>
#  include <WiFiUdp.h>
#else
#  error "Must be ESP32 or ESP8266"
#endif

#if defined(ESP32) || defined(ESP8266)
#  define IDENTIFIER_GENERATOR (WiFi.macAddress())
#endif

#ifndef IDENTIFIER_GENERATOR
#  define IDENTIFIER_GENERATOR "unknown"
#endif

namespace TLogPlus {
    class LOGBase;

    LOGBase &endl(LOGBase &os);

    class LOGBase : public Print {
    public:
        LOGBase(String identifier = IDENTIFIER_GENERATOR) : _identifier(identifier) {};

        ~LOGBase();

        String identifier();

        void setIdentifier(String identifier);

        virtual void begin() { return; };

        virtual void reconnect() { return; };

        virtual void loop() { return; };

        virtual void stop() { return; };

        virtual void _disableStream(bool onOff) { return; };

        void setLevel(Priority::Value level);

        Priority::Value getLevel();

        void setNewline(const char *nl);

        const char *getNewLine();

        void showLevel(bool showLevel);

        bool getShowLevel();

        friend LOGBase &endl(LOGBase &os);

        LOGBase &operator<<(Priority::PriorityLevel priority);

        typedef LOGBase &(*cspf)(LOGBase &);

        LOGBase &operator<<(cspf);

        LOGBase &operator<<(const std::string &t);

        LOGBase &operator<<(const char *t);

        LOGBase &operator<<(bool t);

        LOGBase &operator<<(long t);

        LOGBase &operator<<(unsigned long t);

        LOGBase &operator<<(const double t);

        LOGBase &operator<<(short t);

        LOGBase &operator<<(int t);

        LOGBase &operator<<(unsigned short t);

        LOGBase &operator<<(unsigned int t);

        LOGBase &operator<<(float t);

        LOGBase &operator<<(const void *t);

    protected:
        String _identifier;
        const Priority::Value _defaultLevel = Priority::NOTSET;
        Priority::Value _level = _defaultLevel;
        bool _showLevel = false;
        Priority::Value _streamLevel = _defaultLevel;
        std::stringstream *_buffer = nullptr;
        const char *_newline = "\n";

        template<class T>
        void _printLevel(Priority::Value level, bool cr, T msg, ...) {
            if (level > _level && level != Priority::NOTSET) {
                return;
            }

            if (level > Priority::NOTSET) {
                level = Priority::NOTSET;
            }

            if (_showLevel && level != Priority::NOTSET) {
                print(Priority::getPriorityName(level).c_str());
                print(": ");
            }

            print(msg);

            if (cr) {
                print(_newline);
            }
        }

    private:
        template<typename T>
        void _insertStream(T t);

        void _flush();
    };

    class TLog : public LOGBase {
    public:
        void disableSerial(bool onOff);

        void disableStreams(bool onOff);

        void addPrintStream(const std::shared_ptr<LOGBase> &_handler);

        virtual void begin();

        virtual void loop();

        virtual void stop();

        size_t write(byte a);

        /**
         * Output a fatal error message. Output message contains
         * FATAL: followed by original message
         * Fatal error messages are printed out at
         * loglevels <= Priority::FATAL
         *
         * \param msg format string to output
         * \param ... any number of variables
         * \return void
         */
        template<class T, typename... Args>
        void fatal(T msg, Args... args) {
            _printLevel(Priority::FATAL, false, msg, args...);
        }

        /**
         * Output same as fatal with a carriage return ending
         */
        template<class T, typename... Args>
        void fatalln(T msg, Args... args) {
            _printLevel(Priority::FATAL, true, msg, args...);
        }

        /**
         * Output an alert message. Output message contains
         * ALERT: followed by original message
         * Alert messages are printed out at
         * loglevels <= Priority::ALERT
         *
         * \param msg format string to output
         * \param ... any number of variables
         * \return void
         */
        template<class T, typename... Args>
        void alert(T msg, Args... args) {
            _printLevel(Priority::ALERT, false, msg, args...);
        }

        /**
         * Output same as alert with a carriage return ending
         */
        template<class T, typename... Args>
        void alertln(T msg, Args... args) {
            _printLevel(Priority::ALERT, true, msg, args...);
        }

        /**
         * Output a critical message. Output message contains
         * CRIT: followed by original message
         * Critical messages are printed out at
         * loglevels <= Priority::CRIT
         *
         * \param msg format string to output
         * \param ... any number of variables
         * \return void
         */
        template<class T, typename... Args>
        void critical(T msg, Args...args) {
            _printLevel(Priority::CRIT, false, msg, args...);
        }

        /**
         * Output same as critical with a carriage return ending
         */
        template<class T, typename... Args>
        void criticalln(T msg, Args...args) {
            _printLevel(Priority::CRIT, true, msg, args...);
        }

        /**
         * Output a error message. Output message contains
         * ERROR: followed by original message
         * Error messages are printed out at
         * loglevels <= Priority::ERROR
         *
         * \param msg format string to output
         * \param ... any number of variables
         * \return void
         */
        template<class T, typename... Args>
        void error(T msg, Args...args) {
            _printLevel(Priority::ERROR, false, msg, args...);
        }

        /**
         * Output same as error with a carriage return ending
         */
        template<class T, typename... Args>
        void errorln(T msg, Args...args) {
            _printLevel(Priority::ERROR, true, msg, args...);
        }

        /**
         * Output a warning message. Output message contains
         * WARN: followed by original message
         * Warning messages are printed out at
         * loglevels <= Priority::WARN
         *
         * \param msg format string to output
         * \param ... any number of variables
         * \return void
         */
        template<class T, typename... Args>
        void warning(T msg, Args...args) {
            _printLevel(Priority::WARN, false, msg, args...);
        }

        /**
         * Output same as warning with a carriage return ending
         */
        template<class T, typename... Args>
        void warningln(T msg, Args...args) {
            _printLevel(Priority::WARN, true, msg, args...);
        }

        /**
         * Output a notice message. Output message contains
         * NOTICE: followed by original message
         * Notice messages are printed out at
         * loglevels <= Priority::NOTICE
         *
         * \param msg format string to output
         * \param ... any number of variables
         * \return void
        */
        template<class T, typename... Args>
        void notice(T msg, Args... args) {
            _printLevel(Priority::NOTICE, false, msg, args...);
        }

        /**
         * Output same as notice with a carriage return ending
         */
        template<class T, typename... Args>
        void noticeln(T msg, Args... args) {
            _printLevel(Priority::NOTICE, true, msg, args...);
        }

        /**
         * Output a info message. Output message contains
         * INFO: followed by original message
         * Info messages are printed out at
         * loglevels <= Priority::INFO
         *
         * \param msg format string to output
         * \param ... any number of variables
         * \return void
         */
        template<class T, typename... Args>
        void info(T msg, Args... args) {
            _printLevel(Priority::INFO, false, msg, args...);
        }

        /**
         * Output same as info with a carriage return ending
         */
        template<class T, typename... Args>
        void infoln(T msg, Args... args) {
            _printLevel(Priority::INFO, true, msg, args...);
        }

        /**
         * Output a debug message. Output message contains
         * DEBUG: followed by original message
         * Debug messages are printed out at
         * loglevels <= Priority::DEBUG
         *
         * \param msg format string to output
         * \param ... any number of variables
         * \return void
         */
        template<class T, typename... Args>
        void debug(T msg, Args... args) {
            _printLevel(Priority::DEBUG, false, msg, args...);
        };

        /**
         * Output same as debug with a carriage return ending
         */
        template<class T, typename... Args>
        void debugln(T msg, Args... args) {
            _printLevel(Priority::DEBUG, true, msg, args...);
        }

    private:
        std::vector<std::shared_ptr<LOGBase>> handlers;
        bool _disableSerial = false;
        bool _disableStreams = false;
    };

    extern TLog Log;
    extern TLog Debug;
}
#endif
