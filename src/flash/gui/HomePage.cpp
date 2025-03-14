#include "HomePage.h"
#include "HomeScreen.hpp"
#include "HomePageTeamMember.hpp"
#include "HeroPopup.hpp"

HomePage::HomePage() : DropGUIContainer("sc/ui.sc", "screen_area") {
    player1 = nullptr;
    player1_button = nullptr;

    getMovieClip()->initScreenContainers("mainscreen_", screenContainers);
    // return;
    MovieClip* header_bgr = getClipFromContainers("header_bgr");
    if (header_bgr) {
        float scaled = Stage::getInstance()->matrixX / header_bgr->getWidth();
        if (scaled > 1.0)
            header_bgr->setScaleX(header_bgr->getScaleX() * scaled);
    }
    screenContainers[1]->gotoAndStop("3_players");
    panel_own_invite_2 = new PendingTeamItem(screenContainers[1]->getMovieClipRecursive("panel_own_invite_2"));
    screenContainers[1]->changeTimelineChild("panel_own_invite_2", panel_own_invite_2);
    panel_other_invite_2 = new PendingTeamItem(screenContainers[1]->getMovieClipRecursive("panel_other_invite_2"));
    screenContainers[1]->changeTimelineChild("panel_other_invite_2", panel_other_invite_2);
    panel_own_invite_4 = new PendingTeamItem(screenContainers[1]->getMovieClipRecursive("panel_own_invite_4"));
    screenContainers[1]->changeTimelineChild("panel_own_invite_4", panel_own_invite_4);
    panel_other_invite_4 = new PendingTeamItem(screenContainers[1]->getMovieClipRecursive("panel_other_invite_4"));
    screenContainers[1]->changeTimelineChild("panel_other_invite_4", panel_other_invite_4);
    panel_own_invite_3 = new PendingTeamItem(screenContainers[1]->getMovieClipRecursive("panel_own_invite_3"));
    screenContainers[1]->changeTimelineChild("panel_own_invite_3", panel_own_invite_3);
    panel_other_invite_3 = new PendingTeamItem(screenContainers[1]->getMovieClipRecursive("panel_other_invite_3"));
    screenContainers[1]->changeTimelineChild("panel_other_invite_3", panel_other_invite_3);
    panel_own_invite_5 = new PendingTeamItem(screenContainers[1]->getMovieClipRecursive("panel_own_invite_5"));
    screenContainers[1]->changeTimelineChild("panel_own_invite_5", panel_own_invite_5);
    panel_other_invite_5 = new PendingTeamItem(screenContainers[1]->getMovieClipRecursive("panel_other_invite_5"));
    screenContainers[1]->changeTimelineChild("panel_other_invite_5", panel_other_invite_5);
    panel_own_invite_2->visible = false;
    panel_own_invite_4->visible = false;
    panel_own_invite_3->visible = false;
    panel_own_invite_5->visible = false;
    panel_other_invite_2->visible = false;
    panel_other_invite_4->visible = false;
    panel_other_invite_3->visible = false;
    panel_other_invite_5->visible = false;

    createButtons();
    getButtonByName("button_navi_login_calendar")->visible = false;
    // screenContainers[5]->moveThisToTopLayer();
    // screenContainers[9]->moveThisToTopLayer();
    // auto player_1_area = getClipFromContainers("player_1_area");
    // player_1_area->stop();
    // player_1_area->setChildVisible("bubble", false);
    // player_1_area->setChildVisible("player_status", false);
    // player_1_area->setChildVisible("star_power_ph", false);
    // player_1_area->setChildVisible("item_ph", false);
    button_play_club_league = getButtonByName("button_play_club_league");
    // getClipFromContainers("player_1_area")->visible = false;
    player1_button = addPlayerButton(getClipFromContainers("player_1_area"), "player_1");

    getClipFromContainers("player_2_area")->visible = false;
    getClipFromContainers("player_3_area")->visible = false;
    getClipFromContainers("player_4_area")->visible = false;
    getClipFromContainers("player_5_area")->visible = false;
    brawl_container = screenContainers[4]->getMovieClipByName("brawl_container");
    // brawl_container->visible = false;

    getButtonByName("button_random_reward")->visible = false;


    screenContainers[6]->getMovieClipByName("hamburger_menu")->stop();


    screenContainers[1]->setChildVisible("panel_player_online", false);
    screenContainers[1]->setChildVisible("panel_player_online_ph_left", false);
    screenContainers[1]->setChildVisible("panel_player_online_ph_right", false);
    screenContainers[1]->setChildVisible("panel_player_online_ph_left_left", false);
    screenContainers[1]->setChildVisible("panel_player_online_right_right", false);

    refreshSelectedEvent();
    refreshSelectedCharacters(true);

    //not implenented
    getButtonByName("button_navi_login_calendar_1")->visible = false;
    getButtonByName("button_navi_login_calendar_2")->visible = false;
    getButtonByName("button_navi_login_calendar_3")->visible = false;
    getButtonByName("button_navi_team")->visible = false;
    getButtonByName("button_navi_friends")->visible = false;
    getButtonByName("button_navi_shop")->visible = false;
    getButtonByName("button_navi_esports")->visible = false;
    getButtonByName("button_navi_clan")->visible = false;
    getButtonByName("button_news")->visible = false;
    getButtonByName("button_cctv")->visible = false;
    getButtonByName("button_pending_donation")->visible = false;
    getButtonByName("button_competitive")->visible = false;
    getButtonByName("button_brawl_pass")->visible = false;
    getButtonByName("button_navi_collab")->visible = false;
    getButtonByName("button_recruit_road")->visible = false;
    getButtonByName("button_quests")->visible = false;
    getButtonByName("button_winstreak")->visible = false;
    getClipFromContainers("important_notice")->visible = false;
    getClipFromContainers("ranked_diamond_format_tooltip")->visible = false;
    getClipFromContainers("team_info")->visible = false;
    // getClipFromContainers("collab_play_reward_icon")->visible = false;
    getButtonByName("collab_play_reward_button")->visible = false;
    getClipFromContainers("locked_rank_warning_container")->visible = false;
}

HomePage::~HomePage() {
    deleteAllPages();
    if (panel_own_invite_2) delete panel_own_invite_2;
    if (panel_other_invite_3) delete panel_other_invite_3;
    if (panel_own_invite_4) delete panel_own_invite_4;
    if (panel_other_invite_5) delete panel_other_invite_5;
    if (panel_own_invite_3) delete panel_own_invite_3;
    if (panel_own_invite_5) delete panel_own_invite_5;
    if (panel_other_invite_2) delete panel_other_invite_2;
    if (panel_other_invite_4) delete panel_other_invite_4;
    if (gamemode_icon) delete gamemode_icon;
    if (player1) delete player1;
    if (player1_button) delete player1_button;
    for (MovieClip* mc : screenContainers) {
        if (!mc) continue;
        delete mc;
        mc = nullptr;
    }
}

MovieClip* HomePage::getClipFromContainers(const char* name) {
    for (MovieClip* mc : screenContainers) {
        if (!mc) continue;
        MovieClip* result = mc->getMovieClipByName(name);
        if (result) return result;
    }
    return nullptr;
}

void HomePage::onInitialized() {
    displayEventList();
}

void HomePage::displayEventList() {

}

void HomePage::deleteAllPages() {
    ;
}

void HomePage::refreshSelectedCharacters(bool initialize) {
    if (player1) {

    }
    else {
        player1 = new HomePageTeamMember(this, getClipFromContainers("player_1_area"));
        player1_button->setHeroSprite(player1->sceneRenderer);
    }
}

void HomePage::refreshSelectedEvent() {
    selectedEvent = nullptr;
    int availableslot = 0;
    // selectedEvent = GameStateManager::getInstance()->home->getActiveEventForSlot(availableslot++);
    selectedEvent = GameStateManager::getInstance()->home->getFirstEventOfGMV(20);
    auto button_mode_clip = getButtonByName("button_mode")->timelineClip;
    // button_mode_clip->debugPrintChildNames();
    // button_mode_clip->getMovieClipByName("gamemode")->setAlpha(0.5f);
    auto info = button_mode_clip->getMovieClipByName("info");
    // info->setAlpha(0.1f);

    brawl_container->getMovieClipByName("party_mode_container")->visible = false;
    getClipFromContainers("raid_boss_container")->visible = false;

    button_play_club_league->visible = false;
    // brawl_container->visible = false;//?????????//

    getButtonByName("button_spectate")->visible = false;

    // button_mode_clip->setChildVisible("notification", false);

    button_mode_clip->gotoAndStop("idle");
    auto event_container = getClipFromContainers("event_container");

    event_container->getMovieClipByName("recommended_brawlers_anim")->stop();

    info->setChildVisible("icon_brawler", false);

    getClipFromContainers("rank_team_warning_container")->visible = false;

    gamemode_icon = new DataIcon(nullptr);
    gamemode_icon->setIconClip(std::string("sc/ui.sc"), LogicDataTables::getGameModeVariationData(6)->getGameModeIconName());
    gamemode_icon->replaceInstanceWithIcon(info->getMovieClipByName("gamemode_icon"), "icon", 1, 1);
    info->getMovieClipByName("gamemode_icon")->setChildVisible("icon", false);
    // info->debugForceNewBucket = true;
    // gamemode_icon->setScale(10.0f);
    // info->moveThisToTopLayer();
    // info->moveThisToTopLayer();


    // for (int i = 0;i < event_container->timelineChildrenCount;i++) {
    //     printf("%s\n", event_container->childrenNames[i]);
    // }
    event_container->setChildVisible("mutant_ph", false);

    info->playOnce();

    refreshRankedElement();
    getButtonByName("button_quest_icon")->visible = false;
    button_mode_clip->setChildVisible("pro_league", false);
    refreshSelectedEventChampionshipChallenge();
    refreshSelectedEventSeasonal();

}

void HomePage::refreshRankedElement() {
    setUpRankedModeElementForEvent((MovieClip*)getClipFromContainers("rank_mode_label")->parent, nullptr);
}

void HomePage::setUpRankedModeElementForEvent(MovieClip* movieClip, EventData*) {
    movieClip->setChildVisible("rank_mode_label", false);
}

void HomePage::refreshSelectedEventSeasonal() {
    getButtonByName("button_mode")->timelineClip->setChildVisible("halloween", false);
}

void HomePage::refreshSelectedEventChampionshipChallenge() {
    // getButtonByName("button_mode")->timelineMovieClip->setChildVisible("championship_challenge", false);
}

void HomePage::updateVisibleItems() {
    ;
}

void HomePage::createButtons() {
    for (MovieClip* mc : screenContainers) {
        if (!mc) continue;
        mc->autoCreateButtons(buttons, this);
    }
}

void HomePage::update(float deltaTime) {
    auto event_container = getClipFromContainers("event_container");
    event_container->getChildByName("vfx_overcharge")->visible = false;
    event_container->getChildByName("vfx_overcharge_front")->visible = false;

    //dead buttons club
    getButtonByName("button_pro_league")->visible = false;
    getButtonByName("button_ranked")->visible = false;

    getButtonByName("button_championship_challenge")->visible = false;

    if (player1_button) player1_button->update(deltaTime);

    if (player1) player1->update(deltaTime);
}

void HomePage::handleModeButtonPress() {
    GUI::getInstance()->showPopup(new SelectLocationPopup());
}

void HomePage::buttonClicked(GameButton* button) {
    auto clientHome = GameStateManager::getInstance()->home;
    if (button == player1_button) {
        GUI::getInstance()->showPopup(new HeroPopup());
    }
    else if (strcmp(button->name, "button_mode") == 0) {
        handleModeButtonPress();
    }
    else if (strcmp(button->name, "brawl_button") == 0) {
        startGame(selectedEvent, LogicDataTables::getTrainingGroundsData(), Training, clientHome->characters[0], clientHome->characters);
        // startGame(selectedEvent, nullptr, Trophies, clientHome->characters[0], clientHome->characters);
    }
}

void HomePage::startGame(EventData* event, LogicData* location, int type, LogicCharacterData* character, std::vector<LogicCharacterData*>& characters) {
    auto HomeMode = HomeMode::getInstance();
    HomeMode->getHomeScreen()->openMatchMakingPopup();
    return;
    switch (type) {
    case Trophies:
        MessageManager::getInstance()->sendMessage(new MatchmakeRequestMessage(character, characters, event->id, event->slot));
        break;
    case Training:
        MessageManager::getInstance()->sendMessage(new SinglePlayerMatchRequestMessage(8, character, nullptr, false, location));
        break;
    }
}

RotateHeroButton* HomePage::addPlayerButton(MovieClip* movieClip, const char* name) {
    auto mc = movieClip->getMovieClipByName(name);
    auto btn = new RotateHeroButton(mc);
    buttons.push_back(btn);
    movieClip->addChildAt(btn, mc->indexInParent);
    btn->setButtonListener(this);
    return btn;
}
