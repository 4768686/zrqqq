#include "EnterScene3.h"
#include "SimpleAudioEngine.h"
#include "ChooseLevel.h"
#include "GameMap3.h"

USING_NS_CC;
using namespace CocosDenshion;
using namespace cocos2d::ui;

cocos2d::Scene* EnterScene3::createScene()
{
	return EnterScene3::create();
}

static void problemLoading(const char* filename)
{
	printf("Error while loading:%s\n", filename);
}

bool EnterScene3::init()
{
	if (!InitUI()) return false;

	return true;
}

bool EnterScene3::InitUI()
{
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	
	// ���ӱ���
	auto background = Sprite::create("EnterScene/enterscene04.jpg");
	background->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	
	// �������ű���������Ӧ��Ļ
	float scaleX = visibleSize.width / background->getContentSize().width;
	float scaleY = visibleSize.height / background->getContentSize().height;

	// ���þ��������
	background->setScale(scaleX, scaleY);
	this->addChild(background);

	//���ӷ�����һ���水ť
	auto returnBtn = Button::create("EnterScene/return1.png", "EnterScene/return2.png", "null.png");
	returnBtn->setPosition(Vec2(origin.x + 45, origin.y + 607));
	returnBtn->setPressedActionEnabled(true);
	this->addChild(returnBtn, 1);

	//�����¼�
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
			//��ת����
			auto chooseLevel = ChooseLevel::createScene();
			Director::getInstance()->replaceScene(chooseLevel);
			break;
		}
		default:
			break;
		}
		});

	//���ӿ�ʼ��Ϸ��ť
	auto startBtn = Button::create("EnterScene/startbtn1.png", "EnterScene/startbtn1.png", "null.png");
	startBtn->setPosition(Vec2(origin.x + 570, origin.y + 75));
	startBtn->setPressedActionEnabled(true);
	this->addChild(startBtn, 1);

	//�����¼�
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
			auto GameMap = GameMap3::createScene();
			Director::getInstance()->replaceScene(GameMap);
			break;
		}
		default:
			break;
		}
		});

	return true;
}