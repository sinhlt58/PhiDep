#pragma once
#include "Entity.h"

class MucTieu : public Entity
{
public:
	MucTieu(std::string spritePath);
	void Reset() override;
};
