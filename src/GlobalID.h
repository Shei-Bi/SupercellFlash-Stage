#pragma once
class GlobalID {
public:
    static int createGlobalID(int classID, int instanceID) {
        return instanceID % 1000000 + 1000000 * classID;
    }
    static int getInstanceID(int globalID) {
        return globalID % 1000000;
    }
    static int getClassID(int globalID) {
        return globalID / 1000000;
    }
};