#include "TaskManagement.h"
#include "EconomicSystem.h"

TaskManagement::TaskManagement () {}

// 创建任务
void TaskManagement::createTask ( const Task& task ) {
	tasks.push_back ( task );
}

// 添加已经接受的任务
void TaskManagement::AddAcceptTask ( const Task& task ) {
    acceptTasks.push_back ( task );
}

void TaskManagement::completeTask(const std::string& task_name) {
    auto it = std::find_if(acceptTasks.begin(), acceptTasks.end(),
        [&task_name](const Task& task) {
            return task.name == task_name;
        });

    if (it != acceptTasks.end()) {
        const Task& task = *it;

        // 奖励处理
        if (task.type == NPC_TASK || task.type == SYSTEM_TASK) {
            int reward = (task.rewardCoins > 0) ? task.rewardCoins : 400;
            
            // 使用单例发放金币奖励 -> 触发 Observer -> 更新 UI
            EconomicSystem::getInstance()->addGold(reward);
            
            // 移除任务物品
            if (inventory) {
                for (const auto& requiredItem : task.requiredItems) {
                    inventory->RemoveItem(requiredItem, 1);
                }
            }
        }

        if (task.type == NPC_TASK) {
            npc_relationship->increaseRelationship("player", task.npcName, task.relationshipPoints);
        }
        else if (task.type == FESTIVAL_TASK) {
            if (inventory) {
                for (const Item& item : task.specialRewards) {
                    inventory->AddItem(item);
                }
            }
            // 增加好感度逻辑
            npc_relationship->increaseRelationship("player", "Alex", task.relationshipPoints);
            npc_relationship->increaseRelationship("player", "Abigail", task.relationshipPoints);
            npc_relationship->increaseRelationship("player", "Caroline", task.relationshipPoints);
            npc_relationship->increaseRelationship("player", "Elliott", task.relationshipPoints);
            npc_relationship->increaseRelationship("player", "Emily", task.relationshipPoints);
        }

        acceptTasks.erase(it);
    }
}


//任务接受后从发布任务中删除
void TaskManagement::DeleteAcceptTask ( const Task& task ) {
    // 从 Tasks 中删除任务
    tasks.erase ( std::remove_if ( tasks.begin () , tasks.end () ,
        [&task]( const Task& t ) {
            return t.name == task.name && t.npcName == task.npcName && t.type == task.type;  // 根据多个条件进行匹配
        } ) ,
        tasks.end () );
}

// 返回物品对应的任务
std::string TaskManagement::findTaskByRequiredItems ( const std::string& itemName ) {
    // 遍历acceptTasks寻找包含指定物品的任务
    for (const auto& task : acceptTasks) {
        for (const auto& item : task.requiredItems) {
            if (item.GetName() == itemName) {
                return task.name; // 找到匹配的任务，返回任务名称
            }
        }
    }

    // 如果没有找到，抛出异常
    // throw std::runtime_error ( "No task found with the required item." );
    return "";
}