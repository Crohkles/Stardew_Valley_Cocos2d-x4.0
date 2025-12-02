// CropSpritePool.h
#pragma once
#include "cocos2d.h"
#include <unordered_map>
#include <vector>
#include <string>

USING_NS_CC;

/**
 * 农场作物Sprite对象池
 * 专门用于管理农场作物的精灵对象，减少频繁创建销毁的性能开销
 */
class CropSpritePool {
public:
    static CropSpritePool* getInstance();
    
    /**
     * 获取作物Sprite（根据网格位置和图片路径）
     * @param gridNumber 网格编号
     * @param imagePath 图片路径
     * @return Sprite指针
     */
    Sprite* getCropSprite(int gridNumber, const std::string& imagePath);
    
    /**
     * 更新作物Sprite的显示（重用现有Sprite，只改变纹理）
     * @param gridNumber 网格编号
     * @param newImagePath 新的图片路径
     */
    void updateCropSprite(int gridNumber, const std::string& newImagePath);
    
    /**
     * 移除作物Sprite（返回到池中或隐藏）
     * @param gridNumber 网格编号
     */
    void removeCropSprite(int gridNumber);
    
    /**
     * 清空所有作物精灵
     */
    void clearAllCrops();
    
    /**
     * 获取当前活跃的作物精灵数量
     */
    size_t getActiveCropCount() const;
    
    /**
     * 获取回收池中的精灵数量
     */
    size_t getRecycledSpriteCount() const;

private:
    CropSpritePool() = default;
    ~CropSpritePool();
    
    // 禁止拷贝构造和赋值
    CropSpritePool(const CropSpritePool&) = delete;
    CropSpritePool& operator=(const CropSpritePool&) = delete;
    
    static CropSpritePool* _instance;
    
    // 按网格编号存储Sprite（key: 网格编号, value: Sprite指针）
    std::unordered_map<int, Sprite*> _activeSprites;
    
    // 按图片路径分类的回收池
    std::unordered_map<std::string, std::vector<Sprite*>> _recyclePool;
    
    // 每种图片类型的最大回收池大小
    static const size_t MAX_POOL_SIZE = 10;
    
    /**
     * 从回收池获取或创建新的Sprite
     * @param imagePath 图片路径
     * @return Sprite指针
     */
    Sprite* getOrCreateSprite(const std::string& imagePath);
    
    /**
     * 将Sprite返回到回收池
     * @param sprite Sprite指针
     * @param imagePath 图片路径
     */
    void recycleSprite(Sprite* sprite, const std::string& imagePath);
};
