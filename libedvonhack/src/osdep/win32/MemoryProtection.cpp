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

#ifndef EDVONHACK_OS_WIN32
#  error "Attempt to compile Windows source on other OS"
#endif

// Windows:
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

// C++ Standard Library:
#include <stdexcept>
#include <cassert>
#include <system_error>

// Ed:
#include <edvonhack/MemoryProtection.hpp>

static DWORD buildProtect(int flags) {
   switch (flags) {
   case 0:
      return PAGE_NOACCESS;
   case 1:
      return PAGE_READONLY;
   case 2: // There is no write only
   case 3:
      return PAGE_READWRITE;
   case 4:
      return PAGE_EXECUTE;
   case 5:
      return PAGE_EXECUTE_READ;
   case 6: // There is no execute-write
   case 7:
      return PAGE_EXECUTE_READWRITE;
   default:
      throw std::runtime_error("Invalid memory protection flags");
   }
}

std::size_t ed::getSystemPageSize() {
   static std::size_t pageSize = 0;
   if (pageSize == 0) {
      SYSTEM_INFO systemInfo;
      GetSystemInfo(&systemInfo);
      pageSize = systemInfo.dwPageSize;
   }
   return pageSize;
}

void ed::setMemoryProtection(void* start, std::size_t length, int flags) {
   assert(flags >= 0 && flags < 8);
   DWORD protect = buildProtect(flags);
   std::size_t off;
   void* alignedStart = alignPointerToPageSize(start, off);
   if (!VirtualProtect(alignedStart, length + off, protect, nullptr)) {
      DWORD lastError = GetLastError();
      throw std::system_error(lastError, std::system_category());
   }
}