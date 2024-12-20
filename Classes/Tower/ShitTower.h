#pragma once
#ifndef __SHIT_TOWER_H__
#define __SHIT_TOWER_H__

#include "Tower.h"

class ShitTower : public Tower
{
public:
    // 创建炮塔
    static ShitTower* create(const std::string& towerImage);

    // 构造函数
    ShitTower();

    // 初始化炮塔
    virtual bool init(const std::string& towerImage) override;

    // 重写父类的攻击行为
    virtual void performAttack() override;

    void update(float d);
};

#endif  __SHIT_TOWER_H__
