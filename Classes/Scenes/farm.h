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
#include "CropSpritePool.h"
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

    // 检查玩家位置并更新场景状态（时间 UI、箱子开关等）
    void checkPlayerPosition ();

    // 创建雨天粒子特效
    void createRainEffect();

    // 更新雨滴特效（动态调整雨滴寿命等）
    void updaterain(float deltaTime);



    // 根据坐标计算所在种植区域的小格编号
    int getRegionNumber(Vec2 pos);
    
    // 设置与解绑输入命令（Command Pattern）
    void setupInputCommands();
    
    // 清理已绑定的输入命令
    void cleanupInputCommands();

    // 记录地图上非透明像素的世界坐标，用于碰撞检测
    std::vector<cocos2d::Vec2> nonTransparentPixels;

    // 农场出口区域（离开农场到世界场景）
    Rect Out_Farm = Rect ( 792 , 1187 , 36 , 153 );

    // 种地区域（每格大小约为 48 * 48）
    Rect plant_area = Rect ( 474 , 467 , 864 , 510 );

    // 进入自家房子的判定区域
    Rect myhouse_area = Rect ( 70 , 920 , 75 , 30 );

    // 进入牛棚的判定区域
    Rect barn_area = Rect ( 20 , 170 , 70 , 70 );

    // 进入矿洞的判定区域
    Rect cave_area = Rect ( 635 , 1185 , 90 , 40 );

    // 进入森林的判定区域
    Rect forest_area = Rect ( 750 , -70 , 100 , 50 );

    cocos2d::ParticleRain* emitter;

    // 初始化所有作物（从数据中生成精灵等）
    void AllInitialize_crop ();
    
    // Command Pattern：与种地系统相关的行为封装
    void performPlantAction();
    void performWaterAction();
    void performHarvestAction();

private:

    /* 关闭按钮精灵 */
    Sprite* button;


    // 右上角关闭按钮（未使用或备用的菜单项）
    cocos2d::MenuItemImage* closeItem;

    cocos2d::Menu* menu;

    // 已绑定的命令列表，用于统一解绑
    std::vector<std::shared_ptr<KeyCommand>> boundCommands;

    Sprite* Box;

};

#endif 