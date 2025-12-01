#ifndef __GAME_INTERACTION_FACADE_H__
#define __GAME_INTERACTION_FACADE_H__

#include "cocos2d.h"
#include "EnergySystem.h"
#include "EconomicSystem.h"
#include "TaskManagement.h"
#include "Inventory.h"
#include "../Items/Food.h"
#include "../UI/mini_bag.h"
#include "../Entities/NPC.h"
#include "../Entities/NpcRelationship.h"
#include "../Entities/Player.h"
#include "SkillTree.h"

USING_NS_CC;

/**
 * @brief 游戏交互外观类 - 封装复杂的子系统交互
 * 
 * 提供简化的接口来处理常见的游戏交互操作，
 * 隐藏多个子系统之间复杂的协调逻辑。
 */
class GameInteractionFacade {
private:
    static GameInteractionFacade* instance;
    
    // 子系统引用
    EnergySystem* energySystem;
    EconomicSystem* economicSystem;
    TaskManagement* taskManager;
    NpcRelationship* npcRelationship;
    Inventory* playerInventory;
    SkillTree* skillTree;
    Player* player;
    
    // 私有构造函数（单例模式）
    GameInteractionFacade();
    
    // 禁用拷贝构造和赋值操作
    GameInteractionFacade(const GameInteractionFacade&) = delete;
    GameInteractionFacade& operator=(const GameInteractionFacade&) = delete;
    
public:
    static GameInteractionFacade* getInstance();
    static void destroyInstance();
    
    /**
     * @brief 初始化外观类，设置子系统引用
     */
    void initialize(EconomicSystem* economic, TaskManagement* task, 
                   NpcRelationship* npcRel, Inventory* inventory,
                   SkillTree* skills, Player* playerRef);
    
    // === 食物消费交互外观 ===
    
    /**
     * @brief 食物消费结果结构体
     */
    struct FoodConsumptionResult {
        bool success;
        int energyGained;
        int currentEnergy;
        std::string itemName;
        std::string message;
        
        FoodConsumptionResult() : success(false), energyGained(0), currentEnergy(0) {}
    };
    
    /**
     * @brief 统一的食物消费接口
     * 整合UICommand和mini_bag中重复的逻辑
     * @param food 要消费的食物对象
     * @param miniBag 小背包UI引用
     * @return 消费结果
     */
    FoodConsumptionResult consumeFood(std::shared_ptr<Food> food, mini_bag* miniBag);
    
    /**
     * @brief 通过背包槽位消费食物
     * @param selectedSlot 选中的背包槽位（1-based）
     * @param miniBag 小背包UI引用
     * @return 消费结果
     */
    FoodConsumptionResult consumeFoodFromSlot(int selectedSlot, mini_bag* miniBag);
    
    // === 物品出售交互外观 ===
    
    /**
     * @brief 物品出售结果结构体
     */
    struct ItemSaleResult {
        bool success;
        int goldEarned;
        int currentGold;
        std::string itemName;
        int quantity;
        std::string message;
        
        ItemSaleResult() : success(false), goldEarned(0), currentGold(0), quantity(0) {}
    };
    
    /**
     * @brief 统一的物品出售接口
     * @param item 要出售的物品
     * @param quantity 出售数量
     * @param miniBag 小背包UI引用
     * @return 出售结果
     */
    ItemSaleResult sellItem(std::shared_ptr<Item> item, int quantity, mini_bag* miniBag);
    
    /**
     * @brief 通过背包槽位出售物品
     * @param selectedSlot 选中的背包槽位（1-based）
     * @param quantity 出售数量（默认为1）
     * @param miniBag 小背包UI引用
     * @return 出售结果
     */
    ItemSaleResult sellItemFromSlot(int selectedSlot, int quantity, mini_bag* miniBag);
    
    /**
     * @brief 简化的单个物品出售（出售选中槽位的全部物品）
     * @param miniBag 小背包UI引用
     * @return 出售结果
     */
    ItemSaleResult sellSelectedItem(mini_bag* miniBag);
    
    // === 辅助方法 ===
    
    /**
     * @brief 检查是否有足够的体力
     */
    bool hasEnoughEnergy(int requiredEnergy);
    
    /**
     * @brief 消耗体力
     */
    void consumeEnergy(int amount);
    
    /**
     * @brief 获取当前体力值
     */
    int getCurrentEnergy() const;
    
    /**
     * @brief 检查系统是否已正确初始化
     */
    bool isInitialized() const;
    
private:
    // 内部辅助方法
    bool validateFoodConsumption(std::shared_ptr<Food> food, mini_bag* miniBag);
    bool validateItemSale(std::shared_ptr<Item> item, int quantity, mini_bag* miniBag);
    void updateEnergySystemUI();
    void updateEconomicSystemUI();
};

#endif // __GAME_INTERACTION_FACADE_H__
