#include "Base/Base.h"
#include "Game/Player.h"
#include "Game/Flipper.h"
#include "Game/Wall.h"
#include "Game/SlopeWall.h"
#include "Game/Bumper.h"

Player::Player(const CVector2D& pos) :Base(eType_Player)
{
	m_img = COPY_RESOURCE("Player", CImage);
	m_springImg = COPY_RESOURCE("Spring", CImage);
	m_springImg.SetCenter(12, 60);
	m_pos = pos;
	m_img.SetCenter(16, 16);
	m_rad = 16.0f;
	m_wasHit = false;

	m_vec = CVector2D(0, 0);//ボール生成時に速度をゼロにする

	m_flipperInputTime = 0;
	m_flipperInputSide = 0;
	m_isLaunched = false;
	m_charge = 0;
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
		//フリッパー入力受付時間
		m_flipperInputTime = 8;

		m_flipperInputSide = 1;
	}
	else if (PULL(CInput::eButton7))
	{
		//フリッパー入力受付時間
		m_flipperInputTime = 8;

		m_flipperInputSide = 2;
	}
	else if (m_flipperInputTime > 0)
	{
		m_flipperInputTime--;
	}

	//ランチャーの処理
	//発射前のボールは (1440, 960) に固定
	if (!m_isLaunched)
	{
		m_pos = CVector2D(1440, 960);
	}
	//ボタン8を押している間、バネのチャージ量を増やす
	if (!m_isLaunched)
	{
		if (HOLD(CInput::eButton8))
		{
			m_charge++;
			if (m_charge > 57)
			{
				m_charge = 57;
			}
		}
		else if (FREE(CInput::eButton8) && m_charge > 0)
		{
			// ここで発射する
			m_isLaunched = true;//発射済みにする
			m_vec.x = -5.0f;        // 左方向への初速
			m_vec.y = -m_charge;//上方向に速度を与える
			m_charge = 0;//チャージをリセットする
		}
	}

	//Baseのリストに登録されているFlipperをまとめて取得
	std::vector<Base*> flippers = Base::FindObjects(eType_Flipper);
	//今回のフレームで、フリッパーの入力による打ち返しを行ったかを記録する変数
	bool isFlipperLaunched = false;

	//Baseのリストに登録されているWallをまとめて取得
	std::vector<Base*> walls = Base::FindObjects(eType_Wall);
	//Baseのリストに登録されているBumperをまとめて取得
	std::vector<Base*> bumpers = Base::FindObjects(eType_Bumper);

	//発射前は重力も移動も発生しない
	if (m_isLaunched)
	{
		// 重力は1フレームに1回だけ加える
		m_vec.y += GRAVITY;

		// 斜め壁を取得
		std::vector<Base*> slopeWalls = Base::FindObjects(eType_SlopeWall);

		//当たったかどうかを記録　最初は当たってない状態
		bool isHit = false;

		// 1フレームの移動を8分割する
		for (int i = 0; i < 8; i++)
		{
			// 少しだけ移動
			m_pos += m_vec * (1.0f / 8.0f);

			//Wallから1つずつ取り出す→Base* を Wall* に変換する→Wall専用の関数を使えるようにする
			for (Base* b : walls)
			{
				Wall* w = (Wall*)b;

				//dx：ボールと壁の左右方向の距離
				//dy：ボールと壁の上下方向の距離
				float dx = m_pos.x - w->m_pos.x;
				float dy = m_pos.y - w->m_pos.y;

				if (w->m_pos.x == 1505 &&
					m_pos.x < 1525 && m_pos.x > 1480)
				{
					printf("Near Wall: X=%f Y=%f\n", m_pos.x, m_pos.y);
				}

				//fabs() は小数の絶対値を求める関数 マイナスの距離もプラスにして比較できる
				//ボールと壁の中心座標の距離を調べて、横幅・縦幅の範囲内に入ったかを判定している
				if (fabs(dx) < m_rad + w->m_halfWidth &&
					fabs(dy) < m_rad + w->m_halfHeight)
				{
					//当たったときの処理
					//横方向のめり込み量
					float overlapX = m_rad + w->m_halfWidth - fabs(dx);
					//上下方向のめり込み量
					float overlapY = m_rad + w->m_halfHeight - fabs(dy);

					//横と縦、どちらの衝突か判定　横と縦、めり込みが浅い方からの衝突と考えられる
					if (overlapX < overlapY)
					{
						// 左右から衝突　押し戻し
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
							//90%反発　壁にぶつかると10%勢いが落ちる
							m_vec.x *= -0.9f;
						}
					}
					else
					{
						// 上下から衝突　押し戻し
						if (dy >= 0)
						{
							m_pos.y += overlapY;
						}
						else
						{
							m_pos.y -= overlapY;
						}
						//ポヨンッ
						if (dy * m_vec.y < 0.0f)
						{
							//90%反発　壁にぶつかると10%勢いが落ちる
							m_vec.y *= -0.9f;
						}
					}
				}
			}
			//斜め壁との当たり判定
			for (Base* b : slopeWalls)
			{
				SlopeWall* w = (SlopeWall*)b;

				CVector2D normal;
				float overlap;

				if (w->IsHit(m_pos, m_rad, normal, overlap))
				{
					// めり込みを修正
					m_pos += normal * overlap;

					// 壁に向かって進んでいる場合だけ反射
					float normalSpeed = m_vec.Dot(normal);

					if (normalSpeed < 0.0f)
					{
						m_vec -= normal * (1.3f * normalSpeed);
					}
				}
			}
			//バンパーとの当たり判定
			for (Base* b : bumpers)
			{
				Bumper* bumper = (Bumper*)b;

				CVector2D normal;
				float overlap;

				if (bumper->IsHit(m_pos, m_rad, normal, overlap))
				{
					m_pos += normal * overlap;
					//バンパーの反発処理
					float normalSpeed = m_vec.Dot(normal);//normalSpeed：法線方向への速度

					if (normalSpeed < 0.0f)
					{
						m_vec -= normal * (2.0f * normalSpeed);//2.0f：ぶつかったときの反発の強さを決める係数　マイナスの速度を反転させ、ボールを跳ね返す
					}
				}
			}
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
					if (!isFlipperLaunched &&
						f->m_isLeft &&
						m_flipperInputSide == 1 &&
						m_flipperInputTime > 0)
					{
						m_vec.x += tangent.x * 1.5f;
						//縦　威力
						m_vec.y -= 55.0f * power;
						//ここ
						m_vec.x += f->m_angVelocity * 20.0f;

						m_flipperInputTime = 0;
						m_flipperInputSide = 0;

						isFlipperLaunched = true;
					}
					//右フリッパーに当たっていて、右ボタンを押した瞬間 → 上に発射
					else if (!isFlipperLaunched &&
						!f->m_isLeft &&
						m_flipperInputSide == 2 &&
						m_flipperInputTime > 0)
					{
						m_vec.x += tangent.x * 1.5f;
						//縦　威力
						m_vec.y -= 55.0f * power;
						//ここ
						m_vec.x += f->m_angVelocity * 20.0f;

						m_flipperInputTime = 0;
						m_flipperInputSide = 0;

						isFlipperLaunched = true;
					}


					m_wasHit = true;
					isHit = true;
				}
			}
		}
		//どちらかのフリッパーに当たっている → m_wasHit = true
			//どちらにも当たっていない → m_wasHit = false
		if (!isHit)
		{
			m_wasHit = false;
		}
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
	//ランチャーのチャージが増えるほどバネ画像の高さが小さくなる
	float springHeight = 120 - m_charge;
	// 画像サイズを変更
	m_springImg.SetSize(24, springHeight);
	// 縮んだ画像の中心に合わせて中心位置も変更
	m_springImg.SetCenter(12, springHeight / 2);
	// 下端を固定
	m_springImg.SetPos(CVector2D(1440, 1095 - springHeight / 2));
	m_springImg.Draw();

	//ボール
	CVector2D ballPos = m_pos;

	if (!m_isLaunched)
	{
		ballPos.y += m_charge;
	}

	m_img.SetPos(ballPos);
	m_img.Draw();
}