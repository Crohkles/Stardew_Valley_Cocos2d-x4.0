#ifndef __Town_H__
#define __Town_H__

#include "cocos2d.h"
#include "../Entities/Player.h"
#include "../Core/AppDelegate.h"
#include "../UI/mini_bag.h"
#include "physics/CCPhysicsWorld.h"
#include "ui/CocosGUI.h"
#include "../Core/InputManager.h"
#include "../Commands/SceneInteractionCommand.h"
#include "../Commands/UICommand.h"

USING_NS_CC;

class Town : public cocos2d::Scene
{
public:

    Town();
    ~Town();

    virtual bool init();

    static Town* create();

    // 判断角色的位置
    void checkPlayerPosition();

    // 设置输入命令绑定
    void setupInputCommands();

    // 清理输入命令绑定
    void cleanupInputCommands();

    // 下雨效果
    void createRainEffect();

    // 更改雨滴生命周期
    void updaterain(float deltaTime);
    
    // 创建一个列表，用于保存所有非透明像素的坐标
    std::vector<cocos2d::Vec2> nonTransparentPixels;

    // 创建房子的区域
    cocos2d::Rect Region_supermarket = Rect(163, 518, 130, 60);
    cocos2d::Rect Region_forest = Rect(-940, -400, 200, 110);
    cocos2d::Rect Region_beach = Rect(490, -750, 50, 100);

    cocos2d::ParticleRain* emitter;

private:

    /* 退出按钮 */
    Sprite* button;

    /* 开门按钮 */
    Sprite* opendoor;

    // 退出按钮
    cocos2d::MenuItemImage* closeItem;

    cocos2d::Menu* menu;

    cocos2d::Sprite* In_gettask;

    Sprite* Box;

    // Command Pattern相关的成员变量
    std::vector<std::shared_ptr<KeyCommand>> boundCommands;

};

#endif // __BACKGROUND1_H__
