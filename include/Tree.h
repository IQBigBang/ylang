#ifndef TREE_H_
#define TREE_H_

#include <vector>
#include <string>
#include <iostream>

struct ParseNode
{
    virtual ~ParseNode() {}
    virtual void print() = 0;
};

struct IncludeNode : ParseNode
{
    int lineno;
    std::string modulename;
    IncludeNode(int lineno, std::string modulename) : lineno(lineno), modulename(modulename) {}
    virtual void print()
    {
        std::cerr << "Include{module=" << modulename << "}";
    }
};

struct TypeDefNode : ParseNode
{
    int lineno;
    std::string name;
    std::vector<std::pair<std::string, std::string>> members;
    TypeDefNode(int lineno, std::string name, std::vector<std::pair<std::string, std::string>> members)
        : lineno(lineno), name(name), members(members) {}
    virtual void print()
    {
        std::cerr << "Typedef{name=" << name << ", members=[";
        for (auto p : members)
            std::cerr << "(" << p.second << ", type=" << p.first << ")";
        std::cerr << "]}";
    }
};

struct FuncDefNode : ParseNode
{
    int lineno;
    std::string rettype;
    std::string fname;
    std::vector<std::pair<std::string, std::string>> args;
    ParseNode* body;
    FuncDefNode(int lineno, std::string rettype, std::string fname, std::vector<std::pair<std::string, std::string>> args, ParseNode* body)
        : lineno(lineno), rettype(rettype), fname(fname), args(args), body(body) {}
    virtual void print()
    {
        std::cerr << "FuncDef{ret=" << rettype << ", name=" << fname << ", args=[";
        for (auto i = args.begin(); i != args.end(); ++i)
            std::cerr << "(" << i->first << "," << i->second << ")";
        std::cerr << "], body=";
        body->print();
        std::cerr << "}";
    }
};

struct ExternFuncDefNode : ParseNode
{
    int lineno;
    std::string rettype;
    std::string fname;
    std::vector<std::pair<std::string, std::string>> args;
    ExternFuncDefNode(int lineno, std::string rettype, std::string fname, std::vector<std::pair<std::string, std::string>> args)
        : lineno(lineno), rettype(rettype), fname(fname), args(args) {}  

    virtual void print()
    {
        std::cerr << "ExternFuncDef{ret=" << rettype << ", name=" << fname << ", args=[";
        for (auto i = args.begin(); i != args.end(); ++i)
            std::cerr << "(" << i->first << "," << i->second << ")";
        std::cerr << "]}";
    }  
};

struct BlockNode : ParseNode
{
    int lineno;
    std::vector<ParseNode*> exprs;
    BlockNode(int lineno, std::vector<ParseNode*> exprs) : lineno(lineno), exprs(exprs) {}

    virtual void print()
    {
        std::cerr << "{";
        for (auto n : exprs)
            n->print();
        std::cerr << "}";
    }
};

struct LetNode : ParseNode
{
    int lineno;
    std::string name;
    ParseNode* val;
    LetNode(int lineno, std::string name, ParseNode* val) : lineno(lineno), name(name), val(val) {}

    virtual void print()
    {
        std::cerr << "LetNode{name=" << name << ", val=";
        val->print();
        std::cerr << "}";
    }
};

struct SwitchNode : ParseNode
{
    int lineno;
    ParseNode* lhs;
    std::vector<std::pair<ParseNode*, ParseNode*>> cases;
    ParseNode* elsecase;
    SwitchNode(int lineno, ParseNode* lhs, std::vector<std::pair<ParseNode*, ParseNode*>> cases, ParseNode* elsecase) 
        : lineno(lineno), lhs(lhs), cases(cases), elsecase(elsecase) {}
    
    virtual void print()
    {
        std::cerr << "SwitchNode{lhs=";
        lhs->print();
        std::cerr << ", cases=[";
        for (auto i = cases.begin(); i != cases.end(); ++i)
        {
            std::cerr << "(";
            i->first->print(); std::cerr << ", ";
            i->second->print(); std::cerr << ")";
        }
        std::cerr << "], else=";
        elsecase->print();
        std::cerr << "}";
    }
};

struct IfNode : ParseNode
{
    int lineno;
    ParseNode* cond;
    ParseNode* thenT;
    ParseNode* elseT;
    IfNode(int lineno, ParseNode* cond, ParseNode* thenT, ParseNode* elseT) : 
        lineno(lineno), cond(cond), thenT(thenT), elseT(elseT) {}
    
    virtual void print()
    {
        std::cerr << "IfNode{cond="; cond->print();
        std::cerr << ", then="; thenT->print();
        std::cerr << ", else="; elseT->print();
        std::cerr << "}";
    }

};

struct WhileNode : ParseNode
{
    int lineno;
    ParseNode* cond;
    ParseNode* body;
    WhileNode(int lineno, ParseNode* cond, ParseNode* body) : lineno(lineno), cond(cond), body(body) {}
    
    virtual void print()
    {
        std::cerr << "WhileNode{cond="; cond->print();
        std::cerr << ", body="; body->print();
        std::cerr << "}";
    }

};

struct BinOpNode : ParseNode
{
    int lineno;
    ParseNode* lhs;
    std::string op;
    ParseNode* rhs;
    BinOpNode(int lineno, ParseNode* lhs, std::string op, ParseNode* rhs) : lineno(lineno), lhs(lhs), op(op), rhs(rhs) {}

    virtual void print()
    {
        std::cerr << "BinOp{lhs="; lhs->print();
        std::cerr << ", op=" << op << ", rhs="; rhs->print();
        std::cerr << "}";
    }
};

struct UnaryOpNode : ParseNode
{
    int lineno;
    std::string op;
    ParseNode* inner;
    UnaryOpNode(int lineno, std::string op, ParseNode* inner) : lineno(lineno), op(op), inner(inner) {}

    virtual void print()
    {
        std::cerr << "UnOp{op=" << op << ", in="; 
        inner->print();
        std::cerr << "}";
    }
};

struct MemberAccessNode : ParseNode
{
    int lineno;
    ParseNode* obj;
    std::string member;
    MemberAccessNode(int lineno, ParseNode* obj, std::string member) : lineno(lineno), obj(obj), member(member) {}

    virtual void print()
    {
        std::cerr << "Member{obj="; obj->print();
        std::cerr << ", member=" << member << "}";
    }
};

struct BoolNode : ParseNode
{
    bool val;
    BoolNode(bool val) : val(val) {}

    virtual void print()
    {
        std::cerr << "Bool{" << val << "}";
    }
};

struct NumberNode : ParseNode
{
    double val;
    NumberNode(double val) : val(val) {}

    virtual void print()
    {
        std::cerr << "Number{" << val << "}";
    }
};

struct StringNode : ParseNode
{
    std::string val;
    StringNode(std::string val) : val(val) {}

    virtual void print()
    {
        std::cerr << "String{" << val << "}";
    }
};

struct VariableNode : ParseNode
{
    int lineno;
    std::string var;
    VariableNode(int lineno, std::string var) : lineno(lineno), var(var) {}

    virtual void print()
    {
        std::cerr << "Variable{" << var << "}";
    }
};

struct CallNode : ParseNode
{
    int lineno;
    std::string fname;
    std::vector<ParseNode*> args;
    CallNode(int lineno, std::string fname, std::vector<ParseNode*> args) : lineno(lineno), fname(fname), args(args) {}

    void print()
    {
        std::cerr << "CallNode{fname=" << fname << ", args=[";
        for (auto i = args.begin(); i != args.end(); ++i)
        {
            (*i)->print();
            std::cerr << ",";
        }
        std::cerr << "]}";
    }
};

#endif // TREE_H_