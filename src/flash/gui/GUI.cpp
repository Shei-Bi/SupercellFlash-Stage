#include <flash/gui/GUI.h>

GUI* GUI::sm_pInstance = nullptr;
GUI* GUI::getInstance() {
    return sm_pInstance;
}
GUI::GUI() {

}
void GUI::constructInstance()
{
    if (!GUI::sm_pInstance)
        GUI::sm_pInstance = new GUI();
}