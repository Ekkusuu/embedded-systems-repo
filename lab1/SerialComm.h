#ifndef SERIAL_COMM_H
#define SERIAL_COMM_H

#include <stdint.h>
#include <stdio.h>

#define CMD_BUFFER_SIZE 32

class SerialComm {
  private:
    char    _buffer[CMD_BUFFER_SIZE];
    uint8_t _index;

  public:
    SerialComm();
    void        begin(unsigned long baudRate);
    bool        readLine();
    const char* getCommand() const;
};

#endif // SERIAL_COMM_H