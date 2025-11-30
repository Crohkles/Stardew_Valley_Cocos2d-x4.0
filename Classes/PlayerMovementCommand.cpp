#include "PlayerMovementCommand.h"
#include "Player.h"

// ==================== PlayerMovementCommand 基类实现 ====================

PlayerMovementCommand::PlayerMovementCommand(Player* p) : player(p) {
}

bool PlayerMovementCommand::canExecute() const {
    return player != nullptr && player->getParent() != nullptr;
}

// ==================== MoveUpCommand 实现 ====================

MoveUpCommand::MoveUpCommand(Player* p) : PlayerMovementCommand(p) {
}

void MoveUpCommand::execute() {
    if (canExecute()) {
        player->startMove(3);  // 3 = up direction
    }
}

void MoveUpCommand::undo() {
    if (canExecute()) {
        player->stopMove(3);  // 3 = up direction
    }
}

std::string MoveUpCommand::getDescription() const {
    return "MoveUp";
}

// ==================== MoveDownCommand 实现 ====================

MoveDownCommand::MoveDownCommand(Player* p) : PlayerMovementCommand(p) {
}

void MoveDownCommand::execute() {
    if (canExecute()) {
        player->startMove(0);  // 0 = down direction
    }
}

void MoveDownCommand::undo() {
    if (canExecute()) {
        player->stopMove(0);  // 0 = down direction
    }
}

std::string MoveDownCommand::getDescription() const {
    return "MoveDown";
}

// ==================== MoveLeftCommand 实现 ====================

MoveLeftCommand::MoveLeftCommand(Player* p) : PlayerMovementCommand(p) {
}

void MoveLeftCommand::execute() {
    if (canExecute()) {
        player->startMove(1);  // 1 = left direction
    }
}

void MoveLeftCommand::undo() {
    if (canExecute()) {
        player->stopMove(1);  // 1 = left direction
    }
}

std::string MoveLeftCommand::getDescription() const {
    return "MoveLeft";
}

// ==================== MoveRightCommand 实现 ====================

MoveRightCommand::MoveRightCommand(Player* p) : PlayerMovementCommand(p) {
}

void MoveRightCommand::execute() {
    if (canExecute()) {
        player->startMove(2);  // 2 = right direction
    }
}

void MoveRightCommand::undo() {
    if (canExecute()) {
        player->stopMove(2);  // 2 = right direction
    }
}

std::string MoveRightCommand::getDescription() const {
    return "MoveRight";
}

// ==================== 停止移动命令 ====================

StopMoveUpCommand::StopMoveUpCommand(Player* p) : PlayerMovementCommand(p) {
}

void StopMoveUpCommand::execute() {
    if (canExecute()) {
        player->stopMove(3);
    }
}

void StopMoveUpCommand::undo() {
    if (canExecute()) {
        player->startMove(3);  // restart up movement
    }
}

std::string StopMoveUpCommand::getDescription() const {
    return "StopMoveUp";
}

StopMoveDownCommand::StopMoveDownCommand(Player* p) : PlayerMovementCommand(p) {
}

void StopMoveDownCommand::execute() {
    if (canExecute()) {
        player->stopMove(0);
    }
}

void StopMoveDownCommand::undo() {
    if (canExecute()) {
        player->startMove(0);  // restart down movement
    }
}

std::string StopMoveDownCommand::getDescription() const {
    return "StopMoveDown";
}

StopMoveLeftCommand::StopMoveLeftCommand(Player* p) : PlayerMovementCommand(p) {
}

void StopMoveLeftCommand::execute() {
    if (canExecute()) {
        player->stopMove(1);
    }
}

void StopMoveLeftCommand::undo() {
    if (canExecute()) {
        player->startMove(1);  // restart left movement
    }
}

std::string StopMoveLeftCommand::getDescription() const {
    return "StopMoveLeft";
}

StopMoveRightCommand::StopMoveRightCommand(Player* p) : PlayerMovementCommand(p) {
}

void StopMoveRightCommand::execute() {
    if (canExecute()) {
        player->stopMove(2);
    }
}

void StopMoveRightCommand::undo() {
    if (canExecute()) {
        player->startMove(2);  // restart right movement
    }
}

std::string StopMoveRightCommand::getDescription() const {
    return "StopMoveRight";
}