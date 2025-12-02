#pragma once
// StoreUI.h    
#include "cocos2d.h"  
#include "../Systems/Inventory.h"  
#include "../Core/AppDelegate.h"
#include "../Systems/EconomicSystem.h"
#include "../Core/GameStateObserver.h"

USING_NS_CC;

class StoreUI : public cocos2d::Layer, public GameStateObserver {
public:
    virtual bool init ( Inventory* mybag , Inventory* goods );
    static StoreUI* create ( Inventory* mybag , Inventory* goods );
    virtual ~StoreUI();
    
    // Observer接口实现
    virtual void onEconomicStateChanged(int newGoldAmount, int delta) override;
    virtual void onInventoryStateChanged() override;
    virtual void onEnergyStateChanged(int newEnergy, int maxEnergy) override {} // 不需要处理体力变化

    //背景图展示
    void backgroundcreate ();
    //商品展示
    void ProductDisplay ( Inventory* mybag , Inventory* goods );

    void Itemblock ( Inventory* mybag , Inventory* goods );

    void updateDisplay (); // 更新显示内容  

    void moneyDisplay ();

    void SliderDisplay ();

    void updateCoordinate ( float& x , float& y );

private:

    Inventory* _mybag; // 指向自己背包实例的指针  

    Inventory* _goods; // 指向商品实例的指针  

    cocos2d::Label* _itemLabel;  // 显示物品信息的标签 

    cocos2d::Vector<cocos2d::Sprite*> _itemSlots; // 存储物品槽的 Sprite  

    int _selectedSlot; // 当前选中的槽位  

    void onItemSlotClicked ( cocos2d::Ref* sender ); // 物品槽的点击事件处理 

    bool isClick = false;

    shared_ptr<Item> chosen_Item = nullptr;
    
    cocos2d::Label* Gold_Amount_Label = nullptr; // 金币显示标签
};
