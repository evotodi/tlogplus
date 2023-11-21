#ifndef _H_LOG_STREAM_PLUS
#define _H_LOG_STREAM_PLUS

#include <TLogPlus.h>
#include <Stream.h>

class LogStream : public TLog {
  public:
    LogStream();
    LogStream(Stream &s) : _s(&s) {};

    void begin() {};
    void begin(Stream &s) { _s = &s; };

    void loop() {};
    void stop() {};

  private:
    Stream * _s;
  protected:
};
#endif
