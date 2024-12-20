#include "EnterScene2.h"
#include "SimpleAudioEngine.h"
#include "ChooseLevel.h"
#include "GameMap2.h"
USING_NS_CC;
using namespace CocosDenshion;
using namespace cocos2d::ui;

extern int is_Pass[10];

cocos2d::Scene* EnterScene2::createScene()
{
	return EnterScene2::create();
}

static void problemLoading(const char* filename)
{
	printf("Error while loading:%s\n", filename);
}

bool EnterScene2::init()
{
	if (!InitUI()) return false;

	return true;
}

bool EnterScene2::InitUI()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	// 添加背景
	auto background = Sprite::create("EnterScene/enterscene02.jpg");
	background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	
	// 计算缩放比例，以适应屏幕
	float scaleX = visibleSize.width / background->getContentSize().width;
	float scaleY = visibleSize.height / background->getContentSize().height;

	// 设置精灵的缩放
	background->setScale(scaleX, scaleY);
	this->addChild(background);

	//添加返回上一界面按钮
	auto returnBtn = Button::create("EnterScene/return1.png", "EnterScene/return1.png", "null.png");
	returnBtn->setPosition(Vec2(origin.x + 45, origin.y + 607));
	returnBtn->setPressedActionEnabled(true);
	this->addChild(returnBtn, 1);

	//触摸事件
	returnBtn->addTouchEventListener([](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
			case ui::Widget::TouchEventType::BEGAN:
			{
				SimpleAudioEngine::getInstance()->playEffect("Music/Select.mp3", false, 1.0f, 1.0f, 1.0f);
				break;
			}
			case ui::Widget::TouchEventType::ENDED:
			{
				 //跳转场景
				auto chooseLevel = ChooseLevel::createScene();
				Director::getInstance()->replaceScene(chooseLevel);
				break;
			}
			default:
				break;
		}
		});

	//添加开始游戏按钮
	auto startBtn = Button::create("EnterScene/startbtn1.png", "EnterScene/startbtn2.png", "null.png");
	startBtn->setPosition(Vec2(origin.x + 570, origin.y + 75));
	startBtn->setPressedActionEnabled(true);
	this->addChild(startBtn, 1);

	//触摸事件
	startBtn->addTouchEventListener([](Ref* sender, Widget::TouchEventType type) {
		switch (type)
		{
			case ui::Widget::TouchEventType::BEGAN:
			{
				SimpleAudioEngine::getInstance()->playEffect("/Music/Select.mp3", false, 1.0f, 1.0f, 1.0f);
				break;
			}
			case ui::Widget::TouchEventType::ENDED:
			{
				 //跳转场景
				auto GameMap = GameMap2::createScene();
				Director::getInstance()->replaceScene(GameMap);
				break;
			}
			default:
				break;
		}
		});

	return true;
}