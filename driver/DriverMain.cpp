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
   ed::ApplicationCreatorFunction creator = nullptr;
   try {
      applicationLibrary = ed::loadSharedLibrary(applicationPath);
      creator = ed::toFunctionPointer<ed::Application*>(
         applicationLibrary->getSymbol(SYMNAME_CREATOR));
   } catch(ed::SharedLibraryError const& e) {
      std::cerr << "[ERROR] Ed von Hack: Failed loading application (" <<
         e.what() << ": " << e.getSystemError() << ")" << std::endl;
      if(!creator) {
         applicationLibrary.reset();
      }
      return;
   }

   // Create the application via the creator function.
   try {
      application.reset(creator());
      application->init();
   } catch(std::exception const& e) {
      std::cerr << "[ERROR] Ed von Hack: Application could not be created (" <<
         e.what() << ")" << std::endl;
      applicationLibrary.reset();
      return;
   }
   if(!application) {
      std::cerr <<
         "[ERROR] Ed von Hack: Application could not be created" << std::endl;
      applicationLibrary.reset();
      return;
   }

   // Yay, the application was created.
   std::cout <<
      "[INFO] Ed von Hack: Successfully loaded application" << std::endl;
}

void ed::driverif::onFrame() {
   if(application) {
      application->onFrame();
   }
}