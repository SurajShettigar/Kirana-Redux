// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_CORE_CPU_INFO_HPP
#define KIRANA_CORE_CPU_INFO_HPP

#if defined(_MSC_VER)
#include <intrin.h>
#else
#include <cpuid.h>
#endif

#include <bitset>
#include <iostream>
#include <vector>

namespace kirana::core
{
/// Structure to store CPU information provided by cpuid intrinsic.
struct CPUIDRegisters
{
    int32_t eax = 0x000000;
    int32_t ebx = 0x000000;
    int32_t ecx = 0x000000;
    int32_t edx = 0x000000;
};

/**
 * Single Instruction Multi Data Instruction Set Extensions.
 */
enum class SIMDExtension
{
    NONE = 0,

    MMX = 1 << 1,

    SSE = 1 << 2,
    SSE_2 = 1 << 3,
    SSE_3 = 1 << 4,
    SSSE_3 = 1 << 5,
    SSE_4_1 = 1 << 6,
    SSE_4_2 = 1 << 7,

    AVX = 1 << 8,
    AVX_2 = 1 << 9,
    FMA = 1 << 10,

    AVX_512_F = 1 << 11,
    AVX_512_VL = 1 << 12,
    AVX_512_DQ = 1 << 13,
    AVX_512_BW = 1 << 14,
    AVX_512_FP16 = 1 << 15,
};

inline SIMDExtension operator|(SIMDExtension lhs, SIMDExtension rhs)
{
    return static_cast<SIMDExtension>(static_cast<uint32_t>(lhs) |
                                      static_cast<uint32_t>(rhs));
}

inline SIMDExtension operator&(SIMDExtension lhs, SIMDExtension rhs)
{
    return static_cast<SIMDExtension>(static_cast<uint32_t>(lhs) &
                                      static_cast<uint32_t>(rhs));
}

inline SIMDExtension operator^(SIMDExtension lhs, SIMDExtension rhs)
{
    return static_cast<SIMDExtension>(static_cast<uint32_t>(lhs) ^
                                      static_cast<uint32_t>(rhs));
}

inline SIMDExtension operator~(SIMDExtension key)
{
    return static_cast<SIMDExtension>(~static_cast<uint32_t>(key));
}

struct CPUInfo
{
    std::string vendor;
    std::string brand;
    SIMDExtension simd_support_flags = SIMDExtension::NONE;

    bool isSIMDExtensionSupported(const SIMDExtension simd_ext) const
    {
        return (simd_support_flags & simd_ext) == simd_ext;
    }
};

static CPUInfo getCPUInfo()
{
    CPUInfo info{};
    int32_t max_func_id = 0;
    int32_t max_ext_func_id = 0;
    CPUIDRegisters registers = {};
    std::vector<CPUIDRegisters> data;
    std::vector<CPUIDRegisters> ext_data;

#if defined(_MSC_VER)
    // When function id is 0, EAX register will store the max number of function
    // ids available. EBX, ECX and EDX stores the hardware vendor name.
    __cpuid(&registers.eax, 0x00000000);
    max_func_id = registers.eax;

    // Store all the cpu information.
    for (int32_t i = 0; i < max_func_id; i++)
    {
        CPUIDRegisters reg;
        __cpuidex(&reg.eax, i, 0);
        data.emplace_back(reg);
    }

    // When function id is 0x80000000, EAX register will store the max number of
    // extended function ids available.
    __cpuid(&registers.eax, static_cast<int32_t>(0x80000000));
    max_ext_func_id = registers.eax;

    // Store all the extended cpu information.
    for (auto i = static_cast<int32_t>(0x80000000); i < max_ext_func_id; i++)
    {
        CPUIDRegisters reg;
        __cpuidex(&reg.eax, i, 0);
        ext_data.emplace_back(reg);
    }
#else
  max_func_id = static_cast<int32_t>(__get_cpuid_max(
      0x00000000, nullptr);
  max_ext_func_id = static_cast<int32_t>(__get_cpuid_max(
      0x80000000, nullptr);

  for (int32_t i = 0; i < max_func_id; i++) {
    CPUIDRegisters reg;
    if (__get_cpuid(static_cast<uint32_t>(i),
                    reinterpret_cast<uin32_t *>(&reg.eax),
                    reinterpret_cast<uin32_t *>(&reg.ebx),
                    reinterpret_cast<uin32_t *>(&reg.ecx),
                    reinterpret_cast<uin32_t *>(&reg.edx)))
      data.emplace_back(reg);
  }

  for (auto i = static_cast<int32_t>(0x80000000); i < max_ext_func_id; i++) {
    CPUIDRegisters reg;
    if (__get_cpuid(static_cast<uint32_t>(i),
                    reinterpret_cast<uin32_t *>(&reg.eax),
                    reinterpret_cast<uin32_t *>(&reg.ebx),
                    reinterpret_cast<uin32_t *>(&reg.ecx),
                    reinterpret_cast<uin32_t *>(&reg.edx)))
      ext_data.emplace_back(reg);
  }
#endif

    if (data.size() == 0)
        return info;

    // Get CPU hardware vendor name from ebx, ecx and edx registers with function
    // id 0.
    char vendor[13];
    memset(vendor, 0, sizeof(vendor));
    memcpy(vendor, &data[0].ebx, sizeof(int32_t));
    memcpy(vendor + 4, &data[0].edx, sizeof(int32_t));
    memcpy(vendor + 8, &data[0].ecx, sizeof(int32_t));
    vendor[12] = '\0';
    info.vendor = vendor;

    if (ext_data.size() > 4)
    {
        // Get CPU brand name from extended information from all 4 registers and
        // functions IDs ranging from 0x80000002-0x80000004
        char brand[48];
        memset(brand, 0, sizeof(brand));
        memcpy(brand, &ext_data[2], sizeof(CPUIDRegisters));
        memcpy(brand + 16, &ext_data[3], sizeof(CPUIDRegisters));
        memcpy(brand + 32, &ext_data[4], sizeof(CPUIDRegisters));
        info.brand = brand;
    }

    if (data.size() > 1)
    {
        // Set CPU SIMD Instruction Set Extension support.
        std::bitset<32> bits = data[1].ecx;
        info.simd_support_flags =
            info.simd_support_flags |
            (bits[0] == 1 ? SIMDExtension::SSE_3 : SIMDExtension::NONE);
        info.simd_support_flags =
            info.simd_support_flags |
            (bits[9] == 1 ? SIMDExtension::SSSE_3 : SIMDExtension::NONE);
        info.simd_support_flags =
            info.simd_support_flags |
            (bits[12] == 1 ? SIMDExtension::FMA : SIMDExtension::NONE);
        info.simd_support_flags =
            info.simd_support_flags |
            (bits[19] == 1 ? SIMDExtension::SSE_4_1 : SIMDExtension::NONE);
        info.simd_support_flags =
            info.simd_support_flags |
            (bits[20] == 1 ? SIMDExtension::SSE_4_2 : SIMDExtension::NONE);
        info.simd_support_flags =
            info.simd_support_flags |
            (bits[28] == 1 ? SIMDExtension::AVX : SIMDExtension::NONE);

        bits = data[1].edx;
        info.simd_support_flags =
            info.simd_support_flags |
            (bits[23] == 1 ? SIMDExtension::MMX : SIMDExtension::NONE);
        info.simd_support_flags =
            info.simd_support_flags |
            (bits[25] == 1 ? SIMDExtension::SSE : SIMDExtension::NONE);
        info.simd_support_flags =
            info.simd_support_flags |
            (bits[26] == 1 ? SIMDExtension::SSE_2 : SIMDExtension::NONE);
    }

    if (data.size() > 8)
    {
        // Set CPU SIMD Instruction Set Extension support.
        std::bitset<32> bits = data[7].ebx;
        info.simd_support_flags =
            info.simd_support_flags |
            (bits[5] == 1 ? SIMDExtension::AVX_2 : SIMDExtension::NONE);
        info.simd_support_flags =
            info.simd_support_flags |
            (bits[16] == 1 ? SIMDExtension::AVX_512_F : SIMDExtension::NONE);
        info.simd_support_flags =
            info.simd_support_flags |
            (bits[17] == 1 ? SIMDExtension::AVX_512_DQ : SIMDExtension::NONE);
        info.simd_support_flags =
            info.simd_support_flags |
            (bits[30] == 1 ? SIMDExtension::AVX_512_BW : SIMDExtension::NONE);
        info.simd_support_flags =
            info.simd_support_flags |
            (bits[31] == 1 ? SIMDExtension::AVX_512_VL : SIMDExtension::NONE);

        bits = data[7].edx;
        info.simd_support_flags =
            info.simd_support_flags |
            (bits[23] == 1 ? SIMDExtension::AVX_512_FP16 : SIMDExtension::NONE);
    }
    return info;
}
}

#endif  // KIRANA_CORE_CPU_INFO_HPP