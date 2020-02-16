#include "Visitor.h"
#include "Lexer.h"
#include "Parser.h"
#include "Errors.h"
#include <memory>
#include <typeinfo>
#include <cctype>
#include <fstream>

Value *Visitor::visit(ParseNode *n)
{
    if (dynamic_cast<IncludeNode*>(n))
        return visitInclude(dynamic_cast<IncludeNode*>(n));
    if (dynamic_cast<TypeDefNode*>(n))
        return visitTypeDef(dynamic_cast<TypeDefNode*>(n));
    if (dynamic_cast<FuncDefNode*>(n))
        return visitFuncDef(dynamic_cast<FuncDefNode *>(n));
    if (dynamic_cast<BlockNode*>(n))
        return visitBlock(dynamic_cast<BlockNode*>(n));
    if (dynamic_cast<LetNode*>(n))
        return visitLet(dynamic_cast<LetNode*>(n));
    if (dynamic_cast<SwitchNode*>(n))
        return visitSwitch(dynamic_cast<SwitchNode *>(n));
    if (dynamic_cast<WhileNode*>(n))
        return visitWhile(dynamic_cast<WhileNode*>(n));
    if (dynamic_cast<IfNode*>(n))
        return visitIf(dynamic_cast<IfNode *>(n));
    if (dynamic_cast<BinOpNode*>(n))
        return visitBinOp(dynamic_cast<BinOpNode *>(n));
    if (dynamic_cast<UnaryOpNode*>(n))
        return visitUnOp(dynamic_cast<UnaryOpNode*>(n));
    if (dynamic_cast<MemberAccessNode*>(n))
        return visitMemberAccess(dynamic_cast<MemberAccessNode *>(n));
    if (dynamic_cast<BoolNode*>(n))
        return visitBool(dynamic_cast<BoolNode *>(n));
    if (dynamic_cast<NumberNode*>(n))
        return visitNumber(dynamic_cast<NumberNode *>(n));
    if (dynamic_cast<StringNode*>(n))
        return visitString(dynamic_cast<StringNode *>(n));
    if (dynamic_cast<VariableNode*>(n))
        return visitVariable(dynamic_cast<VariableNode *>(n));
    if (dynamic_cast<CallNode*>(n))
        return visitCall(dynamic_cast<CallNode *>(n));
    err::throwFatal("Internal error", 
                    "Invalid parse node type: " + std::string(typeid(n).name()));
    return nullptr;
}

Type* Visitor::getTypeFromStr(int lineno, std::string str)
{
    if (str == "Num")
        return Type::getDoubleTy(TheContext);
    else if (str == "Bool")
        return Type::getInt1Ty(TheContext);
    else if (str == "Void")
        return Type::getVoidTy(TheContext);
    else if (TheModule->getTypeByName(str))
        return TheModule->getTypeByName(str)->getPointerTo();
    else {
        err::throwNonfatal("Unrecognized type", "Found unrecognized type `" + str + "`", lineno);
        return Type::getVoidTy(TheContext);
    }
}

AllocaInst* Visitor::putAllocaInst(Type* T, std::string Name)
{
    IRBuilder<> tmpB(
        &(Builder.GetInsertBlock()->getParent()->getEntryBlock()), 
        Builder.GetInsertBlock()->getParent()->getEntryBlock().begin());
    return tmpB.CreateAlloca(T, 0, Name);
}

Constant* Visitor::getInt32Const(int val)
{
    return ConstantInt::get(Type::getInt32Ty(TheContext), APInt(32, val, false));
}

Visitor::Visitor() : Builder(TheContext), 
                        TheModule(std::make_unique<Module>("ylang", TheContext))
{
    // machine initialization
    InitializeAllTargetInfos();
    InitializeAllTargets();
    InitializeAllTargetMCs();
    InitializeAllAsmParsers();
    InitializeAllAsmPrinters();

    auto TargetTriple = sys::getDefaultTargetTriple();
    
    std::string error;
    auto Target = TargetRegistry::lookupTarget(TargetTriple, error);
    if (!Target)
    {
        std::cout << "Error initializing target: " << error << std::endl;
        exit(1);
    }

    auto CPU = "generic";
    auto Features = "";

    TargetOptions opt;
    auto RM = Optional<Reloc::Model>();
    Machine.reset(Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM));

    TheModule->setDataLayout(Machine->createDataLayout());
    TheModule->setTargetTriple(TargetTriple);

    // stdlib integration
        
    auto Str = StructType::create(TheContext, {Type::getInt8PtrTy(TheContext), Type::getInt32Ty(TheContext)}, "Str");

    // internal STL

    // void* alloc(int size)
    addSTLFunction(Type::getVoidTy(TheContext)->getPointerTo(), {Type::getInt32Ty(TheContext)}, "alloc");

    // struct Str* strfc(char* const_char, int len)
    addSTLFunction(Str->getPointerTo(),
        {Type::getInt8PtrTy(TheContext), Type::getInt32Ty(TheContext)}, "strfc");
}

void Visitor::addSTLFunction(Type* retType, ArrayRef<Type*> argsType, Twine name)
{
    Function::Create(
        FunctionType::get(retType, argsType, false),
        Function::ExternalLinkage, name, TheModule.get()
    );
}

void Visitor::optimize(int optLevel, int sizeLevel, bool inlinerOn)
{
    legacy::FunctionPassManager TheFunctionPassManager(TheModule.get());
    legacy::PassManager TheModulePassManager;
    PassManagerBuilder PMB;
    PMB.OptLevel = optLevel;
    PMB.SizeLevel = sizeLevel;
    if (inlinerOn) { 
        InlineParams ip = getInlineParams(optLevel, sizeLevel);
        PMB.Inliner = createFunctionInliningPass(ip);
    }
    Machine->adjustPassManager(PMB);
    
    PMB.populateFunctionPassManager(TheFunctionPassManager);
    PMB.populateModulePassManager(TheModulePassManager);
    TheFunctionPassManager.doInitialization();
    for (auto &f : *TheModule)
    {
        TheFunctionPassManager.run(f);
    }
    TheFunctionPassManager.doFinalization();
    TheModulePassManager.run(*TheModule);
}

void Visitor::Emit(std::string filename)
{
    std::error_code EC;
    raw_fd_ostream dest(filename, EC, sys::fs::OpenFlags::F_None);
    if (EC)
    {
        err::throwFatal("Could not open output file",
            EC.message());
    }
    // TODO: Update to non-legacy
    legacy::PassManager pass;
    auto FileType = TargetMachine::CGFT_ObjectFile;

    if (Machine->addPassesToEmitFile(pass, dest, nullptr, FileType))
    {
        err::throwFatal("Could not finish compilation",
            "Error emitting object file");
    }

    pass.run(*TheModule);
    dest.flush();

    //std::cout << "Object file succesfully written" << std::endl;
}

std::string Visitor::mangleFuncName(std::string fname, std::vector<Type*> args)
{
    std::string mangled = "_W" + fname;
    for (Type* at : args)
    {
        if (!at)
            err::throwNonfatal("Internal error", "Unrecognized function argument type `" + fname + "`");
        if (at->isDoubleTy())
            mangled += "_n";
        else if (at->isIntegerTy(1))
            mangled += "_b";
        else if (at->isPointerTy() && at->getContainedType(0)->isStructTy())
        {
            mangled += "_";
            mangled += at->getContainedType(0)->getStructName();
        }
        else
            err::throwNonfatal("Internal error", "Unrecognized function argument type `" + fname + "`");        
    }
    return mangled;
}

Value* Visitor::visitInclude(IncludeNode* context)
{
    if (std::find(IncludedModules.begin(), IncludedModules.end(), context->modulename) != IncludedModules.end())
    {
        // the module was already included, so ignore this statement
        return nullptr;   
    }
    std::string filename = context->modulename + ".yy";
    // first initialize the lexer for the included file
    std::ifstream included; 
    // search in current directory, subdirectory 'src' and the standard library directory
    if (std::ifstream(filename))
        filename = filename; // basically nop
    else if (std::ifstream("src/" + filename))
        filename = "src/" + filename;
    else if (std::ifstream(STDLIBDIR + filename))
        filename = STDLIBDIR + filename;
    else {
        err::throwNonfatal("Invalid include", "Cannot find to-be-included file `" + filename + "`", context->lineno);
        return nullptr;
    }
    // first, save the current file name and update it (for useful error reports)
    std::string currFileName = err::fileName;
    err::fileName = filename;
    IncludedModules.push_back(context->modulename); // add to the list of already included modules
    // now initialize lexer and parser
    included.open(filename);
    Lexer included_lexer(included);
    Parser included_parser(included_lexer);
    auto nodes = included_parser.parse(); // parse the included file
    for (auto& n : nodes)
        this->visit(n); // and compile it with this visitor

    err::fileName = currFileName; // restore the filename
    return nullptr;
}

Value* Visitor::visitFuncDef(FuncDefNode *context)
{
    std::vector<Type*> ArgTypes;
    for (auto a : context->args)
        ArgTypes.push_back(getTypeFromStr(context->lineno, a.first));

    FunctionType* FT
        = FunctionType::get(getTypeFromStr(context->lineno, context->rettype), ArgTypes, false);
        
    Function* F = 
        Function::Create(FT, Function::ExternalLinkage, 
            // check if the function is unmangled
            context->attrs.Unmangled ? context->fname : mangleFuncName(context->fname, ArgTypes), 
            TheModule.get());
    
    if (context->attrs.External) // if it is external, don't generate body
        return nullptr;

    // entry block
    BasicBlock* BB = BasicBlock::Create(TheContext, "entry", F);
    Builder.SetInsertPoint(BB);

    // args
    NamedValues.clear();
    int i = 0;
    for (auto &Arg : F->args())
    {
        Arg.setName(context->args[i].second);
        AllocaInst* all = putAllocaInst(ArgTypes[i], Arg.getName());
        Builder.CreateStore(&Arg, all);
        NamedValues[Arg.getName()] = all;
        ++i;
    }

    // body
    Value* v = visit(context->body);
    if (F->getReturnType()->isVoidTy())
    {
        if (v && !v->getType()->isVoidTy())
        {
            err::throwNonfatal("Non-matching type returned", "", context->lineno);
            return nullptr;
        }
        Builder.CreateRetVoid();
    } else
    {
        if (v->getType() != F->getReturnType())
        {
            err::throwNonfatal("Non-matching type returned in function", "", context->lineno); // TODO: correct line
            return nullptr;
        }
        Builder.CreateRet(v);
    }

    if (verifyFunction(*F, &errs()))
    {
        err::throwNonfatal("Error generating function body in function", "", context->lineno); // TODO: correct line
    }
    return nullptr;
}

Value* Visitor::visitTypeDef(TypeDefNode* context)
{
    if (context->name == "Num" || context->name == "Bool" || context->name == "Void" ||
        TheModule->getTypeByName(context->name))
    {
        err::throwNonfatal("Duplicite structure name", "", context->lineno);
        return nullptr;
    }
    std::vector<Type*> memberTypes;
    std::vector<std::string> memberNames;
    for (auto M : context->members)
    {
        memberNames.push_back(M.second);
        memberTypes.push_back(getTypeFromStr(context->lineno, M.first));
    }
    auto newS = StructType::create(TheContext, memberTypes, context->name, false);
    StructMembers[context->name] = memberNames;

    auto constructor = Function::Create(
        FunctionType::get(
            newS->getPointerTo(),
            memberTypes, 
            false
        ), Function::ExternalLinkage, mangleFuncName(context->name, memberTypes), TheModule.get()
    );
    BasicBlock* BB = BasicBlock::Create(TheContext, "entry", constructor);
    Builder.SetInsertPoint(BB);

    // first allocate place for the structure
    
    Value* allocated_object = (Value*)Builder.CreatePointerCast((Value*)Builder.CreateCall(TheModule->getFunction("alloc"), {
        getInt32Const(TheModule->getDataLayout().getTypeAllocSize(newS))
    }), newS->getPointerTo(), "obj");

    int i = 0;
    for (auto& a : constructor->args())
    {
        Value* member = (Value*)GetElementPtrInst::CreateInBounds(allocated_object, 
            {getInt32Const(0), getInt32Const(i)}, 
            memberNames[i], BB        
        );
        Builder.CreateStore(&a, member);
        ++i;
    }
    Builder.CreateRet(allocated_object);

    return nullptr;
}

Value* Visitor::visitBlock(BlockNode* context)
{
    if (context->exprs.size() == 0)  // empty block
    {
        return (Value*)nullptr; // Void
    }
    Value* V;
    for (auto e : context->exprs)
        V = visit(e);
    return V;
}

Value* Visitor::visitSwitch(SwitchNode *context)
{
    Value* lhs = visit(context->lhs);
    auto mainBlock = Builder.GetInsertBlock();
    std::vector<BasicBlock*> cases;
    std::vector<Value*> results;
    auto switchEndBlock = BasicBlock::Create(TheContext, "switchend", Builder.GetInsertBlock()->getParent());
    BasicBlock* currCase = mainBlock;
    for (auto& c : context->cases)
    { // first expr is the left hand side, last condition is the else expression
        Builder.SetInsertPoint(currCase); // insert to main block
        Value* cond = visit(c.first); // emit the condition (rhs)
        if (!lhs->getType()->isDoubleTy() || !cond->getType()->isDoubleTy())
        {
            std::cerr << "Invalid switch types"; // TODO (with operator overloading)
            return nullptr;
        }
        Value* cmp = Builder.CreateFCmpUEQ(lhs, cond);
        auto Bcase = BasicBlock::Create(TheContext, "case", Builder.GetInsertBlock()->getParent()); // make 'case' block
        auto Bncase = BasicBlock::Create(TheContext, "ncase", Builder.GetInsertBlock()->getParent()); // make 'not-case' block
        Builder.CreateCondBr(cmp, Bcase, Bncase);
        Builder.SetInsertPoint(Bcase);
        Value* res = visit(c.second); // emit the case (if condition is true)
        results.push_back(res); // save the result value (for phi node)
        Builder.CreateBr(switchEndBlock); // and jump to switchend
        Bcase = Builder.GetInsertBlock(); // in case code generation changed the block
        cases.push_back(Bcase); // save the block
        currCase = Bncase;
    }
    Builder.SetInsertPoint(currCase);
    Value* elseVal = visit(context->elsecase);
    Builder.CreateBr(switchEndBlock);
    currCase = Builder.GetInsertBlock();
    Builder.SetInsertPoint(switchEndBlock);
    PHINode* PN = Builder.CreatePHI(lhs->getType(), cases.size() + 1);
    for (int i = 0; i < cases.size(); i++)
        PN->addIncoming(results[i], cases[i]);
    PN->addIncoming(elseVal, currCase);
    return (Value*)PN;
}

Value* Visitor::visitIf(IfNode *context)
{
    Value* cond = visit(context->cond);
    if (!cond->getType()->isIntegerTy(1))
    {
        err::throwNonfatal("Type mismatch", "If condition must be of type boolean", context->lineno);
        return nullptr;
    }
    
    Function* currF = Builder.GetInsertBlock()->getParent();
    
    BasicBlock* thenBlock = BasicBlock::Create(TheContext, "then", currF);
    BasicBlock* elseBlock = BasicBlock::Create(TheContext, "else");
    BasicBlock* endBlock = BasicBlock::Create(TheContext, "ifend");

    Builder.CreateCondBr(cond, thenBlock, elseBlock);

    Builder.SetInsertPoint(thenBlock);
    Value* thenVal = visit(context->thenT);
    Builder.CreateBr(endBlock);
    thenBlock = Builder.GetInsertBlock(); // code may have changed the block

    currF->getBasicBlockList().push_back(elseBlock); // now emit the else block
    Builder.SetInsertPoint(elseBlock);
    Value* elseVal = visit(context->elseT);
    Builder.CreateBr(endBlock);
    elseBlock = Builder.GetInsertBlock();

    if (thenVal->getType() != elseVal->getType())
    {
        err::throwNonfatal("Type mismatch", "If branches must return the same type", context->lineno);
        return nullptr;
    }

    currF->getBasicBlockList().push_back(endBlock);
    Builder.SetInsertPoint(endBlock);

    if (thenVal->getType()->isVoidTy()) return (Value*)nullptr; // if the return type is void, phi would be invalid

    PHINode* phi = Builder.CreatePHI(thenVal->getType(), 2);
    phi->addIncoming(thenVal, thenBlock);
    phi->addIncoming(elseVal, elseBlock);

    return (Value*)phi;
}

Value* Visitor::visitWhile(WhileNode* context) {
    BasicBlock* cond = BasicBlock::Create(TheContext, "whilecond");
    BasicBlock* body = BasicBlock::Create(TheContext, "whilebody");
    BasicBlock* exit = BasicBlock::Create(TheContext, "whileexit");
    Function* currF = Builder.GetInsertBlock()->getParent();

    Builder.CreateBr(cond); // create a jump to the condition
    currF->getBasicBlockList().push_back(cond); // now emit the condition block
    Builder.SetInsertPoint(cond);
    Value* compiled_cond = visit(context->cond);
    if (!compiled_cond->getType()->isIntegerTy(1))
    {
        err::throwNonfatal("Type mismatch", "While condition must be of type boolean", context->lineno);
        return nullptr;
    }
    Builder.CreateCondBr(compiled_cond, body, exit); // the main loop - if cond = true, next loop else exit

    currF->getBasicBlockList().push_back(body); // now emit body
    Builder.SetInsertPoint(body);
    visit(context->body); // compile it
    Builder.CreateBr(cond); // return back to the condition

    currF->getBasicBlockList().push_back(exit);
    Builder.SetInsertPoint(exit);
    
    return (Value*)nullptr; // void
}

Value* Visitor::visitLet(LetNode *context) {
    Value* V = visit(context->val);
    if (!V)
        return nullptr;

    AllocaInst* all;
    if (NamedValues.count(context->name) == 0)
    {
        all = putAllocaInst(V->getType(), context->name);
        NamedValues[context->name] = all;
    } else all = NamedValues[context->name];

    Builder.CreateStore(V, all);
    return V;
}

Value *Visitor::visitCall(CallNode *context)
{
    std::vector<Value *> Args;
    std::vector<Type *> ArgTypes;

    for (auto &a : context->args)
    {
        Args.push_back(visit(a));
        ArgTypes.push_back(Args.back()->getType());
    }

    // We don't need to check types, the mangling algorithm does that
    Function *F = TheModule->getFunction(mangleFuncName(context->fname, ArgTypes));
    if (!F)
    {   // try if there is an unmangled version
        F = TheModule->getFunction(context->fname);
        if (!F) {
            // TODO print function prototype
            err::throwNonfatal("Undefined function or constructor", "", context->lineno);
            return nullptr;
        }
    }
    return (Value *)Builder.CreateCall(F, Args);
}

Value* Visitor::visitNumber(NumberNode *context)
{
    return (Value*)ConstantFP::get(TheContext, APFloat(context->val));
}

Value* Visitor::visitBool(BoolNode *context)
{
    return (Value*)ConstantInt::get(TheContext, APInt(1, context->val, false));
    
}

Value* Visitor::visitMemberAccess(MemberAccessNode *context) {
    Value* obj = visit(context->obj);
    if (!obj->getType()->isPointerTy() || !obj->getType()->getContainedType(0)->isStructTy()
                                || obj->getType()->getContainedType(0)->getStructName() == "Str")
    {
        err::throwNonfatal("Type mismatch", "Primitive types don't have members.", context->lineno);
        return nullptr;
    }
    auto& members = StructMembers[obj->getType()->getContainedType(0)->getStructName()];
    auto it = std::find(members.begin(), members.end(), context->member);
    if (it == members.end())
    {
        err::throwNonfatal("Undefined member", "", context->lineno);
        return nullptr;
    }
    int index = it - members.begin();
    auto member_ptr = GetElementPtrInst::CreateInBounds(obj, 
        {getInt32Const(0), getInt32Const(index)}, 
    "", Builder.GetInsertBlock());
    return Builder.CreateLoad(member_ptr, context->member);
}

Value* Visitor::visitUnOp(UnaryOpNode* context)
{
    Value* inner = visit(context->inner);
    if (inner->getType()->isDoubleTy())
    {
        if (context->op == "-")
            return (Value*)Builder.CreateFNeg(inner);
    } else if (inner->getType()->isIntegerTy(1))
    {
        if (context->op == "!")
            return (Value*)Builder.CreateNot(inner);
    }

    // look for operator overloads
    std::string funcName;
    if (context->op == "-") funcName = "neg";
    else if (context->op == "!") funcName = "not";

    Function* F = TheModule->getFunction(mangleFuncName(funcName, {inner->getType()}));

    if (F) {
        return (Value*)Builder.CreateCall(F, {inner});
    }
    err::throwNonfatal("Invalid unary operation", "", context->lineno);
    return nullptr;
}

Value* Visitor::visitBinOp(BinOpNode *context)
{
    Value* left = visit(context->lhs);
    auto leftTy = left->getType();
    Value* right = visit(context->rhs);
    auto rightTy = right->getType();
    
    std::string op = context->op;

    if (leftTy->isDoubleTy() && rightTy->isDoubleTy())
    {
        if (op == "+") 
            return (Value*)Builder.CreateFAdd(left, right);
        else if (op == "-")
            return (Value*)Builder.CreateFSub(left, right);
        else if (op == "*")
            return (Value*)Builder.CreateFMul(left, right);
        else if (op == "/")
            return (Value*)Builder.CreateFDiv(left, right);
        else if (op == "<")
            return (Value*)Builder.CreateFCmpULT(left, right);
        else if (op == ">")
            return (Value*)Builder.CreateFCmpUGT(left, right);
        else if (op == "<=")
            return (Value*)Builder.CreateFCmpULE(left, right);
        else if (op == ">=")
            return (Value*)Builder.CreateFCmpUGE(left, right);
        else if (op == "==")
            return (Value*)Builder.CreateFCmpUEQ(left, right);
        else if (op == "!=")
            return (Value*)Builder.CreateFCmpUNE(left, right);
    } else if (leftTy->isIntegerTy(1) && rightTy->isIntegerTy(1))
    {
        if (op == "==")
            return (Value*)Builder.CreateICmpEQ(left, right);
        else if (op == "!=")
            return (Value*)Builder.CreateICmpNE(left, right);
        else if (op == "and")
            return (Value*)Builder.CreateAnd(left, right);
        else if (op == "or")
            return (Value*)Builder.CreateOr(left, right);
    }

    // now look for operator-overloads
    std::string funcName;
    if (op == "+") funcName = "add";
    else if (op == "-") funcName = "sub";
    else if (op == "*") funcName = "mul";
    else if (op == "/") funcName = "div";
    else if (op == "^") funcName = "pow";
    else if (op == "==") funcName = "eql";
    else if (op == "!=") funcName = "neq";
    else if (op == "<") funcName = "lower";
    else if (op == "<=") funcName = "loweq";
    else if (op == ">") funcName = "greater";
    else if (op == ">=") funcName = "greateq";
    else if (op == "and") funcName = "andop";
    else if (op == "or") funcName = "orop";

    Function* F = TheModule->getFunction(mangleFuncName(funcName, {leftTy, rightTy}));

    if (F) {
        return (Value*)Builder.CreateCall(F, {left, right});
    }

    err::throwNonfatal("Invalid binary operation", "", context->lineno);
    return nullptr;
}

Value* Visitor::visitString(StringNode *context) {
    Value* globstr = Builder.CreateGlobalStringPtr(context->val, "conststr"); // create string constant

    return (Value*)Builder.CreateCall(TheModule->getFunction("strfc"), {globstr, 
        getInt32Const(context->val.size())
    });
}

Value* Visitor::visitVariable(VariableNode *context) {
    if (NamedValues.count(context->var) == 0)
    {
        err::throwNonfatal("Undefined variable `" + context->var + "`", "", context->lineno);
        return nullptr;
    }

    return (Value*)Builder.CreateLoad(NamedValues[context->var], context->var);
}
