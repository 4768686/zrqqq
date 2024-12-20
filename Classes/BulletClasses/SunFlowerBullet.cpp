#include "Classes\BulletClasses\SunFlowerBullet.h"
#include "Classes\Monster\My_monster.h"

bool SunFlowerBullet::initSunBullet(int grade)
{
	if (!Bullet::init()) return false;
	setAnchorPoint(Vec2(0.5, 0.5));

	myGrade = grade;

	// ���ù�����Χ
	float attackRangeMultiplier[] = { 1.5, 1.8, 2.5 };
	setAttackRange(40 * attackRangeMultiplier[std::min(myGrade - 1, 2)] + 20);

	speed = (grade - 1) * 40 + 100;
	attackDamage = 6 + (grade - 1) * 2;
	bulletSprite->initWithFile("Bullets/SunFlowerBullets/TSun-hd11.png");

	return true;
}

void SunFlowerBullet::spreadSpriteToScope() 
{
	//��ɢ�ľ���
	const float distance = attackRange;

	//��������ʱ��
	const float durTime = (distance / speed) * 100;

	//���þ������ɢ����
	spreadTo = ScaleTo::create(durTime, 100 * distance / 20.0f);
}

void SunFlowerBullet::inputBulletAction(Point towerLoc) 
{
	src = towerLoc;			//���õ�λ��
	this->setScale(0.3);
	this->setPosition(src);	//��ʼ��λ
	this->spreadSpriteToScope(); 
}

void SunFlowerBullet::spread()
{
	bulletSprite->runAction(spreadTo);

	this->schedule(CC_SCHEDULE_SELECTOR(SunFlowerBullet::checkCollision), 0.5f);
}

void SunFlowerBullet::checkCollision(float dt)
{
	bool isMissed = true;

    //��ȡλ�ã�ת��Ϊ��������
    Vec2 childAbsolutePosition = bulletSprite->getParent()->convertToWorldSpace(bulletSprite->getPosition());

    //������ײ����
	auto bulletRect = Rect(
		this->getPositionX() + this->getParent()->getPositionX() - attackRange - 48,
		this->getPositionY() + this->getParent()->getPositionY() - attackRange - 45,
		attackRange * 2, attackRange * 2);

    //�����ϰ�
    for (int j = 0; j < BarrierContainer.size(); j++) {

        auto barrier = BarrierContainer.at(j);
        auto barrierRect = barrier->getBoundingBox();

        if (barrierRect.intersectsRect(bulletRect)) {
            barrier->behurt(this->getAttackDamage(), 1);
        }
    }

    //���й���
    for (int j = 0; j < monsterContainer.size(); j++) {

        auto monster = monsterContainer.at(j);
        auto monsterRect = monster->getBoundingBox();

        if (monsterRect.intersectsRect(bulletRect)) {
            monster->beHurt(this->getAttackDamage(), 1);
        }
    }
	if (isMissed) {
		//������ʧ
		bulletSprite->runAction(Sequence::create(FadeOut::create(1.3f)
			, CallFuncN::create(CC_CALLBACK_0(Bullet::removeFromParent, this))
			, NULL));
	}
	else
		this->removeFromParent();
}