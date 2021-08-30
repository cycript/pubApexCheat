#pragma once
#include <thread>
#include "Maths.h"
#include "Singleton.h"
#include "ApexLegends.h"
#include "Overlay.h"
#include "CFG.h"
#include <string>
#include <cmath>

using namespace std;


namespace Threads
{
	class Variables : public Singleton<Variables>
	{
	public:
		thread thApexLegendsThread;
		thread ItemGlowthread;
		thread tempPlayerGlow;
	};
#define ThreadVars Threads::Variables::Get()
	void Glow();
	void ApexLegendsAimThread();
	void Itemglow();
	void CRC();
}
