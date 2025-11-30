#pragma once
//quitUI.h
//ÍË³ö½çÃæ
#include "cocos2d.h"  
#include "AppDelegate.h"
#include "InputManager.h"
#include "UICommand.h"
#include <memory>

class quitUI : public cocos2d::Layer {
public:
    virtual bool init ( std::string sceneName );
    virtual ~quitUI();

    static quitUI* create ( std::string sceneName );

private:
    // Command Pattern??
    std::shared_ptr<CloseUICommand> escCloseCommand;
    
    void close ();

    void backgroundcreate ();

    void Buttons_switching ();

    void updateCoordinate ( float& x , float& y );

    void quitAndsetting ();

    std::string SceneName;
};