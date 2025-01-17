#pragma once
class LogicGameModeUtil {
public:
    static bool isGemGrab(int v) {
        return v == 0 || v == 46;
    }
    static bool isKingOfHill(int v) {
        return v == 17 || v == 51;
    }
    static bool isKnockOut(int v) {
        return v == 20 || v == 35 || v == 43 || v == 50;
    }
    static bool isDuoMode(int v) {
        return v == 9 || v == 38 || v == 44;
    }
    static bool isModeWith2Bars(int v) {
        return v == 19 || v == 21 || v == 26;
    }
    static bool isPayloadSingle(int v) {
        return v == 47 || v == 54;
    }
    static bool isCollectItemsWithTimer(int v) {
        return v == 11 || v == 27 || v == 41;
    }
};