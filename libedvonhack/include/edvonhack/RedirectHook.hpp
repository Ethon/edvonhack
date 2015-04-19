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

// Ed:
#include <edvonhack/Patch.hpp>
#include <edvonhack/util/FunctionPointerCast.hpp>

namespace ed {
   namespace cpudep {
      std::vector<Byte> getRedirectPatchData(void* target, void* hook);
   }

   template<typename ResultT, typename... ArgTs>
   class RedirectHook {
   private:
      Patch redirectPatch;

   public:
      RedirectHook(ResultT(*target)(ArgTs...), ResultT(*hook)(ArgTs...))
         :  redirectPatch(toObjectPointer(target),
               cpudep::getRedirectPatchData(toObjectPointer(target),
                  toObjectPointer(hook)))
      { }

      void applyRedirection(bool resetProtection = false) {
         redirectPatch.applyPatch(resetProtection);
      }

      void removeRedirection(bool resetProtection = false) {
         redirectPatch.removePatch(resetProtection);
      }
   };

   template<typename ResultT, typename... ArgTs>
   RedirectHook<ResultT, ArgTs...> createRedirectHook(
      ResultT(*target)(ArgTs...), ResultT(*hook)(ArgTs...)) {
      return RedirectHook<ResultT, ArgTs...>(target, hook);
   }
}