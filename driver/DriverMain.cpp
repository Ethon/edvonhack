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
#include <iostream>
#include <cstdlib>

// Ed:
#include "DriverInterface.hpp"
#include <edvonhack/Application.hpp>
#include <edvonhack/util/SharedLibrary.hpp>
#include <edvonhack/util/FunctionPointerCast.hpp>

char const* const VARNAME_APPLICATION = "ED_APPLICATION";
char const* const SYMNAME_CREATOR = "createApplication";

static ed::SharedLibraryPtr applicationLibrary;
static std::unique_ptr<ed::Application> application;

void ed::driverif::init() {
   std::cout << "[INFO] Ed von Hack: Initializing application" << std::endl;

   // The path of the shared library is given as environment variable.
   char const* applicationPath = std::getenv(VARNAME_APPLICATION);
   if(applicationPath == nullptr) {
      std::cerr << "[ERROR] Ed von Hack: ED_APPLICATION not set!" << std::endl;
      return;
   }
   std::cout << "[INFO] Ed von Hack: Loading application from '"
      << applicationPath << "'" << std::endl;

   // Try to load the creator function from the shared library.
   SharedLibraryPtr library;
   ed::ApplicationCreatorFunction creator = nullptr;
   try {
      library = ed::loadSharedLibrary(applicationPath);
      creator = ed::toFunctionPointer<ed::Application*>(
         library->getSymbol(SYMNAME_CREATOR));
   } catch(std::exception const& e) {
      std::cerr << "[ERROR] Ed von Hack: Failed loading application (" <<
         e.what() << ")" << std::endl;
      return;
   }

   // Create the application via the creator function.
   try {
      application.reset(creator());
      if(!application) {
         std::cerr
            << "[ERROR] Ed von Hack: Application could not be created"
            << std::endl;
         return;
      }
      application->init();
   } catch(std::exception const& e) {
      std::cerr << "[ERROR] Ed von Hack: Application could not be created (" <<
         e.what() << ")" << std::endl;
      return;
   }

   // Yay, the application was created.
   applicationLibrary = std::move(library);
   std::cout <<
      "[INFO] Ed von Hack: Successfully loaded application" << std::endl;
}

void ed::driverif::onFrame() {
   if(application) {
      application->onFrame();
   }
}