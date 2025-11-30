#ifndef GENERALITEM_H  
#define GENERALITEM_H  

#include "Item.h"
#include "ItemFlyweight.h"
#include <memory>

// 使用flyweight模式重构的物品管理系统
// 动物设定为购买后直接加入牧场，在背包中不可见  

// 获取物品的便利函数声明 - 保持向后兼容
namespace ItemManager {
    // 初始化物品系统
    void InitializeItems();
    
    // 获取物品的便利函数
    std::shared_ptr<Item> GetItem(const std::string& itemName);
    
    // 类型安全的获取函数
    template<typename T = Item>
    std::shared_ptr<T> GetTypedItem(const std::string& itemName);
    
    // 按类别获取物品列表
    std::vector<std::shared_ptr<Item>> GetAnimalItems();
    std::vector<std::shared_ptr<Item>> GetSeedItems();
    std::vector<std::shared_ptr<Item>> GetToolItems();
    std::vector<std::shared_ptr<Item>> GetGemItems();
}

// 保持原有的全局访问方式 - 向后兼容
// 动物类
std::shared_ptr<Item> GetAnimalChicken();
std::shared_ptr<Item> GetAnimalDuck();
std::shared_ptr<Item> GetAnimalGoat();
std::shared_ptr<Item> GetAnimalPig();
std::shared_ptr<Item> GetAnimalRabbit();
std::shared_ptr<Item> GetAnimalSheep();
std::shared_ptr<Item> GetAnimalCow();

// 春季种子
std::shared_ptr<Item> GetBeanStarter();
std::shared_ptr<Item> GetCarrotSeeds();
std::shared_ptr<Item> GetCauliflowerSeeds();
std::shared_ptr<Item> GetCoffeeBean();
std::shared_ptr<Item> GetGarlicSeeds();
std::shared_ptr<Item> GetJazzSeeds();
std::shared_ptr<Item> GetKaleSeeds();
std::shared_ptr<Item> GetParsnipSeeds();
std::shared_ptr<Item> GetPotatoSeeds();
std::shared_ptr<Item> GetRhubarbSeeds();
std::shared_ptr<Item> GetRiceShoot();
std::shared_ptr<Item> GetStrawberrySeeds();
std::shared_ptr<Item> GetTulipBulb();

// 夏季种子
std::shared_ptr<Item> GetAmaranthSeeds();
std::shared_ptr<Item> GetArtichokeSeeds();
std::shared_ptr<Item> GetBeetSeeds();
std::shared_ptr<Item> GetBlueberrySeeds();
std::shared_ptr<Item> GetBokChoySeeds();
std::shared_ptr<Item> GetBroccoliSeeds();
std::shared_ptr<Item> GetCornSeeds();
std::shared_ptr<Item> GetCranberrySeeds();
std::shared_ptr<Item> GetEggplantSeeds();
std::shared_ptr<Item> GetFairySeeds();
std::shared_ptr<Item> GetGrapeStarter();
std::shared_ptr<Item> GetHopsStarter();
std::shared_ptr<Item> GetMelonSeeds();
std::shared_ptr<Item> GetPepperSeeds();
std::shared_ptr<Item> GetPoppySeeds();
std::shared_ptr<Item> GetPumpkinSeeds();
std::shared_ptr<Item> GetRadishSeeds();
std::shared_ptr<Item> GetRedCabbageSeeds();
std::shared_ptr<Item> GetSpangleSeeds();
std::shared_ptr<Item> GetStarfruitSeeds();
std::shared_ptr<Item> GetSummerSquashSeeds();
std::shared_ptr<Item> GetSunflowerSeeds();
std::shared_ptr<Item> GetTomatoSeeds();
std::shared_ptr<Item> GetWheatSeeds();
std::shared_ptr<Item> GetYamSeeds();

// 冬季种子（游戏中没有冬季种子这个物品）
std::shared_ptr<Item> GetPowdermelonSeeds();  

// 工具类
std::shared_ptr<Item> GetBackpack36();
std::shared_ptr<Item> GetBackpack();
std::shared_ptr<Item> GetAdvancedIridiumRod();
std::shared_ptr<Item> GetAxe();
std::shared_ptr<Item> GetBambooPole();
std::shared_ptr<Item> GetCopperAxe();
std::shared_ptr<Item> GetCopperHoe();
std::shared_ptr<Item> GetCopperPan();
std::shared_ptr<Item> GetCopperPickaxe();
std::shared_ptr<Item> GetCopperWatering();
std::shared_ptr<Item> GetFiberglassRod();
std::shared_ptr<Item> GetGoldAxe();
std::shared_ptr<Item> GetGoldHoe();
std::shared_ptr<Item> GetGoldPan();
std::shared_ptr<Item> GetGoldPickaxe();
std::shared_ptr<Item> GetGoldWateringCan();
std::shared_ptr<Item> GetGoldenScythe();
std::shared_ptr<Item> GetHoe();
std::shared_ptr<Item> GetPickaxe();
std::shared_ptr<Item> GetMilkPail();
std::shared_ptr<Item> GetScythe();
std::shared_ptr<Item> GetSteelAxe();
std::shared_ptr<Item> GetSteelHoe();
std::shared_ptr<Item> GetSteelPan();
std::shared_ptr<Item> GetSteelPickaxe();
std::shared_ptr<Item> GetSteelWatering();
std::shared_ptr<Item> GetTrashCanCopper();
std::shared_ptr<Item> GetWateringCan();
std::shared_ptr<Item> GetTrashCanSteel();

// 树苗类  
std::shared_ptr<Item> GetAppleSapling();
std::shared_ptr<Item> GetApricotSapling();
std::shared_ptr<Item> GetBananaSapling();
std::shared_ptr<Item> GetCherrySapling();
std::shared_ptr<Item> GetMangoSapling();
std::shared_ptr<Item> GetOrangeSapling();
std::shared_ptr<Item> GetPeachSapling();
std::shared_ptr<Item> GetPomegranateSapling();

// 宝石类
std::shared_ptr<Item> GetAmethyst();
std::shared_ptr<Item> GetEmerald();
std::shared_ptr<Item> GetRuby();

// 成熟作物
std::shared_ptr<Item> GetWheat();
std::shared_ptr<Item> GetCorn();
std::shared_ptr<Item> GetPotato();
std::shared_ptr<Item> GetPumpkin();
std::shared_ptr<Item> GetBlueberry();
std::shared_ptr<Item> GetWood();

// 特殊物品
std::shared_ptr<Item> GetGoldMask();
std::shared_ptr<Item> GetRainBow();

#endif // GENERALITEM_H