#pragma once
#ifndef __TOWER_H__
#define __TOWER_H__

#include "cocos2d.h"
#include"Classes\Monster\My_monster.h";

class Tower : public cocos2d::Sprite
{
public:

    // 防御塔的等级
    int level;
    //防御塔在地图数组上的位置
    int pos_i, pos_j;
    //攻击范围
    int attackRange;

    Vec2 my_pos;//炮塔的位置
    Vec2 target_pos;//攻击目标的位置

    // 防御塔的状态
    enum class TowerState
    {
        IDLE,     // 空闲状态
        ATTACKING // 攻击状态
    };TowerState state;

    // 防御塔的攻击间隔
    float attackInterval;

    // 计时器，用于控制攻击间隔
    float attackTimer;

    // 构造函数
    Tower();

    // 初始化防御塔
    virtual bool init(const std::string& towerImage);

    // 创建防御塔
    static Tower* create(const std::string& towerImage);

    // 更新防御塔的逻辑
    void update(float dt) override;

    // 检测目标是否在攻击范围内
    bool isTargetInRange();

    // 执行攻击动作
    virtual void performAttack();
};

#endif __TOWER_H__
