#ifndef __SCENE_BOUNDARY_FACTORY_H__
#define __SCENE_BOUNDARY_FACTORY_H__

#include <string>
#include <unordered_map>

/**
 * @brief 场景边界配置工厂类
 * 
 * 使用Factory Method模式统一管理各个场景的边界配置，
 * 消除UI类中重复的if-else边界判断逻辑。
 */
class SceneBoundaryFactory {
public:
    /**
     * @brief 场景边界配置结构
     */
    struct BoundaryConfig {
        float left;
        float right;
        float upper;
        float lower;
        
        BoundaryConfig() : left(-10000.0f), right(10000.0f), upper(10000.0f), lower(-10000.0f) {}
        BoundaryConfig(float l, float r, float u, float low) : left(l), right(r), upper(u), lower(low) {}
    };
    
    /**
     * @brief 获取指定场景的边界配置
     * @param sceneName 场景名称
     * @return BoundaryConfig 边界配置，如果场景不存在则返回默认边界
     */
    static BoundaryConfig getBoundaryConfig(const std::string& sceneName);
    
    /**
     * @brief 应用边界限制到坐标
     * @param x 要限制的x坐标（引用，会被修改）
     * @param y 要限制的y坐标（引用，会被修改）
     * @param sceneName 场景名称
     */
    static void applyBoundaryLimits(float& x, float& y, const std::string& sceneName);
    
private:
    /**
     * @brief 初始化场景边界配置
     * @return 场景边界配置映射表
     */
    static const std::unordered_map<std::string, BoundaryConfig>& getBoundaryConfigs();
    
    /**
     * @brief 默认边界配置
     */
    static const BoundaryConfig DEFAULT_BOUNDARY;
};

#endif // __SCENE_BOUNDARY_FACTORY_H__