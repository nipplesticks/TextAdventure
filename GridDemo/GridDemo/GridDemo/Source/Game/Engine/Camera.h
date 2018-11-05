#pragma once

#include "../Help_Headers/Structs.h"

class Camera
{
private:
	Vec m_position;
public:
	Camera();
	~Camera();

	void setPosition(const Vec & position);
	const Vec & getPosition() const;
};