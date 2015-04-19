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

// Ed:
#include <edvonhack/RedirectHook.hpp>
#include <edvonhack/asm/x86.hpp>

std::vector<ed::Byte>
   ed::cpudep::getRedirectPatchData(void* target, void* hook) {

   // We must calulate the delta because we use a relative jump.
   std::ptrdiff_t delta =
      static_cast<char*>(hook) - static_cast<char*>(target);

   // Subtract 5 bytes because we need to skip the length of the jump
   // instruction as the IP acts like it is already set to the next 
   // instruction when calculating the jump target.
   delta -= 5;

   // We simply return a relative jmp to the hook's address.
   return std::vector<Byte> { ED_JMP_REL_32(delta) };
}