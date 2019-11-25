#include "Visitor.h"
#include <memory>

Type* YlangVisitor::getTypeFromStr(std::string str)
{
    if (str == "num")
        return Type::getDoubleTy(TheContext);
    else if (str == "bool")
        return Type::getInt1Ty(TheContext);
    else if (str == "void")
        return Type::getVoidTy(TheContext);
    else {
        LogErrorV("Invalid type"); 
        return Type::getVoidTy(TheContext);
    }
}

YlangVisitor::YlangVisitor() : antlr4::tree::AbstractParseTreeVisitor(), Builder(TheContext)
{
    TheModule = std::make_unique<Module>("ylang", TheContext);
}

void YlangVisitor::prepareEmit()
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

void YlangVisitor::Emit(std::string filename)
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

    std::cout << "Object file succesfully written" << std::endl;
}

std::string YlangVisitor::mangleFuncName(std::string fname, std::vector<Type*> args)
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
        else
            LogErrorV("Invalid type");
        
    }
    return mangled;
}


antlrcpp::Any YlangVisitor::visitCode(YlangParser::CodeContext *context)
{
    std::cout << "Visiting code" << std::endl;
    return visitChildren(context);
}

antlrcpp::Any YlangVisitor::visitExternFuncDef(YlangParser::ExternFuncDefContext *context)
{
    std::cout << "Visiting extern function definition" << std::endl;   
    std::vector<Type*> ArgTypes;
    int i = -1;
    for (auto id : context->ID())
    {
        ++i;
        if (i == 0) continue; // function name
        if (i % 2 == 1) continue; // arg name or return type
        ArgTypes.push_back(getTypeFromStr(id->getText()));
    }

    FunctionType* FT;
    if (!context->rettype)
        FT = FunctionType::get(Type::getVoidTy(TheContext), ArgTypes, false);
    else 
        FT = FunctionType::get(getTypeFromStr(context->rettype->getText()), ArgTypes, false);
    
    Function* F = 
        Function::Create(FT, Function::ExternalLinkage, mangleFuncName(context->fname->getText(), ArgTypes), TheModule.get());

    // args
    NamedValues.clear();
    i = 1;
    for (auto &Arg : F->args())
    {
        Arg.setName(context->ID()[i]->getText());
        NamedValues[Arg.getName()] = &Arg;
        i += 2;
        if (context->rettype && i == context->ID().size() - 1) break; // last id is the return type but only if it's defined
    }
    return nullptr;
 
}

antlrcpp::Any YlangVisitor::visitFuncDef(YlangParser::FuncDefContext *context)
{
    std::cout << "Visiting function definition" << std::endl;
    std::vector<Type*> ArgTypes;
    int i = -1;
    for (auto id : context->ID())
    {
        ++i;
        if (i == 0) continue; // function name
        if (i % 2 == 1) continue; // arg name or return type
        ArgTypes.push_back(getTypeFromStr(id->getText()));
    }

    FunctionType* FT;
    if (!context->rettype)
        FT = FunctionType::get(Type::getVoidTy(TheContext), ArgTypes, false);
    else 
        FT = FunctionType::get(getTypeFromStr(context->rettype->getText()), ArgTypes, false);
        
    Function* F = 
        Function::Create(FT, Function::ExternalLinkage, mangleFuncName(context->fname->getText(), ArgTypes), TheModule.get());

    // args
    NamedValues.clear();
    i = 1;
    for (auto &Arg : F->args())
    {
        Arg.setName(context->ID()[i]->getText());
        NamedValues[Arg.getName()] = &Arg;
        i += 2;
        if (context->rettype && i == context->ID().size() - 1) break; // last id is the return type but only if it's defined
    }

    // body
    BasicBlock* BB = BasicBlock::Create(TheContext, "entry", F);
    Builder.SetInsertPoint(BB);
    if (Value* ret = visit(context->body))
    {
        if (F->getReturnType()->isVoidTy())
            Builder.CreateRetVoid();
        else 
            Builder.CreateRet(ret);
        verifyFunction(*F);
    } else
    {
        return LogErrorV("Error generating function body");
    }
    return nullptr;
}

antlrcpp::Any YlangVisitor::visitVarAssign(YlangParser::VarAssignContext *context) {
    std::cout << "Visiting varassign" << std::endl;
    return nullptr;
}

antlrcpp::Any YlangVisitor::visitExprExpr(YlangParser::ExprExprContext *context)
{
    std::cout << "Visiting exprexpr" << std::endl;
    return visit(context->e);
}

antlrcpp::Any YlangVisitor::visitAtomExpr(YlangParser::AtomExprContext *context)
{
    std::cout << "Visiting atomexpr" << std::endl;
    return visit(context->a);
}

antlrcpp::Any YlangVisitor::visitAtomAtom(YlangParser::AtomAtomContext *context)
{
    std::cout << "Visiting atomatom" << std::endl;
    return visit(context->a);
}

antlrcpp::Any YlangVisitor::visitCallExpr(YlangParser::CallExprContext *context)
{
    std::cout << "Visiting callexpr" << std::endl;
    
    std::vector<Value*> Args;
    std::vector<Type*> ArgTypes;
    for (unsigned int i = 0; i < context->atom().size(); i++)
    {
        Args.push_back(visit(context->atom()[i]));
        ArgTypes.push_back(lastValType);
        if (!Args.back())
            return LogErrorV("Invalid argument");
    }

    // We don't need to check types, the mangling algorithm does that
    Function* F = TheModule->getFunction(mangleFuncName(context->fname->getText(), ArgTypes));
    if (!F)
        return LogErrorV("Undefined function. Check function name spelling and correct argument types");

    return (Value*)Builder.CreateCall(F, Args);
}

antlrcpp::Any YlangVisitor::visitNumber(YlangParser::NumberContext *context)
{
    double d = std::stod(context->getText());
    std::cout << "Visiting number " << d << std::endl;
    lastValType = Type::getDoubleTy(TheContext);
    return (Value*)ConstantFP::get(TheContext, APFloat(d));
}

antlrcpp::Any YlangVisitor::visitBool(YlangParser::BoolContext *context)
{
    std::cout << "Visiting bool" << std::endl;
    lastValType = Type::getInt1Ty(TheContext);
    if (context->getText() == "true")
        return (Value*)ConstantInt::get(TheContext, APInt(1, 1, false));
    else if (context->getText() == "false")
        return (Value*)ConstantInt::get(TheContext, APInt(1, 1, false));
    else return LogErrorV("Invalid boolean value");
    
}

antlrcpp::Any YlangVisitor::visitMemberAccess(YlangParser::MemberAccessContext *context) {
    std::cout << "Visiting memberaccess" << std::endl;
    return nullptr;
}

antlrcpp::Any YlangVisitor::visitInfixExpr(YlangParser::InfixExprContext *context)
{
    std::cout << "Visiting infixexpr" << std::endl;
    Value* left = visit(context->lhs).as<Value*>();
    auto leftTy = lastValType;
    Value* right = visit(context->rhs).as<Value*>();
    auto rightTy = lastValType;
    if (!left || !right)
        return LogErrorV("Invalid infix operand");
    
    std::string op = context->op->getText();

    if (leftTy->isDoubleTy() && rightTy->isDoubleTy())
    {
        lastValType = Type::getDoubleTy(TheContext);
        if (op == "+") 
            return (Value*)Builder.CreateFAdd(left, right);
        else if (op == "-")
            return (Value*)Builder.CreateFSub(left, right);
        else if (op == "*")
            return (Value*)Builder.CreateFMul(left, right);
        else if (op == "/")
            return (Value*)Builder.CreateFDiv(left, right);
        
        lastValType = Type::getInt1Ty(TheContext);
        if (op == "<")
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
        lastValType = Type::getInt1Ty(TheContext);
        if (op == "==")
            return (Value*)Builder.CreateICmpEQ(left, right);
        else if (op == "!=")
            return (Value*)Builder.CreateICmpNE(left, right);
        
        return LogErrorV("Invalid infix operation for types bool and bool");
    }

    return LogErrorV("Invalid infix operation operands");
}

antlrcpp::Any YlangVisitor::visitString(YlangParser::StringContext *context) {
    std::cout << "Visiting string" << std::endl;
    return nullptr;
}

antlrcpp::Any YlangVisitor::visitVariable(YlangParser::VariableContext *context) {
    Value* V = NamedValues[context->name->getText()];
    lastValType = V->getType();
    if (!V)
        return LogErrorV("Undefined variable");
    return V;
}

antlrcpp::Any YlangVisitor::visitParenExpr(YlangParser::ParenExprContext *context) {
    std::cout << "Visiting parenexpr" << std::endl;
    return visit(context->e);
}
