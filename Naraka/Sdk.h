#pragma once
#include <string>
#include <math.h>
#include "offsets.h"
#include "Memory.h"

struct Vector3
{
	float x, y, z;

	inline Vector3(float _x, float _y, float _z) :x(_x), y(_y), z(_z) {}

	inline Vector3() {}

	Vector3 operator+(const Vector3& pos)
	{
		Vector3 ret;
		ret.x = x + pos.x;
		ret.y = y + pos.y;
		ret.z = z + pos.z;
		return ret;
	}

	Vector3 operator-(const Vector3& pos)
	{
		Vector3 ret;
		ret.x = x - pos.x;
		ret.y = y - pos.y;
		ret.z = z - pos.z;
		return ret;
	}

	bool operator==(const Vector3& pos)
	{
		if (pos.x == x && pos.y == y && pos.z == z)
			return true;
		return false;
	}

	constexpr auto notNull() const noexcept
	{
		return x || y || z;
	}

	inline Vector3& normalize() noexcept
	{
		x = isfinite(x) ? remainder(x, 360.0f) : 0.0f;
		y = isfinite(y) ? remainder(y, 360.0f) : 0.0f;
		z = 0.0f;
		return *this;
	}

	inline float length() const noexcept
	{
		return sqrt(x * x + y * y + z * z);
	}

	inline float length2D() const noexcept
	{
		return sqrt(x * x + y * y);
	}

	auto distTo(const Vector3& v) const noexcept
	{
		return sqrt((v.x - x) * (v.x - x) + (v.y - y) * (v.y - y) + (v.z - z) * (v.z - z));
	}

};
struct Vector4
{
public:
	Vector4() : x(0.f), y(0.f), z(0.f), w(0.f) {}

	Vector4(float _x, float _y, float _z, float _w) : x(_x), y(_y), z(_z), w(_w) {}

	float x;
	float y;
	float z;
	float w;
};
struct Matrix4x4 {

	float matrix[4][4];

	float* operator[](int index)
	{
		return matrix[index];
	}

	bool ToScreenPos(Vector3& InOutPos, float width, float height)
	{
		Vector3 result;
		float _x = matrix[0][0] * InOutPos.x + matrix[1][0] * InOutPos.y + matrix[2][0] * InOutPos.z + matrix[3][0];
		float _y = matrix[0][1] * InOutPos.x + matrix[1][1] * InOutPos.y + matrix[2][1] * InOutPos.z + matrix[3][1];
		float _w = matrix[0][3] * InOutPos.x + matrix[1][3] * InOutPos.y + matrix[2][3] * InOutPos.z + matrix[3][3];
		if (_w < 0.01f)
			return false;
		float inv_w = 1.f / _w;
		_x *= inv_w;
		_y *= inv_w;
		result.x = width * .5f;
		result.y = height * .5f;
		result.x += 0.5f * _x * width + 0.5f;
		result.y -= 0.5f * _y * height + 0.5f;
		result.z = _w;

		if (result.y  < 0.f || result.x  < 0.f || result.x > width || result.y > height)
			return false;

		InOutPos = result;
		return true;
	}

};
namespace math {

	inline Vector3 QuatToRot(Vector4 Quaternion) {
		Vector3 rot;
		float qzsqr = 0;
		float T0 = 0;
		float T1 = 0;
		float t2 = 0;
		float t3 = 0;
		float t4 = 0;

		qzsqr = Quaternion.z * Quaternion.z;
		T0 = -2.f * (qzsqr + Quaternion.w * Quaternion.w) + 1.f;
		T1 = 2.f * (Quaternion.y * Quaternion.z + Quaternion.x * Quaternion.w);
		t2 = -2.f * (Quaternion.y * Quaternion.w - Quaternion.x * Quaternion.z);
		t3 = 2.f * (Quaternion.z * Quaternion.w + Quaternion.x * Quaternion.y);
		t4 = -2.f * (Quaternion.y * Quaternion.y + qzsqr) + 1.f;

		t2 = t2 > 1.f ? 1.f : t2;
		t2 = t2 < -1.f ? -1.f : t2;

		float RADPI = (float)(180 / 3.1415926f);
		float Yaw = (float)atan2f(T1, T0) * RADPI;
		float Roll = (float)atan2f(t3, t4) * RADPI;
		float Pitch = (float)asin(t2) * RADPI;

		return Vector3(Pitch, Yaw, Roll);
	}

	inline Vector3 FindLookAtVector(Vector3 pos1, Vector3 pos2) {
		Vector3 v1 = Vector3(pos2.x, pos2.z, pos2.y) - Vector3(pos1.x, pos1.z, pos1.y);//ËûÂèµÄ ÅÅÐòÊÇ xzy
		float distance = pos1.distTo(pos2);
		return Vector3(v1.x / distance, v1.y / distance, v1.z / distance);
	}

	inline float VectorToRotationYaw(Vector3 pos) {
		float RADPI = (float)(180 / 3.1415926f);
		float Yaw = (float)atan2f(pos.x, pos.y) * RADPI;
		if (Yaw < 0)
		{
			Yaw += 180;
		}
		if (pos.x < 0)
		{
			Yaw += 180;
		}
		return Yaw;
	}
	inline float ConverseAngle(float x) {
		return (float)(((int)(x + 180.f) % 360 + 180) % 360);
	}

	inline float AngleDifference(float a, float b) {
		int phi = ((int)fabsf((b - a)) % 360);
		int sign = 1;
		if (((a - b) >= 0 && (a - b) <= 180) || ((a - b) <= -180 && (a - b) >= -360))
		{
			sign = -1;
		}

		return ((phi > 180 ? 360 - phi : phi) * sign);
	}
}

enum WeaponId
{
	³¤½£ = 1,
	Ì«µ¶ = 2,
	³¤Ç¹ = 3,
	À«µ¶ = 4,
	Ø°Ê× = 5,
	Ë«½Ø¹÷ = 6,
	Ë«µ¶ = 7,
};
namespace sdk
{
	void initialize(uintptr_t _UnityPlayer, uintptr_t _GameAssembly);
	uint64_t get_uworlds();
	uint64_t get_manager();
	uint64_t get_manager_base();
	uint64_t get_local();
	uint64_t get_actor_manager();
	uint64_t get_palyer_info(uint64_t obj);
	uint32_t get_team(uint64_t obj);
	uint32_t get_weapon(uint64_t obj);
	std::wstring get_name(uint64_t obj);
	Matrix4x4 get_view_matrix();
	uint32_t get_endurelevel(uint64_t obj);
	uint32_t get_reactiontype(uint64_t obj);
	uint32_t get_rangereactiontype(uint64_t obj);
	uint32_t get_playerCount();
	uintptr_t get_playeritems();
	Vector3 get_enitypos(uint64_t obj);
	uint32_t get_reaction_extra_check_combo_type(uint64_t obj);
	uintptr_t get_entityaddr(uintptr_t items_, int count);
	float get_enduretime(uint64_t obj);
	std::wstring get_animname(uint64_t obj);
	uint32_t get_weapon_type(uint32_t weapon_id);
	bool is_weapon(uint32_t weaponid);
	bool is_attacking(uintptr_t obj);
	uint64_t get_root_component(uint64_t obj);
	float get_enityangle(uint64_t obj);
	bool is_bot_player(uint64_t obj);
}