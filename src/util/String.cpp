#include "util/String.hpp"
#include <cstddef>
#include <cstring>

using namespace Conv3d;

String::String()
{
    this->size = 0;
    this->_data.bytes[0] = 0;
}

String::String(const String& s) : String(s.data()) {}

String::String(char* data) : String(data, strlen(data)) {}

String::String(String& s) : String(s.data()) {}

String::String(String* s) : String(*s) {}

String::String(char* data, int size)
{
    this->_data.ptr = NULL;
    this->size = 0;
    this->set(data, size);
}

void String::set(char* data, int size)
{
    if (strlen(data) < size) size = strlen(data);
    this->clear();
    this->size = size;
    if (size <= 7)
    {
        memcpy(this->_data.bytes, data, size);
        this->_data.bytes[size] = 0;
    }
    else
    {
        char* ptr = new char[size + 1];
        ptr[size] = 0;
        memcpy(ptr, data, size);
        this->_data.ptr = ptr;
    }
}

int String::length() { return this->size; }

char* String::data() const { return ((String*)this)->data(); }

char* String::data()
{
    if (this->size <= 7)
        return this->_data.bytes;
    return this->_data.ptr;
}

void String::clear()
{
    if (this->size > 7)
        delete this->_data.ptr;
    this->_data.ptr = NULL;
    this->size = 0;
}

String::~String() { this->clear(); }

bool String::equals(const char* other) { return this->equals((char*)other); }

bool String::equals(char* other)
{
    char* data = this->data();
    int size = this->length();
    if (size != strlen(other))
        return false;
    for (int i = 0; i < size; i++)
        if (data[i] != other[i])
            return false;
    return true;
}

bool String::equals(String* s) { return this->equals(s->data()); }

bool String::equals(String& s) { return this->equals(s.data()); }

void String::into(String& s)
{
    memcpy(s._data.bytes, this->_data.bytes, sizeof(this->_data));
    s.size = this->size;

    this->_data.ptr = NULL;
    this->size = 0; // prevent clear() from deleting pointer if exists.

    delete this;
}

bool String::operator==(const char* other) { return this->equals(other); }

bool String::operator!=(const char* other) { return !this->equals(other); }

String* String::operator=(const char* c)
{
    this->set((char*)c, strlen(c));
    return this;
}
