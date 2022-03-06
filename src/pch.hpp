#undef B0 //rom termios
#define __STDC_LIMIT_MACROS
#define __STDC_CONSTANT_MACROS

#include <iostream>
#include <dlfcn.h>
#include <cstdlib>

#ifdef NDEBUG
#define OLD_NDEBUG
#endif

#ifdef LLVM_NDEBUG
#define NDEBUG 1
#else
#undef NDEBUG
#endif

#include "llvm/Config/llvm-config.h"

#if defined(LLVM_VERSION_MAJOR) && LLVM_VERSION_MAJOR == 3 && LLVM_VERSION_MINOR >= 6
#define LLVM36 1
#endif
#if defined(LLVM_VERSION_MAJOR) && LLVM_VERSION_MAJOR == 3 && LLVM_VERSION_MINOR >= 8
#define LLVM38 1
#endif
#if defined(LLVM_VERSION_MAJOR) && LLVM_VERSION_MAJOR == 3 && LLVM_VERSION_MINOR >= 9
#define LLVM39 1
#endif
#if defined(LLVM_VERSION_MAJOR) && LLVM_VERSION_MAJOR >= 4
#define LLVM36 1
#define LLVM38 1
#define LLVM39 1
#define LLVM40 1
#endif

// LLVM includes
#include "llvm/ADT/DenseMapInfo.h"
#ifdef LLVM40
#include "llvm/Bitcode/BitcodeWriter.h"
#else
#include "llvm/Bitcode/ReaderWriter.h"
#endif
#include "llvm/ADT/DenseSet.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/Support/Host.h"
#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include "llvm/IR/ValueMap.h"
#include "llvm/Transforms/Utils/Cloning.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"


// Clang includes
#include "clang/Sema/ScopeInfo.h"
#include "clang/AST/ASTContext.h"
// Well, yes this is cheating
#define private public
#include "clang/Parse/Parser.h"
#undef private
#include "clang/Parse/ParseDiagnostic.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/StmtVisitor.h"
#include "clang/AST/DeclVisitor.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/Sema/SemaConsumer.h"
#include "clang/Frontend/ASTUnit.h"
#include "clang/Analysis/DomainSpecific/CocoaConventions.h"
#include "clang/Basic/Builtins.h"
#include "clang/Basic/Diagnostic.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/TargetInfo.h"
#include "clang/Lex/PreprocessorOptions.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Lex/HeaderSearch.h"
#include "clang/Parse/ParseAST.h"
#include "clang/Lex/Lexer.h"
#include "clang/Sema/Sema.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Sema/SemaDiagnostic.h"
#include "clang/Sema/Lookup.h"
#include "clang/Sema/Initialization.h"
#include "clang/AST/PrettyDeclStackTrace.h"
#include "clang/Serialization/ASTWriter.h"
#include "clang/Frontend/MultiplexConsumer.h"
#include "llvm/Support/VirtualFileSystem.h"
#include "Sema/TypeLocBuilder.h"
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Basic/CodeGenOptions.h>
#include <clang/AST/Type.h>
#include <clang/AST/ASTContext.h>
#include <clang/AST/DeclTemplate.h>
#include <clang/Basic/Specifiers.h>
#include <clang/CodeGen/CGFunctionInfo.h>
#include <clang/Parse/RAIIObjectsForParser.h>

#include "CodeGen/CodeGenModule.h"
#include <CodeGen/CodeGenTypes.h>
#define private public
#include <CodeGen/CodeGenFunction.h>
#undef private
#include "CodeGen/CGCXXABI.h"
#include "CodeGen/CGRecordLayout.h"

#ifdef _OS_WINDOWS_
#define STDCALL __stdcall
# ifdef LIBRARY_EXPORTS
#  define JL_DLLEXPORT __declspec(dllexport)
# else
#  define JL_DLLEXPORT __declspec(dllimport)
# endif
#else
#define STDCALL
#define JL_DLLEXPORT __attribute__ ((visibility("default")))
#endif

#ifndef OLD_NDEBUG
#undef NDEBUG
#endif

#if defined(_CPU_X86_64_)
#  define _P64
#elif defined(_CPU_X86_)
#  define _P32
#elif defined(_OS_WINDOWS_)
/* Not sure how to determine pointer size on Windows running ARM. */
#  if _WIN64
#    define _P64
#  else
#    define _P32
#  endif
#elif __SIZEOF_POINTER__ == 8
#    define _P64
#elif __SIZEOF_POINTER__ == 4
#    define _P32
#else
#  error pointer size not known for your platform / compiler
#endif