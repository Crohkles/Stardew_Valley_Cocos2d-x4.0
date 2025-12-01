#ifndef __farm_H__
#define __farm_H__

#include "cocos2d.h"
#include "Barn.h"
#include "../Entities/Player.h"
#include "../Entities/Crop.h"
#include "../Core/AppDelegate.h"
#include "physics/CCPhysicsWorld.h"
#include "ui/CocosGUI.h"
#include "../Commands/KeyCommand.h"
#include <memory>
#include <vector>

USING_NS_CC;

class EconomicSystem;


class farm : public cocos2d::Scene
{
public:

    farm ();
    ~farm ();

    virtual bool init ();

    static farm* create ();

    // ???????
    void checkPlayerPosition ();

    // ????
    void createRainEffect();

    // ????????
    void updaterain(float deltaTime);



    // ??????
    int getRegionNumber(Vec2 pos);
    
    // ????????
    void setupInputCommands();
    
    // ????????
    void cleanupInputCommands();

    // ?????????????????????
    std::vector<cocos2d::Vec2> nonTransparentPixels;

    // ???????
    Rect Out_Farm = Rect ( 792 , 1187 , 36 , 153 );

    // ?????(????? 48 * 48 )
    Rect plant_area = Rect ( 474 , 467 , 864 , 510 );

    // ????????
    Rect myhouse_area = Rect ( 70 , 920 , 75 , 30 );

    // ?????????
    Rect barn_area = Rect ( 20 , 170 , 70 , 70 );

    // ?????????
    Rect cave_area = Rect ( 635 , 1185 , 90 , 40 );

    // ?????????
    Rect forest_area = Rect ( 750 , -70 , 100 , 50 );

    cocos2d::ParticleRain* emitter;

    // ????
    void AllInitialize_crop ();
    
    // Command Pattern???????
    void performPlantAction();
    void performWaterAction();
    void performHarvestAction();

private:

    /* ???? */
    Sprite* button;


    // ????
    cocos2d::MenuItemImage* closeItem;

    cocos2d::Menu* menu;

    // Command Pattern???????
    std::vector<std::shared_ptr<KeyCommand>> boundCommands;

    Sprite* Box;

};

#endif 