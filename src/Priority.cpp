/**
 * Pulled from log4cpp
 */

#include "Priority.h"

namespace TLogPlus {
    namespace {
        const std::string *names() {
            static const std::string priority_names[10] = {
                    "FATAL",
                    "ALERT",
                    "CRIT",
                    "ERROR",
                    "WARN",
                    "NOTICE",
                    "INFO",
                    "DEBUG",
                    "NOTSET",
                    "UNKNOWN"
            };
            return priority_names;
        }
    }

    const int TLogPlus::Priority::MESSAGE_SIZE = 8;

    const std::string &Priority::getPriorityName(int priority) {

        priority++;
        priority /= 100;
        return names()[((priority < 0) || (priority > 8)) ? 8 : priority];
    }

    Priority::Value Priority::getPriorityValue(const std::string &priorityName) {
        Priority::Value value = -1;

        for (unsigned int i = 0; i < 10; i++) {
            if (priorityName == names()[i]) {
                value = i * 100;
                break;
            }
        }

        if (value == -1) {
            if (priorityName == "EMERG") {
                value = 0;
            } else {
                char *endPointer;
                value = std::strtoul(priorityName.c_str(), &endPointer, 10);
            }
        }

        return value;
    }
}
