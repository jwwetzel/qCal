# (1) Modern CMake minimum required version with compatibility window
cmake_minimum_required(VERSION 3.16...3.27)
project(qCal LANGUAGES CXX)

# (2) Set C++ standard explicitly and require it
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_DISABLE_FIND_PACKAGE_WrapVulkanHeaders TRUE)

# (3) Find GEANT4 with optional UI/Vis support
option(WITH_GEANT4_UIVIS "Build with Geant4 UI and Visualization support" ON)
if(WITH_GEANT4_UIVIS)
  find_package(Geant4 REQUIRED ui_all vis_all)
else()
  find_package(Geant4 REQUIRED)
endif()

# (4) Define source and include directories
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/src/*.cc)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS ${CMAKE_SOURCE_DIR}/include/*.hh)

# (5) Executable and include paths
add_executable(qCal qCal.cc ${SOURCES} ${HEADERS})
target_include_directories(qCal PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(qCal PRIVATE ${Geant4_LIBRARIES})
target_compile_definitions(qCal PRIVATE G4_USE_GDML)

# (6) Geant4 macro scripts copied to build dir
set(qCal_SCRIPTS
  init_vis.mac
  vis.mac
)

foreach(_script ${qCal_SCRIPTS})
  configure_file(${PROJECT_SOURCE_DIR}/${_script}
                 ${PROJECT_BINARY_DIR}/${_script}
                 COPYONLY)
endforeach()

# (7) Installation
install(TARGETS qCal RUNTIME DESTINATION bin)
