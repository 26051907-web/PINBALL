#include "Result.h"
#include "Title.h"
#include "Game/Game.h"

Result::Result() :Base(eType_Result)
{
	m_cnt = 0;
}

void Result::Update()
{
	m_cnt++;

	//Zキーを押したらもう一度ゲームを開始
	if (m_cnt > 30 && PUSH(CInput::eButton1)) {
		Base::KillAll();
		new Game();
	}
	//タイトルに戻る
	if (m_cnt > 30 && PUSH(CInput::eButton3)) {
		Base::KillAll();
		new Title();
	}
}

void Result::Draw()
{
	//画面全体に半透明の黒い四角形を描画
	Utility::DrawQuad(
		CVector2D(0, 0),
		CVector2D(1920, 1080),
		CVector4D(0.0f, 0.0f, 0.0f, 0.6f)
	);
}