#pragma once
#include <cereal/cereal.hpp>

namespace yEngine
{
	// �ȗ��\�L
	using i8 = char;
	using u8 = unsigned char;
	using i16 = short;
	using u16 = unsigned short;
	using i32 = int;
	using u32 = unsigned int;

	// ���ɍ��킹�ĕύX
#ifdef _WIN32
	using i64 = long long;
	using u64 = unsigned long long;
#else	
	using i64 = long;
	using u64 = unsigned long;
#endif

	static_assert(sizeof(i8) == 1, "i8�̃T�C�Y���s���ł�");
	static_assert(sizeof(i16) == 2, "i16�̃T�C�Y���s���ł�");
	static_assert(sizeof(i32) == 4, "i32�̃T�C�Y���s���ł�");
	static_assert(sizeof(i64) == 8, "i64�̃T�C�Y���s���ł�");

	//// �G���e�B�e�B�Q��
	//struct EntityReference
	//{
	//	i32 index;
	//	bool operator==(const EntityReference &rhs) const { return rhs.index == index; }
	//	bool operator<(const EntityReference &rhs) const { return rhs.index < index; }
	//	bool operator>(const EntityReference &rhs) const { return rhs.index > index; }
	//	bool operator!=(const EntityReference &rhs) const { return rhs.index != index; }
	//	operator EntityReference() const { return EntityReference{ index }; }
	//};

	//// �R���|�[�l���g��r�p
	//struct ComponentType
	//{
	//	enum { MAX_TYPE_COUNT = 64 };
	//	i32 index;
	//	bool operator==(const ComponentType &rhs) const { return rhs.index == index; }
	//	bool operator<(const ComponentType &rhs) const { return rhs.index < index; }
	//	bool operator>(const ComponentType &rhs) const { return rhs.index > index; }
	//	bool operator!=(const ComponentType &rhs) const { return rhs.index != index; }
	//};
	//const ComponentType INVALID_COMPONENT_TYPE{ -1 };

#pragma pack(1)
	// �F�n
	struct Color
	{
		Color() :
			r(0xff),
			g(0xff),
			b(0xff),
			a(0xff)
		{
		}
		// 0xRRGGBBAA
		Color(u32 rgba) :
			r(u8((rgba >> 24) & 0xff)),
			g(u8((rgba >> 16) & 0xff)),
			b(u8((rgba >> 8) & 0xff)),
			a(u8(rgba & 0xff))
		{
		}
		Color(u8 r, u8 g, u8 b, u8 a) :
			r(r), g(g), b(b), a(a)
		{
		}

		enum
		{
			RED = 0xff0000ff,
			YELLOW = 0xffff00ff,
			GREEN = 0x00ff00ff,
			CYAN = 0x00ffffff,
			BLUE = 0x0000ffff,
			MAGENTA = 0xff00ffff,
			BLACK = 0x000000ff,
			WHITE = 0xffffffff
		};

		u8 r, g, b, a;

		// �V���A���C�Y
		template<class Archive>
		void serialize(Archive &archive)
		{
			archive(
				cereal::make_nvp("R", r),
				cereal::make_nvp("G", g),
				cereal::make_nvp("B", b),
				cereal::make_nvp("A", a)
			);
		}
	};
#pragma pack()
}
