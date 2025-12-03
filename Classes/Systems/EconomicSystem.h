#ifndef ECONOMIC_SYSTEM_H  
#define ECONOMIC_SYSTEM_H  

#include "../Core/GameStateObserver.h"

// EconomicSystem 现在是一个纯粹的单例，只负责管理全局金钱状态
// 移除了 Inventory 指针，解除了循环依赖

class EconomicSystem : public GameStateSubject {
private:
    static EconomicSystem* instance;
    int goldAmount;

    // 私有构造函数
    EconomicSystem(); 

public:
    // 禁止拷贝
    EconomicSystem(const EconomicSystem&) = delete;
    EconomicSystem& operator=(const EconomicSystem&) = delete;

    ~EconomicSystem();

    // 获取单例
    static EconomicSystem* getInstance();
    static void destroyInstance();

    // 核心功能：增减金币并通知
    void addGold(int amount);
    void subtractGold(int amount);
    int getGoldAmount() const;
    
    // 注意：原有的 buyItem 和 sellItem 被移除
    // 购买逻辑移至 StoreUI/Facade，出售逻辑移至 GameInteractionFacade
};

#endif