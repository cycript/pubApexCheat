#include "CFG.h"
#include "Threads.h"


void SpeakCFG()
{
	printf("\rFoV [%d]  ||| Smoothing  [%d] ||| Bone Selection = [%s]   ||| Maximum Distance = [%d]  Visible Check [%d]        ", int(CFG.Fov), int(CFG.flSmooth), CFG.BoneName, int(CFG.MaxDistance), CFG.bVischeck);
}
void Configuration::cfg()
{
	// Check for uninitialize
	if (GetAsyncKeyState(VK_F2))
		CFG.bActive = false;
	// VISIBLE CHECK TOGGLE
	if (GetAsyncKeyState(VK_XBUTTON2))
	{
		CFG.bVischeck = !CFG.bVischeck;
		Beep(200, 50);
		SpeakCFG();//printf("	\n", bVischeck);
		Sleep(100);

	}
	if (GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_RIGHT))
	{
		CFG.flSmooth += 1.f;
		SpeakCFG();
		Sleep(50);
	}
	else if (GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_LEFT))
	{
		if (CFG.flSmooth > 7.f)	CFG.flSmooth -= 1.f;
		SpeakCFG();
		Sleep(50);
	}
	if (GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_UP))
	{
		CFG.Fov += 1.f;
		SpeakCFG();
		Sleep(50);
	}
	else if (GetAsyncKeyState(VK_CONTROL) && GetAsyncKeyState(VK_DOWN))
	{
		if (CFG.Fov > 1.f)	CFG.Fov -= 1.f;
		SpeakCFG();
		Sleep(50);
	}

	if (GetAsyncKeyState(VK_F7))
	{
		switch (CFG.bone) {
		case 8:
			CFG.bone = 7;
			CFG.BoneName = "neck";
			Beep(2000, 100);
			Sleep(130);
			break;
		case 7:
			CFG.bone = 5;
			CFG.BoneName = "UpperChest";
			Beep(1500, 100);
			Sleep(130);
			break;
		case 5:
			CFG.bone = 3;
			CFG.BoneName = "Chest";
			Beep(1000, 100);
			Sleep(130);
			break;
		case 3:
			CFG.bone = 2;
			CFG.BoneName = "Stomach";
			Beep(500, 100);
			Sleep(130);
			break;
		case 2:
			CFG.bone = 8;
			CFG.BoneName = "Head";
			Beep(3000, 100);
			Sleep(130);
			break;
		default:
			CFG.bone = 7;
			CFG.BoneName = "neck (default)";
			Beep(2000, 100);
			Sleep(130);
			break;
		}
		SpeakCFG();
	}
	if (GetAsyncKeyState(VK_F8)) {
		switch (CFG.bone) {
		case 2:
			CFG.bone = 7;
			CFG.BoneName = "neck";
			Beep(2000, 100);
			Sleep(130);
			break;
		case 7:
			CFG.bone = 2;
			CFG.BoneName = "Stomach";
			Beep(500, 100);
			Sleep(130);
			break;
		default:
			CFG.bone = 2;
			CFG.BoneName = "Stomach";
			Beep(500, 100);
			Sleep(130);
			break;
		}
		SpeakCFG();
	}
	if (GetAsyncKeyState(VK_F5))
	{
		if (CFG.MaxDistance < 800.f)CFG.MaxDistance += 50.f;
		SpeakCFG();
		Sleep(50);
	}
	else if (GetAsyncKeyState(VK_F6))
	{
		if (CFG.MaxDistance > 100.f)	CFG.MaxDistance -= 50.f;
		SpeakCFG();
		Sleep(50);
	}
	Sleep(3000);
	SpeakCFG();
}



bool Configuration::FileExists(const char* chDir) 
{
	return filesystem::exists(chDir);
}

bool Configuration::WriteToConfigFile()
{
	if (!filesystem::exists(CFG.chConfigDirectory))
	{
		if (!filesystem::create_directory(CFG.chConfigDirectory))
		{
			return false;
		}
	}
	if (filesystem::exists(CFG.chConfigDirectory)) 
	{
		if (!FileExists(CFG.chConfigFile))
		{
			std::ofstream ofsOutfile(CFG.chConfigFile);
			ofsOutfile << ("KEY=NULL\nMARK=TRUE\nAim_Key=0x6\nAIMBOT=TRUE\nVISCHECK=FALSE\nLEFTAIM=TRUE\nSEMI_AIM=TRUE\nDSMOOTH=TRUE\nBONEID=2\nFoV=30.0\nRSMOOTH=FALSE\nMAXSmooth=60.0\nMINSMOOTH=50.0\nDRAWFOV=FALSE\nRANDBONE=FALSE\nCLOSEBONE=FALSE\nPlGLOW=FALSE\nITGLOW=FALSE\nESP=FALSE\nBSHADOW=FALSE\nBSHDAMT=0.3\nESPNAME=FALSE\nSHNAME=FALSE\nNSHAMT=0.7\nDIST=FALSE\nSHDIST=FALSE\nDSHAMT=0.7\nESPHEALTH=FALSE\nESPARMOR=FALSE\nLINES=FALSE\nLINETYPE=0\nBOXTYPE=0\nINITFROMCFG=FALSE\n");
			ofsOutfile.close();
			return true;
		}
		else
		{
			if (ShouldOverwriteCFG()) 
			{
				std::ofstream ofsOutfile(CFG.chConfigFile);
				ofsOutfile << ("KEY=NULL\nMARK=TRUE\nAim_Key=0x6\nAIMBOT=TRUE\nVISCHECK=FALSE\nLEFTAIM=TRUE\nSEMI_AIM=TRUE\nDSMOOTH=TRUE\nBONEID=2\nFoV=30.0\nRSMOOTH=FALSE\nMAXSmooth=60.0\nMINSMOOTH=50.0\nDRAWFOV=FALSE\nRANDBONE=FALSE\nCLOSEBONE=FALSE\nPlGLOW=FALSE\nITGLOW=FALSE\nESP=FALSE\nBSHADOW=FALSE\nBSHDAMT=0.3\nESPNAME=FALSE\nSHNAME=FALSE\nNSHAMT=0.7\nDIST=FALSE\nSHDIST=FALSE\nDSHAMT=0.7\nESPHEALTH=FALSE\nESPARMOR=FALSE\nLINES=FALSE\nLINETYPE=0\nBOXTYPE=0\nINITFROMCFG=FALSE\n");
				ofsOutfile.close();
				return true;
			}
			const char* Watermark = "FALSE";
			const char* Aim = "FALSE";
			const char* Vischeck = "FALSE";
			const char* leftAim = "FALSE";
			const char* Semiaim = "FALSE";
			const char* DynamicSmooth = "FALSE";
			const char* RSMOOTH = "FALSE";
			const char* DRAWFOV = "FALSE";
			const char* RANDBONE = "FALSE";
			const char* CLOSEBONE = "FALSE";
			const char* PlGLOW = "FALSE";
			const char* ITGLOW = "FALSE";
			const char* ESP = "FALSE";
			const char* BSHADOW = "FALSE";
			const char* ESPNAME = "FALSE";
			const char* SHNAME = "FALSE";
			const char* DIST = "FALSE";
			const char* SHDIST = "FALSE";
			const char* ESPHEALTH = "FALSE";
			const char* ESPARMOR = "FALSE";
			const char* LINES = "FALSE";
			const char* AUTOUPDATE = "FALSE";
			const char* SCALENAME = "FALSE";
			const char* SCALEDIST = "FALSE";
			if (CFG.bWatermark) Watermark = "TRUE";
			if (CFG.bAimbot) Aim = "TRUE";
			if (CFG.bVischeck) Vischeck = "TRUE";
			if (CFG.bLeftClickAim) leftAim = "TRUE";
			if (CFG.Safebot) Semiaim = "TRUE";
			if (CFG.DyanamicSmooth) DynamicSmooth = "TRUE";
			if (CFG.RandomizeSmoothness) RSMOOTH = "TRUE";
			if (CFG.DrawFoV) DRAWFOV = "TRUE";
			if (CFG.bRandomBones) RANDBONE = "TRUE";
			if (CFG.bClosestBone) CLOSEBONE = "TRUE";
			if (CFG.bGlowPlayers) PlGLOW = "TRUE";
			if (CFG.bGlowItems) ITGLOW = "TRUE";
			if (CFG.bESP) ESP = "TRUE";
			if (CFG.bDarkenBox) BSHADOW = "TRUE";
			if (CFG.bNames) ESPNAME = "TRUE";
			if (CFG.bDarkenName) SHNAME = "TRUE";
			if (CFG.bDarkenDistance) SHDIST = "TRUE";
			if (CFG.bHealth) ESPHEALTH = "TRUE";
			if (CFG.bArmor) ESPARMOR = "TRUE";
			if (CFG.bLines) LINES = "TRUE";
			if (CFG.bDistance) DIST = "TRUE";
			if (CFG.InitializeFromConfig) AUTOUPDATE = "TRUE";
			if (CFG.bScaleDistance) SCALEDIST = "TRUE";
			if (CFG.bScaleNames)  SCALENAME = "TRUE";
			string toWrite="";
			toWrite += string("KEY=");
			if (CFG.bSaveKey)toWrite += CFG.strKey + string("\n");
			else toWrite += string("NULL\n");
			toWrite += string("MARK=") + string(Watermark) + string("\n");
			toWrite += string("Aim_Key=") + static_cast<std::stringstream const&>(std::stringstream() << "0x" << std::hex << CFG.dwAimKey).str() + string("\n");
			toWrite += string("AIMBOT=") + string(Aim) + string("\n");
			toWrite += string("VISCHECK=") + string(Vischeck) + string("\n");
			toWrite += string("LEFTAIM=") + string(leftAim) + string("\n");
			toWrite += string("SEMI_AIM=") + string(Semiaim) + string("\n");
			toWrite += string("DSMOOTH=") + string(DynamicSmooth) + string("\n");
			toWrite += string("BONEID=") + to_string(CFG.bone) + string("\n");
		
			toWrite += string("FoV=") + to_string(CFG.Fov) + string("\n");
			toWrite += string("RSMOOTH=") + string(RSMOOTH) + string("\n");
			toWrite += string("MAXSmooth=") + to_string(CFG.flSmooth) + string("\n");
			toWrite += string("MINSMOOTH=") + to_string(CFG.MinSmooth) + string("\n");
			toWrite += string("DRAWFOV=") + string(DRAWFOV) + string("\n");
			toWrite += string("RANDBONE=") + string(RANDBONE) + string("\n");
			toWrite += string("CLOSEBONE=") + string(CLOSEBONE) + string("\n");
			toWrite += string("PlGLOW=") + string(PlGLOW) + string("\n");
			
			toWrite += string("ITGLOW=") + string(ITGLOW) + string("\n");
			toWrite += string("ESP=") + string(ESP) + string("\n");
			toWrite += string("BSHADOW=") + string(BSHADOW) + string("\n");
			toWrite += string("BSHDAMT=") + to_string(CFG.flBoxDark)+ string("\n");
			toWrite += string("ESPNAME=") + string(ESPNAME) + string("\n");

			toWrite += string("SCALENAME=") + string(SCALENAME) + string("\n");
			toWrite += string("SHNAME=") + string(SHNAME) + string("\n");
			toWrite += string("NSHAMT=") + to_string(CFG.flNameDark) + string("\n");
			toWrite += string("DIST=") + string(DIST) + string("\n");
			toWrite += string("SCALEDIST=") + string(SCALEDIST) + string("\n");
			toWrite += string("SHDIST=") + string(SHDIST) + string("\n");
			toWrite += string("DSHAMT=") + to_string(CFG.flDistDark) + string("\n");
			toWrite += string("ESPHEALTH=") + string(ESPHEALTH) + string("\n");
			toWrite += string("ESPARMOR=") + string(ESPARMOR) + string("\n");
			toWrite += string("LINES=") + string(LINES) + string("\n");
			toWrite += string("LINETYPE=") + to_string(CFG.lineType) + string("\n");
			toWrite += string("BOXTYPE=") + to_string(CFG.dwBoxType) + string("\n");
			toWrite += string("INITFROMCFG=") + string(AUTOUPDATE);
			std::ofstream ofsOutfile(CFG.chConfigFile);
			ofsOutfile << toWrite;
			ofsOutfile.close();
			return true;

		}

	}
}

bool Configuration::ShouldAutoInitFromFile()
{
	string strContent;
	ifstream inFile(CFG.chConfigFile);
	if (inFile)
	{
		std::string strContent((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
		const char* tru = "=TRUE";
		for (int i =0; i <= 4; i++) 
		{

			char charATi = strContent.at(strContent.length() - 1 - i);
			if (charATi != tru[4 - i]) 
			{
				//printf("STRcontent AT  [%d]  [%c]",i,charATi);
				//printf("\n");
				//printf("TRUE	   AT  [%d]  [%c]", i, tru[4-i]);
				//printf("\n");
				return false;
			}
		}
	}
	return true;
}
bool Configuration::ShouldOverwriteCFG() 
{
	if(!FileExists(CFG.chConfigFile))return true;
	string strContent;
	ifstream inFile(CFG.chConfigFile);
	if (inFile)
	{
		std::string strContent((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
		const char* key= "KEY=";
		for (int i = 0; i <= 3; i++)
		{

			char charATi = strContent.at(i);
			if (charATi != key[i])
			{
				//printf("STRcontent AT  [%d]  [%c]",i,charATi);
				//printf("\n");
			//	printf("TRUE	   AT  [%d]  [%c]", i, key[i]);
				//printf("\n");
				return true;
			}
		}
	}
	return false;
}
string Configuration::GetKeyStr() 
{
	string strContent;
	ifstream inFile(CFG.chConfigFile);
	string key;
	if (inFile && !ShouldOverwriteCFG())
	{
		std::string strContent((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
		sscanf(strContent.c_str(), "KEY=%s", &key);
		key.shrink_to_fit();
	}
	//printf("\n 1 ");
//	printf(key.c_str());
	//printf(" 1 \n");
	//printf("KEY ======= %s\n",key);
	return key;
}
bool Configuration::UpdateConfigFromFile()
{
	string strContent;
	ifstream inFile(CFG.chConfigFile);
	if (inFile)
	{
		std::string strContent((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
		char Watermark[64] = "FALSE";
		char Aim[64] = "FALSE";
		char Vischeck[64] = "FALSE";
		char leftAim[64] = "FALSE";
		char Semiaim[64] = "FALSE";
		char DynamicSmooth[64] = "FALSE";
		char  RSMOOTH[64] = "FALSE";
		char   DRAWFOV[64] = "FALSE";
		char RANDBONE[64] = "FALSE";
		char CLOSEBONE[64] = "FALSE";
		char PlGLOW[64] = "FALSE";
		char ITGLOW[64] = "FALSE";
		char ESP[64] = "FALSE";
		char BSHADOW[64] = "FALSE";
		char ESPNAME[64] = "FALSE";
		char SCALENAME[64] = "FALSE";
		char SHNAME[64] = "FALSE";
		char DIST[64] = "FALSE";
		char SCALEDIST[64] = "FALSE";
		char SHDIST[64] = "FALSE";
		char ESPHEALTH[64] = "FALSE";
		char ESPARMOR[64] = "FALSE";
		char LINES[64] = "FALSE";
		char AUTOUPDATE[64] = "FALSE";
		char key[128] = "";
		sscanf(strContent.c_str(), "KEY=%s\nMARK=%s\nAim_Key=0x%X\nAIMBOT=%s\nVISCHECK=%s\nLEFTAIM=%s\nSEMI_AIM=%s\nDSMOOTH=%s\nBONEID=%d\nFoV=%f\nRSMOOTH=%s\nMAXSmooth=%f\nMINSMOOTH=%f\nDRAWFOV=%s\nRANDBONE=%s\nCLOSEBONE=%s\nPlGLOW=%s\nITGLOW=%s\nESP=%s\nBSHADOW=%s\nBSHDAMT=%f\nESPNAME=%s\nSCALENAME=%s\nSHNAME=%s\nNSHAMT=%f\nDIST=%s\nSCALEDIST=%s\nSHDIST=%s\nDSHAMT=%f\nESPHEALTH=%s\nESPARMOR=%s\nLINES=%s\nLINETYPE=%d\nBOXTYPE=%d\nINITFROMCFG=%s",&key, &Watermark, &CFG.dwAimKey, &Aim, &Vischeck, &leftAim, &Semiaim, &DynamicSmooth, &CFG.bone, &CFG.Fov, &RSMOOTH, &CFG.flSmooth, &CFG.MinSmooth, &DRAWFOV, &RANDBONE, &CLOSEBONE, &PlGLOW, &ITGLOW, &ESP, &BSHADOW, &CFG.flBoxDark, &ESPNAME,&SCALENAME, &SHNAME, &CFG.flNameDark, &DIST, &SCALEDIST, &SHDIST, &CFG.flDistDark, &ESPHEALTH, &ESPARMOR, &LINES, &CFG.lineType, &CFG.dwBoxType, &AUTOUPDATE);
	
		if (strcmp(Watermark, "TRUE") == 0)
			CFG.bWatermark = true;
		else
			CFG.bWatermark = false;
		if (strcmp(Aim, "TRUE") == 0)
			CFG.bAimbot = true;
		else
			CFG.bAimbot = false;
		if (strcmp(Vischeck, "TRUE") == 0)
			CFG.bVischeck = true;
		else
			CFG.bVischeck= false;

		if (strcmp(leftAim, "TRUE") == 0)
			CFG.bLeftClickAim = true;
		else
			CFG.bLeftClickAim = false;

		if (strcmp(Semiaim, "TRUE") == 0)
			CFG.Safebot = true;
		else
			CFG.Safebot = false;

		if (strcmp(DynamicSmooth, "TRUE") == 0)
			CFG.DyanamicSmooth = true;
		else
			CFG.DyanamicSmooth = false;
		if (strcmp(RSMOOTH, "TRUE") == 0)
			CFG.RandomizeSmoothness = true;
		else
			CFG.RandomizeSmoothness = false;
		if (strcmp(DRAWFOV, "TRUE") == 0)
			CFG.DrawFoV = true;
		else
			CFG.DrawFoV = false;
		if (strcmp(RANDBONE, "TRUE") == 0)
			CFG.bRandomBones = true;
		else
			CFG.bRandomBones = false;
		if (strcmp(CLOSEBONE, "TRUE") == 0)
			CFG.bClosestBone = true;
		else
			CFG.bClosestBone = false;
		if (strcmp(PlGLOW, "TRUE") == 0)
			CFG.bGlowPlayers = true;
		else
			CFG.bGlowPlayers = false;
		if (strcmp(ITGLOW, "TRUE") == 0)
			CFG.bGlowItems = true;
		else
			CFG.bGlowItems = false;
		if (strcmp(ESP, "TRUE") == 0)
			CFG.bESP = true;
		else
			CFG.bESP = false;
		if (strcmp(BSHADOW, "TRUE") == 0)
			CFG.bDarkenBox = true;
		else
			CFG.bDarkenBox = false;
		if (strcmp(ESPNAME, "TRUE") == 0)
			CFG.bNames = true;
		else
			CFG.bNames = false;
		if (strcmp(SHNAME, "TRUE") == 0)
			CFG.bDarkenName = true;
		else
			CFG.bDarkenName = false;
		if (strcmp(DIST, "TRUE") == 0)
			CFG.bDistance = true;
		else
			CFG.bDistance = false;
		if (strcmp(SHDIST, "TRUE") == 0)
			CFG.bDarkenDistance = true;
		else
			CFG.bDarkenDistance = false;
		if (strcmp(ESPHEALTH, "TRUE") == 0)
			CFG.bHealth = true;
		else
			CFG.bHealth = false;
		if (strcmp(ESPARMOR, "TRUE") == 0)
			CFG.bArmor = true;
		else
			CFG.bArmor = false;

		if (strcmp(LINES, "TRUE") == 0) 
			CFG.bLines = true;
		else
			CFG.bLines = false;
		if (strcmp(AUTOUPDATE, "TRUE") == 0)
			CFG.InitializeFromConfig = true;
		else
			CFG.InitializeFromConfig = false;

		if (strcmp(SCALENAME, "TRUE") == 0)
			CFG.bScaleNames = true;
		else
			CFG.bScaleNames = false;
		if (strcmp(SCALEDIST, "TRUE") == 0)
			CFG.bScaleDistance = true;
		else
			CFG.bScaleDistance = false;
		if (key!="NULL")
			CFG.strKey = key;
		inFile.close();
		return true;
	}
	return false;
}

bool Configuration::InitializeConfig()
{
	return WriteToConfigFile() && UpdateConfigFromFile();
}