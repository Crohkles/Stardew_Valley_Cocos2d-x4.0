#include "ItemFlyweight.h"
#include <algorithm>

// ItemFlyweight实现
ItemFlyweight::ItemFlyweight(const ItemMetadata& metadata)
    : Item(metadata.name, metadata.imagePath, metadata.baseValue, metadata.maxStackCount)
    , metadata_(&metadata) {
}

ItemFlyweight::ItemFlyweight(const ItemFlyweight& other)
    : Item(other), metadata_(other.metadata_) {
}

std::shared_ptr<Item> ItemFlyweight::GetCopy() const {
    // 返回新的ItemFlyweight实例，但共享相同的元数据
    return std::make_shared<ItemFlyweight>(*this);
}

// ItemFlyweightFactory实现
ItemFlyweightFactory& ItemFlyweightFactory::getInstance() {
    static ItemFlyweightFactory instance;
    return instance;
}

std::shared_ptr<ItemFlyweight> ItemFlyweightFactory::getFlyweight(const std::string& itemName) {
    // 如果flyweight已存在，直接返回
    auto it = flyweights_.find(itemName);
    if (it != flyweights_.end()) {
        return it->second;
    }
    
    // 查找元数据
    auto metaIt = metadata_.find(itemName);
    if (metaIt == metadata_.end()) {
        return nullptr; // 物品不存在
    }
    
    // 创建新的flyweight
    auto flyweight = std::make_shared<ItemFlyweight>(*metaIt->second);
    flyweights_[itemName] = flyweight;
    
    return flyweight;
}

void ItemFlyweightFactory::preloadAllItems() {
    // 清空现有数据
    metadata_.clear();
    flyweights_.clear();
    categoryMap_.clear();
    
    // 初始化所有类别
    initializeAnimals();
    initializeSpringSeed();
    initializeSummerSeeds();
    initializeWinterSeeds();
    initializeTools();
    initializeTreeSaplings();
    initializeGems();
    initializeMatureCrops();
    initializeSpecialItems();
}

std::vector<std::shared_ptr<ItemFlyweight>> ItemFlyweightFactory::getItemsByCategory(ItemCategory category) {
    std::vector<std::shared_ptr<ItemFlyweight>> result;
    
    auto categoryIt = categoryMap_.find(category);
    if (categoryIt != categoryMap_.end()) {
        for (const auto& itemName : categoryIt->second) {
            auto flyweight = getFlyweight(itemName);
            if (flyweight) {
                result.push_back(flyweight);
            }
        }
    }
    
    return result;
}

void ItemFlyweightFactory::createItem(const std::string& name, const std::string& path, 
                                    int value, int stack, ItemCategory category) {
    // 创建元数据
    metadata_[name] = std::make_unique<ItemMetadata>(name, path, value, stack);
    
    // 添加到类别映射
    categoryMap_[category].push_back(name);
}

void ItemFlyweightFactory::initializeAnimals() {
    createItem("AnimalChicken", "Item/Surpermarket/Animal/Brown_Chicken.png", 500, 99, ItemCategory::ANIMAL);
    createItem("AnimalDuck", "Item/Surpermarket/Animal/Duck.png", 1200, 99, ItemCategory::ANIMAL);
    createItem("AnimalGoat", "Item/Surpermarket/Animal/Goat.png", 1000, 99, ItemCategory::ANIMAL);
    createItem("AnimalPig", "Item/Surpermarket/Animal/Pig.png", 16000, 99, ItemCategory::ANIMAL);
    createItem("AnimalRabbit", "Item/Surpermarket/Animal/Rabbit.png", 800, 99, ItemCategory::ANIMAL);
    createItem("AnimalSheep", "Item/Surpermarket/Animal/Sheep.png", 1000, 99, ItemCategory::ANIMAL);
    createItem("AnimalCow", "Item/Surpermarket/Animal/White_Cow.png", 800, 99, ItemCategory::ANIMAL);
}

void ItemFlyweightFactory::initializeSpringSeed() {
    createItem("Bean_Starter", "Item/Surpermarket/Spring/Bean_Starter.png", 60, 99, ItemCategory::SPRING_SEED);
    createItem("Carrot_Seeds", "Item/Surpermarket/Spring/Carrot_Seeds.png", 100, 99, ItemCategory::SPRING_SEED);
    createItem("Cauliflower_Seeds", "Item/Surpermarket/Spring/Cauliflower_Seeds.png", 100, 99, ItemCategory::SPRING_SEED);
    createItem("Coffee_Bean", "Item/Surpermarket/Spring/Coffee_Bean.png", 250, 99, ItemCategory::SPRING_SEED);
    createItem("Garlic_Seeds", "Item/Surpermarket/Spring/Garlic_Seeds.png", 40, 99, ItemCategory::SPRING_SEED);
    createItem("Jazz_Seeds", "Item/Surpermarket/Spring/Jazz_Seeds.png", 100, 99, ItemCategory::SPRING_SEED);
    createItem("Kale_Seeds", "Item/Surpermarket/Spring/Kale_Seeds.png", 70, 99, ItemCategory::SPRING_SEED);
    createItem("Parsnip_Seeds", "Item/Surpermarket/Spring/Parsnip_Seeds.png", 20, 99, ItemCategory::SPRING_SEED);
    createItem("Potato_Seeds", "Item/Surpermarket/Spring/Potato_Seeds.png", 50, 99, ItemCategory::SPRING_SEED);
    createItem("Rhubarb_Seeds", "Item/Surpermarket/Spring/Rhubarb_Seeds.png", 100, 99, ItemCategory::SPRING_SEED);
    createItem("Rice_Shoot", "Item/Surpermarket/Spring/Rice_Shoot.png", 20, 99, ItemCategory::SPRING_SEED);
    createItem("Strawberry_Seeds", "Item/Surpermarket/Spring/Strawberry_Seeds.png", 100, 99, ItemCategory::SPRING_SEED);
    createItem("Tulip_Bulb", "Item/Surpermarket/Spring/Tulip_Bulb.png", 20, 99, ItemCategory::SPRING_SEED);
}

void ItemFlyweightFactory::initializeSummerSeeds() {
    createItem("Amaranth_Seeds", "Item/Surpermarket/Summer/Amaranth_Seeds.png", 70, 99, ItemCategory::SUMMER_SEED);
    createItem("Artichoke_Seeds", "Item/Surpermarket/Summer/Artichoke_Seeds.png", 30, 99, ItemCategory::SUMMER_SEED);
    createItem("Beet_Seeds", "Item/Surpermarket/Summer/Beet_Seeds.png", 100, 99, ItemCategory::SUMMER_SEED);
    createItem("Blueberry_Seeds", "Item/Surpermarket/Summer/Blueberry_Seeds.png", 80, 99, ItemCategory::SUMMER_SEED);
    createItem("Bok_Choy_Seeds", "Item/Surpermarket/Summer/Bok_Choy_Seeds.png", 50, 99, ItemCategory::SUMMER_SEED);
    createItem("Broccoli_Seeds", "Item/Surpermarket/Summer/Broccoli_Seeds.png", 80, 99, ItemCategory::SUMMER_SEED);
    createItem("Corn_Seeds", "Item/Surpermarket/Summer/Corn_Seeds.png", 100, 99, ItemCategory::SUMMER_SEED);
    createItem("Cranberry_Seeds", "Item/Surpermarket/Summer/Cranberry_Seeds.png", 240, 99, ItemCategory::SUMMER_SEED);
    createItem("Eggplant_Seeds", "Item/Surpermarket/Summer/Eggplant_Seeds.png", 20, 99, ItemCategory::SUMMER_SEED);
    createItem("Fairy_Seeds", "Item/Surpermarket/Summer/Fairy_Seeds.png", 100, 99, ItemCategory::SUMMER_SEED);
    createItem("Grape_Starter", "Item/Surpermarket/Summer/Grape_Starter.png", 60, 99, ItemCategory::SUMMER_SEED);
    createItem("Hops_Starter", "Item/Surpermarket/Summer/Hops_Starter.png", 100, 99, ItemCategory::SUMMER_SEED);
    createItem("Melon_Seeds", "Item/Surpermarket/Summer/Melon_Seeds.png", 80, 99, ItemCategory::SUMMER_SEED);
    createItem("Pepper_Seeds", "Item/Surpermarket/Summer/Pepper_Seeds.png", 40, 99, ItemCategory::SUMMER_SEED);
    createItem("Poppy_Seeds", "Item/Surpermarket/Summer/Poppy_Seeds.png", 100, 99, ItemCategory::SUMMER_SEED);
    createItem("Pumpkin_Seeds", "Item/Surpermarket/Summer/Pumpkin_Seeds.png", 100, 99, ItemCategory::SUMMER_SEED);
    createItem("Radish_Seeds", "Item/Surpermarket/Summer/Radish_Seeds.png", 40, 99, ItemCategory::SUMMER_SEED);
    createItem("Red_Cabbage_Seeds", "Item/Surpermarket/Summer/Red_Cabbage_Seeds.png", 100, 99, ItemCategory::SUMMER_SEED);
    createItem("Spangle_Seeds", "Item/Surpermarket/Summer/Spangle_Seeds.png", 100, 99, ItemCategory::SUMMER_SEED);
    createItem("Starfruit_Seeds", "Item/Surpermarket/Summer/Starfruit_Seeds.png", 400, 99, ItemCategory::SUMMER_SEED);
    createItem("Summer_Squash_Seeds", "Item/Surpermarket/Summer/Summer_Squash_Seeds.png", 50, 99, ItemCategory::SUMMER_SEED);
    createItem("Sunflower_Seeds", "Item/Surpermarket/Summer/Sunflower_Seeds.png", 200, 99, ItemCategory::SUMMER_SEED);
    createItem("Tomato_Seeds", "Item/Surpermarket/Summer/Tomato_Seeds.png", 50, 99, ItemCategory::SUMMER_SEED);
    createItem("Wheat_Seeds", "Item/Surpermarket/Summer/Wheat_Seeds.png", 100, 99, ItemCategory::SUMMER_SEED);
    createItem("Yam_Seeds", "Item/Surpermarket/Summer/Yam_Seeds.png", 60, 99, ItemCategory::SUMMER_SEED);
}

void ItemFlyweightFactory::initializeWinterSeeds() {
    createItem("Powdermelon_Seeds", "Item/Surpermarket/Summer/Powdermelon_Seeds.png", 1, 99, ItemCategory::WINTER_SEED);
}

void ItemFlyweightFactory::initializeTools() {
    createItem("Backpack_36", "Item/Surpermarket/Tools/36_Backpack.png", 2000, 99, ItemCategory::TOOL);
    createItem("Backpack", "Item/Surpermarket/Tools/Backpack.png", 2000, 99, ItemCategory::TOOL);
    createItem("Advanced_Iridium_Rod", "Item/Surpermarket/Tools/Advanced_Iridium_Rod.png", 2000, 99, ItemCategory::TOOL);
    createItem("Axe", "Item/Surpermarket/Tools/Axe.png", 200, 99, ItemCategory::TOOL);
    createItem("Bamboo_Pole", "Item/Surpermarket/Tools/Bamboo_Pole.png", 200, 99, ItemCategory::TOOL);
    createItem("Copper_Axe", "Item/Surpermarket/Tools/Copper_Axe.png", 900, 99, ItemCategory::TOOL);
    createItem("Copper_Hoe", "Item/Surpermarket/Tools/Copper_Hoe.png", 900, 99, ItemCategory::TOOL);
    createItem("Copper_Pan", "Item/Surpermarket/Tools/Copper_Pan.png", 200, 99, ItemCategory::TOOL);
    createItem("Copper_Pickaxe", "Item/Surpermarket/Tools/Copper_Pickaxe.png", 900, 99, ItemCategory::TOOL);
    createItem("Copper_Watering", "Item/Surpermarket/Tools/Copper_Watering.png", 900, 99, ItemCategory::TOOL);
    createItem("Fiberglass_Rod", "Item/Surpermarket/Tools/Fiberglass_Rod.png", 1800, 99, ItemCategory::TOOL);
    createItem("Gold_Axe", "Item/Surpermarket/Tools/Gold_Axe.png", 4000, 99, ItemCategory::TOOL);
    createItem("Gold_Hoe", "Item/Surpermarket/Tools/Gold_Hoe.png", 4000, 99, ItemCategory::TOOL);
    createItem("Gold_Pan", "Item/Surpermarket/Tools/Gold_Pan.png", 2000, 99, ItemCategory::TOOL);
    createItem("Gold_Pickaxe", "Item/Surpermarket/Tools/Gold_Pickaxe.png", 4000, 99, ItemCategory::TOOL);
    createItem("Gold_Watering_Can", "Item/Surpermarket/Tools/Gold_Watering_Can.png", 4000, 99, ItemCategory::TOOL);
    createItem("Golden_Scythe", "Item/Surpermarket/Tools/Golden_Scythe.png", 1000, 99, ItemCategory::TOOL);
    createItem("Hoe", "Item/Surpermarket/Tools/Hoe.png", 200, 99, ItemCategory::TOOL);
    createItem("Pickaxe", "Item/Surpermarket/Tools/Pickaxe.png", 200, 99, ItemCategory::TOOL);
    createItem("Milk_Pail", "Item/Surpermarket/Tools/Milk_Pail.png", 200, 99, ItemCategory::TOOL);
    createItem("Scythe", "Item/Surpermarket/Tools/Scythe.png", 200, 99, ItemCategory::TOOL);
    createItem("Steel_Axe", "Item/Surpermarket/Tools/Steel_Axe.png", 2000, 99, ItemCategory::TOOL);
    createItem("Steel_Hoe", "Item/Surpermarket/Tools/Steel_Hoe.png", 2000, 99, ItemCategory::TOOL);
    createItem("Steel_Pan", "Item/Surpermarket/Tools/Steel_Pan.png", 2000, 99, ItemCategory::TOOL);
    createItem("Steel_Pickaxe", "Item/Surpermarket/Tools/Steel_Pickaxe.png", 2000, 99, ItemCategory::TOOL);
    createItem("Steel_Watering", "Item/Surpermarket/Tools/Steel_Watering.png", 2000, 99, ItemCategory::TOOL);
    createItem("Trash_Can_Copper", "Item/Surpermarket/Tools/Trash_Can_Copper.png", 250, 99, ItemCategory::TOOL);
    createItem("Watering_Can", "Item/Surpermarket/Tools/Watering_Can.png", 200, 99, ItemCategory::TOOL);
    createItem("Trash_Can_Steel", "Item/Surpermarket/Tools/Trash_Can_Steel.png", 500, 99, ItemCategory::TOOL);
}

void ItemFlyweightFactory::initializeTreeSaplings() {
    createItem("Apple_Sapling", "Item/Surpermarket/Tree/Apple_Sapling.png", 1000, 99, ItemCategory::TREE_SAPLING);
    createItem("Apricot_Sapling", "Item/Surpermarket/Tree/Apricot_Sapling.png", 2000, 99, ItemCategory::TREE_SAPLING);
    createItem("Banana_Sapling", "Item/Surpermarket/Tree/Banana_Sapling.png", 2000, 99, ItemCategory::TREE_SAPLING);
    createItem("Cherry_Sapling", "Item/Surpermarket/Tree/Cherry_Sapling.png", 3000, 99, ItemCategory::TREE_SAPLING);
    createItem("Mango_Sapling", "Item/Surpermarket/Tree/Mango_Sapling.png", 2000, 99, ItemCategory::TREE_SAPLING);
    createItem("Orange_Sapling", "Item/Surpermarket/Tree/Orange_Sapling.png", 2000, 99, ItemCategory::TREE_SAPLING);
    createItem("Peach_Sapling", "Item/Surpermarket/Tree/Peach_Sapling.png", 3000, 99, ItemCategory::TREE_SAPLING);
    createItem("Pomegranate_Sapling", "Item/Surpermarket/Tree/Pomegranate_Sapling.png", 3000, 99, ItemCategory::TREE_SAPLING);
}

void ItemFlyweightFactory::initializeGems() {
    createItem("Amethyst", "Ore/Amethyst3.png", 1000, 99, ItemCategory::GEM);
    createItem("Emerald", "Ore/Emerald3.png", 1000, 99, ItemCategory::GEM);
    createItem("Ruby", "Ore/Ruby3.png", 1000, 99, ItemCategory::GEM);
}

void ItemFlyweightFactory::initializeMatureCrops() {
    createItem("wheat", "crop/wheat4.png", 1000, 99, ItemCategory::MATURE_CROP);
    createItem("corn", "crop/corn4.png", 1000, 99, ItemCategory::MATURE_CROP);
    createItem("potato", "crop/Potato4.png", 1000, 99, ItemCategory::MATURE_CROP);
    createItem("pumpkin", "crop/Pumpkin4.png", 1000, 99, ItemCategory::MATURE_CROP);
    createItem("blueberry", "crop/blueberry4.png", 1000, 99, ItemCategory::MATURE_CROP);
    createItem("wood", "tree/wood.png", 1000, 99, ItemCategory::MATURE_CROP);
}

void ItemFlyweightFactory::initializeSpecialItems() {
    createItem("GoldMask", "Item/Object/goldmask.png", 10000, 99, ItemCategory::SPECIAL);
    createItem("RainBow", "Item/Object/rainbow.png", 10000, 99, ItemCategory::SPECIAL);
}
