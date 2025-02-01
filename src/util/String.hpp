#ifndef STRING_H
#define STRING_H

namespace Conv3d
{

class String
{
  private:
    union
    {
        char *ptr;
        char bytes[8];
    } _data;
    int size;

  public:
    String();
    String(const String &);
    String(String &);
    String(String *);
    String(char *);
    String(char *, int);

    bool equals(const char *);
    bool equals(char *);
    bool equals(String *);
    bool equals(String &);

    bool operator==(const char *);
    bool operator!=(const char *);

    String *operator=(const char *);

    void set(char *data, int size);

    int length();
    char *data();
    char *data() const;

    void into(String &);

    void clear();
    ~String();
};

} // namespace Conv3d

#endif // STRING_h