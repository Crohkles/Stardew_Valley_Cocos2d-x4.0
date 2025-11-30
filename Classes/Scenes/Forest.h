#ifndef __Forest_H__
#define __Forest_H__

#include "cocos2d.h"
#include "../Entities/Player.h"
#include "../Entities/tree.h"
#include "../Core/AppDelegate.h"
#include "physics/CCPhysicsWorld.h"
#include "ui/CocosGUI.h"
#include "../Commands/KeyCommand.h"
#include <memory>
#include <vector>

USING_NS_CC;

class Forest : public cocos2d::Scene
{
public:

    Forest();
    ~Forest();

    virtual bool init();

    static  Forest* create();

    // 判断角色的位置
    void checkPlayerPosition();

    // 设置输入命令绑定
    void setupInputCommands();

    // 清理输入命令绑定
    void cleanupInputCommands();

    // 返回作物序号
    int getRegionNumber(Vec2 pos);

    // 下雨效果
    void createRainEffect();

    // 更改雨滴生命周期
    void updaterain(float deltaTime);

    // 创建一个列表，用于保存所有非透明像素的坐标
    std::vector<cocos2d::Vec2> nonTransparentPixels;
  
    // 恢复种植
    void AllInitialize_ore(); 

    // 前往农场
    Rect farm_area = Rect(1100, 2500, 150, 300);

    // 前往小镇
    Rect town_area = Rect(2500, 1850, 400, 150);

    cocos2d::ParticleRain* emitter;

   
private:

    /* 退出按钮 */
    Sprite* button;

    // 退出按钮
    cocos2d::MenuItemImage* closeItem;

    cocos2d::Menu* menu;

    // Command Pattern相关的成员变量
    std::vector<std::shared_ptr<KeyCommand>> boundCommands;

};

#endif 
