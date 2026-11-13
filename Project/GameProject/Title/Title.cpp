#include "Title.h"
#include "Game/Game.h"
Title::Title() :Base(eType_Scene)
{
	m_img = COPY_RESOURCE("Title", CImage);
	m_cnt = 0;
}
void Title::Update()
{
	if (m_cnt++ > 2 && PUSH(CInput::eButton1)) {
		Base::KillAll();
		new Game();
	}
}
void Title::Draw()
{
	m_img.Draw();
}