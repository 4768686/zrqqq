#include "Classes\BulletClasses\BottleTowerBullet.h"
#include "Classes\Monster\My_monster.h"

bool GreenTowerBullet::initGreenBullet(int grade)
{
	if (!Bullet::init()) return false;

	myGrade = grade;
	speed = (grade - 1) * 200 + 1000;
	attackDamage = 10 + (grade - 1) * 2;

	bulletSprite->initWithFile("Bullets/GreenBottleBullets/PBottle" + StringUtils::toString(grade * 10 + 1) + ".png");//初始化

	//飞的过程的纹理的变化设置
	Animation* blinkAnimation = Animation::create();
	blinkAnimation->setDelayPerUnit(0.6f / 3);
	blinkAnimation->setRestoreOriginalFrame(true);	//动画执行完后返回第一帧

	Texture2D* texture1 = Director::getInstance()->getTextureCache()->addImage("Bullets/GreenBottleBullets/PBottle" + StringUtils::toString(myGrade * 10 + 1) + ".png");
	blinkAnimation->addSpriteFrameWithTexture(texture1, Rect(0, 0, 70, 70));
	Texture2D* texture2 = Director::getInstance()->getTextureCache()->addImage("Bullets/GreenBottleBullets/PBottle" + StringUtils::toString(myGrade * 10 + 2) + ".png");
	blinkAnimation->addSpriteFrameWithTexture(texture2, Rect(0, 0, 70, 70));
	Texture2D* texture3 = Director::getInstance()->getTextureCache()->addImage("Bullets/GreenBottleBullets/PBottle" + StringUtils::toString(myGrade * 10 + 3) + ".png");
	blinkAnimation->addSpriteFrameWithTexture(texture3, Rect(0, 0, 70, 70));

	bulletSprite->runAction(RepeatForever::create(Animate::create(blinkAnimation)));
	return true;

}

void GreenTowerBullet::rotateSpriteToDirection() 
{
	//方向向量
	const Vec2 direction = dst - src;

	//两点间距离
	const float distance = src.distance(dst);

	//动作持续时间
	const float durTime = distance / speed;

	// 计算角度（弧度）
	const float angle = atan2(direction.y, direction.x);

	// 将弧度转换为角度
	const float rotation = CC_RADIANS_TO_DEGREES(angle);

	// 设置精灵的旋转角度
	bulletSprite->setRotation(-rotation);

	//射击动画
	shootBy = MoveBy::create(durTime, direction);
}

void GreenTowerBullet::inputBulletAction(Point towerLoc, Point monsterLoc) {
	src = towerLoc, dst = monsterLoc;	//设置两个点
	bulletSprite->setPosition(src);		//初始定位
	this->rotateSpriteToDirection();	//计算角度制作动画
}

//射击
void GreenTowerBullet::shoot() 
{
	runAction(Sequence::create(shootBy,CallFuncN::create(CC_CALLBACK_0(GreenTowerBullet::removeBullet, this)), NULL));
}

void GreenTowerBullet::removeBullet()
{
	bool isMissed = true;

	//获取位置，转化为世界坐标
	Vec2 childAbsolutePosition = bulletSprite->getParent()->convertToWorldSpace(bulletSprite->getPosition());
	
	//构建碰撞矩形
	auto bulletRect = Rect(
		childAbsolutePosition.x - this->getContentSize().width / 2,
		childAbsolutePosition.y - this->getContentSize().height / 2,
		this->bulletSprite->getContentSize().width,
		this->bulletSprite->getContentSize().height);

	//击中障碍
	for (int j = 0; j < BarrierContainer.size(); j++) {
		auto barrier = BarrierContainer.at(j);
		auto barrierRect = barrier->getBoundingBox();

		if (barrier->choosed && barrierRect.intersectsRect(bulletRect)) {
			barrier->behurt(this->getAttackDamage(), 1);
			isMissed = false;
		}
	}

	//击中怪物
	for (int j = 0; j < monsterContainer.size(); j++) {
		auto monster = monsterContainer.at(j);
		auto monsterRect = monster->getBoundingBox();

		if (monsterRect.intersectsRect(bulletRect) && monster->beShoot == 1) {
			monster->beHurt(this->getAttackDamage(), 1);
			monster->beShoot = 0;
			isMissed = false;
		}
	}

	if (isMissed) {
		//渐渐消失
		bulletSprite->runAction(Sequence::create(FadeOut::create(0.3f),
			CallFuncN::create(CC_CALLBACK_0(Bullet::removeFromParent, this)),
			NULL));
	}
	else
		this->removeFromParent();
}