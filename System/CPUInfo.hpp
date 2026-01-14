// Got the source of CPUInfo from Wicked Engine
// https://github.com/turanszkij/WickedEngine/blob/master/WickedEngine/Utility/cpuinfo.hpp#L150
// The MIT License(MIT)
//Copyright(c) 2026 Turánszki János

#pragma once

#include "Types.hpp"
#include "String.hpp"
#include "Exceptions.hpp"

class CPUInfo : public Object<CPUInfo>
{
	class CPUID 
	{
		u32 regs[4] = { 0 };

	public:
		explicit CPUID(u32 funcId, u32 subFuncId);

		inline constexpr const u32& EAX() const { return regs[0]; }
		inline constexpr const u32& EBX() const { return regs[1]; }
		inline constexpr const u32& ECX() const { return regs[2]; }
		inline constexpr const u32& EDX() const { return regs[3]; }
	};

public:
	
	CPUInfo();
	
	inline String Vendor() const noexcept { return mVendorId; }
	inline String Model() const noexcept { return mModelName; }
	inline constexpr i32 Cores() const noexcept { return mNumCores; }
	inline constexpr i32 LogicalCPUs() const noexcept { return mNumLogCpus; }
	inline constexpr Boolean IsHyperThreaded() const noexcept  { return mIsHTT; }
	inline constexpr Boolean HaveSSE() const noexcept  { return mIsSSE; }
	inline constexpr Boolean HaveSSE2() const noexcept  { return mIsSSE2; }
	inline constexpr Boolean HaveSSE3() const noexcept  { return mIsSSE3; }
	inline constexpr Boolean HaveSSE41() const noexcept  { return mIsSSE41; }
	inline constexpr Boolean HaveSSE42() const noexcept  { return mIsSSE42; }
	inline constexpr Boolean HaveF16C() const noexcept  { return mIsF16C; }
	inline constexpr Boolean HaveFMA3() const noexcept  { return mIsFMA3; }
	inline constexpr Boolean HaveAVX() const noexcept  { return mIsAVX; }
	inline constexpr Boolean HaveAVX2() const noexcept  { return mIsAVX2; }
	inline constexpr Boolean HaveAVX512F() const noexcept  { return mIsAVX512F; }

private:

	// Bit positions for data extractions
	static constexpr u32 SSE_POS = 0x02000000;
	static constexpr u32 SSE2_POS = 0x04000000;
	static constexpr u32 SSE3_POS = 0x00000001;
	static constexpr u32 SSE41_POS = 0x00080000;
	static constexpr u32 SSE42_POS = 0x00100000;
	static constexpr u32 AVX_POS = 0x10000000;
	static constexpr u32 AVX2_POS = 0x00000020;
	static constexpr u32 FMA3_POS = 1u << 12;
	static constexpr u32 AVX512F_POS = 1u << 15; // Bit 16
	static constexpr u32 F16C_POS = 1u << 29;
	static constexpr u32 LVL_NUM = 0x000000FF;
	static constexpr u32 LVL_TYPE = 0x0000FF00;
	static constexpr u32 LVL_CORES = 0x0000FFFF;

	// Attributes
	String mVendorId;
	String mModelName;
	i32 mNumSMT = 0;
	i32 mNumCores = 0;
	i32 mNumLogCpus = 0;
	Boolean mIsHTT = 0;
	Boolean mIsSSE = false;
	Boolean mIsSSE2 = false;
	Boolean mIsSSE3 = false;
	Boolean mIsSSE41 = false;
	Boolean mIsSSE42 = false;
	Boolean mIsAVX = false;
	Boolean mIsAVX2 = false;
	Boolean mIsAVX512F = false;
	Boolean mIsF16C = false;
	Boolean mIsFMA3 = false;
};