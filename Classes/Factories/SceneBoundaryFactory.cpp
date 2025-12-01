#include "SceneBoundaryFactory.h"
#include <algorithm>

// 默认边界配置
const SceneBoundaryFactory::BoundaryConfig SceneBoundaryFactory::DEFAULT_BOUNDARY(-10000.0f, 10000.0f, 10000.0f, -10000.0f);

const std::unordered_map<std::string, SceneBoundaryFactory::BoundaryConfig>& 
SceneBoundaryFactory::getBoundaryConfigs() {
    static std::unordered_map<std::string, BoundaryConfig> boundaryConfigs = {
        // Town场景边界
        {"Town", BoundaryConfig(-170.0f, 1773.0f, 1498.0f, -222.0f)},
        
        // Cave场景边界
        {"Cave", BoundaryConfig(786.0f, 817.0f, 808.0f, 460.0f)},
        
        // Beach场景边界
        {"Beach", BoundaryConfig(-315.0f, 20000.0f, 920.0f, 360.0f)},
        
        // Forest场景边界
        {"Forest", BoundaryConfig(-600.0f, 2197.0f, 2200.0f, -850.0f)},
        
        // farm场景边界
        {"farm", BoundaryConfig(637.0f, 960.0f, 777.0f, 500.0f)},
        
        // Barn场景边界
        {"Barn", BoundaryConfig(805.0f, 805.0f, 569.0f, 569.0f)},
        
        // Myhouse场景边界
        {"Myhouse", BoundaryConfig(800.0f, 800.0f, 580.0f, 580.0f)},
        
        // supermarket场景边界
        {"supermarket", BoundaryConfig(743.0f, 1773.0f, -82.0f, -82.0f)}
    };
    
    return boundaryConfigs;
}

SceneBoundaryFactory::BoundaryConfig 
SceneBoundaryFactory::getBoundaryConfig(const std::string& sceneName) {
    const auto& configs = getBoundaryConfigs();
    auto it = configs.find(sceneName);
    
    if (it != configs.end()) {
        return it->second;
    }
    
    // 如果找不到对应场景，返回默认边界
    return DEFAULT_BOUNDARY;
}

void SceneBoundaryFactory::applyBoundaryLimits(float& x, float& y, const std::string& sceneName) {
    BoundaryConfig config = getBoundaryConfig(sceneName);
    
    // 应用左右边界限制
    if (x <= config.left) {
        x = config.left;
    }
    else if (x >= config.right) {
        x = config.right;
    }
    
    // 应用上下边界限制
    if (y <= config.lower) {
        y = config.lower;
    }
    else if (y >= config.upper) {
        y = config.upper;
    }
}