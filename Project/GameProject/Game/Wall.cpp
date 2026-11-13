#include "Base/Base.h"
#include "Game/Wall.h"

Wall::Wall(const CVector2D& pos) :Base(eType_Wall)
{
	m_img = COPY_RESOURCE("Wall", CImage);
	m_pos = pos;
	m_img.SetCenter(16,80);
	//m_img.SetSize(1000, 1000);

}
void Wall::Update()
{

}
void Wall::Draw()
{
	m_img.SetPos(m_pos);
	m_img.Draw();
}