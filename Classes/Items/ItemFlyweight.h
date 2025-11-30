#pragma once
#include "Item.h"
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

// 物品元数据结构 - 存储内在状态
struct ItemMetadata {
    std::string name;
    std::string imagePath;
    int baseValue;
    int maxStackCount;
    
    ItemMetadata(const std::string& n, const std::string& path, int value, int stack = 99)
        : name(n), imagePath(path), baseValue(value), maxStackCount(stack) {}
};

// Item的flyweight版本
class ItemFlyweight : public Item {
public:
    ItemFlyweight(const ItemMetadata& metadata);
    
    // 复制构造函数 - 保持兼容性
    ItemFlyweight(const ItemFlyweight& other);
    
    virtual ~ItemFlyweight() = default;
    
    // 重写GetCopy方法以支持flyweight
    virtual std::shared_ptr<Item> GetCopy() const override;
    
private:
    // flyweight对象只存储指向共享元数据的引用
    const ItemMetadata* metadata_;
};

// 物品类型枚举 - 用于分类管理
enum class ItemCategory {
    ANIMAL,
    SPRING_SEED,
    SUMMER_SEED,
    WINTER_SEED,
    TOOL,
    TREE_SAPLING,
    GEM,
    MATURE_CROP,
    SPECIAL
};

// Flyweight工厂类
class ItemFlyweightFactory {
public:
    // 获取单例实例
    static ItemFlyweightFactory& getInstance();
    
    // 获取flyweight对象
    std::shared_ptr<ItemFlyweight> getFlyweight(const std::string& itemName);
    
    // 预加载所有物品元数据
    void preloadAllItems();
    
    // 获取特定类别的所有物品
    std::vector<std::shared_ptr<ItemFlyweight>> getItemsByCategory(ItemCategory category);
    
private:
    ItemFlyweightFactory() = default;
    ~ItemFlyweightFactory() = default;
    
    // 禁止拷贝和赋值
    ItemFlyweightFactory(const ItemFlyweightFactory&) = delete;
    ItemFlyweightFactory& operator=(const ItemFlyweightFactory&) = delete;
    
    // 内部存储
    std::unordered_map<std::string, std::unique_ptr<ItemMetadata>> metadata_;
    std::unordered_map<std::string, std::shared_ptr<ItemFlyweight>> flyweights_;
    std::unordered_map<ItemCategory, std::vector<std::string>> categoryMap_;
    
    // 初始化方法
    void initializeAnimals();
    void initializeSpringSeed();
    void initializeSummerSeeds();
    void initializeWinterSeeds();
    void initializeTools();
    void initializeTreeSaplings();
    void initializeGems();
    void initializeMatureCrops();
    void initializeSpecialItems();
    
    // 创建flyweight的辅助方法
    void createItem(const std::string& name, const std::string& path, int value, 
                   int stack, ItemCategory category);
};
