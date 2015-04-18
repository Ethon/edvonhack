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

#ifndef EDVONHACK_OS_LINUX
#  error "Attempt to compile Linux source on other OS"
#endif

// Unix:
#include <unistd.h>
#include <sys/mman.h>

// C++ Standard Library:
#include <stdexcept>
#include <cassert>
#include <system_error>

// Ed:
#include <edvonhack/MemoryProtection.hpp>

static int buildProtect(int flags) {
   switch (flags) {
   case 0:
      return PROT_NONE;
   case 1:
      return PROT_READ;
   case 2:
      return PROT_WRITE;
   case 3:
      return PROT_WRITE | PROT_READ;
   case 4:
      return PROT_EXEC;
   case 5:
      return PROT_EXEC | PROT_READ;
   case 6:
      return PROT_EXEC | PROT_WRITE;
   case 7:
      return PROT_EXEC | PROT_WRITE | PROT_READ;
   default:
      throw std::runtime_error("Invalid memory protection flags");
   }
}

std::size_t ed::getSystemPageSize() {
   static std::size_t pageSize = 0;
   if (pageSize == 0) {
      long size = sysconf(_SC_PAGESIZE);
      if(size == -1) {
         throw std::system_error(errno, std::system_category(),
            "sysconf(_SC_PAGESIZE) failed");
      }
      pageSize = size;
   }
   return pageSize;
}

void ed::setMemoryProtection(void* start, std::size_t length, int flags) {
   assert(flags >= 0 && flags < 8);
   int protect = buildProtect(flags);
   std::size_t off;
   void* alignedStart = alignPointerToPageSize(start, off);
   if (mprotect(alignedStart, length + off, protect) == -1) {
      throw std::system_error(errno, std::system_category(),
         "mprotect failed");
   }
}