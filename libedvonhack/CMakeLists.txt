# Set header files.
set(LIBED_HDR
   include/edvonhack/Application.hpp
   include/edvonhack/MemoryProtection.hpp
   include/edvonhack/Patch.hpp
   include/edvonhack/RedirectHook.hpp
   ${LIDED_UTIL_HDR}
)
set(LIDED_UTIL_HDR
   include/edvonhack/util/FunctionPointerCast.hpp
   include/edvonhack/util/SharedLibrary.hpp
)
set(LIBED_ASM_HDR
   include/edvonhack/asm/Common.hpp
   include/edvonhack/asm/x86.hpp
)

# Set source files.
set(LIBED_SRC
   src/osdep/${TARGET_OS}/SharedLibrary.cpp
   src/osdep/${TARGET_OS}/MemoryProtection.cpp
   src/cpudep/${TARGET_CPU}/RedirectHook.cpp
   src/Patch.cpp
)

# Compile libedvonhack as shared library.
add_library(edvonhack SHARED
   ${LIBED_SRC}
)

# Install library.
install(TARGETS edvonhack DESTINATION "lib${LIB_FOLDER_POSTFIX}")
install(FILES ${LIBED_HDR} DESTINATION include/edvonhack)
install(FILES ${LIDED_UTIL_HDR} DESTINATION include/edvonhack/util)
install(FILES ${LIDED_ASM_HDR} DESTINATION include/edvonhack/asm)