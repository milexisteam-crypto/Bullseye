#include "string.hpp"

// Podłącz swój allocator:
extern "C" void* kmalloc(size_t size);
// Jeśli jeszcze nie masz: tymczasowo możesz zrobić w userlandzie:
// #include <new>
// #define kmalloc(size) ::operator new(size)

size_t String::kstrlen(const char* s) {
    size_t n = 0;
    if (!s) return 0;
    while (s[n] != '\0') n++;
    return n;
}

void String::kmemcpy(void* dst, const void* src, size_t n) {
    auto* d = (uint8_t*)dst;
    auto* s = (const uint8_t*)src;
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
}

String::String() : data(nullptr), len(0) {
    data = (char*)kmalloc(1);
    data[0] = '\0';
}

String::String(const char* s) {
    len = kstrlen(s);
    data = (char*)kmalloc(len + 1);
    if (len > 0) kmemcpy(data, s, len);
    data[len] = '\0';
}

String::String(const String& other) {
    len = other.len;
    data = (char*)kmalloc(len + 1);
    if (len > 0) kmemcpy(data, other.data, len);
    data[len] = '\0';
}

String& String::operator=(const String& other) {
    // Bez free: po prostu alokujemy nowy bufor
    len = other.len;
    data = (char*)kmalloc(len + 1);
    if (len > 0) kmemcpy(data, other.data, len);
    data[len] = '\0';
    return *this;
}

const char* String::c_str() const {
    return data;
}

size_t String::length() const {
    return len;
}

String String::operator+(const String& other) const {
    String out;
    out.len = len + other.len;
    out.data = (char*)kmalloc(out.len + 1);

    if (len > 0) kmemcpy(out.data, data, len);
    if (other.len > 0) kmemcpy(out.data + len, other.data, other.len);

    out.data[out.len] = '\0';
    return out;
}

String String::operator+(const char* other) const {
    size_t olen = kstrlen(other);
    String out;
    out.len = len + olen;
    out.data = (char*)kmalloc(out.len + 1);

    if (len > 0) kmemcpy(out.data, data, len);
    if (olen > 0) kmemcpy(out.data + len, other, olen);

    out.data[out.len] = '\0';
    return out;
}

String operator+(const char* a, const String& b) {
    String sa(a);
    return sa + b;
}
