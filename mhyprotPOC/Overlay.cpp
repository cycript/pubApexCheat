#include "Overlay.h"
static ImFont* centuryGothic;
static ImFont* centuryGothicH;
static ImFont* headerCenturyGothic;
static ImFont* iLunarian;
static ImFont* iLunarianH;
static ImFont* KarlaFont;
ImGuiContext* imGuiContext;
static int CurrentBoneName = 4;

bool IsKeyDown(int KEY)
{
	return (bool)(GetAsyncKeyState(KEY) & 0x8000);
}

static DWORD WINAPI StaticMessageStart(void* Param)
{
	Overlay* ov = (Overlay*)Param;
	ov->CreateOverlay();
	return 0;
}

BOOL CALLBACK EnumWindowsCallback(HWND hwnd, LPARAM lParam)
{
	wchar_t className[255] = L"";
	GetClassName(hwnd, className, 255);

	if (wcscmp(L"Luneware", className) == 0) //Custom overlay
	{
		HWND* w = (HWND*)lParam;
		*w = hwnd;
		return TRUE;
	}
	return TRUE;
}

// Data
static LPDIRECT3D9              g_pD3D = NULL;
static LPDIRECT3DDEVICE9        g_pd3dDevice = NULL;
static D3DPRESENT_PARAMETERS    g_d3dpp = {};

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void ResetDevice();



/*
 	-----------------		x
	|
	|
	|						y
	|


	const char* text = "testname";
	float X = 80;
	float Y = 150;
	float boxW = 60;
	float boxH = 100;
	DrawOutlinedText(KarlaFont, string("Test"), ImVec2(X, Y-20), 15, WHITE, false);
	DrawCorneredBox(150, 150, 60, 100, ImColor(0.f, 0.f, 255.f), 2.f);
	DrawBox(RED, X, Y, boxW, boxH);
	//DrawOutlinedText(KarlaFont, string("Distance"), ImVec2(X, Y+boxH+1 ), 15, WHITE, false);
		//ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(80, 150), ImVec2(140, 250), WHITE, 5.f);
	//string lol = "ahahah go get fucked";
	//DrawOutlinedText(centuryGothic, lol, ImVec2(200,50), 15, WHITE, true);
	//RectFilled(200-textSize.x/2 -3,50-2,200+textSize.x/2 +1,50+textSize.y +2, ImColor(0.01f, 0.01f, 0.01f, 0.7), 0, 0);

	DrawOutlinedText(KarlaFont, string(dist), ImVec2(X, Y + boxH + 1), 15, WHITE, false);
*/
void Overlay::RenderEsp(int dwEntity)
{
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2((float)getWidth(), (float)getHeight()));
	ImGui::Begin(XorStr("##BOX"), (bool*)true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus);
	auto width = getWidth();
	auto height = getHeight();
	char dist[4];


	if (CFG.DrawFoV) 
	{
		DrawCircle(width / 2, height / 2,Maths::FoV(CFG.Fov), WHITE);
	}
	if (CFG.bESP)
	{
		uintptr_t pLocalPlayer = ApexLegends::GetLocalPlayer();
		for (int i = 0; i <= 60; i++)
		{
			uintptr_t pEntityBase = ApexLegends::GetEntityBase(i);
			if (pEntityBase && (pEntityBase != pLocalPlayer) && pEntityBase != NULL)
			{
				if (ApexLegends::IsPlayer(pEntityBase))
				{
					bool isEnemy =  ApexLegends::IsEnemy(pEntityBase);
					if (isEnemy && ApexLegends::IsAlive(pEntityBase))
					{
						float distance = ApexLegends::GetLocalPosition().Distance(ApexLegends::GetEntityPosition(pEntityBase));
						if (distance <= CFG.MaxDistance * 40 && distance > 50.f)
						{
							auto headpos = ApexLegends::GetEntityBonePos(pEntityBase, 8);
							headpos.z += 5.f;
							Vector3 w2sHead = ApexLegends::WorldToScreen({ headpos.x,headpos.y,headpos.z }, width, height);
							Vector3 w2sBase = ApexLegends::WorldToScreen(ApexLegends::GetEntityPosition(pEntityBase), width, height);
							if ((w2sHead.X < width || w2sHead.Y < height) && ((w2sHead.X > 0 && w2sHead.Y > 0)))
							{
								float box_height = (w2sHead.Y - w2sBase.Y);
								float box_width = box_height / 1.2f;
								if (CFG.dwBoxType == 0) {
									if (APEXVars.Visible[i])
									{
										//ImGui::GetOverlayDrawList()->AddRect(ImVec2(w2sHead.X + (box_width / 2), w2sHead.Y), ImVec2(w2sBase.X - box_width/2, w2sBase.Y), CFG.BoxVisColor, 0.f);
										DrawBox(CFG.BoxVisColor,w2sHead.X - (box_width / 2) , w2sBase.Y, box_width, box_height);
									}
									else
									{
										//ImGui::GetOverlayDrawList()->AddRect(ImVec2(w2sHead.X + (box_width / 2), w2sHead.Y), ImVec2(w2sBase.X - box_width / 2, w2sBase.Y), CFG.BoxInViscolor,0.f);
										DrawBox(CFG.BoxInViscolor, w2sHead.X - (box_width / 2), w2sBase.Y, box_width, box_height);
									}
									if (CFG.bDarkenBox) 
									{
										RectFilled(w2sHead.X - (box_width / 2), w2sBase.Y, w2sHead.X - (box_width / 2) + box_width, w2sBase.Y + box_height, ImColor(0.01f, 0.01f, 0.01f, 0.3), 0, 0);
									}

									if (CFG.bHealth)
									{
										float width = box_width / 10;
										if (width < 2.f) width = 2.;
										if (width > 3) width = 3.;
										
										ProgressBar(w2sHead.X + (box_width / 2) - width, w2sHead.Y, width, w2sBase.Y - w2sHead.Y, APEXVars.PlayersHealth[i], 100, CFG.HealthBarColor, true);
									}
									if (CFG.bArmor)
									{
										float width = box_width / 10;
										if (width < 2.f) width = 2.;
										if (width > 3) width = 3.;
										ProgressBar(w2sHead.X - (box_width / 2), w2sHead.Y, width, w2sBase.Y - w2sHead.Y, APEXVars.PlayersArmor[i], APEXVars.MaxPlayerArmor[i], CFG.ArmorBarColor, true);
									}
								}
								if (CFG.dwBoxType == 1) 
								{
									if (APEXVars.Visible[i])
									{
										DrawCorneredBox(w2sHead.X - (box_width / 2), w2sBase.Y, box_width, box_height, CFG.BoxVisColor, 1.5f);
									}
									else
									{
										DrawCorneredBox(w2sHead.X - (box_width / 2), w2sBase.Y, box_width, box_height, CFG.BoxInViscolor, 1.5f);
									}
									if (CFG.bDarkenBox)
									{
										RectFilled(w2sHead.X - (box_width / 2), w2sBase.Y, w2sHead.X - (box_width / 2) + box_width, w2sBase.Y + box_height, ImColor(0.01f, 0.01f, 0.01f, 0.3), 0, 0);
									}

								}
								if (CFG.bNames) 
								{	
									int size = 15;
									if (distance > 1200.f && CFG.bScaleNames) 
									{
										size -= (distance / 40) / 150;
									}
									DrawOutlinedText(centuryGothic, APEXVars.PlayerNames[i], ImVec2(w2sHead.X, w2sHead.Y - 20), size, WHITE, true);
									if (CFG.bDarkenName)
									{
										ImVec2 textSize = centuryGothic->CalcTextSizeA(size, FLT_MAX, 0.0f, APEXVars.PlayerNames[i].c_str());
										RectFilled(w2sHead.X - textSize.x / 2 - 3, w2sHead.Y - 20 - 1, w2sHead.X + textSize.x / 2 + 1, w2sHead.Y - 20 + textSize.y + 1, ImColor(0.01f, 0.01f, 0.01f, CFG.flNameDark), 0, 0);
										//RectFilled(w2sHead.X - (box_width / 2), w2sHead.Y - 20, w2sHead.X + (box_width / 2), w2sHead.Y, ImColor(0.01f, 0.01f, 0.01f, 0.7), 0, 0);
									}

								}
								if (CFG.bDistance)
								{
									int size = 15;
									if (distance > 1200.f && CFG.bScaleNames)
									{
										size -= (distance / 40) / 150;
									}
									sprintf(dist, "%d", static_cast<int>(distance/40));
									string toPrint = dist + string("M");
									DrawOutlinedText(centuryGothic, toPrint, ImVec2(w2sHead.X, w2sBase.Y + 1), size, WHITE, true);
									if (CFG.bDarkenDistance)
									{
										ImVec2 textSize = centuryGothic->CalcTextSizeA(size, FLT_MAX, 0.0f, toPrint.c_str());
										RectFilled(w2sHead.X - textSize.x / 2 - 3, w2sBase.Y, w2sHead.X + textSize.x / 2 + 1, w2sBase.Y + 1 + textSize.y + 1, ImColor(0.01f, 0.01f, 0.01f, CFG.flNameDark), 0, 0);
										//RectFilled(w2sHead.X - (box_width / 2), w2sBase.Y +1, w2sHead.X + (box_width / 2), w2sBase.Y +3, ImColor(0.01f, 0.01f, 0.01f, 0.7), 0, 0);
									}
								}
								if (ApexLegends::isDowned(pEntityBase)) 
								{
									DrawOutlinedText(KarlaFont, string("Knocked"), ImVec2(w2sHead.X, w2sBase.Y + 1+10), 10, RED, true);
								}
								if (CFG.bLines) 
								{
									if (CFG.lineType == 0) 
									{
										DrawLine(ImVec2(static_cast<float>(width / 2), static_cast<float>(height)), ImVec2(w2sHead.X, w2sBase.Y + 15), CFG.LineColor, 1.3f); //LINE FROM BOTTOM SCREEN
									}
									if (CFG.lineType == 1)
									{
										DrawLine(ImVec2(static_cast<float>(width / 2), 0.f), ImVec2(w2sHead.X, w2sHead.Y - 15), CFG.LineColor, 1); //LINE FROM TOP SCREEN
									}
									if (CFG.lineType == 2)
									{
										DrawLine(ImVec2(static_cast<float>(width / 2), static_cast<float>(height/2)), ImVec2(w2sHead.X, w2sBase.Y + 15), CFG.LineColor, 1.3f); //LINE FROM BOTTOM SCREEN
									}
								}
							}
						}
					}
				}
			}
		}
		if (CFG.bWatermark) 
		{
			ImVec4 vecColorDarkRed = ImVec4(0.5f, 0.0f, 0.0f, 1.0f);
			ImGui::SetCursorPos(ImVec2(5, 7));
			ImGui::PushFont(iLunarianH);
			ImGui::TextColored(vecColorDarkRed, "A");
			ImGui::PopFont();
			ImGui::PushFont(centuryGothic);
			ImGui::SetCursorPos(ImVec2(3,3));
			ImGui::TextColored(ImVec4(0.13, 0.0, 0.0, 1.0), "Lunariel Framework");
			ImGui::PopFont();
		}
	
	}
}
void TranslateBone() 
{
	CFG.bone = CFG.BoneIds[CurrentBoneName];
}
void SeparatePanel(const char* panelTitle, ImVec2 controlLocation = ImGui::GetWindowPos(), float cursorPosY = 30.0f)
{
	ImVec4 vecColorWhite = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 vecColorDarkRed = ImVec4(0.5f, 0.0f, 0.0f, 1.0f);
	ImU32 imColorWhite = ImGui::ColorConvertFloat4ToU32(vecColorWhite);
	ImU32 imColorDarkRed = ImGui::ColorConvertFloat4ToU32(vecColorDarkRed);
	ImGui::Text(panelTitle);
	ImGui::GetWindowDrawList()->AddLine(ImVec2(controlLocation.x, controlLocation.y + 22.5f), ImVec2(controlLocation.x + 150.0f, controlLocation.y + 22.5f), imColorDarkRed, 0.5f);
	ImGui::GetWindowDrawList()->AddLine(ImVec2(controlLocation.x + 150.0f, controlLocation.y + 22.5f), ImVec2(controlLocation.x + 340.0f, controlLocation.y + 22.5f), imColorWhite, 0.5f);
	ImGui::SetCursorPosY(cursorPosY);
}
void AimbotPanel()
{
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.0f, 0.0f, 0.01f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(255.0f, 0.0f, 0.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(15, 0.0f, 0.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(100, 0, 0, 1));
	ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(100, 0, 0, 1));

	SeparatePanel("Aim Assistance Panel");
	ImGui::BeginChild("Slider");
	ImGui::Checkbox("Enabled Aimbot ", &CFG.bAimbot);
	if (CFG.bAimbot)
	{

		ImGui::Checkbox("Visibility Check", &CFG.bVischeck);
		ImGui::Checkbox("Left-Click Aimbot ", &CFG.bLeftClickAim);
		ImGui::TextColored(ImVec4(255, 255, 255, 1), "Aim Assistance Variables");
		ImGui::SliderFloat("Max FoV", &CFG.Fov, 5.f, 90.f);
		ImGui::Checkbox("Draw FoV", &CFG.DrawFoV);
		//
		if (!CFG.RandomizeSmoothness)
		{
			if(	ImGui::SliderFloat("Smoothing", &CFG.flSmooth, 20.f, 100.f))
			{
				if (CFG.flSmooth < CFG.MinSmooth) CFG.MinSmooth = CFG.flSmooth - 2.f;
			}
		}
		ImGui::Checkbox("Randomize Smoothness", &CFG.RandomizeSmoothness);

		//
		if (CFG.RandomizeSmoothness)
		{
			if (ImGui::SliderFloat("Maximum Smoothing", &CFG.flSmooth, 20.f, 100.f))
			{
				if (CFG.flSmooth < CFG.MinSmooth) CFG.MinSmooth = CFG.flSmooth - 2.f;
			}
			if (ImGui::SliderFloat("Minimum Smoothness", &CFG.MinSmooth, 20, 80) )
			{
				if (CFG.MinSmooth > CFG.flSmooth) CFG.flSmooth = CFG.MinSmooth + 2.f;
			}
		}
		//
		ImGui::TextColored(WHITE, "Global Variables");
		ImGui::SliderFloat("Maximum Distance", &CFG.MaxDistance, 100.f, 800.f);
		//
		ImGui::Checkbox("Closest Bone", &CFG.bClosestBone);
		if (!CFG.bClosestBone)
		{
			if (ImGui::Combo("Aim Assist Bones", &CurrentBoneName, CFG.BoneNamess, 5))
				TranslateBone();
		}
		ImGui::Checkbox("Randomize Bone", &CFG.bRandomBones);
		//

		ImGui::Checkbox("Aim Assist Mode", &CFG.Safebot);

		ImGui::Checkbox("Dynamic Smoothness", &CFG.DyanamicSmooth);
		
	
	}
	ImGui::EndChild();
}
void RenderVisuals()
{
	
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.0f, 0.0f, 0.01f));
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(255.0f, 0.0f, 0.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Header, ImVec4(15, 0.0f, 0.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_HeaderHovered, ImVec4(100, 0, 0, 1));
	ImGui::PushStyleColor(ImGuiCol_HeaderActive, ImVec4(100, 0, 0, 1));

	SeparatePanel("Visual Customization Panel");
	ImGui::BeginChild("Slider");
	ImGui::Checkbox("Enabled Player Glow (Internal) ", &CFG.bGlowPlayers);
	ImGui::Checkbox("Enabled Item Glow (Internal) ", &CFG.bGlowItems);
	ImGui::Checkbox("Enabled Box ESP", &CFG.bESP);
	ImGui::SameLine();
	ImGui::Checkbox("Box Dark Background", &CFG.bDarkenBox);
	if (CFG.bDarkenBox)
	{
		ImGui::SliderFloat("Box Darkness", &CFG.flBoxDark, 0.01, 1.0);
	}
	if (CFG.bESP)
	{
		if (ImGui::ColorEdit4("Visible Enemy Box Color", CFG.flBoxVColors))
		{
			CFG.BoxVisColor = ImColor(CFG.flBoxVColors[0], CFG.flBoxVColors[1], CFG.flBoxVColors[2], CFG.flBoxVColors[3]);
		}
		if (ImGui::ColorEdit4("Invisible Enemy Box Color", CFG.flBoxInvisColors))
		{
			CFG.BoxInViscolor = ImColor(CFG.flBoxInvisColors[0], CFG.flBoxInvisColors[1], CFG.flBoxInvisColors[2], CFG.flBoxInvisColors[3]);
		}
		ImGui::Checkbox("Enabled Name ESP", &CFG.bNames);
		if (CFG.bNames) 
		{
			ImGui::SameLine();
			ImGui::Checkbox("Name Text Scaling", &CFG.bScaleNames);
			ImGui::Checkbox("Name Dark Background", &CFG.bDarkenName);
			if (CFG.bDarkenName)
			{
				ImGui::SliderFloat("Name Darkness", &CFG.flNameDark, 0.01, 1.0);
			}
		}
		ImGui::Checkbox("Enabled Distance ESP", &CFG.bDistance);
		if (CFG.bDistance)
		{
			ImGui::SameLine();
			ImGui::Checkbox("Distance Text Scaling", &CFG.bScaleDistance);
			ImGui::Checkbox("Distance Dark Background", &CFG.bDarkenDistance);
			if (CFG.bDarkenDistance)
			{
				ImGui::SliderFloat("Distance Darkness", &CFG.flDistDark, 0.01, 1.0);
			}
		}
		ImGui::Combo(" ESP Box Type", &CFG.dwBoxType, CFG.BoxTypes, 2);
		ImGui::Checkbox("Enabled Line ESP", &CFG.bLines);
		if (CFG.bLines)
		{
			ImGui::Combo("Line ESP Type", &CFG.lineType, CFG.LineTypes, 3);
			if (ImGui::ColorEdit4("Line ESP Color", CFG.LineColors))
			{
				CFG.LineColor = ImColor(CFG.LineColors[0], CFG.LineColors[1], CFG.LineColors[2], CFG.LineColors[3]);
			}
		}
		ImGui::TextColored(ImVec4(255, 0, 0, 255), "May be misaligned ~~ ");
		ImGui::Checkbox("Health Bars", &CFG.bHealth);
		if (CFG.bHealth)
		{
			if (ImGui::ColorEdit4("Health Bar Color", CFG.HealthBar))
			{
				CFG.HealthBarColor = ImColor(CFG.HealthBar[0], CFG.HealthBar[1], CFG.HealthBar[2], CFG.HealthBar[3]);
			}
		}
		ImGui::Checkbox("Armor Bars", &CFG.bArmor);
		if (CFG.bArmor)
		{
			if (ImGui::ColorEdit4("Armor Bar Color", CFG.ArmorBar))
			{
				CFG.ArmorBarColor = ImColor(CFG.ArmorBar[0], CFG.ArmorBar[1], CFG.ArmorBar[2], CFG.ArmorBar[3]);
			}
		}
	}
	ImGui::EndChild();
}
void RenderCFG() 
{
	ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.0f, 0.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(255.0f, 0.0f, 0.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.0f, 0.0f, 0.01f));
	SeparatePanel("Configs Panel");
	ImGui::Checkbox("Watermark", &CFG.bWatermark);
	ImGui::Checkbox("Save User Key", &CFG.bSaveKey);
	if (ImGui::Button("Save Current Configuration"))
	{
		Configuration::WriteToConfigFile();
	}
	if (ImGui::Button("Load Saved Configuration"))
	{
		Configuration::UpdateConfigFromFile();
	}
	ImGui::Checkbox("Load Config when initializing", &CFG.InitializeFromConfig);
	ImGui::Separator();
	if (ImGui::Button("Uninitialize"))
	{
		CFG.bActive = false;
		exit(0);
	}

}
void Overlay::RenderMenu()
{	
	auto width = getWidth();
	auto height = getHeight();
	ImGuiStyle* imGuiStyle = &ImGui::GetStyle();
	ImGui::SetNextWindowSize(ImVec2(640, 480), ImGuiCond_Once);
	ImGui::SetNextWindowPos(ImVec2(width / 2 - (640 / 2), height / 2 - (480 / 2)), ImGuiCond_Once);
	ImGui::SetNextWindowContentSize(ImVec2(800, 600));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 1.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
	if (ImGui::Begin("Jaguar Software Panel", &isVisible, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar))
	{
		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec4 vecColorWhite = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		ImVec4 vecColorDimWhite = ImVec4(1.0f, 1.0f, 1.0f, 0.05f);
		ImVec4 vecColorBorderDimWhite = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);
		ImVec4 vecColorRed = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
		ImVec4 vecColorDarkRed = ImVec4(0.5f, 0.0f, 0.0f, 1.0f);
		ImVec4 vecColorButtonDarkRed = ImVec4(0.5f, 0.0f, 0.0f, 0.01f);
		ImVec4 vecColorTab = ImVec4(0.25f, 0.0f, 0.0f, 1.0f);
		ImVec4 vecColorGreen = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
		ImVec4 vecColorBlue = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
		ImVec4 vecColorYellow = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
		ImVec4 vecColorBlack = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
		ImVec4 vecColorDimBlack = ImVec4(0.0f, 0.0f, 0.0f, 0.5f);
		ImVec4 vecColorGrey = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
		ImU32 imColorWhite = ImGui::ColorConvertFloat4ToU32(vecColorWhite);
		ImU32 imColorDimWhite = ImGui::ColorConvertFloat4ToU32(vecColorDimWhite);
		ImU32 imColorBorderDimWhite = ImGui::ColorConvertFloat4ToU32(vecColorBorderDimWhite);
		ImU32 imColorRed = ImGui::ColorConvertFloat4ToU32(vecColorRed);
		ImU32 imColorDarkRed = ImGui::ColorConvertFloat4ToU32(vecColorDarkRed);
		ImU32 imColorButtonDarkRed = ImGui::ColorConvertFloat4ToU32(vecColorButtonDarkRed);
		ImU32 imColorBlack = ImGui::ColorConvertFloat4ToU32(vecColorBlack);
		ImU32 imColorDimBlack = ImGui::ColorConvertFloat4ToU32(vecColorDimBlack);
		//Header
		ImGui::PushFont(iLunarianH);
		ImGui::TextColored(vecColorDarkRed, "A");
		ImGui::PopFont();
		ImGui::PushFont(headerCenturyGothic);
		ImGui::SetCursorPos(ImVec2(94.5f, 20.0f));
		ImGui::Text("Lunariel Framework ~ Apex Legends");
		ImGui::SameLine(272.5f);
		ImGui::TextColored(vecColorGreen, "1.2");
		ImGui::SameLine(92.5f, -1.0f);
		ImGui::SetCursorPos(ImVec2(92.5f, 25.0f));
		ImGui::PopFont();
		ImGui::PushFont(centuryGothicH);
		ImGui::TextColored(vecColorRed, "J");
		ImGui::SameLine(108.0f, -1);
		ImGui::Text("aguar");
		ImGui::GetWindowDrawList()->AddLine(ImVec2(windowPos.x + 3.0f, windowPos.y + 82.5f), ImVec2(windowPos.x + 120.0f, windowPos.y + 82.5f), imColorDarkRed, 0.5f);
		ImGui::GetWindowDrawList()->AddLine(ImVec2(windowPos.x + 120.0f, windowPos.y + 82.5f), ImVec2(windowPos.x + 480.0f, windowPos.y + 82.5f), imColorWhite, 0.5f);
		ImGui::PopFont();
		//Borders
		ImGui::GetWindowDrawList()->AddRect(ImVec2(windowPos.x, windowPos.y), ImVec2(windowPos.x + 640.0f, windowPos.y), imColorDarkRed, 0.0f, 0, 2.0f);
		ImGui::GetWindowDrawList()->AddRect(ImVec2(windowPos.x, windowPos.y), ImVec2(windowPos.x, windowPos.y + 480.0f), imColorBorderDimWhite, 0.0f, 0, 2.0f);
		//Side Main Button
		ImGui::PushStyleColor(ImGuiCol_ChildBg, vecColorDarkRed);
		ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.0f);
		ImGui::SetNextWindowPos(ImVec2(windowPos.x + 2.0f, windowPos.y + 92.5f), ImGuiCond_Always);
		ImGui::SetNextWindowBgAlpha(0.1f);
		if (ImGui::BeginChild("Jaguar Side Tab", ImVec2(45.0f, 380.0f), false, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove))
		{
			ImVec2 sideTabButtonSize = ImVec2(45.0f, 45.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_Alpha, 1.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, ImVec2(0.5f, -5.0f));
			ImGui::PushStyleColor(ImGuiCol_Button, imColorButtonDarkRed);
			ImGui::PushStyleColor(ImGuiCol_ButtonActive, imGuiStyle->Colors[ImGuiCol_WindowBg]);
			ImGui::PushStyleColor(ImGuiCol_ButtonHovered, imGuiStyle->Colors[ImGuiCol_WindowBg]);
			ImGui::PushFont(iLunarian);
			//----------------------------------------------------------------------------//
			ImGui::SetCursorPosY(0.0f);
			if (currentMenu == 0 && !pushCalled)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, vecColorRed);
				ImGui::PushStyleColor(ImGuiCol_Button, imGuiStyle->Colors[ImGuiCol_WindowBg]);
				pushCalled = true;
			}
			if (ImGui::Button("B", sideTabButtonSize))
			{
				currentMenu = 0;
			}
			if (currentMenu == 0 && pushCalled)
			{
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				pushCalled = false;
			}
			//----------------------------------------------------------------------------//
			ImGui::SetCursorPosY(65.0f);
			if (currentMenu == 1 && !pushCalled)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, vecColorDarkRed);
				ImGui::PushStyleColor(ImGuiCol_Button, imGuiStyle->Colors[ImGuiCol_WindowBg]);
				pushCalled = true;
			}
			if (ImGui::Button("D", sideTabButtonSize))
			{
				currentMenu = 1;
			}
			if (currentMenu == 1 && pushCalled)
			{
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				pushCalled = false;
			}
			//----------------------------------------------------------------------------//
			ImGui::SetCursorPosY(130.0f);
			if (currentMenu == 2 && !pushCalled)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, vecColorYellow);
				ImGui::PushStyleColor(ImGuiCol_Button, imGuiStyle->Colors[ImGuiCol_WindowBg]);
				pushCalled = true;
			}
			if (ImGui::Button("E", sideTabButtonSize))
			{
				currentMenu = 2;
			}
			if (currentMenu == 2 && pushCalled)
			{
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				pushCalled = false;
			}
			//----------------------------------------------------------------------------//
			/*ImGui::SetCursorPosY(198.75f);
			if (currentMenu == 3 && !pushCalled)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, vecColorGrey);
				ImGui::PushStyleColor(ImGuiCol_Button, imGuiStyle->Colors[ImGuiCol_WindowBg]);
				pushCalled = true;
			}
			if (ImGui::Button("E", sideTabButtonSize))
			{
				currentMenu = 3;
			}
			if (currentMenu == 3 && pushCalled)
			{
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				pushCalled = false;
			}
			//----------------------------------------------------------------------------//
			ImGui::SetCursorPosY(267.5f);
			if (currentMenu == 4 && !pushCalled)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, vecColorGreen);
				ImGui::PushStyleColor(ImGuiCol_Button, imGuiStyle->Colors[ImGuiCol_WindowBg]);
				pushCalled = true;
			}
			if (ImGui::Button("F", sideTabButtonSize))
			{
				currentMenu = 4;
			}
			if (currentMenu == 4 && pushCalled)
			{
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				pushCalled = false;
			}
			//----------------------------------------------------------------------------//
			ImGui::SetCursorPosY(335.0f);
			if (currentMenu == 5 && !pushCalled)
			{
				ImGui::PushStyleColor(ImGuiCol_Text, vecColorBlue);
				ImGui::PushStyleColor(ImGuiCol_Button, imGuiStyle->Colors[ImGuiCol_WindowBg]);
				pushCalled = true;
			}
			if (ImGui::Button("G", sideTabButtonSize))
			{
				currentMenu = 5;
			}
			if (currentMenu == 5 && pushCalled)
			{
				ImGui::PopStyleColor();
				ImGui::PopStyleColor();
				pushCalled = false;
			}*/
			//----------------------------------------------------------------------------//
			ImGui::PopFont();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleVar();
			ImGui::PopStyleVar();
		}
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();
		ImGui::EndChild();
		ImGui::SetNextWindowPos(ImVec2(windowPos.x + 57.5f, windowPos.y + 92.5f), ImGuiCond_Always);
		ImGui::PushStyleColor(ImGuiCol_Border, vecColorDimWhite);
		ImGui::PushStyleColor(ImGuiCol_CheckMark, vecColorDarkRed);
		ImGui::PushStyleColor(ImGuiCol_FrameBg, imGuiStyle->Colors[ImGuiCol_WindowBg]);
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, vecColorGrey);
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive, imGuiStyle->Colors[ImGuiCol_WindowBg]);
		ImGui::PushStyleColor(ImGuiCol_SliderGrab, vecColorDarkRed);
		ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, vecColorDarkRed);
		ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.05f);
		ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.1f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, 5.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(7.5f, 7.5f));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(7.5f, 7.5f));
		ImGui::PushFont(centuryGothic);
		if (ImGui::BeginChild("Jaguar Menu", ImVec2(575.0f, 380.0f), true, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar))
		{
			if (currentMenu == 0)
			{
				AimbotPanel();
			}
			if (currentMenu == 1)
			{
				RenderVisuals();
			}
			if (currentMenu == 2)
			{
				RenderCFG();
			}
			if (currentMenu < 0 || currentMenu >2) currentMenu = 0;
			/*if (currentMenu == 3)
			{
				SeparatePanel("Visual3 Assistances Panel");
				ImGui::Text("Misc");
				//RenderOthersPanel();
			}
			if (currentMenu == 4)
			{
				SeparatePanel("Visual2 Assistances Panel");
				ImGui::Text("I dont even know");
				//RenderWeaponModelPanel();
			}
			if (currentMenu == 5)
			{
				SeparatePanel("Visual1 Aassistances Panel");
				ImGui::Text("Config Panel");
				//RenderConfigPanel();
			}
			*/
		}
		ImGui::PopFont();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::EndChild();
		ImGui::End();

	}
	//ImGui::Text("Overlay FPS: %.3f ms/frame (%.1f FPS)"), 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate;
}

void Overlay::RenderInfo()
{
//	ImGui::SetNextWindowPos(ImVec2(0, 0));
///	ImGui::SetNextWindowSize(ImVec2((float)getWidth(), (float)getHeight()));
//	ImGui::Begin(XorStr("##lolgay"), (bool*)true, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoBringToFrontOnFocus);

}

void Overlay::ClickThrough(bool v)
{
	if (v)
	{
		nv_edit = nv_default_in_game | WS_VISIBLE;
		if (GetWindowLong(overlayHWND, GWL_EXSTYLE) != nv_ex_edit)
			SetWindowLong(overlayHWND, GWL_EXSTYLE, nv_ex_edit);
	}
	else
	{
		nv_edit = nv_default | WS_VISIBLE;
		if (GetWindowLong(overlayHWND, GWL_EXSTYLE) != nv_ex_edit_menu)
			SetWindowLong(overlayHWND, GWL_EXSTYLE, nv_ex_edit_menu);
	}
}

DWORD Overlay::CreateOverlay()
{
	Sleep(300);
	if (overlayHWND == 0)
	{
		printf("If you see this The overlay will not work, report it to the developer\n");
		Sleep(10000);
		exit(0);
	}
	//printf("Found the overlay!\n");
	HDC hDC = ::GetWindowDC(NULL);
	dwwidth = ::GetDeviceCaps(hDC, HORZRES);
	dwheight = ::GetDeviceCaps(hDC, VERTRES);

	bActive = true;

	// Initialize Direct3D
	if (!CreateDeviceD3D(overlayHWND))
	{
		CleanupDeviceD3D();
		return 1;
	}

	// Show the window
	::ShowWindow(overlayHWND, SW_SHOWDEFAULT);
	::UpdateWindow(overlayHWND);

	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(overlayHWND);
	ImGui_ImplDX9_Init(g_pd3dDevice);

	ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.00f);

	if (true)
	{
		
		HINSTANCE hResInstance = (HINSTANCE)GetModuleHandle(NULL);//Read the edit
		HRSRC res = FindResource(
			hResInstance,
			MAKEINTRESOURCE(IDR_FONT2),
			RT_FONT);  //Read The Edit
		if (res)
		{
			HGLOBAL mem = LoadResource(hResInstance, res);
			void* data = LockResource(mem);
			size_t len = SizeofResource(hResInstance, res);

			DWORD nFonts;
			HANDLE m_fonthandle = AddFontMemResourceEx(
				data,       // font resource
				len,       // number of bytes in font resource 
				NULL,          // Reserved. Must be 0.
				&nFonts      // number of fonts installed
			);

			if (m_fonthandle == 0)
			{
				printf( "Error");
			}

			
			centuryGothic = io.Fonts->AddFontFromMemoryTTF(data, len, 13.0f);
			centuryGothicH = io.Fonts->AddFontFromMemoryTTF(data, len, 42.0f);
			headerCenturyGothic = io.Fonts->AddFontFromMemoryTTF(data, len, 12.0f);
		}

	}
	
	if (true)
	{
	
		HINSTANCE hResInstance = (HINSTANCE)GetModuleHandle(NULL);//Read the edit
		HRSRC res = FindResource(
			hResInstance,
			MAKEINTRESOURCE(IDR_FONT1),
			RT_FONT);  //Read The Edit
		if (res)
		{
			HGLOBAL mem = LoadResource(hResInstance, res);
			void* data = LockResource(mem);
			size_t len = SizeofResource(hResInstance, res);

			DWORD nFonts;
			HANDLE m_fonthandle = AddFontMemResourceEx(
				data,       // font resource
				len,       // number of bytes in font resource 
				NULL,          // Reserved. Must be 0.
				&nFonts      // number of fonts installed
			);
		
			iLunarian = io.Fonts->AddFontFromMemoryTTF(data, len, 38.0f);
			iLunarianH = io.Fonts->AddFontFromMemoryTTF(data, len, 80.0f);
			if (m_fonthandle == 0)
			{
				printf("Error");
			}
		}
	}

	if (true)
	{
		
		HINSTANCE hResInstance = (HINSTANCE)GetModuleHandle(NULL);//Read the edit
		HRSRC res = FindResource(
			hResInstance,
			MAKEINTRESOURCE(IDR_FONT3),
			RT_FONT);  //Read The Edit
		if (res)
		{
			HGLOBAL mem = LoadResource(hResInstance, res);
			void* data = LockResource(mem);
			size_t len = SizeofResource(hResInstance, res);

			DWORD nFonts;
			HANDLE m_fonthandle = AddFontMemResourceEx(
				data,       // font resource
				len,       // number of bytes in font resource 
				NULL,          // Reserved. Must be 0.
				&nFonts      // number of fonts installed
			);
			
			KarlaFont = io.Fonts->AddFontFromMemoryTTF(data, len, 38.0f);
			if (m_fonthandle == 0)
			{
				printf("Error");
			}
		}
	}
	imGuiContext = ImGui::CreateContext();

	// Main loop
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	ClickThrough(true);
	while (bActive)
	{

		HWND wnd = GetWindow(GetForegroundWindow(), GW_HWNDPREV);
		if (wnd != overlayHWND)
		{
			SetWindowPos(overlayHWND, wnd, 0, 0, 0, 0, SWP_ASYNCWINDOWPOS | SWP_NOMOVE | SWP_NOSIZE);
			::UpdateWindow(overlayHWND);
		}

		if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			continue;
		}

		// Start the Dear ImGui frame
		ImGui_ImplDX9_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		if (IsKeyDown(VK_LBUTTON) && !bLeftclick)
		{
			io.MouseDown[0] = true;
			bLeftclick = true;
		}
		else if (!IsKeyDown(VK_LBUTTON) && bLeftclick)
		{
			io.MouseDown[0] = false;
			bLeftclick = false;
		}

		if (IsKeyDown(VK_INSERT) && !bIns && bActive)
		{
			bMenu = !bMenu;
			ClickThrough(!bMenu);
			bIns = true;
		}
		else if (!IsKeyDown(VK_INSERT) && bIns)
		{
			bIns = false;
		}

		if (bMenu)
			RenderMenu();
		//else
		//if(CFG.bWatermark)	RenderInfo();
		RenderEsp(1);
		// Rendering
		ImGui::EndFrame();
		g_pd3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
		g_pd3dDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
		D3DCOLOR clear_col_dx = D3DCOLOR_RGBA((int)(clear_color.x * 255.0f), (int)(clear_color.y * 255.0f), (int)(clear_color.z * 255.0f), (int)(clear_color.w * 255.0f));
		g_pd3dDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clear_col_dx, 1.0f, 0);
		if (g_pd3dDevice->BeginScene() >= 0)
		{
			ImGui::Render();
			ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
			g_pd3dDevice->EndScene();
		}
		HRESULT result = g_pd3dDevice->Present(NULL, NULL, NULL, NULL);

		// Handle loss of D3D9 device
		if (result == D3DERR_DEVICELOST && g_pd3dDevice->TestCooperativeLevel() == D3DERR_DEVICENOTRESET)
			ResetDevice();
		Sleep(1);
	}
	ClickThrough(true);
	CleanupDeviceD3D();
	::DestroyWindow(overlayHWND);
	return 0;
}


void Overlay::Start(HWND Overlay_hwnd)
{
	overlayHWND = Overlay_hwnd;
	DWORD ThreadID;
	CreateThread(NULL, 0, StaticMessageStart, (void*)this, 0, &ThreadID);
}

void Overlay::Clear()
{
	bActive = false;
	Sleep(50);
}

int Overlay::getWidth()
{
	return dwwidth;
}

int Overlay::getHeight()
{
	return dwheight;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
	if ((g_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL)
		return false;

	// Create the D3DDevice
	ZeroMemory(&g_d3dpp, sizeof(g_d3dpp));
	g_d3dpp.Windowed = TRUE;
	g_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	g_d3dpp.EnableAutoDepthStencil = TRUE;
	g_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
	g_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	g_d3dpp.hDeviceWindow = hWnd;
	g_d3dpp.MultiSampleQuality = D3DMULTISAMPLE_NONE;
	g_d3dpp.BackBufferCount = 1;
	g_d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	g_d3dpp.BackBufferWidth = 0;
	g_d3dpp.BackBufferHeight = 0;
	if (g_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &g_d3dpp, &g_pd3dDevice) < 0)
		return false;

	return true;
}

void CleanupDeviceD3D()
{
	if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = NULL; }
	if (g_pD3D) { g_pD3D->Release(); g_pD3D = NULL; }
}

void ResetDevice()
{


	ImGui_ImplDX9_InvalidateDeviceObjects();
	HRESULT hr = g_pd3dDevice->Reset(&g_d3dpp);
	if (hr == D3DERR_INVALIDCALL)
		IM_ASSERT(0);
	ImGui_ImplDX9_CreateDeviceObjects();
}


void Overlay::DrawLine(ImVec2 a, ImVec2 b, ImColor color, float width)
{
	ImGui::GetWindowDrawList()->AddLine(a, b, color, width);
}

void Overlay::DrawBox(ImColor color, float x, float y, float w, float h)
{
	DrawLine(ImVec2(x, y), ImVec2(x + w, y), color, 1.3f); // top 
	DrawLine(ImVec2(x, y - 1.3f), ImVec2(x, y + h + 1.4f), color, 1.3f); // left
	DrawLine(ImVec2(x + w, y -1.3f), ImVec2(x + w, y + h + 1.4f), color, 1.3f);  // right
	DrawLine(ImVec2(x, y + h ), ImVec2(x + w, y + h), color, 1.3f);   // bottom 
}
void Overlay::DrawCorneredBox(float X, float Y, float W, float H, const ImU32& color, float thickness) {
	float lineW = (W / 3);
	float lineH = (H / 3);
	//black outlines
	auto col = ImGui::GetColorU32(color);
	/*ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X, Y + lineH), col, thickness + 1);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y), ImVec2(X + lineW, Y), col, 3);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W, Y), col, thickness + 1);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y), ImVec2(X + W, Y + lineH), col, thickness + 1);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H), col, thickness + 1);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H), ImVec2(X + lineW, Y + H), col, thickness + 1);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W, Y + H), col, thickness + 1);
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H), col, thickness + 1);
	*/
	//corners
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y-thickness/2), ImVec2(X, Y + lineH), col, thickness);//top left
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X-thickness/2, Y), ImVec2(X + lineW, Y), col, thickness);

	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y), ImVec2(X + W +thickness / 2, Y), col, thickness);//top right horizontal
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y-thickness/2), ImVec2(X + W, Y + lineH), col, thickness);

	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X, Y + H - lineH), ImVec2(X, Y + H+(thickness/2)), col, thickness);//bot left
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X-thickness/2, Y + H), ImVec2(X + lineW, Y + H), col, thickness);

	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W - lineW, Y + H), ImVec2(X + W+thickness/2, Y + H), col, thickness);//bot right
	ImGui::GetOverlayDrawList()->AddLine(ImVec2(X + W, Y + H - lineH), ImVec2(X + W, Y + H+ (thickness/ 2)), col, thickness);

}

void Overlay::Text(ImVec2 pos, ImColor color, const char* text_begin, const char* text_end, float wrap_width, const ImVec4* cpu_fine_clip_rect)
{
	ImGui::GetWindowDrawList()->AddText(ImGui::GetFont(), ImGui::GetFontSize(), pos, color, text_begin, text_end, wrap_width, cpu_fine_clip_rect);
}
void Overlay::DrawText1(float x, float y, const char* str, ImVec4 color)
{
	ImFont a;
	string utf_8_1 = std::string(str);
	string utf_8_2 = string_To_UTF8(utf_8_1);
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(color), utf_8_2.c_str());
}

void Overlay::DrawStrokeText(float x, float y, const char* str)
{
	ImFont a;
	std::string utf_8_1 = std::string(str);
	std::string utf_8_2 = string_To_UTF8(utf_8_1);
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y - 1), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0f, 1 / 255.0f, 1 / 255.0f, 255.f / 255.0)), utf_8_2.c_str());
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y + 1), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0f, 1 / 255.0f, 1 / 255.0f, 255.f / 255.0)), utf_8_2.c_str());
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x - 1, y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0f, 1 / 255.0f, 1 / 255.0f, 255.f / 255.0)), utf_8_2.c_str());
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x + 1, y), ImGui::ColorConvertFloat4ToU32(ImVec4(1 / 255.0f, 1 / 255.0f, 1 / 255.0f, 255.f / 255.0)), utf_8_2.c_str());
	ImGui::GetOverlayDrawList()->AddText(ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(255, 255, 255, 255)), utf_8_2.c_str());
}
float Overlay::DrawOutlinedText(ImFont* pFont, const std::string& text, const ImVec2& pos, float size, ImU32 color, bool center)
{
	stringstream stream(text);
	string line;
	float y = 0.0f;
	int i = 0;
	while (getline(stream, line))
	{
		ImVec2 textSize = pFont->CalcTextSizeA(size, FLT_MAX, 0.0f, line.c_str());
		if (center)
		{
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			//	ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) + 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			//	ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((pos.x - textSize.x / 2.0f) - 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());

			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2(pos.x - textSize.x / 2.0f, pos.y + textSize.y * i), ImGui::GetColorU32(color), line.c_str());
		}
		else
		{//
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * i) +1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
			//	ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((pos.x) + 1, (pos.y + textSize.y * i) - 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());
				//ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2((pos.x) - 1, (pos.y + textSize.y * i) + 1), ImGui::GetColorU32(ImVec4(0, 0, 0, 255)), line.c_str());

			ImGui::GetOverlayDrawList()->AddText(pFont, size, ImVec2(pos.x, pos.y + textSize.y * i), ImGui::GetColorU32(color), line.c_str());
		}

		y = pos.y + textSize.y * (i + 1);
		i++;
	}
	return y;
}




void Overlay::String(ImVec2 pos, ImColor color, const char* text)
{
	Text(pos, color, text, text + strlen(text), 200, 0);
}

void Overlay::RectFilled(float x0, float y0, float x1, float y1, ImColor color, float rounding, int rounding_corners_flags)
{
	ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y1), color, rounding, rounding_corners_flags);
}

void Overlay::ProgressBar(float x, float y, float w, float h, int value, int v_max, ImColor barColor, bool Outlined, ImColor Outlinecolor)
{
	/*DrawLine(ImVec2(x, y), ImVec2(x, y + h), ImColor(0, 0, 0), 1);// left vertical
	DrawLine(ImVec2(x, y), ImVec2(x+w, y), ImColor(0, 0, 0), 1);*/
	if (Outlined)
		ImGui::GetOverlayDrawList()->AddRect(ImVec2(x-1, y-1), ImVec2(x + w +1, y + h+1), ImColor(0, 0, 0));
	RectFilled(x, y, x + w, y + ((h / float(v_max)) * (float)value), barColor, 0.0f, 0);
}
void Overlay::DrawCircle(float x, float y, float radius, ImVec4 color, int segments)
{
	ImGui::GetOverlayDrawList()->AddCircle(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(color), segments);
}
void Overlay::DrawFilledCircle(float x, float y, float radius, ImVec4 color, int segments)
{
	ImGui::GetOverlayDrawList()->AddCircleFilled(ImVec2(x, y), radius, ImGui::ColorConvertFloat4ToU32(color), segments);
}

void Overlay::RectFilled(float x0, float y0, float x1, float y1, ImU32 color, float rounding, int rounding_corners_flags)
{
	ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(x0, y0), ImVec2(x1, y1), color, rounding, rounding_corners_flags);
}
void Overlay::RectOutlined(float x0, float y0, float x1, float y1, ImU32 color, float rounding, int rounding_corners_flags)
{
	ImGui::GetOverlayDrawList()->AddRect(ImVec2(x0, y0), ImVec2(x1, y1), color, rounding, rounding_corners_flags);
}
