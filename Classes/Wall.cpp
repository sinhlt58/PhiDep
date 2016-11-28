#include "Wall.h"
#include <Globals.h>

Wall::Wall(std::string spritePath)
	: Entity(spritePath)
{
	auto selfSize = m_Sprite->getContentSize();
	auto physicsBody = cocos2d::PhysicsBody::createBox(cocos2d::Size(selfSize.width, selfSize.height),
		cocos2d::PhysicsMaterial(0.1f, 0.0f, 0.0f));
	physicsBody->setDynamic(false);
	physicsBody->setCategoryBitmask(int(PhysicsCategory::wall));
	physicsBody->setCategoryBitmask(int(PhysicsCategory::dep) | int(PhysicsCategory::muc_tieu));
	m_Sprite->setPhysicsBody(physicsBody);
}

void Wall::Reset()
{
}
