#include "Classes\BulletClasses\snow.h"
#include "Classes\Monster\My_monster.h"

bool SnowFlowerBullet::initSnowBullet(int grade)
{
	if (!Bullet::init()) return false;
	setAnchorPoint(Vec2(0.5, 0.5));

	myGrade = grade;

	// ���ù�����Χ
	float attackRangeMultiplier[] = { 1.5, 1.8, 2.5 };
	setAttackRange(40 * attackRangeMultiplier[std::min(myGrade - 1, 2)] + 20);

	speed = (grade - 1) * 40 + 100;
	attackDamage = 4 + (grade - 1) * 2;
	_freeze = -10 * grade;
	bulletSprite->initWithFile("Bullets/SnowBullets/snowbullet.png");

	return true;
}

void SnowFlowerBullet::spreadSpriteToScope()
{
	//��ɢ�ľ���
	const float distance = attackRange;

	//��������ʱ��
	const float durTime = (distance / speed) * 100;

	//���þ������ɢ����
	spreadTo = ScaleTo::create(durTime, 100 * distance / 20.0f);
}

void SnowFlowerBullet::inputBulletAction(Point towerLoc)
{
	src = towerLoc;			//���õ�λ��
	this->setScale(0.3);
	this->setPosition(src);	//��ʼ��λ
	this->spreadSpriteToScope();
}

void SnowFlowerBullet::spread()
{
	bulletSprite->runAction(spreadTo);

	this->schedule(CC_SCHEDULE_SELECTOR(SnowFlowerBullet::checkCollision), 0.5f);
}

void SnowFlowerBullet::checkCollision(float dt)
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
			monster->setFreeze(1);
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