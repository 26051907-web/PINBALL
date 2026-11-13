#pragma once
#include "Base/Base.h"

class Result :public Base
{
private:
	int m_cnt;

public:
	Result();
	void Update()override;
	void Draw()override;

};