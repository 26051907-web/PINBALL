#include "Game/Game.h"
#include "Game/Player.h"
#include "Game/Wall.h"
#include "Game/Flipper.h"
#include "Title/Result.h"
#include "Game/SlopeWall.h"

Game::Game() :Base(eType_Scene)
{
    //Gameが残機3を覚えておける
    m_lives = 3;
    //復活待ちか
    m_isRespawning = false;

    //new Player(CVector2D(960, 400));
    new Player(CVector2D(1200, 930));

	new Wall(CVector2D(120, 620));
    new Wall(CVector2D(1800, 620));

    new SlopeWall(CVector2D(400, 750),true);

	new Flipper(CVector2D(800, 980),true);
	new Flipper(CVector2D(1120, 980), false);
}

void::Game::Update()
{
    //ボールがいない かつ まだ復活処理をしていない
    if (!Base::FindObject(eType_Player) && !m_isRespawning)
    {
        // ボールが存在しないとき、残機が1減る
        m_lives--;
        m_isRespawning = true;

        if (m_lives > 0)
        {
            //new Player(CVector2D(960, 400));
            new Player(CVector2D(1200, 930));
            m_isRespawning = false;
        }
        else
        {
            // ゲームオーバー処理
            Base::KillAll();
            new Result();
            //return でGameの更新を終了
            return;
        }
    }
}