#include "ShitTower.h"
#include"..\Classes\Monster\My_monster.h"
#include"..\Classes\BulletClasses\shit.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

ShitTower::ShitTower()
{

}

ShitTower* ShitTower::create(const std::string& towerImage)
{
    ShitTower* tower = new ShitTower();
    if (tower && tower->init(towerImage)){
        tower->autorelease();
        return tower;
    }
    else{
        delete tower;
        return nullptr;
    }
}


bool ShitTower::init(const std::string& towerImage)
{
    if (!Tower::init(towerImage))
        return false;

    schedule([=](float dt) {
        this->update(dt);
        }, "update_key");

    return true;
}

void ShitTower::performAttack()
{
    my_pos = getPosition();//获得我的位置
}

void ShitTower::update(float d)
{
    if (isTargetInRange()) {
        attackTimer += d;
        performAttack();
       
        if (attackTimer >= attackInterval) {
            attackTimer = 0.0f;  // 重置计时器
            
            SimpleAudioEngine::getInstance()->playEffect("Music/TowerMusic/Shit.mp3");
            auto bulletSprite1 = shitBullet::create();
            this->addChild(bulletSprite1, -1);
            bulletSprite1->initshitBullet(level);

            // 获取子弹发射起点和目标点的局部坐标
            auto bulletStartPos = this->convertToNodeSpace(my_pos);
            auto bulletTargetPos = this->convertToNodeSpace(target_pos);
            bulletSprite1->inputBulletAction(bulletStartPos, bulletTargetPos);
            bulletSprite1->shoot();

        }
    }
}