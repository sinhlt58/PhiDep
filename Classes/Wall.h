#pragma once
#include <Entity.h>

class Wall : public Entity
{
public:
	Wall(std::string spritePath);
	void Reset() override;
};
