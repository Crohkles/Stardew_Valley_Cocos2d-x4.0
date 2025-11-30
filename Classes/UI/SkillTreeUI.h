#pragma once
//SkillTreeUI.h
//人物技能树界面
#include "cocos2d.h"  
#include "AppDelegate.h"
#include "SkillTree.h"
#include "InputManager.h"
#include "UICommand.h"
#include <memory>

class SkillTreeUI : public cocos2d::Layer {
public:
    virtual bool init ( std::string sceneName );
    virtual ~SkillTreeUI();

    static SkillTreeUI* create ( std::string sceneName );



private:

    NpcRelationship* NPC_RELATIONSHIP;
    
    // Command Pattern相关
    std::shared_ptr<CloseUICommand> escCloseCommand;

    void close ();

    void SkillDisplay ( int whichSkill , Vec2 Pos , float gap );

    void backgroundcreate ();

    void Buttons_switching ();

    void updateCoordinate ( float& x , float& y );

    std::string SceneName;
};
