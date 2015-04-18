#pragma once

// C++ Standard Library:
#include <memory>

namespace ed {
   class SharedLibraryError : public std::runtime_error {
   private:
      std::string systemError;

   public:
      SharedLibraryError(std::string const& msg, std::string systemError);
      std::string const& getSystemError() const;
   };

   class SharedLibrary {
   public:
      virtual ~SharedLibrary() = 0;
      virtual void* getSymbol(char const* name) = 0;
   };
   typedef std::shared_ptr<SharedLibrary> SharedLibraryPtr;

   SharedLibraryPtr loadSharedLibrary(char const* path);
}