#pragma once

#ifndef __SNOW_TOWER_H__
#define __SNOW_TOWER_H__

#include "Classes\Tower\Tower.h"

class SnowTower : public Tower
{
public:
    // ��������
    static SnowTower* create(const std::string& towerImage);

    // ���캯��
    SnowTower();

    // ��ʼ������
    virtual bool init(const std::string& towerImage) override;

    // ��д����Ĺ�����Ϊ
    virtual void performAttack() override;
    void update(float d);

};

#endif  __SNOW_TOWER_H__
#pragma once
