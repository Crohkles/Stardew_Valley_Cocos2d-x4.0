#ifndef __PLAYER_MOVEMENT_COMMAND_H__
#define __PLAYER_MOVEMENT_COMMAND_H__

#include "KeyCommand.h"

// 前向声明，避免循环包含
class Player;

/**
 * @brief 移动方向枚举
 */
enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

/**
 * @brief 玩家移动命令基类
 * 
 * 为所有与玩家移动相关的命令提供公共基础
 */
class PlayerMovementCommand : public KeyCommand {
protected:
    Player* player;
    
public:
    PlayerMovementCommand(Player* p);
    virtual ~PlayerMovementCommand() = default;
    
    bool canExecute() const override;
};

/**
 * @brief 向上移动命令
 */
class MoveUpCommand : public PlayerMovementCommand {
public:
    MoveUpCommand(Player* p);
    void execute() override;
    void undo() override;
    std::string getDescription() const override;
};

/**
 * @brief 向下移动命令
 */
class MoveDownCommand : public PlayerMovementCommand {
public:
    MoveDownCommand(Player* p);
    void execute() override;
    void undo() override;
    std::string getDescription() const override;
};

/**
 * @brief 向左移动命令
 */
class MoveLeftCommand : public PlayerMovementCommand {
public:
    MoveLeftCommand(Player* p);
    void execute() override;
    void undo() override;
    std::string getDescription() const override;
};

/**
 * @brief 向右移动命令
 */
class MoveRightCommand : public PlayerMovementCommand {
public:
    MoveRightCommand(Player* p);
    void execute() override;
    void undo() override;
    std::string getDescription() const override;
};

/**
 * @brief 停止向上移动命令
 */
class StopMoveUpCommand : public PlayerMovementCommand {
public:
    StopMoveUpCommand(Player* p);
    void execute() override;
    void undo() override;
    std::string getDescription() const override;
};

/**
 * @brief 停止向下移动命令
 */
class StopMoveDownCommand : public PlayerMovementCommand {
public:
    StopMoveDownCommand(Player* p);
    void execute() override;
    void undo() override;
    std::string getDescription() const override;
};

/**
 * @brief 停止向左移动命令
 */
class StopMoveLeftCommand : public PlayerMovementCommand {
public:
    StopMoveLeftCommand(Player* p);
    void execute() override;
    void undo() override;
    std::string getDescription() const override;
};

/**
 * @brief 停止向右移动命令
 */
class StopMoveRightCommand : public PlayerMovementCommand {
public:
    StopMoveRightCommand(Player* p);
    void execute() override;
    void undo() override;
    std::string getDescription() const override;
};

#endif // __PLAYER_MOVEMENT_COMMAND_H__