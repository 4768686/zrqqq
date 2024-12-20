#include "MainScene.h"
#include "SimpleAudioEngine.h"
#include "ChooseLevel.h"
USING_NS_CC;
using namespace CocosDenshion;
using namespace cocos2d::ui;

//场景创建
cocos2d::Scene* MainScene::createScene()
{
	return MainScene::create();
}

static void problemLoading(const char* filename)
{
	printf("无法打开:%s\n", filename);
}

//初始化精灵
void MainScene::InitSprite(cocos2d::Sprite*& outSprite, cocos2d::Node* parentNode, const std::string& fileName, float x, float y, int localZOrder)
{
	auto pinfo = AutoPolygon::generatePolygon(fileName);
	outSprite = Sprite::create(pinfo);
	if (nullptr == outSprite) {
		outSprite = nullptr;
		problemLoading(fileName.c_str());
		return;
	}
	outSprite->setPosition(Vec2(x, y));
	parentNode->addChild(outSprite, localZOrder);
}

bool MainScene::InitUI()
{
	if (!Scene::init()) return false;

	// 播放音乐
	SimpleAudioEngine::getInstance()->playBackgroundMusic("Music/BackGroundMusic/StartBGMusic.mp3", true);
	
	//获取屏幕可视区域
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();		//获取原点

	//背景
	Sprite* rootNode = nullptr;
	InitSprite(rootNode, this, "MainMenu/front_bg.png", origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2, -1);
	if (nullptr == rootNode) return false;

	//开始冒险按钮
	auto startGameBtn = Button::create("MainMenu/front_btn_start_normal.png", "MainMenu/front_btn_start_pressed.png", "null.png");
	if (nullptr == startGameBtn)
		problemLoading("MainMenu/front_btn_start_normal.png||MainMenu/front_btn_start_pressed.png");
	else{
		startGameBtn->setPosition(Vec2(origin.x + 562, origin.y + 395));
		this->addChild(startGameBtn, 1);

		//触摸事件
		startGameBtn->addTouchEventListener([](Ref* sender, Widget::TouchEventType type) {
			switch (type){
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
	}

	//设置按钮
	auto settingGameBtn = Button::create("MainMenu/menu_s_normal.png", "MainMenu/menu_s_pressed.png", "null.png");
	if (nullptr == settingGameBtn)
		problemLoading("MainMenu/menu_s_normal.png||MainMenu/menu_s_pressed.png");
	else {
		settingGameBtn->setPosition(Vec2(origin.x + 222, origin.y + 445));
		this->addChild(settingGameBtn, 1);

		//触摸事件
		settingGameBtn->addTouchEventListener([](Ref* sender, Widget::TouchEventType type) {
			switch (type) {
			case ui::Widget::TouchEventType::BEGAN:
			{
				SimpleAudioEngine::getInstance()->playEffect("Music/Select.mp3", false, 1.0f, 1.0f, 1.0f);
				break;
			}
			case ui::Widget::TouchEventType::ENDED:
			{
				//跳转场景
				break;
			}
			default:
				break;
			}
			});
	}

	//帮助按钮
	auto helpGameBtn = Button::create("MainMenu/menu_h_normal.png", "MainMenu/menu_h_pressed.png", "null.png");
	if (nullptr == helpGameBtn)
		problemLoading("MainMenu/menu_h_normal.png||MainMenu/menu_h_pressed.png");
	else {
		helpGameBtn->setPosition(Vec2(origin.x + 859, origin.y + 442));
		this->addChild(helpGameBtn, 1);

		//触摸事件
		helpGameBtn->addTouchEventListener([](Ref* sender, Widget::TouchEventType type) {
			switch (type) {
			case ui::Widget::TouchEventType::BEGAN:
			{
				SimpleAudioEngine::getInstance()->playEffect("Music/Select.mp3", false, 1.0f, 1.0f, 1.0f);
				break;
			}
			case ui::Widget::TouchEventType::ENDED:
			{
				//跳转场景
				break;
			}
			default:
				break;
			}
			});
	}
	return true;
}

bool MainScene::init()
{
	if (!InitUI()) return false;
	
	return true;
}
