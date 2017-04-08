cmake_minimum_required(VERSION 3.7)
include(ExternalProject)
project(Guir)

set(bd ${CMAKE_CURRENT_LIST_DIR}/..)
set(ed ${bd}/examples)
set(guir_name "GuirApi")
set(guir_version "v1")
set(guir_lib_name "${guir_name}-${COMPILER}-${CMAKE_BUILD_TYPE}-v1")

# Glad
# ---------------------------------------------
ExternalProject_Add(
  glad
  GIT_REPOSITORY  https://github.com/Dav1dde/glad.git
  GIT_SHALLOW 1
  UPDATE_COMMAND ""
  CMAKE_ARGS -DGLAD_INSTALL=On -DCMAKE_INSTALL_PREFIX=${CMAKE_INSTALL_PREFIX}
  )

ExternalProject_Get_Property(glad install_dir)
include_directories(${CMAKE_INSTALL_PREFIX}/include)

if (UNIX)
  list(APPEND libs ${CMAKE_INSTALL_PREFIX}/lib/libglad.a)
elseif(WIN32)
  list(APPEND libs ${CMAKE_INSTALL_PREFIX}/lib/glad.lib)
endif()

# GLFW
# ---------------------------------------------
ExternalProject_Add(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_SHALLOW 1
  UPDATE_COMMAND ""
  CMAKE_ARGS -DGLFW_BUILD_TESTS=Off -DGLFW_BUILD_DOCS=Off -DGLFW_BUILD_EXAMPLES=Off -DCMAKE_INSTALL_PREFIX=<INSTALL_DIR> 
  )

ExternalProject_Get_Property(glfw install_dir)
include_directories(${install_dir}/include)

if (UNIX)
  list(APPEND libs ${install_dir}/lib/libglfw3.a)
elseif(WIN32)
  list(APPEND libs ${install_dir}/lib/glfw3.lib)
endif()

# Guir Release
# ---------------------------------------------
ExternalProject_Add(
  gr
  URL https://github.com/ApolloInteractiveMedia/Guir/releases/download/v1.0/Guir-v1.zip
  CONFIGURE_COMMAND ""
  UPDATE_COMMAND ""
  BUILD_COMMAND ""
  INSTALL_COMMAND ""
  )

ExternalProject_Get_Property(gr source_dir)
include_directories(${source_dir}/include)
link_directories(${source_dir}/lib)
list(APPEND libs "${guir_lib_name}")

# OS dependencies
# ---------------------------------------------
if (APPLE)
    
  find_library(fr_cocoa Cocoa)
  find_library(fr_iokit IOKit)
  find_library(fr_corevideo CoreVideo)
  
  list(APPEND libs
    ${fr_iokit}
    ${fr_cocoa}
    ${fr_corevideo}
    )

elseif(WIN32)
  
  list(APPEND libs
    #Mfplat.lib
    #Mfuuid.lib
    #Mf.lib
    #Mfreadwrite.lib
    #Shlwapi.lib
    #Strmiids.lib
    #wmcodecdspuuid.lib
    #Avrt.lib
    #ws2_32.lib
    )

endif()

# Project
# ---------------------------------------------
include_directories(
  ${bd}/include
  ${ed}
  )

macro(create_example name)
  set(example_name "example_${name}${debug_flag}")
  add_executable(${example_name} ${ed}/example_${name}.cpp)
  target_link_libraries(${example_name} ${vsr_lib_name} ${libs})
  add_dependencies(${example_name} glad glfw gr)
  install(TARGETS ${example_name} DESTINATION bin/)
endmacro()

create_example("guir_net_client")
create_example("guir_visualisation")