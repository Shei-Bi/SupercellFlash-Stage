#pragma once

class LogicVersion
{
public:
    static bool isChinaVersion() { return false; };
    static bool isContentValidationMode() {
#ifdef false
        return true;
#else 
        return false;
#endif
    };
};
