# Base project setup.
project(edvonhack)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# Get the target CPU from the user.
set(TARGET_CPU "x86" CACHE STRING "Select a target CPU")
set_property(CACHE TARGET_CPU PROPERTY STRINGS
   "x86" "x64")
if(${TARGET_CPU} STREQUAL "x86")
   set(LIB_FOLDER_POSTFIX "")
elseif(${TARGET_CPU} STREQUAL "x64")
   set(LIB_FOLDER_POSTFIX "64")
endif()

# Retrieve the current operating system.
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(TARGET_OS "linux")
elseif(WIN32)
   set(TARGET_OS "win32")
endif()

# Set compiler flags.
if(CMAKE_COMPILER_IS_GNUCXX OR "${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
   add_definitions("-std=c++11 -Wall -Wextra -Weffc++")

   if(${TARGET_CPU} STREQUAL "x86")
      add_definitions("-DEDVONHACK_CPU_X86")
      set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m32")
      set(CMAKE_SHARED_LIBRARY_C_FLAGS "${CMAKE_SHARED_LIBRARY_C_FLAGS} -m32")
      set(CMAKE_SHARED_LIBRARY_CXX_FLAGS
         "${CMAKE_SHARED_LIBRARY_CXX_FLAGS} -m32")
   elseif(${TARGET_CPU} STREQUAL "x64")
      add_definitions("-DEDVONHACK_CPU_X64")
      set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m64")
      set(CMAKE_SHARED_LIBRARY_C_FLAGS "${CMAKE_SHARED_LIBRARY_C_FLAGS} -m64")
      set(CMAKE_SHARED_LIBRARY_CXX_FLAGS
         "${CMAKE_SHARED_LIBRARY_CXX_FLAGS} -m64")
   endif()

   if(${TARGET_OS} STREQUAL "linux")
      add_definitions("-DEDVONHACK_OS_LINUX")
   elseif(${TARGET_OS} STREQUAL "win32")
      add_definitions("-DEDVONHACK_OS_WIN32")
   endif()
endif()

# Set common include directories.
include_directories(
   ${CMAKE_SOURCE_DIR}/libedvonhack/include
)

# Compile the parts of Ed von Hack.
add_subdirectory(libedvonhack)
add_subdirectory(driver)