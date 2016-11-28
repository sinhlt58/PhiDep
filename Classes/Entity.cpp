#include "Entity.h"
#include "Globals.h"

Entity::Entity()
{
	m_Sprite = nullptr;
}

Entity::Entity(std::string spritePath)
{
	m_Sprite = cocos2d::Sprite::create(spritePath);
}

Entity::~Entity()
{
}

void Entity::setDefaultPos(cocos2d::Vec2 pos)
{
	m_defaultPos = pos;
}

bool Entity::isOutOffTheScreen(cocos2d::Size winSize, float upperYMax)
{
	auto selfPos = m_Sprite->getPosition();
	float testRange = 100;

	if (selfPos.y >= upperYMax)
		return true;

	if (selfPos.x < -testRange || selfPos.x > winSize.width + testRange ||
		selfPos.y < 0 || selfPos.y > winSize.height*3)
		return true;
	return false;
}

void Entity::setOnceTimeAnimationAction(const char *format, int count)
{
	auto frames = getAnimationFrames(format, count);
	auto animation = cocos2d::Animation::createWithSpriteFrames(frames, 1.0f / 60);
	m_OnceTimeAnimation =cocos2d::Animate::create(animation);
}

void Entity::setLoopAnimationAction(const char *format, int count)
{
	auto frames = getAnimationFrames(format, count);
	auto animation = cocos2d::Animation::createWithSpriteFrames(frames, 1.0f/60);
	m_LoopAnimation = cocos2d::RepeatForever::create(cocos2d::Animate::create(animation));
}

void Entity::startOnceTimeAnimation()
{
	m_Sprite->runAction(m_OnceTimeAnimation);
}

void Entity::startLoopAnimation()
{
	m_Sprite->runAction(m_LoopAnimation);
}

void Entity::stopOnceTimeAnimation()
{
	m_Sprite->stopAction(m_OnceTimeAnimation);
}

void Entity::stopLoopAnimation()
{
	m_Sprite->stopAction(m_LoopAnimation);
}

void Entity::Reset()
{
}
