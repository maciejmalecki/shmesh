#ifndef Msg_h
#define Msg_h

#include "Arduino.h"

/*
 * Buffer object that can be used for assembling messages.
 */
class Msg {
  public:
    /*
     * Constructs new message buffer with given maximal length (in bytes).
     */
    Msg(int _maxLength);
    /*
     * Destroys buffer objects and releases all memory.
     */
    ~Msg();
    /*
     * Returns current length of message buffer.
     */
    int length();
    /*
     * Appends char array to the buffer.
     */
    void append(char* data);
    /*
     * Appends number to the buffer.
     */
    void append(int data);
    /*
     * Appends float value to the buffer.
     */
    void append(float data, int width, int precision);
    /*
     * Clears message buffer. It can be now reused.
     */
    void clear();
    /*
     * Returns pointer to the buffer's content.
     */
    void* content();
  private:
    int maxLength;
    int ptr;
    void* buffer;
    byte i2str(int i, char *buf);
};

#endif

