cd(@__DIR__)
using Pkg
Pkg.activate(".")
# Pkg.instantiate()
using LLVM_full_jll

#https://discourse.julialang.org/t/how-to-find-out-the-version-of-a-package-from-its-module/37755/10
pkgproject(m::Core.Module) = Pkg.Operations.read_project(Pkg.Types.projectfile_path(pkgdir(m)))
pkgversion(m::Core.Module) = pkgproject(m).version

using LibGit2
llvm_repo = "https://github.com/JuliaLang/llvm-project"
llvm_branch = "julia-" * replace(string(pkgversion(LLVM_full_jll)), "+" => "-")
llvm_source = "build/llvm-src"
if !isdir(llvm_source)
    mkpath(llvm_source)
    # doesn't work on 1.3, i dunno
    # LibGit2.clone(llvm_repo, llvm_source; branch = llvm_branch)
    clone_cmd = `git clone --depth 1 -b $llvm_branch $llvm_repo $llvm_source`
    run(@show clone_cmd)
end

llvm_root = LLVM_full_jll.artifact_dir
julia_root = Sys.BINDIR |> dirname
julia_source = julia_root |> dirname

args = [
    "-DLLVM_ROOT=$llvm_root"
    "-DClang_ROOT=$llvm_root"
    "-DLLVM_SOURCE=$llvm_source"
    "-DJULIA_ROOT=$julia_root"
    "-DJULIA_SOURCE=$julia_source"
    "-DFIND_LLVM=OFF"
    "-DCMAKE_CXX_FLAGS=-ferror-limit=1 -ggdb"
    "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON"
    "-DCMAKE_CXX_COMPILER=clang++"
]

config_cmd = `cmake -Bbuild $args`

run(@show config_cmd)

build_cmd = `cmake --build build --verbose -j`
run(@show build_cmd)

# includes = split("-I/home/melonedo/.julia/artifacts/8f2360be2c9c10c33d49eebc75d61a874d4ed029/include -I/home/melonedo/Cxx.jl/deps/build/llvm-src/clang/include -I/home/melonedo/Cxx.jl/deps/build/llvm-src/clang/lib -I/home/melonedo/packages/julias/src/support -I/home/melonedo/packages/julias/julia-1.7/include")
# test_cmd = `clang-check --extra-arg=$includes test-parse.cpp`
# run(@show test_cmd)