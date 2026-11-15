#pragma once
#include "Base/Base.h"

class Game :public Base
{
private:
	//c‹@
	int m_lives;
	//¡A•œŠˆ‘Ò‚¿‚©
	bool m_isRespawning;
public:
	Game();
	void Update()override;
};