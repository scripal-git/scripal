cmake_minimum_required(VERSION 3.16)

project(scripal VERSION 1.2.0 DESCRIPTION "Scripal text processor" LANGUAGES CXX)

# set for gprof usage
# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pg")
# SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
# SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")

# set for gcov usage
# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -fPIC -fprofile-arcs -ftest-coverage")
# SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}  -fPIC -fprofile-arcs -ftest-coverage")
# SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}  -fPIC -fprofile-arcs -ftest-coverage")

# set for fsanitize usage
# SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer -static-libasan")
# SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address -fno-omit-frame-pointer -static-libasan")
# SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fsanitize=address -fno-omit-frame-pointer -static-libasan")

# experiment with static linking
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wl,-Bstatic -l:libPocoFoundation.a -l:libPocoUtil.a -l:libPocoJSON.a -l:libPocoEncodings.a -l:libPocoZip.a -Wl,-Bdynamic")
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-Bstatic -l:libPocoFoundation.a -l:libPocoUtil.a -l:libPocoJSON.a -l:libPocoEncodings.a -l:libPocoZip.a -Wl,-Bdynamic")
#SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,-Bstatic -l:libPocoFoundation.a -l:libPocoUtil.a -l:libPocoJSON.a -l:libPocoEncodings.a -l:libPocoZip.a -Wl,-Bdynamic")

#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
#SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fPIC -Wl,-Bstatic -l:libPocoFoundation.a -l:libPocoUtil.a -l:libPocoJSON.a -l:libPocoEncodings.a -l:libPocoZip.a ")
#SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -fPIC -Wl,-Bstatic -l:libPocoFoundation.a -l:libPocoUtil.a -l:libPocoJSON.a -l:libPocoEncodings.a -l:libPocoZip.a ")

add_compile_options(-Wno-attributes -Wno-write-strings -finline-functions)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set( CMAKE_VERBOSE_MAKEFILE on )
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../")

file(GLOB scripal_SRC
     "../inc/*.hpp"
     "../src/*.cpp"
)

file(GLOB scripal_LIB
     "../inc/*.hpp"
)

include_directories( 
  ../inc/
  ../src/
)

add_executable(
  scripal 
  ${scripal_SRC}
)

add_executable(
  scripal_d 
  ${scripal_SRC}
)

add_library(
  so_scripal SHARED
  ${scripal_SRC}
)

add_library(
  a_scripal STATIC
  ${scripal_SRC}
)

add_library(
  so_scripal_d SHARED
  ${scripal_SRC}
)

add_library(
  a_scripal_d STATIC
  ${scripal_SRC}
)
add_compile_definitions(_SCRIPAL=1)
set_target_properties(scripal_d PROPERTIES OUTPUT_NAME "scripal")
set_target_properties(so_scripal so_scripal_d PROPERTIES PUBLIC_HEADER  "${scripal_LIB}")
set_target_properties(a_scripal a_scripal_d PROPERTIES PUBLIC_HEADER  "${scripal_LIB}")
set_target_properties(so_scripal so_scripal_d PROPERTIES OUTPUT_NAME scripal)
set_target_properties(so_scripal so_scripal_d PROPERTIES ENABLE_EXPORTS 1)
target_compile_definitions(so_scripal PUBLIC _SHARED_LIB_)
target_compile_definitions(so_scripal_d PUBLIC _SHARED_LIB_)
set_target_properties(a_scripal a_scripal_d PROPERTIES OUTPUT_NAME scripal)
set_property(TARGET a_scripal PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET so_scripal PROPERTY POSITION_INDEPENDENT_CODE ON)

set_property(TARGET scripal_d PROPERTY OUTPUT_NAME scripal)

install (TARGETS so_scripal so_scripal_d 
  LIBRARY DESTINATION /usr/lib/scripal
  INCLUDES DESTINATION /usr/include/scripal
  PUBLIC_HEADER DESTINATION /usr/include/scripal
)

install (TARGETS a_scripal a_scripal_d 
  LIBRARY DESTINATION /usr/lib/scripal
  INCLUDES DESTINATION /usr/include/scripal
  PUBLIC_HEADER DESTINATION /usr/include/scripal
)

SET(MY_LIB_LINK_LIBRARIES -Wl,--whole-archive PocoUtil.a PocoJSON.a  PocoEncodings.a  PocoZip.a  PocoFoundation.a PocoXML.a -Wl,--no-whole-archive)
target_link_libraries( scripal ${MY_LIB_LINK_LIBRARIES}
)

SET(MY_LIB_LINK_LIBRARIES PocoUtil PocoJSON  PocoEncodings  PocoZip  PocoFoundation PocoXML)
target_link_libraries( scripal_d ${MY_LIB_LINK_LIBRARIES}
)

target_link_libraries( a_scripal
  PocoUtil.a PocoJSON.a  PocoEncodings.a  PocoZip.a  PocoFoundation.a PocoXML.a -static
)

target_link_libraries( a_scripal_d
  PocoUtil.a PocoJSON.a  PocoEncodings.a  PocoZip.a  PocoFoundation.a PocoXML.a -static
)


SET(MY_LIB_LINK_LIBRARIES -Wl,--whole-archive PocoUtil.a PocoJSON.a  PocoEncodings.a  PocoZip.a  PocoFoundation.a PocoXML.a -Wl,--no-whole-archive)
target_link_libraries( so_scripal ${MY_LIB_LINK_LIBRARIES}
)

SET(MY_LIB_LINK_LIBRARIES_D PocoUtil PocoJSON  PocoEncodings  PocoZip  PocoFoundation PocoXML)
target_link_libraries( so_scripal_d ${MY_LIB_LINK_LIBRARIES_D}
)
