#pragma once

#ifndef __FAN_TOWER_H__
#define __FAN_TOWER_H__
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Tower/Tower.h"

class FanTower : public Tower
{
public:
    // ���� FanTower
    static FanTower* create(const std::string& towerImage);

    // ���캯��
    FanTower();

    // ��ʼ�� FanTower
    virtual bool init(const std::string& towerImage) override;

    // ��д������Ϊ
    virtual void performAttack() override;
    void update(float d);
};

#endif  __FAN_TOWER_H__
