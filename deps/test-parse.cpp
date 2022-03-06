    #define __STDC_LIMIT_MACROS
    #define __STDC_CONSTANT_MACROS
    // Need to use TentativeParsingAction which is private
    #define private public
    #include "clang/Parse/Parser.h"
    #undef private
    #include "clang/Frontend/CompilerInstance.h"
