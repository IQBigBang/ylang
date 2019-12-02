#include "Visitor.h"
#include <memory>
#include <typeinfo>

Value *Visitor::visit(ParseNode *n)
{
    if (dynamic_cast<FuncDefNode*>(n))
        return visitFuncDef(dynamic_cast<FuncDefNode *>(n));
    if (dynamic_cast<ExternFuncDefNode*>(n))
        return visitExternFuncDef(dynamic_cast<ExternFuncDefNode *>(n));
    if (dynamic_cast<LetInNode*>(n))
        return visitLetIn(dynamic_cast<LetInNode *>(n));
    if (dynamic_cast<SwitchNode*>(n))
        return visitSwitch(dynamic_cast<SwitchNode *>(n));
    if (dynamic_cast<IfNode*>(n))
        return visitIf(dynamic_cast<IfNode *>(n));
    if (dynamic_cast<BinOpNode*>(n))
        return visitBinOp(dynamic_cast<BinOpNode *>(n));
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
    std::cerr << "Invalid node type: " << typeid(n).name() << std::endl;
}

Type* Visitor::getTypeFromStr(std::string str)
{
    if (str == "Num")
        return Type::getDoubleTy(TheContext);
    else if (str == "Bool")
        return Type::getInt1Ty(TheContext);
    else if (str == "Void")
        return Type::getVoidTy(TheContext);
    else if (str == "Str")
        return TheModule->getTypeByName("Str")->getPointerTo();
    else {
        LogErrorV("Invalid type"); 
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


Visitor::Visitor() : Builder(TheContext)
{
    TheModule = std::make_unique<Module>("ylang", TheContext);

    PassBuilder PB;
    PB.registerFunctionAnalyses(TheAnalysisManager);
    ThePassManager = 
        PB.buildFunctionSimplificationPipeline(PassBuilder::OptimizationLevel::O2, PassBuilder::ThinLTOPhase::None);

    // stdlib integration
        
    auto Str = StructType::create(TheContext, {Type::getInt8PtrTy(TheContext), Type::getInt32Ty(TheContext)}, "Str");

    // Void print(Num n)
    addSTLFunction(Type::getVoidTy(TheContext), {Type::getDoubleTy(TheContext)}, "_Wprint_n");
    // Void print(Str s)
    addSTLFunction(Type::getVoidTy(TheContext),
        { Str->getPointerTo() }, "_Wprint_Str");
    
    // internal STL
    // void Strfc(struct Str* s, char* const_char, int len)
    addSTLFunction(Type::getVoidTy(TheContext),
        { Str->getPointerTo(), Type::getInt8PtrTy(TheContext), Type::getInt32Ty(TheContext)}, "Strfc");
    // void Strcc(struct Str* s1, struct Str* s2, struct Str* dest)
    addSTLFunction(Type::getVoidTy(TheContext),
        { Str->getPointerTo(), Str->getPointerTo(), Str->getPointerTo()}, "Strcc");
}

void Visitor::addSTLFunction(Type* retType, ArrayRef<Type*> argsType, Twine name)
{
    Function::Create(
        FunctionType::get(retType, argsType, false),
        Function::ExternalLinkage, name, TheModule.get()
    );
}

void Visitor::prepareEmit()
{
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
    Machine = Target->createTargetMachine(TargetTriple, CPU, Features, opt, RM);

    TheModule->setDataLayout(Machine->createDataLayout());
    TheModule->setTargetTriple(TargetTriple);
}

void Visitor::Emit(std::string filename)
{
    std::error_code EC;
    raw_fd_ostream dest(filename, EC, sys::fs::OpenFlags::F_None);
    if (EC)
    {
        std::cerr << "Could not open output file: " << EC.message();
        exit(1);
    }
    // TODO: Update to non-legacy
    legacy::PassManager pass;
    auto FileType = TargetMachine::CGFT_ObjectFile;

    if (Machine->addPassesToEmitFile(pass, dest, FileType))
    {
        std::cerr << "Error emitting object file" << std::endl;
        exit(1);
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
            LogErrorV("Invalid type");
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
            LogErrorV("Invalid type");
        
    }
    return mangled;
}

Value* Visitor::visitExternFuncDef(ExternFuncDefNode *context)
{
    //std::cout << "Visiting extern function definition" << std::endl;   
    std::vector<Type*> ArgTypes;
    for (auto a : context->args)
        ArgTypes.push_back(getTypeFromStr(a.first));

    FunctionType* FT
        = FunctionType::get(getTypeFromStr(context->rettype), ArgTypes, false);
    
    Function* F = 
        Function::Create(FT, Function::ExternalLinkage, mangleFuncName(context->fname, ArgTypes), TheModule.get());

    return nullptr;
 
}

Value* Visitor::visitFuncDef(FuncDefNode *context)
{
    //std::cout << "Visiting function definition" << std::endl;
    std::vector<Type*> ArgTypes;
    for (auto a : context->args)
        ArgTypes.push_back(getTypeFromStr(a.first));

    FunctionType* FT
        = FunctionType::get(getTypeFromStr(context->rettype), ArgTypes, false);
        
    Function* F = 
        Function::Create(FT, Function::ExternalLinkage, mangleFuncName(context->fname, ArgTypes), TheModule.get());

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
            return LogErrorV("Non-matching type returned");
        Builder.CreateRetVoid();
    } else
    {
        if (v->getType() != F->getReturnType())
            return LogErrorV("Non-matching type returned");
        Builder.CreateRet(v);
    }

    if (verifyFunction(*F, &errs()))
    {
        return LogErrorV("Error generating function body");
    }
    ThePassManager.run(*F, TheAnalysisManager);
    return nullptr;
}

Value* Visitor::visitSwitch(SwitchNode *context)
{
    // std::cout << "Visiting switchexpr" << std::endl;
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
            return LogErrorV("Invalid types");
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
    // std::cout << "Visiting ifstmt" << std::endl;
    Value* cond = visit(context->cond);
    if (!cond->getType()->isIntegerTy(1))
        return LogErrorV("If statement requires a boolean");
    
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
        return LogErrorV("If expression branches must return the same type");

    currF->getBasicBlockList().push_back(endBlock);
    Builder.SetInsertPoint(endBlock);
    PHINode* phi = Builder.CreatePHI(thenVal->getType(), 2);
    phi->addIncoming(thenVal, thenBlock);
    phi->addIncoming(elseVal, elseBlock);

    return (Value*)phi;
}

Value* Visitor::visitLetIn(LetInNode *context) {
    //std::cout << "Visiting letin" << std::endl;
    Value* V = visit(context->val);
    if (!V)
        return nullptr;

    if (NamedValues.count(context->name) != 0)
        return LogErrorV("Duplicite let-in defintion");
    
    AllocaInst* all = putAllocaInst(V->getType(), context->name);
    NamedValues[context->name] = all;
    Builder.CreateStore(V, all);
    return visit(context->in_expr);
}

Value* Visitor::visitCall(CallNode *context)
{
    //std::cout << "Visiting callexpr" << std::endl;
    
    std::vector<Value*> Args;
    std::vector<Type*> ArgTypes;
    for (auto& a : context->args)
    {
        Args.push_back(visit(a));
        ArgTypes.push_back(Args.back()->getType());
        if (!Args.back())
            return LogErrorV("Invalid argument");
    }

    // We don't need to check types, the mangling algorithm does that
    Function* F = TheModule->getFunction(mangleFuncName(context->fname, ArgTypes));
    if (!F)
        return LogErrorV("Undefined function. Check function name spelling and correct argument types");

    return (Value*)Builder.CreateCall(F, Args);
}

Value* Visitor::visitNumber(NumberNode *context)
{
    //std::cout << "Visiting number " << d << std::endl;
    return (Value*)ConstantFP::get(TheContext, APFloat(context->val));
}

Value* Visitor::visitBool(BoolNode *context)
{
    //std::cout << "Visiting bool" << std::endl;
    return (Value*)ConstantInt::get(TheContext, APInt(1, context->val, false));
    
}

Value* Visitor::visitMemberAccess(MemberAccessNode *context) {
    //std::cout << "Visiting memberaccess" << std::endl;
    return nullptr;
}

Value* Visitor::visitBinOp(BinOpNode *context)
{
    //std::cout << "Visiting infixexpr" << std::endl;
    Value* left = visit(context->lhs);
    auto leftTy = left->getType();
    Value* right = visit(context->rhs);
    auto rightTy = right->getType();
    if (!left || !right)
        return LogErrorV("Invalid infix operand");
    
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
        
        return LogErrorV("Invalid infix operation for types num and num");
    } else if (leftTy->isIntegerTy(1) && rightTy->isIntegerTy(1))
    {
        if (op == "==")
            return (Value*)Builder.CreateICmpEQ(left, right);
        else if (op == "!=")
            return (Value*)Builder.CreateICmpNE(left, right);
        
        return LogErrorV("Invalid infix operation for types bool and bool");
    } else if (leftTy->isPointerTy() && leftTy->getContainedType(0)->isStructTy()
            && rightTy->isPointerTy() && rightTy->getContainedType(0)->isStructTy())
    {
        if (leftTy->getContainedType(0)->getStructName() == "Str"
            && rightTy->getContainedType(0)->getStructName() == "Str"
            && op == "+")
        {
            Value* newstr = (Value*)putAllocaInst(TheModule->getTypeByName("Str"), "");
            Builder.CreateCall(TheModule->getFunction("Strcc"), {left, right, newstr});
            return newstr;
        }
    }

    return LogErrorV("Invalid infix operation operands");
}

Value* Visitor::visitString(StringNode *context) {
    // std::cout << "Visiting string" << std::endl;

    Value* strobj = putAllocaInst(TheModule->getTypeByName("Str"), ""); // create Str object
    Value* globstr = Builder.CreateGlobalStringPtr(context->val, "conststr"); // create string constant

    Builder.CreateCall(TheModule->getFunction("Strfc"), {strobj, globstr, 
        ConstantInt::get(Type::getInt32Ty(TheContext), APInt(32, context->val.size(), true))
    });

    return (Value*)strobj;
}

Value* Visitor::visitVariable(VariableNode *context) {
    if (NamedValues.count(context->var) == 0)
        return LogErrorV("Undefined variable");

    return (Value*)Builder.CreateLoad(NamedValues[context->var], context->var);
}
