#include "LogicSkillClient.h"
#include "LogicCharacterClient.h"

LogicSkillClient::LogicSkillClient(LogicSkillData* data) :data(data) {
    maxCharge = data->getMaxCharge() * 1000;
}

void LogicSkillClient::decode(BitStream* stream, bool isOwn, LogicCharacterClient* character, LogicBattleModeClient* battleClient) {
    activeTime = stream->readPositiveVIntMax255OftenZero() * 50;
    maxActiveTime = stream->readPositiveVIntMax255OftenZero() * 50;
    onActivate = stream->readBoolean();
    stream->readBoolean();
    cooldown = stream->readPositiveVIntMax255OftenZero() * 50;
    stream->readBoolean();
    if (data->getMaxCharge() >= 1) {
        charge = stream->readPositiveIntMax4095() * (data->isHoldToShoot() ? 20 : 1);
    }
    if (data->getSkillChangeType() != 0) abort();
    if (((LogicCharacterData*)character->getData())->getUniqueProperty() == 18)
        stream->readBoolean();
}
