cmake_minimum_required(VERSION 3.13)
project(cxxffi CXX)

# LLVM 10 has wrong export config, so can not simply find_package
add_library(dependency INTERFACE)
if(FIND_LLVM)
    find_package(LLVM REQUIRED)
    find_package(Clang REQUIRED)
    # TODO
else()
    target_compile_options(dependency INTERFACE -fno-rtti)
    target_link_directories(dependency INTERFACE ${LLVM_ROOT}/lib)
    target_link_libraries(dependency INTERFACE LLVM)
    target_link_directories(dependency INTERFACE ${Clang_ROOT}/lib)
    target_include_directories(dependency INTERFACE ${Clang_ROOT}/include)
    target_link_libraries(dependency INTERFACE clang-cpp)
        # clangFrontendTool clangBasic clangLex clangDriver clangFrontend clangParse 
        # clangAST clangASTMatchers clangSema clangAnalysis clangEdit 
        # clangRewriteFrontend clangRewrite clangSerialization clangStaticAnalyzerCheckers 
        # clangStaticAnalyzerCore clangStaticAnalyzerFrontend clangTooling clangToolingCore 
        # clangCodeGen clangARCMigrate clangFormat)
endif()

target_include_directories(dependency INTERFACE ${LLVM_SOURCE}/clang/include ${LLVM_SOURCE}/clang/lib)

# julia
find_library(JULIA_LIB NAMES julia PATHS ${JULIA_ROOT}/lib ${JULIA_ROOT} REQUIRED)
target_link_libraries(dependency INTERFACE ${JULIA_LIB})
target_include_directories(dependency INTERFACE ${JULIA_SOURCE}/src/support ${JULIA_ROOT}/include)

add_library(cxxffi SHARED ../src/bootstrap.cpp)
target_link_libraries(cxxffi PRIVATE dependency)
target_precompile_headers(cxxffi PRIVATE ../src/pch.hpp)
