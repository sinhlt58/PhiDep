#include "GamePlayScene.h"
#include "editor-support/cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include "Globals.h"
#include "SimpleAudioEngine.h"
#include <fstream>


#ifdef SDKBOX_ENABLED
#include "PluginAdMob/PluginAdMob.h"
#endif

USING_NS_CC;

using namespace cocostudio::timeline;

Vec2 gTouchPos(0, 0);
Vec2 preMovedPos(0, 0);
Vec2 curMovedPos(0, 0);
Vec2 depShootOrigin(0, 0);
Vec2 endedPos(0, 0);
Vec2 depShootDir(0, 0);
bool isSetShootOrigin = false;

Scene* GamePlayScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vec2(0, 0));

    // 'layer' is an autorelease object
    auto layer = GamePlayScene::create();
    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GamePlayScene::init()
{ 
    //////////////////////////////
    // 1. super init first
    if ( !LayerColor::initWithColor(Color4B(160, 160, 160, 255)))
    {
        return false;
    }
	
	srand((unsigned int)time(nullptr));
	m_VisibleOrigin= Director::getInstance()->getVisibleOrigin();
	m_WinVisibleSize = Director::getInstance()->getVisibleSize();

	m_Background = Sprite::create("background.png");
	m_Background->setAnchorPoint(Vec2(0, 0));
	m_Dep = new Dep("dep.png");
	m_MucTieu = new MucTieu("muc_tieu.png");
	m_LeftWall = new Wall("wall.png");
	m_RightWall = new Wall("wall.png");

	auto depSize = m_Dep->m_Sprite->getContentSize();
	auto mucTieuSize = m_MucTieu->m_Sprite->getContentSize();
	auto wallSize = m_LeftWall->m_Sprite->getContentSize();

	m_Dep->setDefaultPos(Vec2(m_WinVisibleSize.width/2, depSize.height / 2));
	m_MucTieu->setDefaultPos(Vec2(m_WinVisibleSize.width*0.5, 
		m_WinVisibleSize.height - mucTieuSize.height / 2 - m_WinVisibleSize.height*0.025));

	m_Dep->m_Sprite->setPosition(m_Dep->m_defaultPos);
	m_MucTieu->m_Sprite->setPosition(m_MucTieu->m_defaultPos);

	m_LeftWall->m_Sprite->setAnchorPoint(Vec2(0, 0));
	m_RightWall->m_Sprite->setAnchorPoint(Vec2(0, 0));
	m_RightWall->m_Sprite->setPosition(Vec2(m_WinVisibleSize.width - wallSize.width, 0));

	addChild(m_Background);
	addChild(m_Dep->m_Sprite);
	addChild(m_MucTieu->m_Sprite);
	addChild(m_LeftWall->m_Sprite);
	addChild(m_RightWall->m_Sprite);

	auto eventListenerTouch = EventListenerTouchOneByOne::create();
	eventListenerTouch->onTouchBegan = CC_CALLBACK_2(GamePlayScene::onTouchBegan, this);
	eventListenerTouch->onTouchMoved = CC_CALLBACK_2(GamePlayScene::onTouchMoved, this);
	eventListenerTouch->onTouchEnded = CC_CALLBACK_2(GamePlayScene::onTouchEnded, this);
	getEventDispatcher()->addEventListenerWithSceneGraphPriority(eventListenerTouch, this);

	m_State = welcoming;
	m_IsDonePreparing = false;

	m_DefaultCameraPos = Vec2(m_WinVisibleSize.width/2, m_WinVisibleSize.height/2);

	/*UI components here*/
	m_ScoreLabel = Label::createWithTTF("0", "fonts/arial.ttf", 40);
	m_ScoreLabel->setAnchorPoint(Vec2(0, 0));
	addChild(m_ScoreLabel);
	m_ReplayButton = ui::Button::create("replay_button.png");
	m_ReplayButton->setAnchorPoint(Vec2(0, 0));
	m_ReplayButton->addTouchEventListener(CC_CALLBACK_2(GamePlayScene::handelResetButton, this));
	addChild(m_ReplayButton);
	m_CurrentScore = 0;
	m_HighScore = getHighScore(HIGH_SCORE_PATH_FILE);
	m_Arrow = new Arrow("arrow_head.png", "arrow_body.png");
	m_Arrow->setDefaultPos(m_Dep->m_defaultPos);
	m_Arrow->resetToDefaultPos();
	addChild(m_Arrow->m_Head);
	addChild(m_Arrow->m_Body);

	/*Welcoming Components*/
	m_HighScoreText = Label::createWithTTF("High Score", "fonts/arial.ttf", 30);
	m_HighScoreLabel = Label::createWithTTF("0", "fonts/arial.ttf", 60);
	m_TutorialBackGround = Sprite::create("tutorial_background.png");
	auto highScoreLabelSize = m_HighScoreLabel->getContentSize();
	auto tutorialBackGroundSize= m_TutorialBackGround->getContentSize();
	m_HighScoreLabel->setPosition(Vec2(m_WinVisibleSize.width/2, m_WinVisibleSize.height/2));
	m_HighScoreText->setPosition(m_HighScoreLabel->getPosition() + Vec2(0, highScoreLabelSize.height));
	m_TutorialBackGround->setPosition(Vec2(m_WinVisibleSize.width/2, m_WinVisibleSize.height/2));
	auto glview = Director::getInstance()->getOpenGLView();
	auto deviceScreenSize = glview->getFrameSize();
	float scaleYFroTutotrialBackground = deviceScreenSize.height / tutorialBackGroundSize.height;
	m_TutorialBackGround->setScaleY(scaleYFroTutotrialBackground);
	
	addChild(m_TutorialBackGround);
	addChild(m_HighScoreLabel);
	addChild(m_HighScoreText);

	/*Animation*/
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("animation/click_here.plist");
	m_ClickHere = new Entity();
	m_ClickHere->m_Sprite = Sprite::createWithSpriteFrameName("click_here_000.png");
	m_ClickHere->m_Sprite->setPosition(Vec2(150, 200));
	m_ClickHere->setLoopAnimationAction("click_here_%03d.png", 61);
	addChild(m_ClickHere->m_Sprite);
	m_ClickHere->startLoopAnimation();
	m_PointedArrow = new Entity();
	m_PointedArrow->m_Sprite = Sprite::createWithSpriteFrameName("pointed_arrow_000.png");
	m_PointedArrow->m_Sprite->setVisible(false);
	m_PointedArrow->setLoopAnimationAction("pointed_arrow_%03d.png", 61);
	addChild(m_PointedArrow->m_Sprite);
	m_PointedArrow->startLoopAnimation();
	m_MovingHand = new Entity();
	m_MovingHand->m_Sprite = Sprite::createWithSpriteFrameName("moving_hand_000.png");
	m_MovingHand->m_Sprite->setVisible(false);
	m_MovingHand->setLoopAnimationAction("moving_hand_%03d.png", 61);
	addChild(m_MovingHand->m_Sprite);
	m_MovingHand->startLoopAnimation();
	m_IsFirstTime = true;

	/*Contact event*/
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GamePlayScene::onContactBegan, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);

	/*Init ads*/
	createTestAd();

	this->scheduleUpdate();

    return true;
}

bool GamePlayScene::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event)
{
	if(m_State == waiting_for_input)
	{
		if (m_IsFirstTime)
		{
			m_PointedArrow->m_Sprite->setVisible(false);
			m_MovingHand->m_Sprite->setVisible(true);
		}
			
		auto touchPos = touch->getLocation();
		gTouchPos = touchPos;
		preMovedPos = touchPos;
		curMovedPos = touchPos;
		auto clickHerePos = m_ClickHere->m_Sprite->getPosition();
		float validRadius = m_ClickHere->m_Sprite->getContentSize().width/2;
		if ((touchPos - clickHerePos).length() > validRadius)
			return false;

		return true;
	}
	if (m_State == welcoming){
		m_State = prepairing_to_shoot;
		return false;
	}
	return false;
}

void GamePlayScene::onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event)
{
	auto touchPos = touch->getLocation();
	preMovedPos = curMovedPos;
	curMovedPos = touchPos;
	auto dirMove = curMovedPos - preMovedPos;
	if (dirMove.y > 0 && !isSetShootOrigin)
	{
		depShootOrigin = preMovedPos;
		isSetShootOrigin = true;
	}
	float portionX = touchPos.y - gTouchPos.y;
	float portionY = touchPos.x - gTouchPos.x;
	float angle = RadianToDegree(atan2(portionY, portionX));
	auto dirFromTouchBegan = touchPos - gTouchPos;
	if (dirFromTouchBegan.length() != 0)
	{
		m_Arrow->scale(1 / dirFromTouchBegan.length()*100);
	}
	m_Arrow->rotate(angle);

}

void GamePlayScene::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event)
{
	auto touchPos = touch->getLocation();
	depShootDir = touchPos - depShootOrigin;

//	log("Ended pos: %f %f", touchPos.x, touchPos.y);
//	log("Dep shoot dir: %f %f", depShootDir.x, depShootDir.y);
//
//	if (depShootDir != Vec2(0, 0))
//	{
//		m_State = dep_is_going;
//		auto normalizeDir = depShootDir.getNormalized();
//		auto amoutOfForce = gTouchPos - depShootOrigin;
//		m_Dep->m_Sprite->getPhysicsBody()->setVelocity(normalizeDir * amoutOfForce.getLength());
//		m_Dep->m_Sprite->getPhysicsBody()->setAngularVelocity(6);
//	}

	auto newShootingDir = touchPos - gTouchPos;
	if (newShootingDir != Vec2(0, 0))
	{
		auto amoutOfFroce = 1 / newShootingDir.length();
		auto normalizeDir = newShootingDir.getNormalized();
		float maxForce = 30000;
		m_Dep->m_Sprite->getPhysicsBody()->setVelocity(normalizeDir * amoutOfFroce * maxForce);
		m_Dep->m_Sprite->getPhysicsBody()->setAngularVelocity(3);
		m_State = dep_is_going;
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(THROWING_SOUND);
	}

	if (m_IsFirstTime)
	{
		m_MovingHand->m_Sprite->setVisible(false);
		m_IsFirstTime = false;
	}
}

void GamePlayScene::update(float delta)
{
	float minX, maxX, rangeX, randomX;
	float cameraSpeed = 4.0f;
	auto depPos = m_Dep->m_Sprite->getPosition();
	auto mucTieuPos = m_MucTieu->m_Sprite->getPosition();
	switch(m_State)
	{
	case welcoming:
		m_ScoreLabel->setVisible(false);
		m_ReplayButton->setVisible(false);
		m_TutorialBackGround->setVisible(true);
		m_HighScoreLabel->setVisible(true);
		m_HighScoreText->setVisible(true);
		m_Dep->Reset();
		m_MucTieu->Reset();
		m_HighScoreLabel->setString(StringUtils::toString(m_HighScore));
		Camera::getDefaultCamera()->setPosition(m_DefaultCameraPos);
		break;
	case prepairing_to_shoot:
		if(!m_IsDonePreparing)
		{
			m_ScoreLabel->setVisible(true);
			m_ReplayButton->setVisible(true);
			m_TutorialBackGround->setVisible(false);
			m_HighScoreLabel->setVisible(false);
			m_HighScoreText->setVisible(false);

			isSetShootOrigin = false;
			m_IsGetingScore = false;
			m_Dep->Reset();
			m_MucTieu->Reset();
			m_Arrow->scale(1);
			m_Arrow->rotate(0);

			minX = m_MucTieu->m_Sprite->getContentSize().width / 2;
			maxX = m_WinVisibleSize.width
				- m_MucTieu->m_Sprite->getContentSize().width / 2;
			rangeX = maxX - minX;
			randomX = (rand() % int(rangeX)) + minX;
			m_MucTieu->m_Sprite->setPosition(randomX, m_MucTieu->m_defaultPos.y + (rand()%500));
			int dirMoving = randomX < m_WinVisibleSize.width / 2 ? 1 : -1;
			//m_MucTieu->m_Sprite->getPhysicsBody()->setVelocity(Vec2(dirMoving * 25, 0));
			float marginBoundry = 200;
			m_YUpperBoundry = m_MucTieu->m_Sprite->getPosition().y + marginBoundry;

			m_ClickHere->m_Sprite->setPosition(getRandomClickHere());
			if(m_IsFirstTime)
			{
				m_PointedArrow->m_Sprite->setPosition(
					m_ClickHere->m_Sprite->getPosition() + Vec2(0, 50));
				m_MovingHand->m_Sprite->setPosition(
					m_ClickHere->m_Sprite->getPosition() + Vec2(0, 50));
				m_PointedArrow->m_Sprite->setVisible(true);
			}

			auto camera = Camera::getDefaultCamera();
			auto cameraPos = camera->getPosition();
			Camera::getDefaultCamera()->setPosition(m_DefaultCameraPos);

			mucTieuPos = m_MucTieu->m_Sprite->getPosition();
			if (mucTieuPos.y > m_WinVisibleSize.height)
			{
				m_CurrentCameraPos = Vec2(m_WinVisibleSize.width / 2, mucTieuPos.y);
				m_State = showing_before_shoot;
			}else
			{
				m_State = waiting_for_input;
			}

			m_IsDonePreparing = true;	
		}

		break;
	case showing_before_shoot:
		if (m_CurrentCameraPos.y >= m_DefaultCameraPos.y)
		{
			m_CurrentCameraPos += Vec2(0, -1)*cameraSpeed;
			Camera::getDefaultCamera()->setPosition(m_CurrentCameraPos);
		}else
		{
			m_State = waiting_for_input;
		}
		break;
	case waiting_for_input:
		break;
	case dep_is_going:
		if (depPos.y >= m_WinVisibleSize.height/2)
		{
			auto awsomeCameraPos = m_DefaultCameraPos + Vec2(0, depPos.y - m_DefaultCameraPos.y);
			Camera::getDefaultCamera()->setPosition(awsomeCameraPos);
		}
		
		if (m_Dep->isOutOffTheScreen(m_WinVisibleSize, m_YUpperBoundry) ||
			m_MucTieu->isOutOffTheScreen(m_WinVisibleSize, m_YUpperBoundry))
		{
			m_State = prepairing_to_shoot;
			m_IsDonePreparing = false;
			if(!m_IsGetingScore)
			{
				if (m_CurrentScore > m_HighScore)
				{
					m_HighScore = m_CurrentScore;
					saveHighScore(HIGH_SCORE_PATH_FILE, m_HighScore);
				}
				m_CurrentScore = 0;
				m_State = welcoming;
				CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(LOSING_SOUND);
			}
		}
		break;
	}
	/*UI Components*/
	auto cameraPos = Camera::getDefaultCamera()->getPosition();
	auto scoreLabelSize = m_ScoreLabel->getContentSize();
	auto wallSize = m_LeftWall->m_Sprite->getContentSize();
	m_ScoreLabel->setPosition(Vec2(wallSize.width,
		cameraPos.y + m_WinVisibleSize.height/2 - scoreLabelSize.height));
	m_ScoreLabel->setString(StringUtils::toString(m_CurrentScore));
	auto replayButtonSize = m_ReplayButton->getContentSize();
	float margin = 15;
	m_ReplayButton->setPosition(Vec2(m_WinVisibleSize.width - wallSize.width - replayButtonSize.width - margin,
								cameraPos.y - m_WinVisibleSize.height/2 + margin));
	
}

GamePlayScene::~GamePlayScene()
{
	delete m_Dep;
	delete m_MucTieu;
	delete m_LeftWall;
	delete m_RightWall;
	delete m_Arrow;
	delete m_ClickHere;
	delete m_PointedArrow;
	delete m_MovingHand;
}

void GamePlayScene::handelResetButton(Ref* sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (m_State == dep_is_going)
	{
		m_State = prepairing_to_shoot;
		m_IsDonePreparing = false;
		m_CurrentScore = 0;
	}	
}

bool GamePlayScene::onContactBegan(cocos2d::PhysicsContact& contact)
{
	auto nodeA = contact.getShapeA()->getBody()->getNode();
	auto nodeB = contact.getShapeB()->getBody()->getNode();

	if (nodeA && nodeB)
	{
		int sum = nodeA->getTag() | nodeB->getTag();
		int depAndMuctieu = int(PhysicsCategory::dep) | int(PhysicsCategory::muc_tieu);
		if (sum == depAndMuctieu)
		{
			m_CurrentScore += 1;
			m_IsGetingScore = true;
			CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(HITTING_MUC_TIEU);
		}
	}

	return true;
}

static std::string homeAd = "home";

cocos2d::Vec2 GamePlayScene::getRandomClickHere()
{
	auto clickHerePos = m_ClickHere->m_Sprite->getPosition();
	auto clickHereSize = m_ClickHere->m_Sprite->getContentSize();
	float minX = 1.5f * clickHereSize.width;
	float maxX = m_WinVisibleSize.width - minX;
	float minY = 1.0f / 6 * m_WinVisibleSize.height;
	float maxY = 1.0f / 2 * m_WinVisibleSize.height;
	float randomX = minX + (rand() % int((maxX - minX)));
	float randomY = minY + (rand() % int((maxY - minY)));
	return Vec2(randomX, randomY);
}

void GamePlayScene::createTestAd()
{
	
	std::string gameOverAd = "gameover";

#ifdef SDKBOX_ENABLED
	{
//		Menu* menu = Menu::create(
//			MenuItemFont::create("load banner", [](Ref*) {sdkbox::PluginAdMob::cache(homeAd); }),
//			MenuItemFont::create("show banner", [](Ref*) {sdkbox::PluginAdMob::show(homeAd); }),
//			MenuItemFont::create("hide banner", [](Ref*) {sdkbox::PluginAdMob::hide(homeAd); }),
//			nullptr
//		);
//
//		menu->setPosition(m_WinVisibleSize.width/2, m_WinVisibleSize.height/2);
//		menu->alignItemsVerticallyWithPadding(20);
//		addChild(menu);
	}
#endif
}

void GamePlayScene::saveHighScore(std::string filePath, int score)
{
	cocos2d::FileUtils * file_utils = cocos2d::FileUtils::getInstance();
	std::string full_path = file_utils->getWritablePath() + filePath;
	file_utils->writeStringToFile(StringUtils::toString(score), full_path);
}

int GamePlayScene::getHighScore(std::string filePath)
{
	int highScore;

	cocos2d::FileUtils * file_utils = cocos2d::FileUtils::getInstance();
	std::string full_path = file_utils->getWritablePath() + filePath;
	std::string file_data = FileUtils::getInstance()->getStringFromFile(full_path);

	//file_utils->writeStringToFile("haha", full_path);
	highScore = atoi(file_data.c_str());

	return highScore;
}
