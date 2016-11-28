#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/UIWidget.h"
#include "Dep.h"
#include "MucTieu.h"
#include <Wall.h>
#include <ui/UIButton.h>
#include <Arrow.h>

enum GamePlaySceneState
{
	welcoming,
	prepairing_to_shoot,
	showing_before_shoot,
	waiting_for_input,
	dep_is_going
};

#define THROWING_SOUND "sound/throwing.wav"
#define HITTING_MUC_TIEU "sound/hitting_muc_tieu.wav"
#define LOSING_SOUND "sound/losing.wav"

#define HIGH_SCORE_PATH_FILE "highscore.score"

class GamePlayScene : public cocos2d::LayerColor
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

	bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchMoved(cocos2d::Touch* touch, cocos2d::Event* event);
	void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);

	void update(float delta) override;
	~GamePlayScene();
	cocos2d::Size m_WinVisibleSize;
	cocos2d::Vec2 m_VisibleOrigin;

	Dep* m_Dep;
	MucTieu* m_MucTieu;
	GamePlaySceneState m_State;
	cocos2d::Sprite* m_Background;
	Wall* m_LeftWall;
	Wall* m_RightWall;

	bool m_IsDonePreparing;

	cocos2d::Vec2 m_DefaultCameraPos;
	cocos2d::Vec2 m_CurrentCameraPos;

	/*UI components*/
	cocos2d::Label* m_ScoreLabel;
	cocos2d::ui::Button* m_ReplayButton;
	void handelResetButton(Ref* sender, cocos2d::ui::Widget::TouchEventType type);
	int m_CurrentScore;
	int m_HighScore;
	Arrow* m_Arrow;

	/*Collision*/
	bool onContactBegan(cocos2d::PhysicsContact &contact);
	float m_YUpperBoundry;
	bool m_IsGetingScore;

	/*Welcome*/
	cocos2d::Label* m_HighScoreText;
	cocos2d::Label* m_HighScoreLabel;
	cocos2d::Sprite* m_TutorialBackGround;

	/*Animation*/
	Entity* m_ClickHere;
	Entity* m_PointedArrow;
	Entity* m_MovingHand;
	bool m_IsFirstTime;
	cocos2d::Vec2 getRandomClickHere();
	
	/*Ads*/
	void createTestAd();

	void saveHighScore(std::string, int score);
	int getHighScore(std::string);
    // implement the "static create()" method manually
    CREATE_FUNC(GamePlayScene);
private:
	
};

#endif // __HELLOWORLD_SCENE_H__
