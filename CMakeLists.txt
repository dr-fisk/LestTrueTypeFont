cmake_minimum_required(VERSION 4.0)

project(LestTtf)

add_library(lestttf SHARED)

target_include_directories(lestttf PRIVATE include)

target_sources(lestttf
  PRIVATE
    cmap.cpp
    cvt.cpp
    endianGeneral.cpp
    fpgm.cpp
    gasp.cpp
    glyf.cpp
    head.cpp
    hhea.cpp
    hmtx.cpp
    kern.cpp
    lestTtf.cpp
    loca.cpp
    maxp.cpp
    name.cpp
    os2.cpp
    post.cpp
    prep.cpp
    vdmx.cpp
    vhea.cpp
    vmtx.cpp
  PUBLIC
    FILE_SET include
    TYPE HEADERS
    BASE_DIRS
      .
    FILES
      include/cmap.hpp
      include/cvt.hpp
      include/endianGeneral.hpp
      include/fpgm.hpp
      include/gasp.hpp
      include/glyf.hpp
      include/head.hpp
      include/hhea.hpp
      include/hmtx.hpp
      include/kern.hpp
      include/lestTableData.hpp
      include/lestTtf.hpp
      include/lestTtfData.hpp
      include/maxp.hpp
      include/loca.hpp
      include/name.hpp
      include/os2.hpp
      include/post.hpp
      include/prep.hpp
      include/ttfTable.hpp
      include/vdmx.hpp
      include/vhea.hpp
      include/vmtx.hpp)

if(LINUX_BUILD)
target_compile_options(lestttf
  PRIVATE -g -O2 -std=c++17 -MMD -Wall -Wextra)
else()
set(CMAKE_SHARED_LIBRARY_PREFIX "")
target_link_options(lestttf PRIVATE
  -static-libgcc -static-libstdc++)

target_compile_options(lestttf
  PRIVATE -g -O2 -std=c++17 -MMD -Wall -Wextra -pthread)
endif()
