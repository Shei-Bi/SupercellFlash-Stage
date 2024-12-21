#pragma once

class GUI {
public:
    GUI();
    static GUI* sm_pInstance;
    static GUI* getInstance();
    static void constructInstance();
};