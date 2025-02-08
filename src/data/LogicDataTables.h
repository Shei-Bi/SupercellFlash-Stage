#ifndef LOGICDATATABLES_H
#define LOGICDATATABLES_H
class CSVNode;
class LogicDataTable;
#include "LogicData.h"
class LogicGameModeVariationData;
class LogicLocationData;
class LogicTileData;
class LogicAccessoryData;
class LogicAreaEffectData;
class LogicSkillData;
class LogicProjectileData;
class LogicCharacterData;
class LogicSkinConfData;
class LogicSkinData;

extern LogicDataTable* TABLES[101];
enum DataType {
    texts_patch = 0,
    Locale = 1,
    BillingPackage = 2,
    Global = 3,
    Sound = 4,
    Resource = 5,
    Projectile = 6,
    Effect = 7,
    AllianceBadge = 8,
    ClientGlobal = 9,
    ParticleEmitter = 10,
    HealthBar = 11,
    Music = 12,
    Credit = 13,
    Region = 14,
    Location = 15,
    Character = 16,
    AreaEffect = 17,
    Item = 18,
    Map = 19,
    Skill = 20,
    Campaign = 21,
    Boss = 22,
    Card = 23,
    Animati0n = 24,
    AllianceRole = 25,
    Tutorial = 26,
    Tile = 27,
    PlayerThumbnail = 28,
    Skin = 29,
    Hint = 36,
    Milestone = 39,
    Theme = 41,
    NameColor = 43,
    SkinConf = 44,
    ShopItem = 45,
    ColorGradient = 46,
    LocationTheme = 47,
    GameModeVariation = 48,
    Challenge = 49,
    Accessory = 50,
    LocalNotification = 51,
    Emote = 52,
    EmoteBundle = 53,
    PlayerMapEnvironment = 54,
    MapTemplate = 55,
    SeasonalSkinSection = 56,
    RankedRank = 58,
    RankedLocation = 59,
    Carryab1e = 60,
    GearLevel = 61,
    GearBoost = 62,
    AllianceLeagueMode = 63,
    AllianceLeagueRank = 64,
    BpPurchasePopup = 65,
    LocationFeature = 66,
    LoginCalendarItem = 67,
    Spray = 68,
    ShopPanelLayout = 69,
    ShopStyleSet = 70,
    FameTier = 72,
    MasteryLevel = 73,
    MasteryPoint = 75,
    PlayerTitle = 76,
    BattleFeat = 78,
    RandomReward = 79,
    RandomRewardContainers = 80,
    PlayerFrame = 85,
    SkinRarities = 86,
    StatusEffect = 87,
    ClassArchetype = 90,
    night_market_bundles = 91,
    night_market_items = 92,
    availability_window = 100
};
class LogicDataTables {
    static inline bool sm_loaded = false;
    static inline LogicTileData* sm_pOpenTileData = nullptr;
public:
    static void initDataTable(CSVNode* csvNode, int index);
    static void createReferences();
    static bool isLoaded();
    static LogicLocationData* getLocationByName(const std::string& name);
    static LogicLocationData* getTrainingGroundsData();
    static LogicGameModeVariationData* getGameModeVariationByName(const std::string& name);
    static LogicGameModeVariationData* getGameModeVariationData(int v);
    static LogicAccessoryData* getAccessoryByName(const std::string& name);
    static LogicAreaEffectData* getAreaEffectByName(const std::string& name);
    static LogicSkillData* getSkillByName(const std::string& name);
    static LogicProjectileData* getProjectileByName(const std::string& name);
    static LogicCharacterData* getCharacterByName(const std::string& name);
    static LogicSkinConfData* getSkinConfByName(const std::string& name);
    static LogicSkinData* getSkinByName(const std::string& name);
    static LogicData* getDataById(int globalID);
    static LogicTileData* getOpenTileData();
};
#endif