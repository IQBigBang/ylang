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
#include "llvm/Transforms/IPO.h"
#include "llvm/Transforms/IPO/PassManagerBuilder.h"
#include "llvm/Transforms/IPO/Inliner.h"
#include "llvm/Analysis/InlineCost.h"
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

// The STDLIBDIR macro should be passed as an argument (see the Makefile)
// It ensures that the location of the standard library is always known
// no matter where you call the compiler
#ifndef STDLIBDIR
#error Standard Library Directory is not defined!
#define STDLIBDIR "" // this is just to soothe the IDE and stop it from reporting errors
#endif

using namespace llvm;


/**
 * This class defines an abstract visitor for a parse tree
 * produced by the parser
 */
class  Visitor {
public:

    Visitor();

    ~Visitor() {
        TheModule.release();
        Machine.release();
    }
    
    void optimize(int optLevel, int sizeLevel, bool inlinerOn);

    void addSTLFunction(Type* retType, ArrayRef<Type*> argsType, Twine name);

    // Visit a node of unknown type
    Value* visit(ParseNode* n);
    // Visit a node on the left side of an assignment
    Value* visit_lval(ParseNode* n);
   /**
   * Visit parse trees produced by Parser.
   */
    Value* visitInclude(IncludeNode* context);
    Value* visitTypeDef(TypeDefNode* context);
    Value* visitFuncDef(FuncDefNode* context);
    Value* visitBlock(BlockNode* context);
    Value* visitLet(LetNode* context);
    Value* visitAssign(AssignNode* context);
    Value* visitSwitch(SwitchNode* context);
    Value* visitIf(IfNode* context);
    Value* visitWhile(WhileNode* context);
    Value* visitBinOp(BinOpNode* context);
    Value* visitUnOp(UnaryOpNode* context);
    Value* visitMemberAccess(MemberAccessNode* context, bool lval); // This node can be used on the left side of assignment (e.g. a->b = 2)
    Value* visitBool(BoolNode* context);
    Value* visitNumber(NumberNode* context);
    Value* visitString(StringNode* context);
    Value* visitVariable(VariableNode* context, bool lval); // This node can be used on the left side of assignment (e.g. c = 3)
    Value* visitCall(CallNode* context);

    // line number for debugging purposes
    Type* getTypeFromStr(int lineno, std::string str);

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
    std::vector<std::string> IncludedModules;
    std::unique_ptr<TargetMachine> Machine;
};