#include "scw/Wend.hpp"
#include "file/Writer.hpp"

using SCW::Wend;

void Wend::Encode(Conv3d::Writer *) {}

const char *Wend::Tag() { return "WEND"; }