#ifndef BITCAST_H
#define BITCAST_H

#include <cstring>
namespace Util
{

template <typename T, typename E> T bit_cast(E e)
{
    T ret;
    std::memcpy(&ret, &e, sizeof(T));
    return ret;
};

} // namespace Util

#endif // BITCAST_H