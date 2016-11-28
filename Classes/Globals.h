#pragma once
#include "cocos2d.h"

enum class PhysicsCategory
{
	none = 0,
	dep = 1 << 0,
	muc_tieu = 1 << 1,
	wall = 1 << 2,
	all = dep | muc_tieu | wall
};
#define PI 3.141592653589793238462643383279502884

inline float DegreeToRadian(float degree)
{
	return (degree / 180) * PI;
}

inline float RadianToDegree(float radian)
{
	return (radian / PI) * 180;
}

/*Animation*/
inline cocos2d::Vector<cocos2d::SpriteFrame*> getAnimationFrames(const char *format, int count)
{
	auto spritecache = cocos2d::SpriteFrameCache::getInstance();
	char str[100];
	cocos2d::Vector<cocos2d::SpriteFrame*> animFrames;
	for (int i = 1; i <= count; i++)
	{
		sprintf(str, format, i);
		animFrames.pushBack(spritecache->getSpriteFrameByName(str));
	}
	return animFrames;
}
