#include "TLogPlus.h"

namespace TLogPlus {
    TLog Log, Debug;

    /**
     * LOGBase
     */
    LOGBase::~LOGBase() {
        flush();
    }

    String LOGBase::identifier() {
        return _identifier;
    }

    void LOGBase::setIdentifier(String identifier) {
        _identifier = identifier;
    };

    void LOGBase::setLevel(Priority::Value level) {
        _level = level;
        if (level > Priority::NOTSET) {
            level = Priority::NOTSET;
        }
    };

    Priority::Value LOGBase::getLevel() {
        return _level;
    };

    const char *LOGBase::getNewLine() {
        return _newline;
    }

    void LOGBase::setNewline(const char * nl) {
        _newline = nl;
    };

    void LOGBase::showLevel(bool showLevel) {
        _showLevel = showLevel;
    }

    bool LOGBase::getShowLevel() {
        return _showLevel;
    }

    void LOGBase::flush() {
        if (_buffer) {
            printLevel(_streamLevel, true, _buffer->str().c_str());
            delete _buffer;
            _buffer = nullptr;
        }
        _streamLevel = _defaultLevel;
    }

    template<typename T>
    void LOGBase::_insertStream(T t) {
        if (getLevel() != Priority::NOTSET) {
            if (!_buffer) {
                if(!(_buffer = new std::stringstream)) {
                    // XXX help help help
                }
            }
            (*_buffer) << t;
        }
    }

    LOGBase &LOGBase::operator<<(cspf pf) {
        return (*pf)(*this);
    }

    LOGBase &endl(LOGBase &os) {
        if (os._buffer) {
            os.flush();
        }

        return os;
    }

    LOGBase &LOGBase::operator<<(Priority::PriorityLevel priority) {
        this->_streamLevel = priority;
        return *this;
    }

    LOGBase &LOGBase::operator<<(const char *t) {
        _insertStream(t);
        return *this;
    }

    LOGBase &LOGBase::operator<<(const std::string &t) {
        _insertStream(t);
        return *this;
    }

    LOGBase &LOGBase::operator<<(bool t) {
        _insertStream(t);
        return *this;
    }

    LOGBase &LOGBase::operator<<(long t) {
        _insertStream(t);
        return *this;
    }

    LOGBase &LOGBase::operator<<(unsigned long t) {
        _insertStream(t);
        return *this;
    }

    LOGBase &LOGBase::operator<<(double t) {
        _insertStream(t);
        return *this;
    }

    LOGBase &LOGBase::operator<<(short t) {
        _insertStream(t);
        return *this;
    }

    LOGBase &LOGBase::operator<<(int t) {
        _insertStream(t);
        return *this;
    }

    LOGBase &LOGBase::operator<<(unsigned short t) {
        _insertStream(t);
        return *this;
    }

    LOGBase &LOGBase::operator<<(unsigned int t) {
        _insertStream(t);
        return *this;
    }

    LOGBase &LOGBase::operator<<(float t) {
        _insertStream(t);
        return *this;
    }

    LOGBase &LOGBase::operator<<(const void *t) {
        _insertStream(t);
        return *this;
    }

    /**
     * TLog
     */

    void TLog::disableSerial(bool onOff) { _disableSerial = onOff; };

    void TLog::disableStreams(bool onOff) {
        for (auto it = handlers.begin(); it != handlers.end(); ++it) {
            (*it)->_disableStream(onOff);
        }
    };

    void TLog::addPrintStream(const std::shared_ptr<LOGBase> &_handler) {
        auto it = find(handlers.begin(), handlers.end(), _handler);
        if (handlers.end() == it)
            // we're not using push_back; that copies; but use a reference.
            // As it can see reuse.
            handlers.emplace_back(_handler);
    };

    void TLog::begin() {
        for (auto it = handlers.begin(); it != handlers.end(); ++it) {
            (*it)->begin();
        }
    }

    void TLog::loop() {
        for (auto it = handlers.begin(); it != handlers.end(); ++it) {
            (*it)->loop();
        }
    };

    void TLog::stop() {
        for (auto it = handlers.begin(); it != handlers.end(); ++it) {
            (*it)->stop();
        }
    };

    size_t TLog::write(byte a) {
        for (auto it = handlers.begin(); it != handlers.end(); ++it) {
            (*it)->write(a);
        }
        if (_disableSerial)
            return 1;
        return Serial.write(a);
    }

}
