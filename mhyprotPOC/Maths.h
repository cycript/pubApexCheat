#pragma once
#include <math.h>
#include <algorithm>
#include "Structs.h"
#include "ApexLegends.h"
# define M_PI 3.14159265358979323846

namespace Maths
{
	float FPS(int dwFPS);
	int RNG(int dwArray[]);
	float fRNG(float flMin, float flMax);
	float ToRadians(float flVal);
	float ToDegree(float flVal);
	int Quadrant(Vector2 vecCoordinate);
	void NormalizeDelta(Vector2 vecCoordinate, Vector2 vecDelta);
	Vector3 ClampAngle(Vector3 vecAngle);
	Vector3 NormalizeAngle(Vector3 vecAngle);
	Vector3 FinalizeAngle(Vector3 vecAngle);
	Vector2 AimAngleToRelatives(Vector3 vecViewAngle, Vector3 vecTargetAngle, float flSensitivity, float flZoomSensitivity, float flSmooth, bool bScoping);
	float CalculateFoV(Vector3 vecLocalPosition, Vector3 vecTargetPosition, Vector3 vecDestination, Vector3 vecViewAngle);
	float CalculateMaxAimValue(float flSensitivity);
	Vector2 WorldToMap(Vector3 vecPosition, Vector2 vecWorldOrigin, float flScale);
	int RNG(int dwArray[]);
	float Hypotenuse(Vector3 vecDelta);
	float Hypotenuse2D(Vector3 vecDelta);
	float ToRadians(float flVal);
	float CalculateDistance(Vector3 vecStart, Vector3 vecDestination);
	float CalculateFoV(Vector3 vecDestination, Vector3 vecViewAngle);
	QAngle CalculatePredictionAngle(uintptr_t pLocalPlayer, QAngle LocalCam, QAngle LocalSway, Vector TargetBone, Vector TargetAbsVelocity, float smooth);
	QAngle CalcAngle(Vector& src, Vector& dst);
	Vector3 CalculateAngle(Vector3 vecSource, Vector3 vecDestination);
	Vector3 CalculateAimAngle(Vector3 vecSource, Vector3 vecDestination, Vector3 LocalSway, Vector3 LocalCam, float flSmooth);
	Vector3 SmoothifyAngle(Vector3 vecViewAngle, Vector3 vecAngle, float flSmooth);
	void NormalizeAngles(QAngle& angle);
	Vector3 NormalizeAngle(Vector3 pAngle);
	Vector3 FinalizeAngle(Vector3 pAngle);
	QAngle FinalQAngle(QAngle Angle);
	float FoV(float flFoV);
	double GetFov(const QAngle& viewAngle, const QAngle& aimAngle);
}
class WeaponXEntity
{
public:
	void update(uintptr_t LocalPlayer);
	float get_projectile_speed();
	float get_projectile_gravity();
	float get_zoom_fov();

private:
	float projectile_scale;
	float projectile_speed;
	float zoom_fov;
};