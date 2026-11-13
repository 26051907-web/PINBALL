#pragma once
#include "Base/Base.h"
class Player :public Base {
public:
	//ボタン入力を受け付ける残り時間を記録する変数 PULLの判定を甘くするために用意
	int m_flipperInputTime;
	CImage m_img;
	bool m_wasHit;
	int m_flipperInputSide;
public:
	Player(const CVector2D& pos);
	void Update();
	void Draw();
};