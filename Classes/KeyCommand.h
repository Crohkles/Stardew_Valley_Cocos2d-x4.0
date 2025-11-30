#ifndef __KEY_COMMAND_H__
#define __KEY_COMMAND_H__

#include "cocos2d.h"
#include <memory>

USING_NS_CC;

/**
 * @brief 键盘命令基础接口
 * 
 * 这是Command Pattern的核心接口，所有具体的键盘操作命令都需要继承此接口。
 * 提供了execute()执行命令、undo()撤销命令、canExecute()条件检查等基础方法。
 */
class KeyCommand {
public:
    virtual ~KeyCommand() = default;
    
    /**
     * @brief 执行命令
     * 
     * 具体的命令执行逻辑，由子类实现
     */
    virtual void execute() = 0;
    
    /**
     * @brief 撤销命令
     * 
     * 撤销命令的执行结果，为可能的撤销/重做功能预留
     * 某些命令（如场景切换）可能不支持撤销，可以提供空实现
     */
    virtual void undo() = 0;
    
    /**
     * @brief 检查是否可以执行命令
     * 
     * 在执行命令前进行条件检查，例如：
     * - 玩家是否有足够的体力
     * - 是否在正确的位置
     * - 是否持有必要的物品等
     * 
     * @return true 如果可以执行命令
     * @return false 如果不能执行命令
     */
    virtual bool canExecute() const { return true; }
    
    /**
     * @brief 获取命令描述
     * 
     * 用于调试和日志记录
     * @return 命令的描述字符串
     */
    virtual std::string getDescription() const { return "KeyCommand"; }
};

#endif // __KEY_COMMAND_H__