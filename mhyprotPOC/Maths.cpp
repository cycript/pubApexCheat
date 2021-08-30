#include "Maths.h"
#include "ApexPrediction.h"

float Maths::FPS(int dwFPS)
{
	return (float)(1000 / dwFPS);
}

int Maths::RNG(int dwArray[])
{
    return dwArray[rand() % 5];
}

float Maths::fRNG(float flMin, float flMax)
{
    return (flMin + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (flMax - flMin))));
}

float Maths::ToRadians(float flVal)
{
	return (float)(M_PI / 180.0f) * flVal;
}

float Maths::ToDegree(float flVal)
{
	return (float)(180.0f / M_PI) * flVal;
}

int Maths::Quadrant(Vector2 vecCoordinate)
{
    if (vecCoordinate.X > 0 && vecCoordinate.Y > 0)
        return 1;
    else if (vecCoordinate.X < 0 && vecCoordinate.Y > 0)
        return 2;
    else if (vecCoordinate.X < 0 && vecCoordinate.Y < 0)
        return 3;
    else if (vecCoordinate.X > 0 && vecCoordinate.Y < 0)
        return 4;
    return 0;
}

void Maths::NormalizeDelta(Vector2 vecCoordinate, Vector2 vecDelta)
{
    int dwQuadrant = Quadrant(vecCoordinate);
    if (dwQuadrant == 1)
    {
        if (vecCoordinate.X < 0)
            vecDelta.X = fabsf(vecCoordinate.X);
        if (vecCoordinate.Y < 0)
            vecDelta.Y = fabsf(vecCoordinate.Y);
    }
    else if (dwQuadrant == 2)
    {
        if (vecCoordinate.X > 0)
            vecDelta.X *= -1.0f;
        if (vecCoordinate.Y < 0)
            vecDelta.Y = fabsf(vecCoordinate.Y);
    }
    else if (dwQuadrant == 3)
    {
        if (vecCoordinate.X > 0)
            vecDelta.X *= -1.0f;
        if (vecCoordinate.Y > 0)
            vecDelta.Y *= -1.0f;
    }
    else if (dwQuadrant == 4)
    {
        if (vecCoordinate.X < 0)
            vecDelta.X = fabsf(vecDelta.X);
        if (vecCoordinate.Y > 0)
            vecDelta.Y *= -1.0f;
    }
}
Vector3 Maths::CalculateAimAngle(Vector3 vecSource, Vector3 vecDestination,Vector3 LocalSway, Vector3 LocalCam, float flSmooth)
{
    /*
    Vector3 vecDelta;
    vecDelta.X = vecDestination.X - vecSource.X;
    vecDelta.Y = vecDestination.Y - vecSource.Y;
    vecDelta.Z = vecDestination.Z - vecSource.Z;
    float flHypotenuse = Hypotenuse2D(vecDelta);
    Vector3 vecAngle;
    vecAngle.X = atan2f(-vecDelta.Z, flHypotenuse) * (float)(180.0f / M_PI);
    vecAngle.Y = atan2f(vecDelta.Y, vecDelta.X) * (float)(180.0f / M_PI);
    vecAngle.Z = 0.0f;
    vecAngle = vecAngle - (LocalSway - LocalCam);
    Vector3 Delta = vecAngle - LocalCam;
    Vector3 SmoothedAngles = LocalCam + (Delta / flSmooth);
    SmoothedAngles.Z = 0.f;
    return FinalizeAngle( SmoothedAngles);
    */
    Vector3 angle;
    vecSource.Z += 30.f;
    Vector3 delta = { (vecSource.X - vecDestination.X), (vecSource.Y - vecDestination.Y), (vecSource.Z - vecDestination.Z) };
    float hyp = Hypotenuse(delta);
    angle.X = atan(delta.Z / hyp) * (180.0f / M_PI);
    angle.Y        = atan(delta.Y / delta.X) * (180.0f / M_PI);
    angle.Z = 0;
    if (delta.X  >= 0.0) angle.Y += 180.0f;
    return angle;
}
Vector3 Maths::CalculateAngle(Vector3 vecSource, Vector3 vecDestination)
{
    Vector3 angle;

    Vector3 delta = { (vecSource.X - vecDestination.X), (vecSource.Y - vecDestination.Y), (vecSource.Z - vecDestination.Z) };
    double hyp = sqrt(delta.X * delta.X + delta.Y * delta.Y);
    angle.X = atan(delta.Z / hyp) * (180.0f / M_PI);
    angle.Y = atan(delta.Y / delta.X) * (180.0f / M_PI);
    angle.Z = 0;
    if (delta.X >= 0.0) angle.Y += 180.0f;

    return angle;
}

Vector3 Maths::ClampAngle(Vector3 vecAngle)
{
    Vector3 clampedVector = vecAngle;
    if (clampedVector.X > 89.0f)
    {
        clampedVector.X -= 179.0f;
    }
    if (clampedVector.X < -89.0f)
    {
        clampedVector.X += 179.0f;
    }
    if (clampedVector.Y > 179.0f)
    {
        clampedVector.Y -= 360.0f;
    }
    if (clampedVector.Y < -179.0f)
    {
        clampedVector.Y += 360.0f;
    }
    if (clampedVector.X < -89.0f)
    {
        clampedVector.X = -89.0f;
    }
    if (clampedVector.X > 89.0f)
    {
        clampedVector.X = 89.0f;
    }
    if (clampedVector.Y > 179.0f)
    {
        clampedVector.Y = 179.0f;
    }
    if (clampedVector.Y < -179.0f)
    {
        clampedVector.Y = -179.0f;
    }
    clampedVector.Z = 0.0f;
    return clampedVector;
}

Vector3 Maths::NormalizeAngle(Vector3 vecAngle)
{
    Vector3 normalizedAngles = vecAngle;
    if (normalizedAngles.X > 89.0f)
    {
        normalizedAngles.X = 89.0f;
    }
    else if (-89.0f > normalizedAngles.X)
    {
        normalizedAngles.X = -89.0f;
    }

    if (normalizedAngles.Y > 180.0f)
    {
        normalizedAngles.Y -= 360.0f;
    }
    else if (-180.0f > normalizedAngles.Y)
    {
        normalizedAngles.Y += 360.0f;
    }
    normalizedAngles.Z = 0.0f;
    return normalizedAngles;
}

Vector3 Maths::FinalizeAngle(Vector3 vecAngle)
{
    return NormalizeAngle(ClampAngle(vecAngle));
}
QAngle Maths::FinalQAngle(QAngle Angle)
{
    Vector3 Ang = { Angle.x,Angle.y,Angle.z };
    Vector3 angle = Maths::FinalizeAngle(Ang);
    return QAngle(angle.X, angle.Y, angle.Z);
}
Vector2 Maths::AimAngleToRelatives(Vector3 vecViewAngle, Vector3 vecTargetAngle, float flSensitivity, float flZoomSensitivity, float flSmooth, bool bScoping)
{
    float flAimX;
    float flAimY;
    float flMouseAimX;
    float flMouseAimY;
    Vector2 vecAimRelatives;
    flAimY = vecViewAngle.X - vecTargetAngle.X, flAimX = vecViewAngle.Y - vecTargetAngle.Y;
    if (!bScoping) 
    {
        flAimX = ((flAimX / flSensitivity) / 0.022f);
        flAimY = ((flAimY / flSensitivity) / -0.022f);
    }
    else
    {
        flAimX = ((flAimX / flZoomSensitivity) / 0.022f);
        flAimY = ((flAimY / flZoomSensitivity) / -0.022f);
    }
    if (flSmooth > 1.0f)
    {
        flMouseAimX = 0.0f, flMouseAimY = 0.0f;
        if (flMouseAimX < flAimX)
        {
            flMouseAimX += 1.0f + (flAimX / flSmooth);
        }
        else if (flMouseAimX > flAimX)
        {
            flMouseAimX -= 1.0f - (flAimX / flSmooth);
        }
        if (flMouseAimY < flAimY)
        {
            flMouseAimY += 1.0f + (flAimY / flSmooth);
        }
        else if (flMouseAimY > flAimY)
        {
            flMouseAimY -= 1.0f - (flAimY / flSmooth);
        }
    }
    else
    {
        flMouseAimX = flAimX;
        flMouseAimY = flAimY;
    }
    vecAimRelatives.X = flMouseAimX;
    vecAimRelatives.Y = flMouseAimY;
    return vecAimRelatives;
}

float Maths::CalculateFoV(Vector3 vecLocalPosition, Vector3 vecTargetPosition, Vector3 vecDestination, Vector3 vecViewAngle)
{
    float flPitchDifference = fabsf(vecViewAngle.X - vecDestination.X);
    float flYawDifference = fabsf(vecViewAngle.Y - vecDestination.Y);
    float flDistance = vecTargetPosition.Distance(vecLocalPosition);
    float flPitchDistance = sinf(ToRadians(flPitchDifference)) * flDistance;
    float flYawDistance = sinf(ToRadians(flYawDifference)) * flDistance;
    float flRealDistance = sqrtf((flPitchDistance * flPitchDistance) + (flYawDistance * flYawDistance));
    if (flYawDifference <= 90.0f)
    {
        return flRealDistance;
    }
    else
    {
        return 666.0f;
    }
}

float Maths::CalculateMaxAimValue(float flSensitivity)
{
    return (2.5f * (powf(flSensitivity, 2.0f))) - (24.5f * flSensitivity) + 71.0f;
}

Vector2 Maths::WorldToMap(Vector3 vecPosition, Vector2 vecWorldOrigin, float flScale)
{
    Vector2 vecOffset;
    vecOffset.X = (vecPosition.X - vecWorldOrigin.X) / flScale;
    vecOffset.Y = (vecPosition.Y - vecWorldOrigin.Y) / -flScale;
    return vecOffset;
}

float Maths::Hypotenuse(Vector3 vecDelta)
{
    return sqrtf((vecDelta.X * vecDelta.X) + (vecDelta.Y * vecDelta.Y) + (vecDelta.Z * vecDelta.Z));
}

float Maths::Hypotenuse2D(Vector3 vecDelta)
{
    return sqrtf((vecDelta.X * vecDelta.X) + (vecDelta.Y * vecDelta.Y));
}

float Maths::CalculateDistance(Vector3 vecStart, Vector3 vecDestination)
{
    Vector3 vecDelta;
    vecDelta.X = vecStart.X - vecDestination.X;
    vecDelta.Y = vecStart.Y - vecDestination.Y;
    vecDelta.Z = vecStart.Z - vecDestination.Z;
    return Hypotenuse(vecDelta);
}

float Maths::CalculateFoV(Vector3 vecDestination, Vector3 vecViewAngle)
{
    Vector3 vecDelta = vecDestination - vecViewAngle;
    return sqrtf(powf(vecDelta.X, 2.0f) + powf(vecDelta.Y, 2.0f));
}


QAngle Maths::CalculatePredictionAngle(uintptr_t pLocalPlayer, QAngle  LocalCam, QAngle  LocalSway, Vector TargetBone, Vector TargetAbsVelocity, float smooth) {
    QAngle CalculatedAngles = QAngle(0, 0, 0);
    WeaponXEntity curweap = WeaponXEntity();
    curweap.update(pLocalPlayer);
    float BulletSpeed = curweap.get_projectile_speed();
    float BulletGrav = curweap.get_projectile_gravity();
    Vector StartPos = ApexLegends::GetEntityBonePos(pLocalPlayer, 8);
    //if (StartPos.DistTo(TargetBone) < 75.f * 15)
     //   StartPos.z += 10.f;
   // else StartPos.z += 5.f;
    if (BulletSpeed > 1.f)       
    {
        PredictCtx Ctx;
        Ctx.StartPos = StartPos;
        Ctx.TargetPos = TargetBone;
        Ctx.BulletSpeed = BulletSpeed;// - (BulletSpeed * 0.08);
        Ctx.BulletGravity = BulletGrav;
        Ctx.TargetVel = TargetAbsVelocity;

        if (BulletPredict(Ctx)) {
            CalculatedAngles = QAngle{ Ctx.AimAngles.x, Ctx.AimAngles.y, 0.f };
        }

    }
    if (CalculatedAngles == QAngle(0, 0, 0)) {
        CalculatedAngles = Maths::CalcAngle(StartPos, TargetBone);
    }
    CalculatedAngles -= LocalSway - LocalCam;
    Maths::NormalizeAngles(CalculatedAngles);
    QAngle Delta = CalculatedAngles - LocalCam;
    Maths::NormalizeAngles(Delta);
    QAngle SmoothedAngles = LocalCam + (Delta / smooth);
    SmoothedAngles.z = 0.f;
    return SmoothedAngles;
}


QAngle Maths::CalcAngle(Vector& src, Vector& dst)
{
    QAngle angle = QAngle();
    Vector delta = Vector((src.x - dst.x), (src.y - dst.y), (src.z - dst.z));

    double hyp = sqrtf(delta.x * delta.x + delta.y * delta.y);

    angle.x = atanf(delta.z / hyp) * (180.0f / M_PI);
    angle.y = atanf(delta.y / delta.x) * (180.0f / M_PI);
    angle.z = 0;
    if (delta.x >= 0.0) angle.y += 180.0f;

    return angle;
}

Vector3 Maths::SmoothifyAngle(Vector3 vecViewAngle, Vector3 vecAngle, float flSmooth)
{
    Vector3 smoothifiedAngle;
    Vector3 vecDeltaAngle = vecAngle - vecViewAngle;
    vecAngle.X = vecViewAngle.X - vecDeltaAngle.X / flSmooth;
    vecAngle.Y = vecViewAngle.Y - vecDeltaAngle.Y / flSmooth;
    vecAngle.Z = 0.0f;
    smoothifiedAngle = vecAngle;
    return smoothifiedAngle;
}

void Maths::NormalizeAngles(QAngle& angle)
{
    while (angle.x > 89.0f)
        angle.x -= 180.f;

    while (angle.x < -89.0f)
        angle.x += 180.f;

    while (angle.y > 180.f)
        angle.y -= 360.f;

    while (angle.y < -180.f)
        angle.y += 360.f;
}


float Maths::FoV(float flFoV)
{
    return flFoV * 10.0f;
}















/////////////////////////////////////////////////////////////////////////////////////////////// WeaponXEntity for APEX LEGENDS

void WeaponXEntity::update(uintptr_t LocalPlayer)
{
    uintptr_t wephandle = Memory::ReadProcessMemory<uintptr_t>(APEXVars.dwProcessId, LocalPlayer + APEXOffsets.OFFSET_WEAPON); //ptr to player weapon
    wephandle &= 0xffff;
    uintptr_t wep_entity = ApexLegends::GetEntityBase(wephandle);
    projectile_speed = Memory::ReadProcessMemory<float>(APEXVars.dwProcessId, wep_entity + APEXOffsets.OFFSET_BULLET_SPEED);
    projectile_scale = Memory::ReadProcessMemory<float>(APEXVars.dwProcessId, wep_entity + APEXOffsets.OFFSET_BULLET_SCALE);
    zoom_fov = Memory::ReadProcessMemory<float>(APEXVars.dwProcessId, wep_entity + APEXOffsets.OFFSET_ZOOM_FOV);
}

float WeaponXEntity::get_projectile_speed()
{
    return projectile_speed;
}

float WeaponXEntity::get_projectile_gravity()
{
    return 750.0f * projectile_scale;
}

float WeaponXEntity::get_zoom_fov()
{
    return zoom_fov;
}
double Maths::GetFov(const QAngle& viewAngle, const QAngle& aimAngle)
{
    QAngle delta = aimAngle - viewAngle;
    NormalizeAngles(delta);
    return sqrt(pow(delta.x, 2.0f) + pow(delta.y, 2.0f));
}
