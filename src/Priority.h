/**
 * Pulled from log4cpp
 */

#ifndef TLOGPLUS_PRIORITY_H
#define TLOGPLUS_PRIORITY_H

#include <string>

namespace TLogPlus {
    class Priority {
    public:
        static const int MESSAGE_SIZE; // = 8

        /**
         * Predefined Levels of Priorities. These correspond to the
         * priority levels used by syslog(3).
         **/
        typedef enum {
            FATAL  = 0,
            ALERT  = 100,
            CRIT   = 200,
            ERROR  = 300,
            WARN   = 400,
            NOTICE = 500,
            INFO   = 600,
            DEBUG  = 700,
            NOTSET = 800
        } PriorityLevel;

        /**
         * The type of Priority Values
         **/
        typedef int Value;

        /**
         * Returns the name of the given priority value.
         * Currently, if the value is not one of the PriorityLevel values,
         * the method returns the name of the largest priority smaller
         * the given value.
         * @param priority the numeric value of the priority.
         * @returns a string representing the name of the priority.
         **/
        static const std::string& getPriorityName(int priority);

        /**
         * Returns the value of the given priority name.
         * This can be either one of EMERG ... NOTSET or a
         * decimal string representation of the value, e.g. '700' for DEBUG.
         * @param priorityName the string containing the the of the priority
         * @return the value corresponding with the priority name
         * @throw std::invalid_argument if the priorityName does not
         * correspond with a known Priority name or a number
         **/
        static Value getPriorityValue(const std::string& priorityName);
    };
}

#endif //TLOGPLUS_PRIORITY_H
