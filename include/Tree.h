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

struct TypeDefNode : ParseNode
{
    std::string name;
    std::vector<std::pair<std::string, std::string>> members;
    TypeDefNode(std::string name, std::vector<std::pair<std::string, std::string>> members)
        : name(name), members(members) {}
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
    std::string rettype;
    std::string fname;
    std::vector<std::pair<std::string, std::string>> args;
    ParseNode* body;
    FuncDefNode(std::string rettype, std::string fname, std::vector<std::pair<std::string, std::string>> args, ParseNode* body)
        : rettype(rettype), fname(fname), args(args), body(body) {}
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
    std::string rettype;
    std::string fname;
    std::vector<std::pair<std::string, std::string>> args;
    ExternFuncDefNode(std::string rettype, std::string fname, std::vector<std::pair<std::string, std::string>> args)
        : rettype(rettype), fname(fname), args(args) {}  

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
    std::vector<ParseNode*> exprs;
    BlockNode(std::vector<ParseNode*> exprs) : exprs(exprs) {}

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
    std::string name;
    ParseNode* val;
    LetNode(std::string name, ParseNode* val) : name(name), val(val) {}

    virtual void print()
    {
        std::cerr << "LetNode{name=" << name << ", val=";
        val->print();
        std::cerr << "}";
    }
};

struct SwitchNode : ParseNode
{
    ParseNode* lhs;
    std::vector<std::pair<ParseNode*, ParseNode*>> cases;
    ParseNode* elsecase;
    SwitchNode(ParseNode* lhs, std::vector<std::pair<ParseNode*, ParseNode*>> cases, ParseNode* elsecase) 
        : lhs(lhs), cases(cases), elsecase(elsecase) {}
    
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
    ParseNode* cond;
    ParseNode* thenT;
    ParseNode* elseT;
    IfNode(ParseNode* cond, ParseNode* thenT, ParseNode* elseT) : cond(cond), thenT(thenT), elseT(elseT) {}
    
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
    ParseNode* cond;
    ParseNode* body;
    WhileNode(ParseNode* cond, ParseNode* body) : cond(cond), body(body) {}
    
    virtual void print()
    {
        std::cerr << "WhileNode{cond="; cond->print();
        std::cerr << ", body="; body->print();
        std::cerr << "}";
    }

};

struct BinOpNode : ParseNode
{
    ParseNode* lhs;
    std::string op;
    ParseNode* rhs;
    BinOpNode(ParseNode* lhs, std::string op, ParseNode* rhs) : lhs(lhs), op(op), rhs(rhs) {}

    virtual void print()
    {
        std::cerr << "BinOp{lhs="; lhs->print();
        std::cerr << ", op=" << op << ", rhs="; rhs->print();
        std::cerr << "}";
    }
};

struct UnaryOpNode : ParseNode
{
    std::string op;
    ParseNode* inner;
    UnaryOpNode(std::string op, ParseNode* inner) : op(op), inner(inner) {}

    virtual void print()
    {
        std::cerr << "UnOp{op=" << op << ", in="; 
        inner->print();
        std::cerr << "}";
    }
};

struct MemberAccessNode : ParseNode
{
    ParseNode* obj;
    std::string member;
    MemberAccessNode(ParseNode* obj, std::string member) : obj(obj), member(member) {}

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
    std::string var;
    VariableNode(std::string var) : var(var) {}

    virtual void print()
    {
        std::cerr << "Variable{" << var << "}";
    }
};

struct CallNode : ParseNode
{
    std::string fname;
    std::vector<ParseNode*> args;
    CallNode(std::string fname, std::vector<ParseNode*> args) : fname(fname), args(args) {}

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