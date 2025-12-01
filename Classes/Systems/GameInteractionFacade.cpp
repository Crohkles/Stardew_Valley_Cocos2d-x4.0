#include "GameInteractionFacade.h"
#include "../Core/AppDelegate.h"
#include "../Systems/Timesystem.h"
#include "cocos2d.h"

USING_NS_CC;

// 访问全局变量
extern int strength;
extern Inventory* inventory;
extern Timesystem* TimeUI;

// 静态成员初始化
GameInteractionFacade* GameInteractionFacade::instance = nullptr;

GameInteractionFacade::GameInteractionFacade() 
    : energySystem(nullptr)
    , economicSystem(nullptr)
    , taskManager(nullptr)
    , npcRelationship(nullptr)
    , playerInventory(nullptr)
    , skillTree(nullptr)
    , player(nullptr)
{
}

GameInteractionFacade* GameInteractionFacade::getInstance() {
    if (instance == nullptr) {
        instance = new GameInteractionFacade();
    }
    return instance;
}

void GameInteractionFacade::destroyInstance() {
    if (instance != nullptr) {
        delete instance;
        instance = nullptr;
    }
}

void GameInteractionFacade::initialize(EconomicSystem* economic, TaskManagement* task, 
                                     NpcRelationship* npcRel, Inventory* inventoryRef,
                                     SkillTree* skills, Player* playerRef) {
    economicSystem = economic;
    taskManager = task;
    npcRelationship = npcRel;
    playerInventory = inventoryRef;
    skillTree = skills;
    player = playerRef;
    
    // 获取能量系统单例
    energySystem = EnergySystem::getInstance();
    
    CCLOG("GameInteractionFacade initialized with all subsystems");
}

bool GameInteractionFacade::isInitialized() const {
    return energySystem != nullptr && 
           playerInventory != nullptr && 
           player != nullptr;
}

GameInteractionFacade::FoodConsumptionResult 
GameInteractionFacade::consumeFood(std::shared_ptr<Food> food, mini_bag* miniBag) {
    FoodConsumptionResult result;
    
    // 基础验证
    if (!validateFoodConsumption(food, miniBag)) {
        result.message = "Invalid food consumption parameters";
        return result;
    }
    
    // 获取食物信息
    int energyGain = food->GetEnergy();
    result.itemName = food->GetName();
    
    // 计算体力恢复（最大值100）
    int oldEnergy = getCurrentEnergy();
    int newEnergy = std::min(100, oldEnergy + energyGain);
    result.energyGained = newEnergy - oldEnergy;
    result.currentEnergy = newEnergy;
    
    // 更新体力系统
    if (energySystem) {
        energySystem->setEnergy(newEnergy);
    }
    
    // 同时更新全局变量（兼容现有系统）
    strength = newEnergy;
    
    // 从背包中移除食物
    bool removedFromInventory = false;
    if (playerInventory) {
        // 尝试从主背包移除
        if (playerInventory->RemoveItem(*food) != -1) {
            removedFromInventory = true;
            playerInventory->is_updated = true;
        }
    }
    
    if (removedFromInventory) {
        // 重置mini_bag的选择状态
        if (miniBag) {
            miniBag->getSelectBack();
        }
        
        // 更新UI显示
        updateEnergySystemUI();
        
        result.success = true;
        result.message = "Consumed " + result.itemName + ", gained " + 
                        std::to_string(result.energyGained) + " energy";
        
        CCLOG("GameFacade: Successfully consumed %s, energy: %d -> %d", 
              result.itemName.c_str(), oldEnergy, newEnergy);
    } else {
        result.message = "Failed to remove food from inventory";
        CCLOG("GameFacade: Failed to consume %s - could not remove from inventory", 
              result.itemName.c_str());
    }
    
    return result;
}

GameInteractionFacade::FoodConsumptionResult 
GameInteractionFacade::consumeFoodFromSlot(int selectedSlot, mini_bag* miniBag) {
    FoodConsumptionResult result;
    
    if (!miniBag || selectedSlot <= 0) {
        result.message = "Invalid slot or mini_bag reference";
        return result;
    }
    
    // 获取选中的物品
    auto selectedItem = miniBag->getSelectedItem();
    if (!selectedItem) {
        result.message = "No item selected in mini bag";
        return result;
    }
    
    // 检查是否为食物
    auto food = std::dynamic_pointer_cast<Food>(selectedItem);
    if (!food) {
        result.message = "Selected item is not food";
        return result;
    }
    
    // 调用主要的食物消费逻辑
    return consumeFood(food, miniBag);
}

bool GameInteractionFacade::validateFoodConsumption(std::shared_ptr<Food> food, mini_bag* miniBag) {
    if (!food) {
        CCLOG("GameFacade: Food object is null");
        return false;
    }
    
    if (!miniBag) {
        CCLOG("GameFacade: mini_bag reference is null");
        return false;
    }
    
    if (!isInitialized()) {
        CCLOG("GameFacade: Facade not properly initialized");
        return false;
    }
    
    return true;
}

void GameInteractionFacade::updateEnergySystemUI() {
    // 更新TimeUI中的能量显示
    if (TimeUI) {
        TimeUI->UpdateEnergy();
    }
}

bool GameInteractionFacade::hasEnoughEnergy(int requiredEnergy) {
    int current = getCurrentEnergy();
    return current >= requiredEnergy;
}

void GameInteractionFacade::consumeEnergy(int amount) {
    if (amount <= 0) return;
    
    int current = getCurrentEnergy();
    int newEnergy = std::max(0, current - amount);
    
    if (energySystem) {
        energySystem->setEnergy(newEnergy);
    }
    
    // 同步全局变量
    strength = newEnergy;
    
    // 更新UI
    updateEnergySystemUI();
    
    CCLOG("GameFacade: Consumed %d energy, current: %d", amount, newEnergy);
}

int GameInteractionFacade::getCurrentEnergy() const {
    if (energySystem) {
        return energySystem->getCurrentEnergy();
    }
    
    // 回退到全局变量
    return strength;
}
