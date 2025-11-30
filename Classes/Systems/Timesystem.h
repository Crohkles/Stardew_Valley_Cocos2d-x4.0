#pragma once
#include "cocos2d.h"  
#include "AppDelegate.h"
#include <NPC.h>
#include "GameStateObserver.h"

class Timesystem : public cocos2d::Node, public GameStateObserver {  // 继承自Node和GameStateObserver
public:
    bool init ( std::string place );
    virtual ~Timesystem();

    static Timesystem* create ( std::string place );

    void UpdateEnergy();
    
    // Observer接口实现
    virtual void onEconomicStateChanged(int newGoldAmount, int delta) override;
    virtual void onEnergyStateChanged(int newEnergy, int maxEnergy) override;

    // 其他成员变量和方法
    cocos2d::Label* timer_label_day;
    cocos2d::Label* timer_label_hour;
    cocos2d::Label* timer_label_season;
    cocos2d::Label* timer_label_weather;
    cocos2d::Label* timer_label_festival;
    cocos2d::Sprite* time_pic;
    cocos2d::Sprite* energy_frame;
    cocos2d::Sprite* energy_bar;

    cocos2d::Sprite* currency_frame;
    cocos2d::Label* currency_num;

    cocos2d::Sprite* daily_record;

    // 所处的场景
    std::string Place;
};