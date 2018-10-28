#include "Camera.h"

Camera::Camera()
{
	m_position = { 0,0,0 };
}

Camera::~Camera()
{

}

void Camera::setPosition(const Vec & position)
{
	m_position = position;
}

const Vec & Camera::getPosition() const
{
	return m_position;
}
