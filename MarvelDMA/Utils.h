#pragma once

#include <cstdint>
#include <corecrt_math.h>
#include <d3d9.h>
#include <vector>

#define M_PI 3.14159265359

struct Vector2 {
	float x = 0.0f;
	float y = 0.0f;

	Vector2() = default;
	Vector2(float x, float y) : x(x), y(y) {}
};


class Vector3
{
public:
	Vector3() : x(0.f), y(0.f), z(0.f)
	{

	}

	Vector3(double _x, double _y, double _z) : x(_x), y(_y), z(_z)
	{

	}
	~Vector3()
	{

	}

	double x;
	double y;
	double z;

	bool operator==(const Vector3& other) const {
		return x == other.x && y == other.y && z == other.z;
	}

	inline double Dot(Vector3 v)
	{
		return x * v.x + y * v.y + z * v.z;
	}

	inline double Distance(Vector3 v)
	{
		return double(sqrtf(powf(v.x - x, 2.0) + powf(v.y - y, 2.0) + powf(v.z - z, 2.0)));
	}

	inline double Length() {
		return sqrt(x * x + y * y + z * z);
	}

	Vector3 operator+(Vector3 v)
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	Vector3 operator-(Vector3 v)
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	Vector3 operator*(double flNum) { return Vector3(x * flNum, y * flNum, z * flNum); }
};

template <class T>
class TArray
{
public:
	int Num() const
	{
		return count;
	}

	int getIdentifier()
	{
		return data + count * max;
	}

	bool isValid() const
	{
		if (count > max)
			return false;
		if (!data)
			return false;
		return true;
	}

	uint64_t getAddress() const
	{
		return data;
	}

protected:
	uintptr_t data;
	uint32_t count;
	uint32_t max;
};

struct FQuat
{
	double x;
	double y;
	double z;
	double w;
};

struct FTransform
{
	FQuat rot;
	Vector3 translation;
	char pad[4];
	Vector3 scale;
	char pad1[4];

	D3DMATRIX ToMatrixWithScale()
	{
		D3DMATRIX m;
		m._41 = translation.x;
		m._42 = translation.y;
		m._43 = translation.z;

		float x2 = rot.x + rot.x;
		float y2 = rot.y + rot.y;
		float z2 = rot.z + rot.z;

		float xx2 = rot.x * x2;
		float yy2 = rot.y * y2;
		float zz2 = rot.z * z2;
		m._11 = (1.0f - (yy2 + zz2)) * scale.x;
		m._22 = (1.0f - (xx2 + zz2)) * scale.y;
		m._33 = (1.0f - (xx2 + yy2)) * scale.z;

		float yz2 = rot.y * z2;
		float wx2 = rot.w * x2;
		m._32 = (yz2 - wx2) * scale.z;
		m._23 = (yz2 + wx2) * scale.y;

		float xy2 = rot.x * y2;
		float wz2 = rot.w * z2;
		m._21 = (xy2 - wz2) * scale.y;
		m._12 = (xy2 + wz2) * scale.x;

		float xz2 = rot.x * z2;
		float wy2 = rot.w * y2;
		m._31 = (xz2 + wy2) * scale.z;
		m._13 = (xz2 - wy2) * scale.x;

		m._14 = 0.0f;
		m._24 = 0.0f;
		m._34 = 0.0f;
		m._44 = 1.0f;

		return m;
	}
};

struct Camera
{
	Vector3 Location;
	Vector3 Rotation;
	float FieldOfView;
};

struct Player {
	uintptr_t playerState = 0;
	uintptr_t pawnPrivate = 0;
	uintptr_t childActorComponent = 0;
	uintptr_t childActor = 0;
	uintptr_t mesh = 0;
	TArray<FTransform> boneArray;
	TArray<FTransform> boneArrayCache;
	std::vector<FTransform> bones;
	int heroID;

	int teamState;

	FTransform componentToWorld;
	FTransform head3DF;
	FTransform bottom3DF;

	Vector2 headW2S;
	Vector2 bottomW2S;

	float lastSubmitTime;
	float lastRenderTime;

	bool visible = false;
};

struct PlayerRender {
	Vector2 head = { 0, 0 };
	Vector2 bottom = { 0, 0 };

	bool visible;
};

enum AimType {
	AIM_NONE,
	BPRO,
	NET,
	MEMORY
};

inline int getHeadBone(int HeroIDHD) {
	switch (HeroIDHD) {
	case 1011: // Hulk
		return 186;
		break;
	case 1020: // Mantis
		return 115;
		break;
	case 1033: // Black Widow
		return 157;
		break;
	case 1052: //Iron First
		return 184;
		break;
	case 1031: // Luna Snow
		return 120;
		break;
	case 1014: // STORM
		return 199;
		break;
	case 1015: // STORM
		return 171;
		break;
	case 1016: // Loki
		return 234;
		break;
	case 1018: // Doctor Strange
		return 171;
		break;
	case 1021: // Hawekye
		return 221;
		break;
	case 1022: // Captain America
		return 147;
		break;
	case 1023: // ROCKET RACCOON
		return 120;
		break;
	case 1024: // Hela
		return 169;
		break;
	case 1025: // DAGGER
		return 109;
		break;
	case 1026: // Black Panther
		return 111;
		break;
	case 1027: // Groot
		return 177;
		break;
	case 1029: // MAGIK
		return 136;
		break;
	case 1030: // Moon Knight
		return 180;
		break;
	case 1032: // Squirrel Girl
		return 150;
		break;
	case 1034: // Iron Man
		return 184;
		break;
	case 1035: // 
		return 140;
		break;
	case 1036: // Spider-Man
		return 146;
		break;
	case 1037: // Magneto
		return 190;
		break;
	case 1038: // Scarlet Witch
		return 125;
		break;
	case 1039: // Thor
		return 220;
		break;
	case 1041: // Winter Soldier
		return 212;
		break;
	case 1042: // Peni Parker
		return 159;
		break;
	case 1043: // Star-Lord
		return 140;
		break;
	case 1045: // NAMOR
		return 131;
		break;
	case 1046: // Adam Warlock
		return 192;
		break;
	case 1047: // Land Shark
		return 143;
		break;
	case 1048: // PSYLOCKE
		return 234;
		break;
	case 1049: // Wolverine
		return 274;
		break;
	case 1017: // Human Torch
		return 117;
		break;
	case 1051: // The Thing
		return 226;
		break;
	case 1040:
		return 111;
		break;
	case 1050:
		return 109;
		break;
	case 4016: // Bots
		return 5;
		break;
	case 4018: // Bots
		return 5;
		break;
	default: // other
		return 120;
		break;
	}
}

inline D3DMATRIX Matrix(Vector3 rot, Vector3 origin) {
	float radPitch = (rot.x * float(M_PI) / 180.f);
	float radYaw = (rot.y * float(M_PI) / 180.f);
	float radRoll = (rot.z * float(M_PI) / 180.f);

	float SP = sinf(radPitch);
	float CP = cosf(radPitch);
	float SY = sinf(radYaw);
	float CY = cosf(radYaw);
	float SR = sinf(radRoll);
	float CR = cosf(radRoll);

	D3DMATRIX matrix;
	matrix.m[0][0] = CP * CY;
	matrix.m[0][1] = CP * SY;
	matrix.m[0][2] = SP;
	matrix.m[0][3] = 0.f;

	matrix.m[1][0] = SR * SP * CY - CR * SY;
	matrix.m[1][1] = SR * SP * SY + CR * CY;
	matrix.m[1][2] = -SR * CP;
	matrix.m[1][3] = 0.f;

	matrix.m[2][0] = -(CR * SP * CY + SR * SY);
	matrix.m[2][1] = CY * SR - CR * SP * SY;
	matrix.m[2][2] = CR * CP;
	matrix.m[2][3] = 0.f;

	matrix.m[3][0] = origin.x;
	matrix.m[3][1] = origin.y;
	matrix.m[3][2] = origin.z;
	matrix.m[3][3] = 1.f;

	return matrix;
}

inline D3DMATRIX MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2)
{
	D3DMATRIX pOut;
	pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
	pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
	pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
	pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
	pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
	pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
	pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
	pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
	pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
	pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
	pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
	pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
	pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
	pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
	pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
	pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

	return pOut;
}

inline Vector2 doMatrix(FTransform Bone, FTransform C2W, Camera camera, int screenWidth, int screenHeight) {
	D3DMATRIX matrix = MatrixMultiplication(Bone.ToMatrixWithScale(), C2W.ToMatrixWithScale());
	Vector3 BoneV = Vector3(matrix._41, matrix._42, matrix._43);

	D3DMATRIX tempMatrix = Matrix(camera.Rotation, Vector3(0, 0, 0));

	Vector3 vAxisX = Vector3(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
	Vector3 vAxisY = Vector3(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
	Vector3 vAxisZ = Vector3(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

	Vector3 vDelta = BoneV - camera.Location;

	Vector3 vTransformed = Vector3(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

	if (vTransformed.z < 1.f)
		vTransformed.z = 1.f;

	return Vector2((screenWidth / 2.0f) + vTransformed.x * (((screenWidth / 2.0f) / tanf(camera.FieldOfView * (float)M_PI / 360.f))) / vTransformed.z, (screenHeight / 2.0f) - vTransformed.y * (((screenWidth / 2.0f) / tanf(camera.FieldOfView * (float)M_PI / 360.f))) / vTransformed.z);
}

inline float GetCrossDistance(double x1, double y1, double x2, double y2) {
	double dx = x2 - x1;
	double dy = y2 - y1;
	return sqrt(dx * dx + dy * dy);
}