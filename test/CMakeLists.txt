# 指定cmake最低编译版本
CMAKE_MINIMUM_REQUIRED(VERSION 3.14)
# 指定工程的名称
PROJECT(gctest)
set(CMAKE_CXX_STANDARD 17)
#指定头文件目录位置
INCLUDE_DIRECTORIES(../)
# INCLUDE_DIRECTORIES(/usr/local/Cellar/protobuf/3.15.5/include)
# #添加共享库搜索路径
# LINK_DIRECTORIES(/usr/local/Cellar/protobuf/3.15.5/lib)
#GTEST
# INCLUDE_DIRECTORIES(/usr/local/include)
# LINK_DIRECTORIES(/usr/local/lib)
# INCLUDE_DIRECTORIES(/root/app/thirdlib/mysql/include)
# LINK_DIRECTORIES(/root/app/thirdlib/mysql/lib)

# INCLUDE_DIRECTORIES(/usr/local/tars/cpp/include)
# LINK_DIRECTORIES(/usr/local/tars/cpp/lib)

cmake_policy(SET CMP0077 NEW)


# link tars
# if(WIN32)
#     include (c:\\tars\\cpp\\makefile\\tars-tools.cmake)
# else()
#     include (/usr/local/tars/cpp/makefile/tars-tools.cmake)
# endif()
# set(CMAKE_CXX_FLAGS " ${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
include(FetchContent)
# FetchContent_Declare(
#   protobuf
#   # use auto sync mirror repo maintained by ourself
#   # GIT_REPOSITORY https://github.com/protocolbuffers/protobuf.git
#   GIT_REPOSITORY https://e.coding.net/muca/mirrorlib/protobuf.git
#   GIT_TAG        v3.13.0
#   SOURCE_SUBDIR  cmake
# )
# set(protobuf_BUILD_TESTS OFF)
FetchContent_Declare(
  googletest
  # use auto sync mirror repo maintained by ourself
  # GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_REPOSITORY https://e.coding.net/muca/mirrorlib/googletest.git
  GIT_TAG        release-1.10.0
)
# FetchContent_GetProperties(googletest)
fetchcontent_declare(yaml
    # use auto sync mirror repo maintained by ourself
    # GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
    GIT_REPOSITORY https://e.coding.net/muca/mirrorlib/yaml-cpp.git
    GIT_TAG yaml-cpp-0.6.3
    )

option(YAML_CPP_BUILD_TESTS "Enable testing" OFF)
option(YAML_CPP_BUILD_TOOLS "Enable parse tools" OFF)
option(YAML_CPP_BUILD_CONTRIB "Enable contrib stuff in library" OFF)
option(YAML_CPP_INSTALL "Enable generation of install target" OFF)
# fetchcontent_makeavailable(yaml)
if(NOT yaml_POPULATED)
  FetchContent_Populate(yaml)
  add_subdirectory(${yaml_SOURCE_DIR} ${yaml_BINARY_DIR})
endif()

option(BUILD_GMOCK "option for not build gmock" OFF)
option(protobuf_BUILD_TESTS "Build tests" OFF)
# option(protobuf_BUILD_PROTOC_BINARIES "Build libprotoc and protoc compiler" OFF)

if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()


# FetchContent_MakeAvailable(protobuf)
# FetchContent_GetProperties(protobuf )
# if( NOT protobuf_POPULATED )
#   FetchContent_Populate(protobuf)
#   add_subdirectory(${protobuf_SOURCE_DIR}/cmake ${protobuf_BINARY_DIR})
# endif()


# set(protobuf_MODULE_COMPATIBLE ON CACHE BOOL "")
# file(GLOB PROTO_DEF ../proto)
# IF (APPLE)
#   SET(PROTOBUF_PROTOC_EXECUTABLE ../../thirdlib/mac-protoc)
# ELSEIF (UNIX)
#   SET(PROTOBUF_PROTOC_EXECUTABLE   ../../thirdlib/protoc-3.13.0.0)
#   SET(TARS2CPP_EXECUTABLE   /usr/local/tars/cpp/tools/tars2cpp)
#   # SET(PROTOBUF_PROTOC_EXECUTABLE   /root/app/thirdlib/protoc-3.13.0.0)
#   EXEC_PROGRAM(${PROTOBUF_PROTOC_EXECUTABLE} ARGS -I=../../proto --cpp_out=../../protogen ../../proto/*.proto)
#   EXEC_PROGRAM(${TARS2CPP_EXECUTABLE} ARGS  ../../proto/*.tars --dir=../../proto) 
# ENDIF ()
# AUX_SOURCE_DIRECTORY(../proto DIR_SRCS)
# AUX_SOURCE_DIRECTORY(../protogen DIR_SRCS)
AUX_SOURCE_DIRECTORY(../frame DIR_SRCS)
# AUX_SOURCE_DIRECTORY(../database DIR_SRCS)
AUX_SOURCE_DIRECTORY(../thirdlib DIR_SRCS)
AUX_SOURCE_DIRECTORY(../shm_manager DIR_SRCS)
AUX_SOURCE_DIRECTORY(../utils DIR_SRCS)


SET(SRC
${DIR_SRCS}
)

#生成可执行文件
ADD_EXECUTABLE(${PROJECT_NAME} unittest.cpp ${SRC} )
# ADD_EXECUTABLE(${PROJECT_NAME} testcpp.cpp ${SRC})
#为hello添加共享库链接
IF (APPLE)
  TARGET_LINK_LIBRARIES(${PROJECT_NAME} gtest yaml-cpp)
	MESSAGE(STATUS "Now is Apple")
ELSEIF (UNIX)
  TARGET_LINK_LIBRARIES(${PROJECT_NAME} gtest yaml-cpp dl)
	MESSAGE(STATUS "Now is UNIX-like OS's.")
ENDIF ()



