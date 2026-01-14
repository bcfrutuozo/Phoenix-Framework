#include "CPUInfo.hpp"

#include <string>

#ifdef _WIN32
#include <intrin.h>
#endif

CPUInfo::CPUID::CPUID(u32 funcId, u32 subFuncId) {
#ifdef _WIN32
	::__cpuidex((int*)regs, (int)funcId, (int)subFuncId);
#else
	asm volatile
		("cpuid" : "=a" (regs[0]), "=b" (regs[1]), "=c" (regs[2]), "=d" (regs[3])
			: "a" (funcId), "c" (subFuncId));
#endif
}

CPUInfo::CPUInfo()
{
	// Get vendor name EAX=0
	CPUID cpuID0(0, 0);
	const u32 HFS = cpuID0.EAX();
	// Reinterpret bytes as ASCII characters
	mVendorId += String((const char*)&cpuID0.EBX(), 4);
	mVendorId += String((const char*)&cpuID0.EDX(), 4);
	mVendorId += String((const char*)&cpuID0.ECX(), 4);
	// Get SSE instructions availability
	CPUID cpuID1(1, 0);
	mIsHTT = cpuID1.EDX() & AVX_POS;
	mIsSSE = cpuID1.EDX() & SSE_POS;
	mIsSSE2 = cpuID1.EDX() & SSE2_POS;
	mIsSSE3 = cpuID1.ECX() & SSE3_POS;
	mIsSSE41 = cpuID1.ECX() & SSE41_POS;
	mIsSSE42 = cpuID1.ECX() & SSE41_POS;
	mIsAVX = cpuID1.ECX() & AVX_POS;
	mIsF16C = cpuID1.ECX() & F16C_POS;
	mIsFMA3 = cpuID1.ECX() & FMA3_POS;
	// Get AVX2 instructions availability
	CPUID cpuID7(7, 0);
	mIsAVX2 = cpuID7.EBX() & AVX2_POS;
	mIsAVX512F = cpuID7.EBX() & AVX512F_POS;

	String vendorIdUppercase = mVendorId.ToUpper();

	// Get num of cores
	if (vendorIdUppercase.Contains("INTEL"))
	{
		if (HFS >= 11)
		{
			static constexpr i32 MAX_INTEL_TOP_LVL = 4;
			for (int lvl = 0; lvl < MAX_INTEL_TOP_LVL; ++lvl)
			{
				CPUID cpuID4(0x0B, lvl);
				uint32_t currLevel = (LVL_TYPE & cpuID4.ECX()) >> 8;
				switch (currLevel)
				{
				case 0x01: mNumSMT = LVL_CORES & cpuID4.EBX(); break; //  EAX=0xB, ECX=0 - EBX is the number of logical processors (threads) per core
				case 0x02: mNumLogCpus = LVL_CORES & cpuID4.EBX(); break; // EAX=0xB, ECX=1 - EBX is the number of logical processors per processor package
				default: break;
				}
			}
			mNumCores = mNumLogCpus / mNumSMT;
			mIsHTT = mNumSMT > 1;
		}
		else
		{
			if (HFS >= 1)
			{
				mNumLogCpus = (cpuID1.EBX() >> 16) & 0xFF;
				if (HFS >= 4)
				{
					mNumCores = 1 + ((CPUID(4, 0).EAX() >> 26) & 0x3F);
				}
			}
			if (mIsHTT)
			{
				if (!(mNumCores > 1))
				{
					mNumCores = 1;
					mNumLogCpus = (mNumLogCpus >= i32(2) ? mNumLogCpus : i32(2));
				}
			}
			else
			{
				mNumCores = mNumLogCpus = 1;
			}
		}
	}
	else if (vendorIdUppercase.Contains("AMD"))
	{
		// From https://github.com/time-killer-games/ween/blob/db69cafca2222c634a1d3a9e58262b5a2dc8d508/system.cpp#L1469-L1528
		mNumSMT = 1 + ((CPUID(0x8000001e, 0).EBX() >> 8) & 0xff);
		if (mNumLogCpus > 0 && mNumSMT > 0) {
			mNumCores = mNumLogCpus / mNumSMT;
		}
		else {
			if (HFS >= 1) {
				if (CPUID(0x80000000, 0).EAX() >= 8) {
					mNumCores = 1 + (CPUID(0x80000008, 0).ECX() & 0xFF);
				}
			}
			if (mIsHTT) {
				if (mNumCores < 1) {
					mNumCores = 1;
				}
			}
			else {
				mNumCores = 1;
			}
		}
	}

	// Get processor brand string
	// This seems to be working for both Intel & AMD vendors
	for (int i = 0x80000002; i < 0x80000005; ++i)
	{
		CPUID cpuID(i, 0);
		mModelName += String((const char*)&cpuID.EAX(), 4);
		mModelName += String((const char*)&cpuID.EBX(), 4);
		mModelName += String((const char*)&cpuID.ECX(), 4);
		mModelName += String((const char*)&cpuID.EDX(), 4);
	}
}