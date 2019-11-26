#pragma once

#include "antlr4-runtime.h"
#include "YlangParser.h"
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

using namespace llvm;

enum YType {
    TNUMBER,
    TBOOLEAN
};


/**
 * This class defines an abstract visitor for a parse tree
 * produced by YlangParser.
 */
class  YlangVisitor : public antlr4::tree::AbstractParseTreeVisitor {
public:

    YlangVisitor();

    void prepareEmit();

   /**
   * Visit parse trees produced by YlangParser.
   */
    antlrcpp::Any visitCode(YlangParser::CodeContext *context);

    antlrcpp::Any visitExternFuncDef(YlangParser::ExternFuncDefContext *context);

    antlrcpp::Any visitFuncDef(YlangParser::FuncDefContext *context);

    antlrcpp::Any visitVarAssign(YlangParser::VarAssignContext *context);

    antlrcpp::Any visitRetExpr(YlangParser::RetExprContext *context);

    antlrcpp::Any visitDefinLine(YlangParser::DefinLineContext *context);

    antlrcpp::Any visitStmtLine(YlangParser::StmtLineContext *context);

    antlrcpp::Any visitExprExpr(YlangParser::ExprExprContext *context);

    antlrcpp::Any visitAtomExpr(YlangParser::AtomExprContext *context);

    antlrcpp::Any visitAtomAtom(YlangParser::AtomAtomContext *context);

    antlrcpp::Any visitCallExpr(YlangParser::CallExprContext *context);

    antlrcpp::Any visitNumber(YlangParser::NumberContext *context);

    antlrcpp::Any visitMemberAccess(YlangParser::MemberAccessContext *context);

    antlrcpp::Any visitInfixExpr(YlangParser::InfixExprContext *context);

    antlrcpp::Any visitString(YlangParser::StringContext *context);

    antlrcpp::Any visitBool(YlangParser::BoolContext *context);

    antlrcpp::Any visitVariable(YlangParser::VariableContext *context);

    antlrcpp::Any visitParenExpr(YlangParser::ParenExprContext *context);

    Type* getTypeFromStr(std::string str);

    Type* getTypeFromEnum(YType yt);

    Value* LogErrorV(const char* str)
    {
        std::cerr << str << std::endl;
        return nullptr;
    };

    void print()
    {
        TheModule->print(errs(), nullptr, false, true);
    };

    void Emit(std::string filename);

    std::string mangleFuncName(std::string fname, std::vector<Type*> args);

    // Put alloca instruction at the top of a function and return it
    AllocaInst* putAllocaInst(Function *F, std::string Name);

private:
    LLVMContext TheContext;
    FunctionPassManager ThePassManager;
    FunctionAnalysisManager TheAnalysisManager;
    IRBuilder<> Builder;
    std::unique_ptr<Module> TheModule;
    std::map<std::string, std::pair<AllocaInst*, Type*>> NamedValues;
    TargetMachine* Machine;
    Type* lastValType;
};