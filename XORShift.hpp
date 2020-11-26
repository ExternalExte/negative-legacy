#pragma once
namespace XorShift 
{
	class XorShift32 
	{
		static_assert((sizeof 0u) == 4u);
		struct { unsigned x = 0u, y = 0u, z = 0, w = 0u; }seeds;
	public:
		XorShift32()
		{
			seeds =
			{
				123456789u,
				362436069u,
				521288629u,
				88675123u
			};
		}
		void seed(unsigned seed)
		{
			seeds.x = seed << 13u;
			seeds.y = (seed >> 9u) ^ (seeds.x << 6u);
			seeds.z = seeds.y >> 7u;
			seeds.w = seed;
		}
		XorShift32(unsigned seed){this->seed(seed);	}
		unsigned operator()()
		{
			auto t = seeds.x ^ (seeds.x << 11u);
			seeds.x = seeds.y;
			seeds.y = seeds.z;
			seeds.z = seeds.w;
			seeds.w = (seeds.w ^ (seeds.w >> 19u)) ^ (t ^ (t >> 8u));
			return seeds.w;
		}
		static unsigned max(){	return 0xffffffffu;}
		static unsigned min() { return 0u; }
		using result_type = unsigned;
	};
}
