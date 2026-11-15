#pragma once
#include "Base/Base.h"
class Player :public Base {
public:
	//ボタン入力を受け付ける残り時間を記録する変数
	int m_flipperInputTime;
	CImage m_img;
	CImage m_springImg;
	bool m_wasHit;
	int m_flipperInputSide;
	//発射済みか
	bool m_isLaunched;
	//バネを引いている強さ
	int m_charge;
public:
	Player(const CVector2D& pos);
	void Update();
	void Draw();
};