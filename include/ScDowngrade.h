#include <filesystem>
namespace fs = std::filesystem;
extern "C" int ScDowngrade(char* input, char* output, float version);