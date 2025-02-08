#pragma once

class LogicVersion
{
public:
    static bool isChinaVersion() { return false; };
    static bool isContentValidationMode() {
#ifndef false
        return true;
#else 
        return false;
#endif
    };
};
