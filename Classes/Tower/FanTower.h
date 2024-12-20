#pragma once

#ifndef __FAN_TOWER_H__
#define __FAN_TOWER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Tower/Tower.h"

class FanTower : public Tower
{
public:
    // 创建 FanTower
    static FanTower* create(const std::string& towerImage);

    // 构造函数
    FanTower();

    // 初始化 FanTower
    virtual bool init(const std::string& towerImage) override;

    // 重写攻击行为
    virtual void performAttack() override;
    void update(float d);
};

#endif  __FAN_TOWER_H__
