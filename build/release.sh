#!/bin/sh
d=${PWD}
bd=${d}/../
id=${bd}/install
ed=${d}/../
rd=${d}/../reference/
d=${PWD}
is_debug="n"
build_dir="build_unix"
cmake_build_type="Release"
cmake_config="Release"
debug_flag=""
debugger=""
cmake_generator=""
compiler=""

# Detect OS.
if [ "$(uname)" == "Darwin" ]; then
    if [ "${cmake_generator}" = "" ] ; then
        cmake_generator="Unix Makefiles"
    fi
    os="mac"
    compiler="Clang800"
elif [ "$(expr substr $(uname -s) 1 5)" = "Linux" ]; then
    if [ "${cmake_generator}" = "" ] ; then
        cmake_generator="Unix Makefiles"
    fi
    os="linux"
else
    if [ "${cmake_generator}" = "" ] ; then
        cmake_generator="Visual Studio 14 2015 Win64"
        build_dir="build_vs2015"
        compiler="Msvc2015"
    fi
    os="win"
fi

# Detect Command Line Options
for var in "$@"
do
    if [ "${var}" = "debug" ] ; then
        is_debug="y"
        cmake_build_type="Debug"
        cmake_config="Debug"
        debug_flag="_debug"
        if [ "${os}" = "mac" ] ; then 
            debugger="lldb"
        fi
    elif [ "${var}" = "xcode" ] ; then
        build_dir="build_xcode"
        cmake_generator="Xcode"
        build_dir="build_xcode"
    elif [ "${var}" = "vs2013" ] ; then
        cmake_generator="Visual Studio 12 2013 Win64"
        build_dir="build_vs2013"
        compiler="Msvc2013"
    fi
done

# Create unique name for this build type.
bd="${d}/${build_dir}.${cmake_build_type}"

if [ ! -d ${bd} ] ; then 
    mkdir ${bd}
fi

# Compile
cd ${bd}

cmake -DCMAKE_INSTALL_PREFIX=${id} \
      -DCMAKE_BUILD_TYPE=${cmake_build_type} \
      -DCOMPILER="${compiler}" \
      -G "${cmake_generator}" \
      ..

if [ $? -ne 0 ] ; then
    exit
fi

cmake --build . --config ${cmake_build_type} --target install

if [ $? -ne 0 ] ; then
    exit
fi

cd ${id}/bin
#${debugger} ./example_guir_net_client${debug_flag}
${debugger} ./example_guir_visualisation${debug_flag}
