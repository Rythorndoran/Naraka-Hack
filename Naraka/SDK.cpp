#include"sdk.h"

extern MemoryToolsWrapper* memorytools;

namespace memory
{
	template<typename T>
	T read(uintptr_t dst)
	{
		return memorytools->read<T>(dst);
	}
	template<typename T>
	void write(uintptr_t dst, T src)
	{
		return memorytools->write<T>(dst, src);

	}
	void read(uintptr_t dst, void* buffer, size_t len)
	{
		return memorytools->read(dst, buffer, len);
	}
}

namespace sdk
{

	uintptr_t UnityPlayer = 0;
	uintptr_t GameAssembly = 0;
	void initialize(uintptr_t _UnityPlayer, uintptr_t _GameAssembly)
	{
		UnityPlayer = _UnityPlayer;
		GameAssembly = _GameAssembly;
	}
	uint64_t get_uworlds()
	{
		return memory::read<uint64_t>(GameAssembly + CharacterManager);
	}

	uint64_t get_manager()
	{
		return memory::read<uint64_t>(get_uworlds() + Manager_offset);
	}

	uint64_t get_manager_base()
	{
		return memory::read<uint64_t>(get_manager() + ManagerBase_offset);
	}

	uint64_t get_local()
	{
		return memory::read<uint64_t>(memory::read<uint64_t>(get_manager() + LocalPlayer_offset1) + LocalPlayer_offset2);
	}

	uint64_t get_actor_manager()
	{
		return memory::read<uint64_t>(get_manager_base() + AcotrManager_offset);
	}

	uint64_t get_palyer_info(uint64_t obj)
	{
		return memory::read<uint64_t>(obj + PlayerInfo_offset);
	}

	uint32_t get_team(uint64_t obj)
	{
		return memory::read<uint32_t>(get_palyer_info(obj) + ObjTeam_offset);
	}

	//返回的是武器id
	uint32_t get_weapon(uint64_t obj)
	{
		return memory::read<uint32_t>(get_palyer_info(obj) + ObjWeapon_offse);
	}

	bool is_bot_player(uint64_t obj)
	{
		return (memory::read<uint32_t>(get_palyer_info(obj) + ObjBot_offset) > 0);
	}

	std::wstring get_name(uint64_t obj)
	{

		wchar_t name[64]{ 0 };
		memory::read(memory::read<uint64_t>(get_palyer_info(obj) + 人物昵称_offset) + 0x14, name, sizeof(name));
		return std::wstring(name);
	}
	uint32_t get_alive(uint64_t obj)
	{
		return (memory::read<uint32_t>(get_palyer_info(obj) + ObjAlive_offset) == 1);
	}

	Matrix4x4 get_view_matrix()
	{
		auto _ptr = get_manager() + 8;
		_ptr = memory::read<uint64_t>(_ptr) + 0x40;
		_ptr = memory::read<uint64_t>(_ptr) + 0x38;
		_ptr = memory::read<uint64_t>(_ptr) + 0x10;
		_ptr = memory::read<uint64_t>(_ptr) + 0xDC;
		return memory::read<Matrix4x4>(_ptr);
	}


	uint32_t get_playerCount() {//get_manager_base
		return memory::read<uint32_t>(get_manager_base() + 0x18);
	}

	uintptr_t get_playeritems() {

		return memory::read<uintptr_t>(get_manager_base() + 0x10);
	}
	Vector3 get_enitypos(uint64_t obj) {
		uintptr_t v1 = memory::read<uint64_t>(obj + 0x60);//<transform>k__BackingField
		uintptr_t v2 = memory::read<uint64_t>(v1 + 0x10);
		uintptr_t v3 = memory::read<uint64_t>(v2 + 0x30);
		uintptr_t v4 = memory::read<uint64_t>(v3 + 0x30);
		uintptr_t v5 = memory::read<uint64_t>(v4 + 0x8);
		uintptr_t v6 = memory::read<uint64_t>(v5 + 0x38);
		uintptr_t v7 = v6 + 0x90;
		return memory::read<Vector3>(v7);
	}

	//霸体类型 20蓝霸体 21紫霸体 5站着不动 ,30 31 振刀处决状态 
	uint32_t get_endurelevel(uint64_t obj) {
		uintptr_t v1 = memory::read<uint64_t>(obj + 0xB8);//actorkit
		uintptr_t v2 = memory::read<uint64_t>(v1 + 0xC0);//actorKitHitSimulateData
		return memory::read<uint32_t>(v2 + 0x90);
	}

	//2为出刀
	uint32_t get_reactiontype(uint64_t obj) {
		uintptr_t v1 = memory::read<uint64_t>(obj + 0xB8);//actorkit
		uintptr_t v2 = memory::read<uint64_t>(v1 + 0xC0);//actorKitHitSimulateData
		return memory::read<uint32_t>(v2 + 0x48);//ReactionType
	}

	//默认10 出刀12
	uint32_t get_rangereactiontype(uint64_t obj) {
		uintptr_t v1 = memory::read<uint64_t>(obj + 0xB8);//actorkit
		uintptr_t v2 = memory::read<uint64_t>(v1 + 0xC0);//actorKitHitSimulateData
		return memory::read<uint32_t>(v2 + 0x4C);//RangeReactionType
	}

	//是否振刀?  2为震 默认0
	uint32_t get_reaction_extra_check_combo_type(uint64_t obj) {
		uintptr_t v1 = memory::read<uint64_t>(obj + 0xB8);//actorkit
		uintptr_t v2 = memory::read<uint64_t>(v1 + 0xC0);//actorKitHitSimulateData
		return memory::read<uint32_t>(v2 + 0x74);//ReactionExtraCheckComboType
	}

	uintptr_t get_entityaddr(uintptr_t items_, int count) {
		uintptr_t v1 = items_ + 0x20;
		return memory::read<uintptr_t>(v1 + count * 8);
	}

	//大于0.45完成蓄力
	float get_enduretime(uint64_t obj) {
		uintptr_t v1 = memory::read<uint64_t>(obj + 0xB8);//actorkit
		uintptr_t v2 = memory::read<uint64_t>(v1 + 0x38);//<AniParams>k__BackingField
		uintptr_t v3 = memory::read<uint64_t>(v2 + 0x18);
		uintptr_t v4 = memory::read<uint64_t>(v3 + 0x20);
		uintptr_t v5 = memory::read<uint64_t>(v4 + 0x10);
		return memory::read<float>(v5 + 0x54);//ReactionExtraCheckComboType
	}

	//取动画名称
	std::wstring get_animname(uint64_t obj) {
		wchar_t name[64] { 0 };
		uintptr_t v1 = memory::read<uint64_t>(obj + actorkit_offset);//actorkit
		uintptr_t v2 = memory::read<uint64_t>(v1 + 0x1e8);//<AnimPlayableAgent>k__BackingField
		uintptr_t v3 = memory::read<uint64_t>(v2 + 0x68);//playbleTrackDriver
		uintptr_t v4 = memory::read<uint64_t>(v3 + 0x28);//om
		uintptr_t v5 = memory::read<uint64_t>(v4 + 0x30);//EventTracksForLayer
		uintptr_t v6 = memory::read<uint64_t>(v5 + 0x20);//Item[0]
		uintptr_t v7 = memory::read<uint64_t>(v6 + 0x10);//animClipName
		memory::read(v7 + 0x14, name, sizeof(name));
		//printf("Name = %ws\n", name);
		return std::wstring(name);
	}

	//1长剑2太刀3长枪4阔刀5匕首6双截棍7双刀
	uint32_t get_weapon_type(uint32_t weapon_id)
	{
		switch (weapon_id)
		{
		case 3200001:
			return 1;
		case 3200011:
			return 1;
		case 3200021:
			return 1;
		case 3200031:
			return 1;
		case 3200300:
			return 2;
		case 3200310:
			return 2;
		case 3200320:
			return 2;
		case 3200330:
			return 2;
		case 3200200:
			return 3;
		case 3200210:
			return 3;
		case 3200220:
			return 3;
		case 3200230:
			return 3;
		case 3200400:
			return 4;
		case 3200410:
			return 4;
		case 3200420:
			return 4;
		case 3200430:
			return 4;
		case 3200101:
			return 5;
		case 3200111:
			return 5;
		case 3200121:
			return 5;
		case 3200131:
			return 5;
		case 3201540:
			return 6;
		case 3201530:
			return 6;
		case 3201520:
			return 6;
		case 3201510:
			return 6;
		case 3201610://双刀
			return 7;
		case 3201620://双刀
			return 7;
		case 3201630://双刀
			return 7;
		case 3201640://双刀
			return 7;
		default:
			break;
		}
		return 0;
	}

	bool is_weapon(uint32_t weaponid)
	{
		if (weaponid == 3209901)//拳头
		{
			return false;
		}
		return true;
	}

	bool is_attacking(uintptr_t obj)
	{
		if (sdk::get_endurelevel(obj) == 20 && sdk::get_rangereactiontype(obj) == 12)
		{
			return true;
		}
		return false;
	}

	uint64_t get_root_component(uint64_t obj) {
		uintptr_t v1 = memory::read<uint64_t>(obj + 0x60);//<transform>k__BackingField
		uintptr_t v2 = memory::read<uint64_t>(v1 + 0x10);
		uintptr_t v3 = memory::read<uint64_t>(v2 + 0x30);
		uintptr_t v4 = memory::read<uint64_t>(v3 + 0x30);
		uintptr_t v5 = memory::read<uint64_t>(v4 + 0x8);
		uintptr_t v6 = memory::read<uint64_t>(v5 + 0x38);

		return v6;
	}

	float get_enityangle(uint64_t obj) {
		Vector4 quat;
		Vector3 rot;
		uintptr_t v7 = get_root_component(obj) + 0xA0;
		quat = memory::read<Vector4>(v7);
		quat.x = 0;
		quat.z = 0;
		rot = math::QuatToRot(quat);
		rot.x = rot.x * -1.f;
		if (fabsf(quat.y) >= 0.7f)
		{
			rot.x = 180.f - rot.x;
		}
		return math::ConverseAngle(rot.x);
	}
}
