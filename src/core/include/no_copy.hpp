// Copyright 2025 Suraj Shettigar
// SPDX-License-Identifier: Apache-2.0

#ifndef KIRANA_CORE_NO_COPY_HPP
#define KIRANA_CORE_NO_COPY_HPP

namespace kirana::core
{
/**
 * Any class that inherits this will not be copyable.
 */
class NoCopy
{
public:
    NoCopy(const NoCopy &non_copy) = delete;
    NoCopy &operator=(const NoCopy &non_copy) = delete;

protected:
    NoCopy() = default;
    ~NoCopy() = default;
};
}

#endif  // KIRANA_CORE_NO_COPY_HPP