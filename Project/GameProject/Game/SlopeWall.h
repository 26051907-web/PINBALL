#pragma once
#include "Base/Base.h"

class SlopeWall : public Base
{
public:
	CImage m_img;
	bool m_isLeft;
	float m_ang;
	//1フレームでフリッパーの角度がどれだけ変化したかを記録
	float m_angVelocity;
	bool m_isPressed;
public:
	SlopeWall(const CVector2D& pos, bool isLeft);
	void Update();
	void Draw();
	bool IsHit(const CVector2D& ballPos, CVector2D& normal, 
		 float& overlap);

};