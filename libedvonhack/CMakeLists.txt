# Set header files.
set(LIBED_HDR
   include/edvonhack/Application.hpp
   ${LIDED_UTIL_HDR}
)
set(LIDED_UTIL_HDR
   include/edvonhack/util/FunctionPointerCast.hpp
   include/edvonhack/util/SharedLibrary.hpp
)

# Set source files.
set(LIBED_SRC
   src/util/SharedLibrary.cpp
)

# Compile libedvonhack as shared library.
add_library(edvonhack SHARED
   ${LIBED_SRC}
)

# Install library.
install(TARGETS edvonhack DESTINATION "lib${LIB_FOLDER_POSTFIX}")
install(FILES ${LIBED_HDR} DESTINATION include/edvonhack)
install(FILES ${LIDED_UTIL_HDR} DESTINATION include/edvonhack/util)