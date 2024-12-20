#pragma once

#ifndef __BOTTLETOWER_TOWER_H__
#define __BOTTLETOWER_TOWER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Tower.h"

class BottleTower : public Tower
{
public:
    // 创建炮塔
    static BottleTower* create(const std::string& towerImage);

    // 构造函数
    BottleTower();

    // 初始化炮塔
    virtual bool init(const std::string& towerImage) override;

    // 重写父类的攻击行为
    virtual void performAttack() override;
    void update(float d);

};

#endif  __BOTTLETOWER_TOWER_H__
