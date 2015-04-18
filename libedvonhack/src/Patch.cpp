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

// C++ Standard Library:
#include <cassert>
#include <cstring>

// Ed:
#include <edvonhack/Patch.hpp>
#include <edvonhack/MemoryProtection.hpp>

ed::Patch::Patch(void* address, std::vector<Byte> patchData)
   :  address(address), patchData(std::move(patchData)),
      oldData(patchData.size()) {

   assert(address != nullptr);
   std::memcpy(oldData.data(), address, oldData.size());
}

ed::Patch::~Patch() {
   removePatch();
}

void ed::Patch::applyPatch(bool resetProtection) {
   assert(patchData.size() == oldData.size());
   ed::setMemoryProtection(address, patchData.size(),
      prot::Read | prot::Write | prot::Execute);
   std::memcpy(address, patchData.data(), patchData.size());
   if(resetProtection) {
      ed::setMemoryProtection(address, patchData.size(),
         prot::Read | prot::Execute);
   }
}

void ed::Patch::removePatch(bool resetProtection) {
   assert(patchData.size() == oldData.size());
   ed::setMemoryProtection(address, oldData.size(),
      prot::Read | prot::Write | prot::Execute);
   std::memcpy(address, oldData.data(), oldData.size());
   if(resetProtection) {
      ed::setMemoryProtection(address, oldData.size(),
         prot::Read | prot::Execute);
   }
}