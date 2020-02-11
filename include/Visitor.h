#pragma once

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/Optional.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/LegacyPassManager.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Support/FileSystem.h"
#include "llvm/Support/Host.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Support/TargetRegistry.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <memory>
#include <string>
#include <system_error>
#include <utility>
#include <vector>

#include "Tree.h"

using namespace llvm;


/**
 * This class defines an abstract visitor for a parse tree
 * produced by YlangParser.
 */
class  Visitor {
public:

    Visitor();
    
    void optimize(bool doFullOptimizations);

    void addSTLFunction(Type* retType, ArrayRef<Type*> argsType, Twine name);

    // Visit a node of unknown type
    Value* visit(ParseNode* n);
   /**
   * Visit parse trees produced by Parser.
   */
    Value* visitTypeDef(TypeDefNode* context);
    Value* visitFuncDef(FuncDefNode* context);
    Value* visitExternFuncDef(ExternFuncDefNode* context);
    Value* visitBlock(BlockNode* context);
    Value* visitLet(LetNode* context);
    Value* visitSwitch(SwitchNode* context);
    Value* visitIf(IfNode* context);
    Value* visitBinOp(BinOpNode* context);
    Value* visitMemberAccess(MemberAccessNode* context);
    Value* visitBool(BoolNode* context);
    Value* visitNumber(NumberNode* context);
    Value* visitString(StringNode* context);
    Value* visitVariable(VariableNode* context);
    Value* visitCall(CallNode* context);

    Type* getTypeFromStr(std::string str);

    Value* LogErrorV(const char* str)
    {
        std::cerr << str << std::endl;
        return nullptr;
    };

    void print(std::string outFile)
    {
        std::error_code EC;
        raw_fd_ostream out((StringRef)outFile, EC, sys::fs::OpenFlags::F_None);
        TheModule->print(out, nullptr, false, true);
    };

    void Emit(std::string filename);

    std::string mangleFuncName(std::string fname, std::vector<Type*> args);

    // Put alloca instruction at the top of a function and return it
    AllocaInst* putAllocaInst(Type *T, std::string Name);

    Constant* getInt32Const(int val);

private:
    LLVMContext TheContext;
    IRBuilder<> Builder;
    std::unique_ptr<Module> TheModule;
    std::map<std::string, AllocaInst*> NamedValues;
    std::map<std::string, std::vector<std::string>> StructMembers;
    TargetMachine* Machine;
};