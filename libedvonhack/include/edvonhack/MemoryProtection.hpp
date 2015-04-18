// Copyright (c) <2015> <Florian Erler>
//
// This software is provided 'as-is', without any express or implied warranty.
// In no event will the authors be held liable for any damages arising
// from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented;
// you must not claim that you wrote the original software.
// If you use this software in a product, an acknowledgment in the product
// documentation would be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
// be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source distribution.

#pragma once

// C++ Standard Library:
#include <cstdint>

namespace ed {
   enum class MemoryProtection {
      Read = 1 << 0,
      Write = 1 << 1,
      Execute = 1 << 2
   };

   std::size_t getSystemPageSize();

   inline void* alignPointerToPageSize(void* pointer, std::size_t& off) {
      std::uintptr_t asInt = reinterpret_cast<std::uintptr_t>(pointer);
      off = asInt % getSystemPageSize();
      asInt -= off;
      return reinterpret_cast<void*>(asInt);
   }

   void setMemoryProtection(void* start, std::size_t length, int flags);
}