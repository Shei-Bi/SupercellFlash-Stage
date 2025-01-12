#pragma once
class LogicGameModeUtil {
public:
    static bool isGemGrab(int v) {
        return v == 0 || v == 46;
    }
    static bool isKingOfHill(int v) {
        return v == 17 || v == 51;
    }
};