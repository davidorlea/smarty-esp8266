#pragma once

#include <gtest/gtest.h>
#include <gmock/gmock.h>

class Print {
public:
    virtual size_t write(uint8_t) = 0;
    virtual size_t write(const char*) = 0;
    virtual size_t write(const uint8_t*, size_t) = 0;

    virtual size_t print(const char[]) = 0;
    virtual size_t print(char) = 0;
    virtual size_t print(unsigned char, int) = 0;
    virtual size_t print(int, int) = 0;
    virtual size_t print(unsigned int, int) = 0;
    virtual size_t print(long, int) = 0;
    virtual size_t print(unsigned long, int) = 0;
    virtual size_t print(double, int) = 0;

    virtual size_t println(const char[]) = 0;
    virtual size_t println(char) = 0;
    virtual size_t println(int, int) = 0;
    virtual size_t println(void) = 0;
};

class HardwareSerial : public Print {
    virtual uint8_t begin(uint16_t) = 0;
};

class HardwareSerialMock : public HardwareSerial {
public:
    MOCK_METHOD1(begin, uint8_t(uint16_t));

    MOCK_METHOD1(write, size_t(uint8_t));
    MOCK_METHOD1(write, size_t(const char*));
    MOCK_METHOD2(write, size_t(const uint8_t*, size_t));

    MOCK_METHOD1(print, size_t(const char[]));
    MOCK_METHOD1(print, size_t(char));
    MOCK_METHOD2(print, size_t(unsigned char, int));
    MOCK_METHOD2(print, size_t(int, int));
    MOCK_METHOD2(print, size_t(unsigned int, int));
    MOCK_METHOD2(print, size_t(long, int));
    MOCK_METHOD2(print, size_t(unsigned long, int));
    MOCK_METHOD2(print, size_t(double, int));

    MOCK_METHOD1(println, size_t(const char[]));
    MOCK_METHOD1(println, size_t(char));
    MOCK_METHOD2(println, size_t(int, int));
    MOCK_METHOD0(println, size_t(void));
};

extern HardwareSerialMock Serial;
