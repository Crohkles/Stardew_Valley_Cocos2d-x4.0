// CropSpritePool.cpp
#include "CropSpritePool.h"

CropSpritePool* CropSpritePool::_instance = nullptr;

CropSpritePool* CropSpritePool::getInstance() {
    if (!_instance) {
        _instance = new CropSpritePool();
    }
    return _instance;
}

CropSpritePool::~CropSpritePool() {
    clearAllCrops();
}

Sprite* CropSpritePool::getCropSprite(int gridNumber, const std::string& imagePath) {
    // 检查该网格位置是否已有Sprite
    auto it = _activeSprites.find(gridNumber);
    if (it != _activeSprites.end()) {
        // 如果已存在，更新纹理
        Sprite* sprite = it->second;
        sprite->setTexture(imagePath);
        sprite->setVisible(true);
        CCLOG("CropSpritePool: Reusing sprite at grid %d with texture %s", gridNumber, imagePath.c_str());
        return sprite;
    }
    
    // 获取或创建新的Sprite
    Sprite* sprite = getOrCreateSprite(imagePath);
    if (sprite) {
        _activeSprites[gridNumber] = sprite;
        sprite->setVisible(true);
        CCLOG("CropSpritePool: Created new sprite at grid %d with texture %s", gridNumber, imagePath.c_str());
    }
    
    return sprite;
}

void CropSpritePool::updateCropSprite(int gridNumber, const std::string& newImagePath) {
    auto it = _activeSprites.find(gridNumber);
    if (it != _activeSprites.end()) {
        it->second->setTexture(newImagePath);
        it->second->setVisible(true);
        CCLOG("CropSpritePool: Updated sprite at grid %d to texture %s", gridNumber, newImagePath.c_str());
    }
    else {
        CCLOG("CropSpritePool: Warning - tried to update non-existent sprite at grid %d", gridNumber);
    }
}

void CropSpritePool::removeCropSprite(int gridNumber) {
    auto it = _activeSprites.find(gridNumber);
    if (it != _activeSprites.end()) {
        Sprite* sprite = it->second;
        
        // 获取当前纹理路径用于分类回收
        std::string texturePath = "unknown";
        if (sprite->getTexture()) {
            texturePath = sprite->getTexture()->getPath();
        }
        
        // 隐藏并回收
        sprite->setVisible(false);
        sprite->removeFromParent();
        recycleSprite(sprite, texturePath);
        
        _activeSprites.erase(it);
        CCLOG("CropSpritePool: Removed sprite from grid %d", gridNumber);
    }
    else {
        CCLOG("CropSpritePool: Warning - tried to remove non-existent sprite at grid %d", gridNumber);
    }
}

Sprite* CropSpritePool::getOrCreateSprite(const std::string& imagePath) {
    // 尝试从回收池获取
    auto poolIt = _recyclePool.find(imagePath);
    if (poolIt != _recyclePool.end() && !poolIt->second.empty()) {
        Sprite* sprite = poolIt->second.back();
        poolIt->second.pop_back();
        
        // 重置基本状态
        sprite->setPosition(Vec2::ZERO);
        sprite->setScale(1.0f);
        sprite->setRotation(0.0f);
        sprite->setOpacity(255);
        sprite->setVisible(true);
        
        CCLOG("CropSpritePool: Retrieved sprite from recycle pool for %s", imagePath.c_str());
        return sprite;
    }
    
    // 创建新的Sprite
    Sprite* sprite = Sprite::create(imagePath);
    if (sprite) {
        CCLOG("CropSpritePool: Created new sprite for %s", imagePath.c_str());
    }
    else {
        CCLOG("CropSpritePool: Error - failed to create sprite for %s", imagePath.c_str());
    }
    return sprite;
}

void CropSpritePool::recycleSprite(Sprite* sprite, const std::string& imagePath) {
    if (!sprite) return;
    
    // 重置状态
    sprite->setPosition(Vec2::ZERO);
    sprite->setScale(1.0f);
    sprite->setRotation(0.0f);
    sprite->setOpacity(255);
    sprite->setVisible(false);
    sprite->removeFromParent();
    
    // 限制回收池大小（防止内存过度使用）
    auto& pool = _recyclePool[imagePath];
    if (pool.size() < MAX_POOL_SIZE) {
        sprite->retain(); // 防止被自动释放
        pool.push_back(sprite);
        CCLOG("CropSpritePool: Recycled sprite for %s (pool size: %zu)", imagePath.c_str(), pool.size());
    }
    else {
        CCLOG("CropSpritePool: Pool full for %s, letting sprite auto-release", imagePath.c_str());
    }
}

void CropSpritePool::clearAllCrops() {
    CCLOG("CropSpritePool: Clearing all crops...");
    
    // 清空活跃精灵
    for (auto& pair : _activeSprites) {
        if (pair.second) {
            pair.second->removeFromParent();
        }
    }
    _activeSprites.clear();
    
    // 清空回收池
    size_t totalReleased = 0;
    for (auto& pool : _recyclePool) {
        for (Sprite* sprite : pool.second) {
            sprite->release();
            totalReleased++;
        }
        pool.second.clear();
    }
    _recyclePool.clear();
    
    CCLOG("CropSpritePool: Cleared %zu active sprites and released %zu recycled sprites", 
          _activeSprites.size(), totalReleased);
}

size_t CropSpritePool::getActiveCropCount() const {
    return _activeSprites.size();
}

size_t CropSpritePool::getRecycledSpriteCount() const {
    size_t total = 0;
    for (const auto& pool : _recyclePool) {
        total += pool.second.size();
    }
    return total;
}
