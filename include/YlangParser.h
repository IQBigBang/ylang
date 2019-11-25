
// Generated from Ylang.g4 by ANTLR 4.7.1

#pragma once


#include "antlr4-runtime.h"




class  YlangParser : public antlr4::Parser {
public:
  enum {
    T__0 = 1, T__1 = 2, T__2 = 3, T__3 = 4, T__4 = 5, T__5 = 6, T__6 = 7, 
    T__7 = 8, T__8 = 9, T__9 = 10, T__10 = 11, T__11 = 12, T__12 = 13, T__13 = 14, 
    T__14 = 15, T__15 = 16, T__16 = 17, T__17 = 18, T__18 = 19, T__19 = 20, 
    T__20 = 21, T__21 = 22, T__22 = 23, ID = 24, NUMBER = 25, STRING = 26, 
    WS = 27
  };

  enum {
    RuleCode = 0, RuleLine = 1, RuleDefin = 2, RuleStmt = 3, RuleExpr = 4, 
    RuleFnatom = 5, RuleAtom = 6
  };

  YlangParser(antlr4::TokenStream *input);
  ~YlangParser();

  virtual std::string getGrammarFileName() const override;
  virtual const antlr4::atn::ATN& getATN() const override { return _atn; };
  virtual const std::vector<std::string>& getTokenNames() const override { return _tokenNames; }; // deprecated: use vocabulary instead.
  virtual const std::vector<std::string>& getRuleNames() const override;
  virtual antlr4::dfa::Vocabulary& getVocabulary() const override;


  class CodeContext;
  class LineContext;
  class DefinContext;
  class StmtContext;
  class ExprContext;
  class FnatomContext;
  class AtomContext; 

  class  CodeContext : public antlr4::ParserRuleContext {
  public:
    YlangParser::LineContext *lines = nullptr;;
    CodeContext(antlr4::ParserRuleContext *parent, size_t invokingState);
    virtual size_t getRuleIndex() const override;
    antlr4::tree::TerminalNode *EOF();
    std::vector<LineContext *> line();
    LineContext* line(size_t i);

    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
   
  };

  CodeContext* code();

  class  LineContext : public antlr4::ParserRuleContext {
  public:
    LineContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    LineContext() : antlr4::ParserRuleContext() { }
    void copyFrom(LineContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  DefinLineContext : public LineContext {
  public:
    DefinLineContext(LineContext *ctx);

    YlangParser::DefinContext *d = nullptr;
    DefinContext *defin();
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  StmtLineContext : public LineContext {
  public:
    StmtLineContext(LineContext *ctx);

    YlangParser::StmtContext *s = nullptr;
    StmtContext *stmt();
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  LineContext* line();

  class  DefinContext : public antlr4::ParserRuleContext {
  public:
    DefinContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    DefinContext() : antlr4::ParserRuleContext() { }
    void copyFrom(DefinContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  ExternFuncDefContext : public DefinContext {
  public:
    ExternFuncDefContext(DefinContext *ctx);

    antlr4::Token *rettype = nullptr;
    antlr4::Token *fname = nullptr;
    antlr4::Token *type = nullptr;
    antlr4::Token *arg = nullptr;
    std::vector<antlr4::tree::TerminalNode *> ID();
    antlr4::tree::TerminalNode* ID(size_t i);
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  FuncDefContext : public DefinContext {
  public:
    FuncDefContext(DefinContext *ctx);

    antlr4::Token *rettype = nullptr;
    antlr4::Token *fname = nullptr;
    antlr4::Token *type = nullptr;
    antlr4::Token *arg = nullptr;
    YlangParser::StmtContext *body = nullptr;
    std::vector<antlr4::tree::TerminalNode *> ID();
    antlr4::tree::TerminalNode* ID(size_t i);
    std::vector<StmtContext *> stmt();
    StmtContext* stmt(size_t i);
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  DefinContext* defin();

  class  StmtContext : public antlr4::ParserRuleContext {
  public:
    StmtContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    StmtContext() : antlr4::ParserRuleContext() { }
    void copyFrom(StmtContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  ExprExprContext : public StmtContext {
  public:
    ExprExprContext(StmtContext *ctx);

    YlangParser::ExprContext *e = nullptr;
    ExprContext *expr();
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  VarAssignContext : public StmtContext {
  public:
    VarAssignContext(StmtContext *ctx);

    antlr4::Token *name = nullptr;
    YlangParser::ExprContext *e = nullptr;
    ExprContext *expr();
    antlr4::tree::TerminalNode *ID();
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  StmtContext* stmt();

  class  ExprContext : public antlr4::ParserRuleContext {
  public:
    ExprContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    ExprContext() : antlr4::ParserRuleContext() { }
    void copyFrom(ExprContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  MemberAccessContext : public ExprContext {
  public:
    MemberAccessContext(ExprContext *ctx);

    YlangParser::ExprContext *obj = nullptr;
    antlr4::Token *name = nullptr;
    ExprContext *expr();
    antlr4::tree::TerminalNode *ID();
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  InfixExprContext : public ExprContext {
  public:
    InfixExprContext(ExprContext *ctx);

    YlangParser::ExprContext *lhs = nullptr;
    antlr4::Token *op = nullptr;
    YlangParser::ExprContext *rhs = nullptr;
    std::vector<ExprContext *> expr();
    ExprContext* expr(size_t i);
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  AtomExprContext : public ExprContext {
  public:
    AtomExprContext(ExprContext *ctx);

    YlangParser::FnatomContext *a = nullptr;
    FnatomContext *fnatom();
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  ExprContext* expr();
  ExprContext* expr(int precedence);
  class  FnatomContext : public antlr4::ParserRuleContext {
  public:
    FnatomContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    FnatomContext() : antlr4::ParserRuleContext() { }
    void copyFrom(FnatomContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  CallExprContext : public FnatomContext {
  public:
    CallExprContext(FnatomContext *ctx);

    antlr4::Token *fname = nullptr;
    YlangParser::AtomContext *args = nullptr;
    antlr4::tree::TerminalNode *ID();
    std::vector<AtomContext *> atom();
    AtomContext* atom(size_t i);
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  AtomAtomContext : public FnatomContext {
  public:
    AtomAtomContext(FnatomContext *ctx);

    YlangParser::AtomContext *a = nullptr;
    AtomContext *atom();
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  FnatomContext* fnatom();

  class  AtomContext : public antlr4::ParserRuleContext {
  public:
    AtomContext(antlr4::ParserRuleContext *parent, size_t invokingState);
   
    AtomContext() : antlr4::ParserRuleContext() { }
    void copyFrom(AtomContext *context);
    using antlr4::ParserRuleContext::copyFrom;

    virtual size_t getRuleIndex() const override;

   
  };

  class  NumberContext : public AtomContext {
  public:
    NumberContext(AtomContext *ctx);

    antlr4::Token *num = nullptr;
    antlr4::tree::TerminalNode *NUMBER();
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  BoolContext : public AtomContext {
  public:
    BoolContext(AtomContext *ctx);

    antlr4::Token *val = nullptr;
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  StringContext : public AtomContext {
  public:
    StringContext(AtomContext *ctx);

    antlr4::Token *s = nullptr;
    antlr4::tree::TerminalNode *STRING();
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  VariableContext : public AtomContext {
  public:
    VariableContext(AtomContext *ctx);

    antlr4::Token *name = nullptr;
    antlr4::tree::TerminalNode *ID();
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  class  ParenExprContext : public AtomContext {
  public:
    ParenExprContext(AtomContext *ctx);

    YlangParser::ExprContext *e = nullptr;
    ExprContext *expr();
    virtual antlrcpp::Any accept(antlr4::tree::ParseTreeVisitor *visitor) override;
  };

  AtomContext* atom();


  virtual bool sempred(antlr4::RuleContext *_localctx, size_t ruleIndex, size_t predicateIndex) override;
  bool exprSempred(ExprContext *_localctx, size_t predicateIndex);

private:
  static std::vector<antlr4::dfa::DFA> _decisionToDFA;
  static antlr4::atn::PredictionContextCache _sharedContextCache;
  static std::vector<std::string> _ruleNames;
  static std::vector<std::string> _tokenNames;

  static std::vector<std::string> _literalNames;
  static std::vector<std::string> _symbolicNames;
  static antlr4::dfa::Vocabulary _vocabulary;
  static antlr4::atn::ATN _atn;
  static std::vector<uint16_t> _serializedATN;


  struct Initializer {
    Initializer();
  };
  static Initializer _init;
};

