#include "Tower.h"

USING_NS_CC;


Tower::Tower()
    : state(TowerState::IDLE),
    attackInterval(0.5f),//设置的初始攻击间隔是0.5s
    attackTimer(0.0f)
{
    level = 1;
}


Tower* Tower::create(const std::string& towerImage)
{
    Tower* tower = new Tower();
    if (tower && tower->init(towerImage))
    {
        tower->autorelease();
        return tower;
    }
    CC_SAFE_DELETE(tower);
    return nullptr;
}

bool Tower::init(const std::string& towerImage)
{
    if (!Sprite::initWithFile(towerImage))
    {
        return false;
    }
    attackRange = 80 * 0.6 * (level + 3);


    return true;
}

void Tower::update(float dt)
{
    if (state == TowerState::ATTACKING)
    {
        attackTimer += dt;

        if (attackTimer >= attackInterval)
        {
            // 执行攻击动作
            performAttack();

            // 重置计时器
            attackTimer = 0.0f;
        }
    }
}

bool Tower::isTargetInRange()
{
    my_pos = getPosition();//获得我的位置

    for (auto barrier : BarrierContainer) {
        if (barrier->choosed) {
            target_pos = barrier->getPosition();
            float distance = my_pos.distance(target_pos);
            if (distance <= attackRange) {
                return true;
            }
        }
    }
    //有被选中的怪物
    for (auto target : monsterContainer) {
        if (target->choosed) {
            target_pos = target->getPosition();//获得目标位置
            float distance = my_pos.distance(target_pos);
            if (distance <= attackRange) {
                target->beShoot = 1;
                return true;
            }
        }
    }
    //什么也没有选中，就只攻击距离最近的怪物
    for (auto target : monsterContainer) {

        target_pos = target->getPosition();//获得目标位置
        float distance = my_pos.distance(target_pos);
        if (distance <= attackRange) {
            target->beShoot = 1;
            return true;
        }
    }

    return false;
}

void Tower::performAttack()
{
    // 简单地输出一条信息
    CCLOG("Tower attacks the target!");
}
