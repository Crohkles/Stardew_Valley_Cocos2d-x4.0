#ifndef __INPUT_TEST_SCENE_H__
#define __INPUT_TEST_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

/**
 * @brief 输入系统测试场景
 * 
 * 用于测试新的InputManager和Command系统的基本功能
 * 包括Player移动、UI操作等基础功能测试
 */
class InputTestScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // 设置输入命令绑定
    void setupInputBindings();
    
    CREATE_FUNC(InputTestScene);

private:
    class Player* testPlayer;           // 测试用的玩家对象
    class Inventory* testInventory;     // 测试用的背包
    cocos2d::Label* statusLabel;        // 状态显示标签
    
    // 测试用的命令计数器
    static int commandExecuteCount;
    
public:
    // 用于测试的静态方法
    static void incrementCommandCount();
    static int getCommandCount();
    static void resetCommandCount();
};

#endif // __INPUT_TEST_SCENE_H__