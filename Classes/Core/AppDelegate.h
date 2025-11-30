#ifndef  _APP_DELEGATE_H_  // 防止头文件被多次包含的宏
#define  _APP_DELEGATE_H_

#include "../Systems/Timesystem.h"
#include "cocos2d.h"  
#include "audio/include/AudioEngine.h" 
//#include "GameBeginUI.h"
#include "../Scenes/Barn.h"
#include "../Scenes/Beach.h"
#include "../Scenes/Cave.h"
#include "../Scenes/farm.h"
#include "../Scenes/Forest.h"
#include "../Scenes/Myhouse.h"
#include "../Scenes/supermarket.h"
#include "../Scenes/Town.h"

#include "../Entities/Player.h"

#include "../UI/mini_bag.h"
#include "../Systems/Inventory.h"
#include "../UI/InventoryUI.h"

#include "../Items/Item.h"
#include "../Items/Generaltem.h"

#include "BasicInformation.h"
#include "../Entities/Crop.h"
#include "../Items/Ore.h"
#include "../Entities/tree.h"

#include "../Entities/Livestock.h"
#include "../Entities/Chicken.h"
#include "../Entities/Cow.h"
#include "../Entities/Sheep.h"
#include "../Systems/TaskManagement.h"
#include "../Entities/NPC.h"
#include "../Entities/NPCreate.h"
#include "../UI/NPCtalkUI.h"
#include "../Entities/NpcRelationship.h"
#include "../UI/intimacyUI.h"
#include "../UI/StoreUI.h"
#include "../Systems/EconomicSystem.h"
#include "../Systems/SkillTree.h"
#include <memory>
#include "../Systems/EnergySystem.h"

#define Daytime 43200

USING_NS_CC;

class Player;
class Town;
class farm;
class supermarket;
class Myhouse;
class Crop;
class Item;
class CropBasicInformation;
class Barn;
class Inventory;
class NpcRelationship;
class mini_bag;
class TaskManagement;
class Timesystem;
/******************************** 全局变量声明区 ***************************************/
extern int remainingTime;  // 声明，不初始化
extern int day;
extern int strength;
extern int GoldAmount;
extern bool IsNextDay;
extern bool frombed;
extern bool IsSleep;
extern bool GoldMaskfirst;
extern bool RainBowfirst;
extern std::string Season;
extern std::string Weather;
extern std::string Festival;
extern std::map<std::string , int> season;
// 每种作物的基本信息
extern std::map<std::string , Crop> cropbasicinformation;
extern std::map<std::pair<std::string , Vec2> , bool> T_lastplace;
extern std::map<std::pair<std::string, Vec2>, bool> F_lastplace;
extern std::map<std::pair<std::string, Vec2>, bool> W_lastplace;
// 已经种植的作物的状态信息
extern std::vector<std::shared_ptr<Crop>> Crop_information;
extern std::vector<std::shared_ptr<Ore>> Ore_information; 
extern std::vector<std::shared_ptr<Tree>> Tree_information; 
// 初始化各作物的基本信息
extern CropBasicInformation WHEAT;
extern CropBasicInformation CORN;
extern CropBasicInformation POTATO;
extern CropBasicInformation PUMPKIN;
extern CropBasicInformation BLUEBERRY;
extern Crop wheat; 
extern Crop corn;
extern Crop potato;
extern Crop pumpkin;
extern Crop blueberry;

//人物基本信息
extern std::string protagonistName;
extern std::string FarmName;

extern Player* player1;  // 声明指针变量，不初始化
extern SkillTree* skill_tree;
extern mini_bag* miniBag;
extern Inventory* inventory;
extern NpcRelationship* npc_relationship;
extern Inventory* StoreItem;
extern TaskManagement* taskManager;
extern Timesystem* TimeUI;
//存储畜棚中可有家畜活动的矩形区域，并记录该区域是否已存在家畜
extern std::vector<std::pair<Rect , bool>> barn_space;
//存储畜棚中的家畜
extern std::vector<Livestock*> livestocks;
/**************************************************************************************/


 /**
  @brief    Cocos2d 应用程序的委托类（AppDelegate）

  此类负责处理应用程序的生命周期管理，包括应用启动、进入后台、进入前台等操作。
  继承自 cocos2d::Application 类，但通过私有继承来隐藏部分接口，确保用户只能访问公共接口。
  */

  /************************************ 全局变量声明区 ******************************************/
  // 设定游戏画面大小：1600, 1280
static cocos2d::Size designResolutionSize = cocos2d::Size(1600, 1280);  // 设计分辨率


class AppDelegate : private cocos2d::Application  // 继承自 cocos2d::Application 类，并私有化继承
{
public:
    // 构造函数
    AppDelegate();

    // 析构函数
    virtual ~AppDelegate();

    // 初始化 OpenGL 上下文属性的函数
    virtual void initGLContextAttrs();

    /**
    @brief    应用程序启动时的初始化函数
    @return true    初始化成功，应用继续运行。
    @return false   初始化失败，应用程序退出。
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief    应用程序进入后台时调用。
    @param    the pointer of the application 指向应用程序的指针（Cocos2d 内部会调用此函数）
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief    应用程序重新进入前台时调用。
    @param    the pointer of the application 指向应用程序的指针（Cocos2d 内部会调用此函数）
    */
    virtual void applicationWillEnterForeground();

    void runScene(cocos2d::Director* director);

    void Initialize();


};

#endif  _APP_DELEGATE_H_  // 结束防止头文件被多次包含的宏
