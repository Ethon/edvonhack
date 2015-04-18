# Get the desired driver from the user.
if(TARGET_OS STREQUAL "linux")
   set(DRIVER "WineGlx" CACHE STRING "Select a driver")
   set_property(CACHE DRIVER PROPERTY STRINGS
      "WineGlx")
endif()

# Set the necessary source files, libs and compiler flags for the driver.
if(${DRIVER} STREQUAL "WineGlx")
   set(DRIVER_SRC linux/WineGlx.cpp)
   set(DRIVER_LIBS dl)
endif()

# Build the driver as shared library.
add_library(driver_${DRIVER} SHARED
   DriverMain.cpp ${DRIVER_SRC}
)
target_link_libraries(driver_${DRIVER} edvonhack ${DRIVER_LIBS})

# Install the driver.
install(TARGETS driver_${DRIVER} DESTINATION
   "lib${LIB_FOLDER_POSTFIX}/edvonhack")