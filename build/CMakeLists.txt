cmake_minimum_required(VERSION 2.8)
project(Guir)
set(CMAKE_CXX_STANDARD 11)
set(bd ${CMAKE_CURRENT_LIST_DIR}/../)
set(example_sources ${bd}/examples/extern/src/glad.c)

if (APPLE)

  set(ed ${bd}/examples/extern/mac)

  find_library(fr_corefoundation CoreFoundation)
  find_library(fr_cocoa Cocoa)
  find_library(fr_carbon Carbon)
  find_library(fr_iokit IOKit)
  find_library(fr_avfoundation AVFoundation)
  find_library(fr_corevideo CoreVideo)
  find_library(fr_opengl OpenGL)

  list(APPEND example_libs
    ${ed}/lib/libglfw3.a
    ${fr_corefoundation}
    ${fr_cocoa}
    ${fr_carbon}
    ${fr_iokit}
    ${fr_avfoundation}
    ${fr_corevideo}
    ${fr_opengl}
    pthread
    z
    )

elseif(WIN32)

  set(ed ${bd}/examples/extern/win)
  
  list(APPEND example_libs
    ${ed}/lib/${COMPILER}/glfw3.lib
    ws2_32.lib
    )

else()
  message(FATAL_ERROR "For now we only support Mac and Windows")
endif()

if(CMAKE_BUILD_TYPE MATCHES Debug)
  set(debug_flag "_debug")
  if (APPLE)
    set (CMAKE_CXX_FLAGS "-fsanitize=address ${CMAKE_CXX_FLAGS} -g")
  endif()
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -ffast-math -std=c++11 -g -funroll-loops -fslp-vectorize-aggressive")
endif()

include_directories(
  ${bd}/include
  ${bd}/examples
  ${bd}/examples/extern/src
  ${ed}/include
  )

link_directories(
  ${bd}/lib
  )

set(guir_lib_name "GuirApi-${COMPILER}-${CMAKE_BUILD_TYPE}-v1")

macro(create_example name)
  set(example_name "example_${name}${debug_flag}")
  add_executable(${example_name} ${bd}/examples/example_${name}.cpp ${example_sources})
  target_link_libraries(${example_name} ${guir_lib_name} ${example_libs})
  install(TARGETS ${example_name} DESTINATION bin/)
endmacro()

create_example(guir_net_client)
create_example(guir_visualisation)