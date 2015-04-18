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

// GNU:
#include <dlfcn.h>

// Ed:
#include <edvonhack/util/SharedLibrary.hpp>

#ifndef EDVONHACK_OS_LINUX
#  error "Attempt to compile Linux Library on other OS"
#endif

class SharedLibraryImpl : public ed::SharedLibrary {
private:
   void* handle;

public:
   explicit SharedLibraryImpl(char const* path)
      : handle(dlopen(path, RTLD_LAZY)) {

      if(handle == nullptr) {
         throw std::runtime_error(
            std::string("dlopen failed: ") + dlerror());
      }
   }

   virtual ~SharedLibraryImpl() override {
      if(handle != nullptr) {
         if(dlclose(handle) != 0) {
            throw std::runtime_error(
               std::string("dlclose failed: ") + dlerror());
         }
      }
   }

   virtual void* getSymbol(char const* name) override {
      dlerror();
      void* result = dlsym(handle, name);
      char const* error = dlerror();
      if(error != nullptr) {
         throw std::runtime_error(
               std::string("dlsym failed: ") + error);
      }
      return result;
   }

   // Disallow copies.
   SharedLibraryImpl(SharedLibraryImpl const&) = delete;
   SharedLibraryImpl& operator=(SharedLibraryImpl const&) = delete;
};

ed::SharedLibraryPtr ed::loadSharedLibrary(const char* path) {
   return std::make_shared<SharedLibraryImpl>(path);
}


