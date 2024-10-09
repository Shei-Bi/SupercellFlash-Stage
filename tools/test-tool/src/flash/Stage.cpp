#include <flash/Stage.h>

Stage* Stage::getInstance() {
    return sm_pInstance;
}
void Stage::constructInstance()
{
    if (!Stage::sm_pInstance)
        Stage::sm_pInstance = new Stage();
}
Stage* Stage::sm_pInstance = nullptr;