#include "ChooseLevel.h"
#include "MainScene.h"
#include "EnterScene1.h"
#include "EnterScene2.h"
#include "EnterScene3.h"
#include "EnterScene4.h"
#include "SimpleAudioEngine.h"
#include <vector>

USING_NS_CC;
using namespace CocosDenshion;
using namespace cocos2d::ui;
using namespace std;

extern int is_Pass[10] = { 0 };


//枚举，其中保存着各类节点的层级
enum zOrderMap
{
	route = 1,//道路层级
	routeButtonEffect = 5,//按钮特效层级
	levelButton = 10//按钮层级
};

//创建选择关卡画面
cocos2d::Scene* ChooseLevel::createScene()
{
	return ChooseLevel::create();
}

//选择关卡界面初始化
//在ChooseLevel类中的init()函数中调用
bool ChooseLevel::init()
{
	if (!Scene::init()) return false;

	InitUI();

	InitEvent();

	return true;
}

//UI层初始化
void ChooseLevel::InitUI()
{
	//背景滚动界面
	auto scrollView = ScrollView::create();
	//设置滚动方向
	scrollView->setDirection(ScrollView::Direction::BOTH);
	//开启触摸事件
	scrollView->setTouchEnabled(true);
	//设置内容大小
	scrollView->setContentSize(Director::getInstance()->getVisibleSize());
	//隐藏滚动条
	scrollView->setScrollBarEnabled(false);
	this->addChild(scrollView);

	//创建背景地图
	Sprite* imageView = NULL;
	string filePath;
	float nextPosX = 0.0f;
	float nextPosY = 0.0f;
	filePath = "ChooseLevel/chooselevel1.png";
	imageView = Sprite::create(filePath);
	imageView->setAnchorPoint(Vec2(0, 0));								//更改锚点
	nextPosX += imageView->getContentSize().width;						//累加横向偏移量
	nextPosY += imageView->getContentSize().height;						//累加竖向偏移量
	scrollView->addChild(imageView);
	//设置ScrollView的布局容器大小
	scrollView->setInnerContainerSize(Size(nextPosX, nextPosY));

	//UI按钮控件
	auto layerUI = Layer::create();
	this->addChild(layerUI);
	auto leftPanelPinfo = AutoPolygon::generatePolygon("ChooseLevel/stagemap_toolbar.png");
	auto leftPanel = Sprite::create(leftPanelPinfo);
	//设置该背景图始终在屏幕的左上角
	leftPanel->setAnchorPoint(Vec2(0, 1));
	leftPanel->setPosition(0, Director::getInstance()->getVisibleSize().height);
	leftPanel->setScale(float(0.8));
	layerUI->addChild(leftPanel);

	//创建背景图精灵的子节点（三个按钮）
	//按钮1
	auto toolbarHomeFilePath = "ChooseLevel/stagemap_toolbar_home.png";
	_toolbarHome = Button::create(toolbarHomeFilePath, toolbarHomeFilePath, "");
	_toolbarHome->setAnchorPoint(Vec2(0, 0));
	_toolbarHome->setPosition(Vec2(11, 10));
	_toolbarHome->setPressedActionEnabled(true);
	leftPanel->addChild(_toolbarHome);

	//按钮2
	auto toolbarShopFilePath = "ChooseLevel/stagemap_toolbar_shop.png";
	_toolbarShop = Button::create(toolbarShopFilePath, toolbarShopFilePath, "");
	_toolbarShop->setAnchorPoint(Vec2(0, 0));
	_toolbarShop->setPosition(Vec2(123, 10));
	_toolbarShop->setPressedActionEnabled(true);
	leftPanel->addChild(_toolbarShop);

	//按钮3
	auto toolbarLeaderboardFilePath = "ChooseLevel/stagemap_toolbar_leaderboard.png";
	_toolbarLeaderboard = Button::create(toolbarLeaderboardFilePath, toolbarLeaderboardFilePath, "");
	_toolbarLeaderboard->setAnchorPoint(Vec2(0, 0));
	_toolbarLeaderboard->setPosition(Vec2(235, 10));
	_toolbarLeaderboard->setPressedActionEnabled(true);
	leftPanel->addChild(_toolbarLeaderboard);

	TMXTiledMap* map = TMXTiledMap::create("ChooseLevel/tiledmap.tmx");
	//获取瓦片地图中的point对象层
	auto objectGroup = map->getObjectGroup("point");
	//将对象层中的所有对象获取出来
	auto objs = objectGroup->getObjects();
	//遍历对象
	for (int i = 0; i < objs.size(); i++) {
		//图片纹理
		string texture = "ChooseLevel/stagepoint.png";

		//根据瓦片地图中位置坐标信息配置关卡按钮
		auto button = Button::create(texture, texture, "");
		scrollView->addChild(button, zOrderMap::levelButton, i);
		std::vector<cocos2d::ui::Button*> routeButtonArray;
		routeButtonArray.push_back(button);

		auto valueMap = objs[i].asValueMap();//获得属性值：Value转换成ValueMap

		//按钮的位置坐标
		button->setPosition(Vec2(valueMap["x"].asFloat() - 5, valueMap["y"].asFloat() + 60));//获取对象的属性
		//设置当前关卡数，索引从0开始，即0表示第1关，1表示第2关，以此类推。
		button->setTag(i);						//设置编号
		button->setScale(1.0);					//设置大小
		button->setPressedActionEnabled(true);	//设置按动效果

		switch (i) {
		case 0:
			//触摸事件监听器
			button->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
				if (type == ui::Widget::TouchEventType::ENDED) {
					SimpleAudioEngine::getInstance()->playEffect("Music/Select.mp3", false, 1.0f, 1.0f, 1.0f);
					//跳转场景：关卡1
					SimpleAudioEngine::getInstance()->stopBackgroundMusic();
					auto enterscene = EnterScene1::createScene();
					Director::getInstance()->replaceScene(enterscene);
				}
				});
			break;
		case 1:
			//触摸事件监听器
			button->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
				if (type == ui::Widget::TouchEventType::ENDED) {
					SimpleAudioEngine::getInstance()->playEffect("Music/Select.mp3", false, 1.0f, 1.0f, 1.0f);
					//跳转场景：关卡2
					if (is_Pass[1] == 0) {
						// 创建遮罩层
						auto maskLayer = LayerColor::create(Color4B(0, 0, 0, 128)); // 浅黑色遮罩
						this->addChild(maskLayer, 200);

						// 添加触摸吞噬事件，防止穿透
						auto listener = EventListenerTouchOneByOne::create();
						listener->setSwallowTouches(true);
						listener->onTouchBegan = [](Touch* touch, Event* event) {
							return true; // 总是吞噬触摸事件
							};
						_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, maskLayer);

						auto lockmenu = Sprite::create("ChooseLevel/lock_image.png");

						SimpleAudioEngine::getInstance()->pauseBackgroundMusic();

						// 设置菜单位置
						lockmenu->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
							Director::getInstance()->getVisibleSize().height * 2));
						this->addChild(lockmenu, 201);

						// 执行菜单从上方移动到中央的动作
						auto moveAction = EaseBackOut::create(MoveTo::create(0.5f, Vec2(Director::getInstance()->getVisibleSize().width / 2,
							Director::getInstance()->getVisibleSize().height / 2)));
						lockmenu->runAction(moveAction);

						auto lock_btn = Button::create("ChooseLevel/lock_btn_normal.png", "ChooseLevel/lock_btn_selected.png", "");
						lockmenu->addChild(lock_btn);
						lock_btn->setPressedActionEnabled(true);
						lock_btn->setPosition(Vec2(333, 70));
						lock_btn->setScale(1.12);
						lock_btn->addClickEventListener([=](Ref* sender) {
							SimpleAudioEngine::getInstance()->playEffect("Music/Select.mp3");
							Director::getInstance()->resume();
							lockmenu->removeFromParent();
							maskLayer->removeFromParent(); // 移除遮罩
							});
					}
					else {
						SimpleAudioEngine::getInstance()->stopBackgroundMusic();
						auto enterscene = EnterScene2::createScene();
						Director::getInstance()->replaceScene(enterscene);
					}
				}
			});
			break;
		case 2:
			//触摸事件监听器
			button->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
				if (type == ui::Widget::TouchEventType::ENDED) {
					SimpleAudioEngine::getInstance()->playEffect("Music/Select.mp3", false, 1.0f, 1.0f, 1.0f);
					//跳转场景：关卡3
					if (is_Pass[2] == 0) {
						// 创建遮罩层
						auto maskLayer = LayerColor::create(Color4B(0, 0, 0, 128)); // 浅黑色遮罩
						this->addChild(maskLayer, 200);

						// 添加触摸吞噬事件，防止穿透
						auto listener = EventListenerTouchOneByOne::create();
						listener->setSwallowTouches(true);
						listener->onTouchBegan = [](Touch* touch, Event* event) {
							return true; // 总是吞噬触摸事件
							};
						_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, maskLayer);

						auto lockmenu = Sprite::create("ChooseLevel/lock_image.png");

						SimpleAudioEngine::getInstance()->pauseBackgroundMusic();

						// 设置菜单位置
						lockmenu->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
							Director::getInstance()->getVisibleSize().height * 2));
						this->addChild(lockmenu, 201);

						// 执行菜单从上方移动到中央的动作
						auto moveAction = EaseBackOut::create(MoveTo::create(0.5f, Vec2(Director::getInstance()->getVisibleSize().width / 2,
							Director::getInstance()->getVisibleSize().height / 2)));
						lockmenu->runAction(moveAction);

						auto lock_btn = Button::create("ChooseLevel/lock_btn_normal.png", "ChooseLevel/lock_btn_selected.png", "");
						lockmenu->addChild(lock_btn);
						lock_btn->setPressedActionEnabled(true);
						lock_btn->setPosition(Vec2(333, 70));
						lock_btn->setScale(1.12);
						lock_btn->addClickEventListener([=](Ref* sender) {
							SimpleAudioEngine::getInstance()->playEffect("Music/Select.mp3");
							Director::getInstance()->resume();
							lockmenu->removeFromParent();
							maskLayer->removeFromParent(); // 移除遮罩
							});
					}
					else {
						SimpleAudioEngine::getInstance()->stopBackgroundMusic();
						auto enterscene = EnterScene3::createScene();
						Director::getInstance()->replaceScene(enterscene);
					}
				}
				});
			break;
		case 3:
			//触摸事件监听器
			button->addTouchEventListener([=](Ref* sender, Widget::TouchEventType type) {
				if (type == ui::Widget::TouchEventType::ENDED) {
					SimpleAudioEngine::getInstance()->playEffect("Music/Select.mp3", false, 1.0f, 1.0f, 1.0f);
					//跳转场景：关卡4
					if (is_Pass[2] == 0) {
						// 创建遮罩层
						auto maskLayer = LayerColor::create(Color4B(0, 0, 0, 128)); // 浅黑色遮罩
						this->addChild(maskLayer, 200);

						// 添加触摸吞噬事件，防止穿透
						auto listener = EventListenerTouchOneByOne::create();
						listener->setSwallowTouches(true);
						listener->onTouchBegan = [](Touch* touch, Event* event) {
							return true; // 总是吞噬触摸事件
							};
						_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, maskLayer);

						auto lockmenu = Sprite::create("ChooseLevel/lock_image.png");

						SimpleAudioEngine::getInstance()->pauseBackgroundMusic();

						// 设置菜单位置
						lockmenu->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2,
							Director::getInstance()->getVisibleSize().height * 2));
						this->addChild(lockmenu, 201);

						// 执行菜单从上方移动到中央的动作
						auto moveAction = EaseBackOut::create(MoveTo::create(0.5f, Vec2(Director::getInstance()->getVisibleSize().width / 2,
							Director::getInstance()->getVisibleSize().height / 2)));
						lockmenu->runAction(moveAction);

						auto lock_btn = Button::create("ChooseLevel/lock_btn_normal.png", "ChooseLevel/lock_btn_selected.png", "");
						lockmenu->addChild(lock_btn);
						lock_btn->setPressedActionEnabled(true);
						lock_btn->setPosition(Vec2(333, 70));
						lock_btn->setScale(1.12);
						lock_btn->addClickEventListener([=](Ref* sender) {
							SimpleAudioEngine::getInstance()->playEffect("Music/Select.mp3");
							Director::getInstance()->resume();
							lockmenu->removeFromParent();
							maskLayer->removeFromParent(); // 移除遮罩
							});
					}
					else {
						SimpleAudioEngine::getInstance()->stopBackgroundMusic();
						auto enterscene = EnterScene4::createScene();
						Director::getInstance()->replaceScene(enterscene);
					}
				}
				});
			break;
		//按需添加关卡
		case 4:
		case 5:
		case 6:
		case 7:
		case 8:

		default:
			break;
		}
	}
}

//底层事件层初始化
void ChooseLevel::InitEvent()
{
	//初始化首页按钮、商店按钮、排行榜按钮的三个事件
	//首页按钮
	_toolbarHome->addTouchEventListener([](Ref* sender, Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED){
			SimpleAudioEngine::getInstance()->playEffect("Music/Select.mp3", false, 1.0f, 1.0f, 1.0f);
			auto mainScene = MainScene::createScene();
			Director::getInstance()->replaceScene(mainScene);
		}
	});
	//商店按钮
	_toolbarShop->addTouchEventListener([](Ref* sender, Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED) 
			SimpleAudioEngine::getInstance()->playEffect("Music/Select.mp3", false, 1.0f, 1.0f, 1.0f);
		//...跳转
		});
	//排行榜按钮
	_toolbarLeaderboard->addTouchEventListener([](Ref* sender, Widget::TouchEventType type) {
		if (type == ui::Widget::TouchEventType::ENDED)
			SimpleAudioEngine::getInstance()->playEffect("Music/Select.mp3", false, 1.0f, 1.0f, 1.0f);
		//...跳转
		});
}
