#include "Base/Base.h"
#include "Game/Player.h"
#include "Game/Flipper.h"

Player::Player(const CVector2D& pos) :Base(eType_Player)
{
	m_img = COPY_RESOURCE("Player", CImage);
	m_pos = pos;
	m_img.SetCenter(16, 16);
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
		m_flipperInputTime = 40;
		m_flipperInputSide = 1;
	}
	else if (PULL(CInput::eButton7))
	{
		m_flipperInputTime = 40;
		m_flipperInputSide = 2;
	}
	else if (m_flipperInputTime > 0)
	{
		m_flipperInputTime--;
	}

	m_vec.y += GRAVITY;
	m_pos += m_vec;

	std::vector<Base*> flippers = Base::FindObjects(eType_Flipper);

	//当たったかどうかを記録　最初は当たってない状態
	bool isHit = false;

	//flippersから1つずつ取り出す→Base* を Flipper* に変換する→Flipper専用の関数を使えるようにする
	for (Base* b : flippers)
	{
		Flipper* f = (Flipper*)b;

		CVector2D normal;
		CVector2D tangent;
		float overlap;

		if (f->IsHit(m_pos, normal, tangent, overlap))
		{
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
				m_vec.x += tangent.x * 5.0f;

				m_vec.y -= 25.0f;

				m_flipperInputTime = 0;
				m_flipperInputSide = 0;
			}
			//右フリッパーに当たっていて、右ボタンを押した瞬間 → 上に発射
			else if (!f->m_isLeft && m_flipperInputSide == 2 && m_flipperInputTime > 0)
			{
				m_vec.x += tangent.x * -5.0f;

				m_vec.y -= 25.0f;

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
}
void Player::Draw()
{
	m_img.SetPos(m_pos);
	m_img.Draw();
}