#pragma once
#include "Base/Base.h"

class SlopeWall : public Base
{
public:
	CImage m_img;
	bool m_isLeft;
	float m_ang;
	float m_length;
public:
	SlopeWall(const CVector2D& pos, bool isLeft, float length);
	void Update();
	void Draw();
	bool IsHit(const CVector2D& ballPos, float ballRad, CVector2D& normal,
		 float& overlap);

};