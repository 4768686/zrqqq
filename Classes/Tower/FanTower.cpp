#include "Classes\Tower\FanTower.h"
#include "Classes\BulletClasses\fan.h"
#include"Classes\Monster\My_monster.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

FanTower::FanTower() 
{

}

FanTower* FanTower::create(const std::string& towerImage) 
{
    FanTower* tower = new FanTower();
    if (tower && tower->init(towerImage)) {
        tower->autorelease();
        return tower;
    }
    else {
        delete tower;
        return nullptr;
    }
}

bool FanTower::init(const std::string& towerImage) 
{
    if (!Tower::init(towerImage))
        return false;

    schedule([=](float dt) {
        this->update(dt);
        }, "update_key");

    return true;
}

void FanTower::performAttack()
{
    my_pos = getPosition();//����ҵ�λ��
}

void FanTower::update(float d) {
    if (isTargetInRange()) {
        attackTimer += d;
        performAttack();
        if (attackTimer >= attackInterval) {
            attackTimer = 0.0f;  // ���ü�ʱ��
            SimpleAudioEngine::getInstance()->playEffect("Music/TowerMusic/Fan.mp3");
            auto bulletSprite1 = fanBullet::create();
            this->addChild(bulletSprite1, -1);
            bulletSprite1->initfanBullet(level);

            // ��ȡ�ӵ���������Ŀ���ľֲ�����
            auto bulletStartPos = this->convertToNodeSpace(my_pos);
            auto bulletTargetPos = this->convertToNodeSpace(target_pos);
            bulletSprite1->inputBulletAction(bulletStartPos, bulletTargetPos);
            bulletSprite1->shoot();

        }
    }
}
