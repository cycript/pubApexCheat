#pragma once
#include "ApexLegends.h"

#include <string>
#include <sstream>
#include <iostream>
#include "imgui/imgui.h"


inline namespace Configuration
{
	class Settings : public Singleton<Settings>
	{
	public:
		int dwMajor = 1;
		int dwMinor = 2;
		//MISC
		const char* BoxTypes[2] = { "Full Box","Cornered Box" };
		const char* LineTypes[3] = { "Bottom To Enemy","Top To Enemy"," Crosshair To Enemy" };
		string BoneName = "Stomach";
		const char* BoneNamess[5] = { "Head","neck","UpperChest","Chest","Stomach" };
		int BoneIds[5] = { 8,7,5,3,2 };
		bool bWatermark = true;
		bool bActive = true;
		bool bSaveKey = false;
		string strKey = "";

		//
		const char* chConfigDirectory = "C:/Jaguar/";
		const char* chConfigFile = "C:/Jaguar/LunewareApex.txt";

		//

		int dwAimKey = 0x12;   // 0x12 = alt  0x6 = XBTN2
		
		//Aimbot
		bool bAimbot = true;
		bool bVischeck = false;
		bool bLeftClickAim = false;
		bool Safebot = false;
		bool DyanamicSmooth = false;
		int bone = 2;
		float Fov = 20.f;
		
		bool RandomizeSmoothness = false;
		float flSmooth = 60.f;
		float MinSmooth = 50.f;

		bool DrawFoV = false;
		//
		bool bRandomBones = false;
		bool bClosestBone = true;
		// 
		//ESP Configuration 
		bool bGlowPlayers = false;
		bool bGlowItems = false;
		bool bESP = true;
		ImColor BoxVisColor = ImColor(255.f/255,0.f,0.f,1.f);
		float flBoxVColors[4] = { 255.f/255,0.f,0.f,1.f };//
		ImColor BoxInViscolor = ImColor(0.f, 0.f, 255.f/255, 1.f);
		float flBoxInvisColors[4] = { 0.f,0.f,255.f/255,1.f };//
		bool bDarkenBox = false;
		float flBoxDark = 0.3;

		bool bNames = false;
		bool bScaleNames = false;
		 bool bDarkenName = false;
		float flNameDark = 0.7;

		bool bDistance = false;
		bool bScaleDistance = false;
		 bool bDarkenDistance = false;
		float flDistDark = 0.7;

		bool bHealth = false;
		ImColor HealthBarColor = ImColor(0.f, 255.f/255.f, 0.f, 1.f); 
		float  HealthBar[4] = { 0.f,255.f/255,0.f,1.f };//
		bool bArmor = false;
		ImColor ArmorBarColor = ImColor(105.f/255, 13.f/255, 173.f/255, 1.f);
		float ArmorBar[4] = { 105.f/255, 13.f/255, 173.f/255, 1 };//
		int dwBoxType= 0;
		// Lines
		bool bLines = false;
		int lineType = 0;                 //  0 bottom to enemy, 1  top TO enemy 2 , crosshair to enemy
		ImColor LineColor = ImColor(0.f, 0.f, 255.f/255, 1.f);
		float LineColors[4] = { 0,0,255.f/255,1 };  //
		//Universal 
		float MaxDistance = 300.f;
		bool InitializeFromConfig = FALSE;
	};
#define CFG Configuration::Settings::Get()
	void cfg();
	bool FileExists(const char* chDir);
	bool WriteToConfigFile();
	bool UpdateConfigFromFile();
	bool InitializeConfig();
	bool ShouldAutoInitFromFile();
	bool ShouldOverwriteCFG();
	string GetKeyStr();
}