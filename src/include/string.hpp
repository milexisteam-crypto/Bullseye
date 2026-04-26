#pragma once
#include <stdint.h>
#include <stddef.h>

class String {
public:
    String();
    String(const char* s);
    String(const String& other);
    String& operator=(const String& other);

    const char* c_str() const;
    size_t length() const;

    String operator+(const String& other) const;
    String operator+(const char* other) const;

private:
    char* data;
    size_t len;

    static size_t kstrlen(const char* s);
    static void kmemcpy(void* dst, const void* src, size_t n);
};

// Pozwala na: "a" + String("b")
String operator+(const char* a, const String& b);
