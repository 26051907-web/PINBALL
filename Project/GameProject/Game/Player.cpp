#include "Base/Base.h"
#include "Game/Player.h"
#include "Game/Flipper.h"
#include "Game/Wall.h"

Player::Player(const CVector2D& pos) :Base(eType_Player)
{
	m_img = COPY_RESOURCE("Player", CImage);
	m_pos = pos;
	m_img.SetCenter(16, 16);
	m_rad = 16.0f;
	m_wasHit = false;
	m_flipperInputTime = 0;
	m_flipperInputSide = 0;
	//m_img.SetSize(1000, 1000);
	
}
void Player::Update()
{
	//フリッパーの一覧を取得
	//flippersの中身 Flipperオブジェクトへのポインタ
	//Base*はオブジェクトへのポインタ 取得したFlipperにアクセスできる

	//1:左フリッパーの入力 2:右フリッパーの入力 0:まだ入力なし
	//6:A 7:D
	if (PULL(CInput::eButton6))
	{
		//フリッパー押した判定の緩さ
		m_flipperInputTime = 7;
		m_flipperInputSide = 1;
	}
	else if (PULL(CInput::eButton7))
	{
		//フリッパー押した判定の緩さ
		m_flipperInputTime = 7;
		m_flipperInputSide = 2;
	}
	else if (m_flipperInputTime > 0)
	{
		m_flipperInputTime--;
	}

	m_vec.y += GRAVITY;
	m_pos += m_vec;

	//Baseのリストに登録されているFlipperをまとめて取得
	std::vector<Base*> flippers = Base::FindObjects(eType_Flipper);
	//Baseのリストに登録されているWallをまとめて取得
	std::vector<Base*> walls = Base::FindObjects(eType_Wall);

	//Wallから1つずつ取り出す→Base* を Wall* に変換する→Wall専用の関数を使えるようにする
	for (Base* b : walls)
	{
		Wall* w = (Wall*)b;

		//dx：ボールと壁の左右方向の距離
		//dy：ボールと壁の上下方向の距離
		float dx = m_pos.x - w->m_pos.x;
		float dy = m_pos.y - w->m_pos.y;

		//fabs() は小数の絶対値を求める関数 マイナスの距離もプラスにして比較できる
		if (fabs(dx) < m_rad + w->m_halfWidth &&
			fabs(dy) < m_rad + w->m_halfHeight)
		{
			//当たったときの処理
			float overlapX = m_rad + w->m_halfWidth - fabs(dx);

			if (dx >= 0)
			{
				m_pos.x += overlapX;
			}
			else
			{
				m_pos.x -= overlapX;
			}
			//ポヨンッ
			if (dx * m_vec.x < 0.0f)
			{
				m_vec.x *= -0.9f;
			}
		}
	}

	//当たったかどうかを記録　最初は当たってない状態
	bool isHit = false;

	//flippersから1つずつ取り出す→Base* を Flipper* に変換する→Flipper専用の関数を使えるようにする
	for (Base* b : flippers)
	{
		Flipper* f = (Flipper*)b;

		CVector2D normal;
		CVector2D tangent;
		float overlap;
		float hitPosition;

		if (f->IsHit(m_pos, normal, tangent, overlap, hitPosition))
		{
			//当たった位置によって強さが変わる
			float power = 0.75f + hitPosition * 0.25f;

			//ボールがフリッパーにめり込んだら、表面まで押し戻す
			m_pos += normal * overlap;

			float normalSpeed = m_vec.Dot(normal);

			if (normalSpeed < 0.0f)
			{
				m_vec -= normal * normalSpeed;
			}

			//左フリッパーに当たっていて、左ボタンを押した瞬間 → 上に発射
			if (f->m_isLeft && m_flipperInputSide == 1 && m_flipperInputTime > 0)
			{
				m_vec.x += tangent.x * 1.5f;

				m_vec.y -= 55.0f * power;
				//ここ
				m_vec.x += f->m_angVelocity * 20.0f;
				//m_vec.y += f->m_angVelocity * 10.0f;

				m_flipperInputTime = 0;
				m_flipperInputSide = 0;
			}
			//右フリッパーに当たっていて、右ボタンを押した瞬間 → 上に発射
			else if (!f->m_isLeft && m_flipperInputSide == 2 && m_flipperInputTime > 0)
			{
				m_vec.x += tangent.x * 1.5f;

				m_vec.y -= 55.0f * power;
				//ここ
				m_vec.x += f->m_angVelocity * 20.0f;
				//m_vec.y -= f->m_angVelocity * 10.0f;

				m_flipperInputTime = 0;
				m_flipperInputSide = 0;
			}
			

			m_wasHit = true;
			isHit = true;
		}
	}
	//どちらかのフリッパーに当たっている → m_wasHit = true
	//どちらにも当たっていない → m_wasHit = false
	if (!isHit)
	{
		m_wasHit = false;
	}
	//画面外に出たら削除予約
	if (m_pos.x + m_rad < 0 ||
		m_pos.x - m_rad > 1920 ||
		m_pos.y + m_rad < 0 ||
		m_pos.y - m_rad > 1080)
	{
		SetKill();
	}
}
void Player::Draw()
{
	m_img.SetPos(m_pos);
	m_img.Draw();
}