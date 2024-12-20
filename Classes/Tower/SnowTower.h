#pragma once

#ifndef __SNOW_TOWER_H__
#define __SNOW_TOWER_H__

#include "Classes\Tower\Tower.h"

class SnowTower : public Tower
{
public:
    // 创建炮塔
    static SnowTower* create(const std::string& towerImage);

    // 构造函数
    SnowTower();

    // 初始化炮塔
    virtual bool init(const std::string& towerImage) override;

    // 重写父类的攻击行为
    virtual void performAttack() override;
    void update(float d);

};

#endif  __SNOW_TOWER_H__
#pragma once
