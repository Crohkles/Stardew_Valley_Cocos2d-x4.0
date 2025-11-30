#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"
#include "../Systems/Inventory.h"
#include "Crop.h"
#include "../Core/AppDelegate.h"

const int kDefaultEnergy = 100;

USING_NS_CC;

class Player : public cocos2d::Sprite
{
public:
    Player();
    ~Player();

    // 创建 Player 对象的方法
    static Player* create();

    // 初始化角色
    bool init();

    // 按键按下时触发的回调函数（已废弃，保留用于兼容性）
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // 按键释放时触发的回调函数（已废弃，保留用于兼容性）
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    
    // 设置输入绑定的方法（新Command系统）
    void setupInputBindings();
    
    // 新增：Command系统用的移动状态控制方法
    void startMove(int direction);  // direction: 0=down, 1=left, 2=right, 3=up
    void stopMove(int direction);
    bool canMove(int direction) const;
    
    // 碰撞上下文管理方法
    void setCollisionContext(const std::vector<cocos2d::Vec2>& collisionPoints);
    void updateMovementPermissions();

    void player1_move();

    void player_change();


    bool moveLeft, moveDown, moveUp, moveRight;

    bool leftpressed = false, downpressed = false, uppressed = false, rightpressed = false;

    // 角色的移动速度
    float speed;

    int look_state = 0;

    std::string pic_path;

    //int energy_limit = kDefaultEnergy;
    //int current_energy = kDefaultEnergy;

private:
    // 碰撞检测相关
    std::vector<cocos2d::Vec2> collisionPoints;
    float collisionRadius = 15.0f;  // 碰撞检测半径

};

#endif // __PLAYER_H__
