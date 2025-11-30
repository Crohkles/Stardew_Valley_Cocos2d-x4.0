#include "Generaltem.h"

// Flyweight模式实现 - 按需获取物品，节省内存

// 内部辅助函数
static bool initialized = false;

// ItemManager命名空间实现
namespace ItemManager {
    void InitializeItems() {
        if (!initialized) {
            ItemFlyweightFactory::getInstance().preloadAllItems();
            initialized = true;
        }
    }
    
    std::shared_ptr<Item> GetItem(const std::string& itemName) {
        InitializeItems();
        return ItemFlyweightFactory::getInstance().getFlyweight(itemName);
    }
    
    template<typename T>
    std::shared_ptr<T> GetTypedItem(const std::string& itemName) {
        auto item = GetItem(itemName);
        return std::static_pointer_cast<T>(item);
    }
    
    std::vector<std::shared_ptr<Item>> GetAnimalItems() {
        InitializeItems();
        auto flyweights = ItemFlyweightFactory::getInstance().getItemsByCategory(ItemCategory::ANIMAL);
        std::vector<std::shared_ptr<Item>> result;
        for (auto& fw : flyweights) {
            result.push_back(std::static_pointer_cast<Item>(fw));
        }
        return result;
    }
    
    std::vector<std::shared_ptr<Item>> GetSeedItems() {
        InitializeItems();
        auto result = GetAnimalItems(); // 复用逻辑
        result.clear();
        
        auto spring = ItemFlyweightFactory::getInstance().getItemsByCategory(ItemCategory::SPRING_SEED);
        auto summer = ItemFlyweightFactory::getInstance().getItemsByCategory(ItemCategory::SUMMER_SEED);
        auto winter = ItemFlyweightFactory::getInstance().getItemsByCategory(ItemCategory::WINTER_SEED);
        
        for (auto& fw : spring) result.push_back(std::static_pointer_cast<Item>(fw));
        for (auto& fw : summer) result.push_back(std::static_pointer_cast<Item>(fw));
        for (auto& fw : winter) result.push_back(std::static_pointer_cast<Item>(fw));
        
        return result;
    }
    
    std::vector<std::shared_ptr<Item>> GetToolItems() {
        InitializeItems();
        auto flyweights = ItemFlyweightFactory::getInstance().getItemsByCategory(ItemCategory::TOOL);
        std::vector<std::shared_ptr<Item>> result;
        for (auto& fw : flyweights) {
            result.push_back(std::static_pointer_cast<Item>(fw));
        }
        return result;
    }
    
    std::vector<std::shared_ptr<Item>> GetGemItems() {
        InitializeItems();
        auto flyweights = ItemFlyweightFactory::getInstance().getItemsByCategory(ItemCategory::GEM);
        std::vector<std::shared_ptr<Item>> result;
        for (auto& fw : flyweights) {
            result.push_back(std::static_pointer_cast<Item>(fw));
        }
        return result;
    }
}

// 向后兼容的全局访问函数 - 动物类

std::shared_ptr<Item> GetAnimalChicken() { return ItemManager::GetItem("AnimalChicken"); }
std::shared_ptr<Item> GetAnimalDuck() { return ItemManager::GetItem("AnimalDuck"); }
std::shared_ptr<Item> GetAnimalGoat() { return ItemManager::GetItem("AnimalGoat"); }
std::shared_ptr<Item> GetAnimalPig() { return ItemManager::GetItem("AnimalPig"); }
std::shared_ptr<Item> GetAnimalRabbit() { return ItemManager::GetItem("AnimalRabbit"); }
std::shared_ptr<Item> GetAnimalSheep() { return ItemManager::GetItem("AnimalSheep"); }
std::shared_ptr<Item> GetAnimalCow() { return ItemManager::GetItem("AnimalCow"); }

// 春季种子getter函数
std::shared_ptr<Item> GetBeanStarter() { return ItemManager::GetItem("Bean_Starter"); }
std::shared_ptr<Item> GetCarrotSeeds() { return ItemManager::GetItem("Carrot_Seeds"); }
std::shared_ptr<Item> GetCauliflowerSeeds() { return ItemManager::GetItem("Cauliflower_Seeds"); }
std::shared_ptr<Item> GetCoffeeBean() { return ItemManager::GetItem("Coffee_Bean"); }
std::shared_ptr<Item> GetGarlicSeeds() { return ItemManager::GetItem("Garlic_Seeds"); }
std::shared_ptr<Item> GetJazzSeeds() { return ItemManager::GetItem("Jazz_Seeds"); }
std::shared_ptr<Item> GetKaleSeeds() { return ItemManager::GetItem("Kale_Seeds"); }
std::shared_ptr<Item> GetParsnipSeeds() { return ItemManager::GetItem("Parsnip_Seeds"); }
std::shared_ptr<Item> GetPotatoSeeds() { return ItemManager::GetItem("Potato_Seeds"); }
std::shared_ptr<Item> GetRhubarbSeeds() { return ItemManager::GetItem("Rhubarb_Seeds"); }
std::shared_ptr<Item> GetRiceShoot() { return ItemManager::GetItem("Rice_Shoot"); }
std::shared_ptr<Item> GetStrawberrySeeds() { return ItemManager::GetItem("Strawberry_Seeds"); }
std::shared_ptr<Item> GetTulipBulb() { return ItemManager::GetItem("Tulip_Bulb"); }

// 夏季种子getter函数
std::shared_ptr<Item> GetAmaranthSeeds() { return ItemManager::GetItem("Amaranth_Seeds"); }
std::shared_ptr<Item> GetArtichokeSeeds() { return ItemManager::GetItem("Artichoke_Seeds"); }
std::shared_ptr<Item> GetBeetSeeds() { return ItemManager::GetItem("Beet_Seeds"); }
std::shared_ptr<Item> GetBlueberrySeeds() { return ItemManager::GetItem("Blueberry_Seeds"); }
std::shared_ptr<Item> GetBokChoySeeds() { return ItemManager::GetItem("Bok_Choy_Seeds"); }
std::shared_ptr<Item> GetBroccoliSeeds() { return ItemManager::GetItem("Broccoli_Seeds"); }
std::shared_ptr<Item> GetCornSeeds() { return ItemManager::GetItem("Corn_Seeds"); }
std::shared_ptr<Item> GetCranberrySeeds() { return ItemManager::GetItem("Cranberry_Seeds"); }
std::shared_ptr<Item> GetEggplantSeeds() { return ItemManager::GetItem("Eggplant_Seeds"); }
std::shared_ptr<Item> GetFairySeeds() { return ItemManager::GetItem("Fairy_Seeds"); }
std::shared_ptr<Item> GetGrapeStarter() { return ItemManager::GetItem("Grape_Starter"); }
std::shared_ptr<Item> GetHopsStarter() { return ItemManager::GetItem("Hops_Starter"); }
std::shared_ptr<Item> GetMelonSeeds() { return ItemManager::GetItem("Melon_Seeds"); }
std::shared_ptr<Item> GetPepperSeeds() { return ItemManager::GetItem("Pepper_Seeds"); }
std::shared_ptr<Item> GetPoppySeeds() { return ItemManager::GetItem("Poppy_Seeds"); }
std::shared_ptr<Item> GetPumpkinSeeds() { return ItemManager::GetItem("Pumpkin_Seeds"); }
std::shared_ptr<Item> GetRadishSeeds() { return ItemManager::GetItem("Radish_Seeds"); }
std::shared_ptr<Item> GetRedCabbageSeeds() { return ItemManager::GetItem("Red_Cabbage_Seeds"); }
std::shared_ptr<Item> GetSpangleSeeds() { return ItemManager::GetItem("Spangle_Seeds"); }
std::shared_ptr<Item> GetStarfruitSeeds() { return ItemManager::GetItem("Starfruit_Seeds"); }
std::shared_ptr<Item> GetSummerSquashSeeds() { return ItemManager::GetItem("Summer_Squash_Seeds"); }
std::shared_ptr<Item> GetSunflowerSeeds() { return ItemManager::GetItem("Sunflower_Seeds"); }
std::shared_ptr<Item> GetTomatoSeeds() { return ItemManager::GetItem("Tomato_Seeds"); }
std::shared_ptr<Item> GetWheatSeeds() { return ItemManager::GetItem("Wheat_Seeds"); }
std::shared_ptr<Item> GetYamSeeds() { return ItemManager::GetItem("Yam_Seeds"); }

// 冬季种子getter函数
std::shared_ptr<Item> GetPowdermelonSeeds() { return ItemManager::GetItem("Powdermelon_Seeds"); }  

// 工具类getter函数
std::shared_ptr<Item> GetBackpack36() { return ItemManager::GetItem("Backpack_36"); }
std::shared_ptr<Item> GetBackpack() { return ItemManager::GetItem("Backpack"); }
std::shared_ptr<Item> GetAdvancedIridiumRod() { return ItemManager::GetItem("Advanced_Iridium_Rod"); }
std::shared_ptr<Item> GetAxe() { return ItemManager::GetItem("Axe"); }
std::shared_ptr<Item> GetBambooPole() { return ItemManager::GetItem("Bamboo_Pole"); }
std::shared_ptr<Item> GetCopperAxe() { return ItemManager::GetItem("Copper_Axe"); }
std::shared_ptr<Item> GetCopperHoe() { return ItemManager::GetItem("Copper_Hoe"); }
std::shared_ptr<Item> GetCopperPan() { return ItemManager::GetItem("Copper_Pan"); }
std::shared_ptr<Item> GetCopperPickaxe() { return ItemManager::GetItem("Copper_Pickaxe"); }
std::shared_ptr<Item> GetCopperWatering() { return ItemManager::GetItem("Copper_Watering"); }
std::shared_ptr<Item> GetFiberglassRod() { return ItemManager::GetItem("Fiberglass_Rod"); }
std::shared_ptr<Item> GetGoldAxe() { return ItemManager::GetItem("Gold_Axe"); }
std::shared_ptr<Item> GetGoldHoe() { return ItemManager::GetItem("Gold_Hoe"); }
std::shared_ptr<Item> GetGoldPan() { return ItemManager::GetItem("Gold_Pan"); }
std::shared_ptr<Item> GetGoldPickaxe() { return ItemManager::GetItem("Gold_Pickaxe"); }
std::shared_ptr<Item> GetGoldWateringCan() { return ItemManager::GetItem("Gold_Watering_Can"); }
std::shared_ptr<Item> GetGoldenScythe() { return ItemManager::GetItem("Golden_Scythe"); }
std::shared_ptr<Item> GetHoe() { return ItemManager::GetItem("Hoe"); }
std::shared_ptr<Item> GetPickaxe() { return ItemManager::GetItem("Pickaxe"); }
std::shared_ptr<Item> GetMilkPail() { return ItemManager::GetItem("Milk_Pail"); }
std::shared_ptr<Item> GetScythe() { return ItemManager::GetItem("Scythe"); }
std::shared_ptr<Item> GetSteelAxe() { return ItemManager::GetItem("Steel_Axe"); }
std::shared_ptr<Item> GetSteelHoe() { return ItemManager::GetItem("Steel_Hoe"); }
std::shared_ptr<Item> GetSteelPan() { return ItemManager::GetItem("Steel_Pan"); }
std::shared_ptr<Item> GetSteelPickaxe() { return ItemManager::GetItem("Steel_Pickaxe"); }
std::shared_ptr<Item> GetSteelWatering() { return ItemManager::GetItem("Steel_Watering"); }
std::shared_ptr<Item> GetTrashCanCopper() { return ItemManager::GetItem("Trash_Can_Copper"); }
std::shared_ptr<Item> GetWateringCan() { return ItemManager::GetItem("Watering_Can"); }
std::shared_ptr<Item> GetTrashCanSteel() { return ItemManager::GetItem("Trash_Can_Steel"); }

// 树苗类getter函数
std::shared_ptr<Item> GetAppleSapling() { return ItemManager::GetItem("Apple_Sapling"); }
std::shared_ptr<Item> GetApricotSapling() { return ItemManager::GetItem("Apricot_Sapling"); }
std::shared_ptr<Item> GetBananaSapling() { return ItemManager::GetItem("Banana_Sapling"); }
std::shared_ptr<Item> GetCherrySapling() { return ItemManager::GetItem("Cherry_Sapling"); }
std::shared_ptr<Item> GetMangoSapling() { return ItemManager::GetItem("Mango_Sapling"); }
std::shared_ptr<Item> GetOrangeSapling() { return ItemManager::GetItem("Orange_Sapling"); }
std::shared_ptr<Item> GetPeachSapling() { return ItemManager::GetItem("Peach_Sapling"); }
std::shared_ptr<Item> GetPomegranateSapling() { return ItemManager::GetItem("Pomegranate_Sapling"); }

// 宝石类getter函数
std::shared_ptr<Item> GetAmethyst() { return ItemManager::GetItem("Amethyst"); }
std::shared_ptr<Item> GetEmerald() { return ItemManager::GetItem("Emerald"); }
std::shared_ptr<Item> GetRuby() { return ItemManager::GetItem("Ruby"); }

// 成熟作物getter函数
std::shared_ptr<Item> GetWheat() { return ItemManager::GetItem("wheat"); }
std::shared_ptr<Item> GetCorn() { return ItemManager::GetItem("corn"); }
std::shared_ptr<Item> GetPotato() { return ItemManager::GetItem("potato"); }
std::shared_ptr<Item> GetPumpkin() { return ItemManager::GetItem("pumpkin"); }
std::shared_ptr<Item> GetBlueberry() { return ItemManager::GetItem("blueberry"); }
std::shared_ptr<Item> GetWood() { return ItemManager::GetItem("wood"); }

// 特殊物品getter函数
std::shared_ptr<Item> GetGoldMask() { return ItemManager::GetItem("GoldMask"); }
std::shared_ptr<Item> GetRainBow() { return ItemManager::GetItem("RainBow"); }


