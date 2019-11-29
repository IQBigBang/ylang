
// Generated from Ylang.g4 by ANTLR 4.7.1


#include "Visitor.h"

#include "YlangParser.h"


using namespace antlrcpp;
using namespace antlr4;

YlangParser::YlangParser(TokenStream *input) : Parser(input) {
  _interpreter = new atn::ParserATNSimulator(this, _atn, _decisionToDFA, _sharedContextCache);
}

YlangParser::~YlangParser() {
  delete _interpreter;
}

std::string YlangParser::getGrammarFileName() const {
  return "Ylang.g4";
}

const std::vector<std::string>& YlangParser::getRuleNames() const {
  return _ruleNames;
}

dfa::Vocabulary& YlangParser::getVocabulary() const {
  return _vocabulary;
}


//----------------- CodeContext ------------------------------------------------------------------

YlangParser::CodeContext::CodeContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}

tree::TerminalNode* YlangParser::CodeContext::EOF() {
  return getToken(YlangParser::EOF, 0);
}

std::vector<YlangParser::LineContext *> YlangParser::CodeContext::line() {
  return getRuleContexts<YlangParser::LineContext>();
}

YlangParser::LineContext* YlangParser::CodeContext::line(size_t i) {
  return getRuleContext<YlangParser::LineContext>(i);
}


size_t YlangParser::CodeContext::getRuleIndex() const {
  return YlangParser::RuleCode;
}

antlrcpp::Any YlangParser::CodeContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<YlangVisitor*>(visitor))
    return parserVisitor->visitCode(this);
  else
    return visitor->visitChildren(this);
}

YlangParser::CodeContext* YlangParser::code() {
  CodeContext *_localctx = _tracker.createInstance<CodeContext>(_ctx, getState());
  enterRule(_localctx, 0, YlangParser::RuleCode);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    enterOuterAlt(_localctx, 1);
    setState(13); 
    _errHandler->sync(this);
    _la = _input->LA(1);
    do {
      setState(12);
      dynamic_cast<CodeContext *>(_localctx)->lines = line();
      setState(15); 
      _errHandler->sync(this);
      _la = _input->LA(1);
    } while (_la == YlangParser::T__0);
    setState(17);
    match(YlangParser::EOF);
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- LineContext ------------------------------------------------------------------

YlangParser::LineContext::LineContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t YlangParser::LineContext::getRuleIndex() const {
  return YlangParser::RuleLine;
}

void YlangParser::LineContext::copyFrom(LineContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- DefinLineContext ------------------------------------------------------------------

YlangParser::DefinContext* YlangParser::DefinLineContext::defin() {
  return getRuleContext<YlangParser::DefinContext>(0);
}

YlangParser::DefinLineContext::DefinLineContext(LineContext *ctx) { copyFrom(ctx); }

antlrcpp::Any YlangParser::DefinLineContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<YlangVisitor*>(visitor))
    return parserVisitor->visitDefinLine(this);
  else
    return visitor->visitChildren(this);
}
YlangParser::LineContext* YlangParser::line() {
  LineContext *_localctx = _tracker.createInstance<LineContext>(_ctx, getState());
  enterRule(_localctx, 2, YlangParser::RuleLine);

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    _localctx = dynamic_cast<LineContext *>(_tracker.createInstance<YlangParser::DefinLineContext>(_localctx));
    enterOuterAlt(_localctx, 1);
    setState(19);
    dynamic_cast<DefinLineContext *>(_localctx)->d = defin();
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- DefinContext ------------------------------------------------------------------

YlangParser::DefinContext::DefinContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t YlangParser::DefinContext::getRuleIndex() const {
  return YlangParser::RuleDefin;
}

void YlangParser::DefinContext::copyFrom(DefinContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- ExternFuncDefContext ------------------------------------------------------------------

std::vector<tree::TerminalNode *> YlangParser::ExternFuncDefContext::ID() {
  return getTokens(YlangParser::ID);
}

tree::TerminalNode* YlangParser::ExternFuncDefContext::ID(size_t i) {
  return getToken(YlangParser::ID, i);
}

YlangParser::ExternFuncDefContext::ExternFuncDefContext(DefinContext *ctx) { copyFrom(ctx); }

antlrcpp::Any YlangParser::ExternFuncDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<YlangVisitor*>(visitor))
    return parserVisitor->visitExternFuncDef(this);
  else
    return visitor->visitChildren(this);
}
//----------------- FuncDefContext ------------------------------------------------------------------

std::vector<tree::TerminalNode *> YlangParser::FuncDefContext::ID() {
  return getTokens(YlangParser::ID);
}

tree::TerminalNode* YlangParser::FuncDefContext::ID(size_t i) {
  return getToken(YlangParser::ID, i);
}

YlangParser::ExprContext* YlangParser::FuncDefContext::expr() {
  return getRuleContext<YlangParser::ExprContext>(0);
}

YlangParser::FuncDefContext::FuncDefContext(DefinContext *ctx) { copyFrom(ctx); }

antlrcpp::Any YlangParser::FuncDefContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<YlangVisitor*>(visitor))
    return parserVisitor->visitFuncDef(this);
  else
    return visitor->visitChildren(this);
}
YlangParser::DefinContext* YlangParser::defin() {
  DefinContext *_localctx = _tracker.createInstance<DefinContext>(_ctx, getState());
  enterRule(_localctx, 4, YlangParser::RuleDefin);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(62);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 6, _ctx)) {
    case 1: {
      _localctx = dynamic_cast<DefinContext *>(_tracker.createInstance<YlangParser::ExternFuncDefContext>(_localctx));
      enterOuterAlt(_localctx, 1);
      setState(21);
      match(YlangParser::T__0);
      setState(22);
      match(YlangParser::T__1);
      setState(23);
      dynamic_cast<ExternFuncDefContext *>(_localctx)->rettype = match(YlangParser::ID);
      setState(24);
      dynamic_cast<ExternFuncDefContext *>(_localctx)->fname = match(YlangParser::ID);
      setState(25);
      match(YlangParser::T__2);
      setState(30);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == YlangParser::ID) {
        setState(26);
        dynamic_cast<ExternFuncDefContext *>(_localctx)->type = match(YlangParser::ID);
        setState(27);
        dynamic_cast<ExternFuncDefContext *>(_localctx)->arg = match(YlangParser::ID);
        setState(32);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(33);
      match(YlangParser::T__3);
      setState(37);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == YlangParser::T__4) {
        setState(34);
        match(YlangParser::T__4);
        setState(39);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      break;
    }

    case 2: {
      _localctx = dynamic_cast<DefinContext *>(_tracker.createInstance<YlangParser::FuncDefContext>(_localctx));
      enterOuterAlt(_localctx, 2);
      setState(40);
      match(YlangParser::T__0);
      setState(41);
      dynamic_cast<FuncDefContext *>(_localctx)->rettype = match(YlangParser::ID);
      setState(42);
      dynamic_cast<FuncDefContext *>(_localctx)->fname = match(YlangParser::ID);
      setState(43);
      match(YlangParser::T__2);
      setState(48);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == YlangParser::ID) {
        setState(44);
        dynamic_cast<FuncDefContext *>(_localctx)->type = match(YlangParser::ID);
        setState(45);
        dynamic_cast<FuncDefContext *>(_localctx)->arg = match(YlangParser::ID);
        setState(50);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      setState(51);
      match(YlangParser::T__3);
      setState(53);
      _errHandler->sync(this);

      _la = _input->LA(1);
      if (_la == YlangParser::T__4) {
        setState(52);
        match(YlangParser::T__4);
      }
      setState(55);
      dynamic_cast<FuncDefContext *>(_localctx)->body = expr(0);
      setState(59);
      _errHandler->sync(this);
      _la = _input->LA(1);
      while (_la == YlangParser::T__4) {
        setState(56);
        match(YlangParser::T__4);
        setState(61);
        _errHandler->sync(this);
        _la = _input->LA(1);
      }
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- ExprContext ------------------------------------------------------------------

YlangParser::ExprContext::ExprContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t YlangParser::ExprContext::getRuleIndex() const {
  return YlangParser::RuleExpr;
}

void YlangParser::ExprContext::copyFrom(ExprContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- MemberAccessContext ------------------------------------------------------------------

YlangParser::ExprContext* YlangParser::MemberAccessContext::expr() {
  return getRuleContext<YlangParser::ExprContext>(0);
}

tree::TerminalNode* YlangParser::MemberAccessContext::ID() {
  return getToken(YlangParser::ID, 0);
}

YlangParser::MemberAccessContext::MemberAccessContext(ExprContext *ctx) { copyFrom(ctx); }

antlrcpp::Any YlangParser::MemberAccessContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<YlangVisitor*>(visitor))
    return parserVisitor->visitMemberAccess(this);
  else
    return visitor->visitChildren(this);
}
//----------------- InfixExprContext ------------------------------------------------------------------

std::vector<YlangParser::ExprContext *> YlangParser::InfixExprContext::expr() {
  return getRuleContexts<YlangParser::ExprContext>();
}

YlangParser::ExprContext* YlangParser::InfixExprContext::expr(size_t i) {
  return getRuleContext<YlangParser::ExprContext>(i);
}

YlangParser::InfixExprContext::InfixExprContext(ExprContext *ctx) { copyFrom(ctx); }

antlrcpp::Any YlangParser::InfixExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<YlangVisitor*>(visitor))
    return parserVisitor->visitInfixExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- IfExprContext ------------------------------------------------------------------

std::vector<YlangParser::ExprContext *> YlangParser::IfExprContext::expr() {
  return getRuleContexts<YlangParser::ExprContext>();
}

YlangParser::ExprContext* YlangParser::IfExprContext::expr(size_t i) {
  return getRuleContext<YlangParser::ExprContext>(i);
}

YlangParser::IfExprContext::IfExprContext(ExprContext *ctx) { copyFrom(ctx); }

antlrcpp::Any YlangParser::IfExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<YlangVisitor*>(visitor))
    return parserVisitor->visitIfExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AtomExprContext ------------------------------------------------------------------

YlangParser::FnatomContext* YlangParser::AtomExprContext::fnatom() {
  return getRuleContext<YlangParser::FnatomContext>(0);
}

YlangParser::AtomExprContext::AtomExprContext(ExprContext *ctx) { copyFrom(ctx); }

antlrcpp::Any YlangParser::AtomExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<YlangVisitor*>(visitor))
    return parserVisitor->visitAtomExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- LetInExprContext ------------------------------------------------------------------

tree::TerminalNode* YlangParser::LetInExprContext::ID() {
  return getToken(YlangParser::ID, 0);
}

std::vector<YlangParser::ExprContext *> YlangParser::LetInExprContext::expr() {
  return getRuleContexts<YlangParser::ExprContext>();
}

YlangParser::ExprContext* YlangParser::LetInExprContext::expr(size_t i) {
  return getRuleContext<YlangParser::ExprContext>(i);
}

YlangParser::LetInExprContext::LetInExprContext(ExprContext *ctx) { copyFrom(ctx); }

antlrcpp::Any YlangParser::LetInExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<YlangVisitor*>(visitor))
    return parserVisitor->visitLetInExpr(this);
  else
    return visitor->visitChildren(this);
}

YlangParser::ExprContext* YlangParser::expr() {
   return expr(0);
}

YlangParser::ExprContext* YlangParser::expr(int precedence) {
  ParserRuleContext *parentContext = _ctx;
  size_t parentState = getState();
  YlangParser::ExprContext *_localctx = _tracker.createInstance<ExprContext>(_ctx, parentState);
  YlangParser::ExprContext *previousContext = _localctx;
  size_t startState = 6;
  enterRecursionRule(_localctx, 6, YlangParser::RuleExpr, precedence);

    size_t _la = 0;

  auto onExit = finally([=] {
    unrollRecursionContexts(parentContext);
  });
  try {
    size_t alt;
    enterOuterAlt(_localctx, 1);
    setState(91);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case YlangParser::T__18: {
        _localctx = _tracker.createInstance<IfExprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;

        setState(65);
        match(YlangParser::T__18);
        setState(66);
        dynamic_cast<IfExprContext *>(_localctx)->cond = expr(0);
        setState(68);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == YlangParser::T__4) {
          setState(67);
          match(YlangParser::T__4);
        }
        setState(70);
        dynamic_cast<IfExprContext *>(_localctx)->thenT = expr(0);
        setState(72);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == YlangParser::T__4) {
          setState(71);
          match(YlangParser::T__4);
        }
        setState(74);
        match(YlangParser::T__19);
        setState(76);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == YlangParser::T__4) {
          setState(75);
          match(YlangParser::T__4);
        }
        setState(78);
        dynamic_cast<IfExprContext *>(_localctx)->elseT = expr(3);
        break;
      }

      case YlangParser::T__20: {
        _localctx = _tracker.createInstance<LetInExprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(80);
        match(YlangParser::T__20);
        setState(81);
        dynamic_cast<LetInExprContext *>(_localctx)->name = match(YlangParser::ID);
        setState(82);
        match(YlangParser::T__21);
        setState(83);
        dynamic_cast<LetInExprContext *>(_localctx)->val = expr(0);
        setState(84);
        match(YlangParser::T__22);
        setState(86);
        _errHandler->sync(this);

        _la = _input->LA(1);
        if (_la == YlangParser::T__4) {
          setState(85);
          match(YlangParser::T__4);
        }
        setState(88);
        dynamic_cast<LetInExprContext *>(_localctx)->e = expr(2);
        break;
      }

      case YlangParser::T__2:
      case YlangParser::T__23:
      case YlangParser::T__24:
      case YlangParser::ID:
      case YlangParser::NUMBER:
      case YlangParser::STRING: {
        _localctx = _tracker.createInstance<AtomExprContext>(_localctx);
        _ctx = _localctx;
        previousContext = _localctx;
        setState(90);
        dynamic_cast<AtomExprContext *>(_localctx)->a = fnatom();
        break;
      }

    default:
      throw NoViableAltException(this);
    }
    _ctx->stop = _input->LT(-1);
    setState(110);
    _errHandler->sync(this);
    alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 13, _ctx);
    while (alt != 2 && alt != atn::ATN::INVALID_ALT_NUMBER) {
      if (alt == 1) {
        if (!_parseListeners.empty())
          triggerExitRuleEvent();
        previousContext = _localctx;
        setState(108);
        _errHandler->sync(this);
        switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 12, _ctx)) {
        case 1: {
          auto newContext = _tracker.createInstance<InfixExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          newContext->lhs = previousContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(93);

          if (!(precpred(_ctx, 7))) throw FailedPredicateException(this, "precpred(_ctx, 7)");
          setState(94);
          dynamic_cast<InfixExprContext *>(_localctx)->op = match(YlangParser::T__6);
          setState(95);
          dynamic_cast<InfixExprContext *>(_localctx)->rhs = expr(7);
          break;
        }

        case 2: {
          auto newContext = _tracker.createInstance<InfixExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          newContext->lhs = previousContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(96);

          if (!(precpred(_ctx, 6))) throw FailedPredicateException(this, "precpred(_ctx, 6)");
          setState(97);
          dynamic_cast<InfixExprContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << YlangParser::T__7)
            | (1ULL << YlangParser::T__8)
            | (1ULL << YlangParser::T__9))) != 0))) {
            dynamic_cast<InfixExprContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(98);
          dynamic_cast<InfixExprContext *>(_localctx)->rhs = expr(7);
          break;
        }

        case 3: {
          auto newContext = _tracker.createInstance<InfixExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          newContext->lhs = previousContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(99);

          if (!(precpred(_ctx, 5))) throw FailedPredicateException(this, "precpred(_ctx, 5)");
          setState(100);
          dynamic_cast<InfixExprContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!(_la == YlangParser::T__10

          || _la == YlangParser::T__11)) {
            dynamic_cast<InfixExprContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(101);
          dynamic_cast<InfixExprContext *>(_localctx)->rhs = expr(6);
          break;
        }

        case 4: {
          auto newContext = _tracker.createInstance<InfixExprContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          newContext->lhs = previousContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(102);

          if (!(precpred(_ctx, 4))) throw FailedPredicateException(this, "precpred(_ctx, 4)");
          setState(103);
          dynamic_cast<InfixExprContext *>(_localctx)->op = _input->LT(1);
          _la = _input->LA(1);
          if (!((((_la & ~ 0x3fULL) == 0) &&
            ((1ULL << _la) & ((1ULL << YlangParser::T__12)
            | (1ULL << YlangParser::T__13)
            | (1ULL << YlangParser::T__14)
            | (1ULL << YlangParser::T__15)
            | (1ULL << YlangParser::T__16)
            | (1ULL << YlangParser::T__17))) != 0))) {
            dynamic_cast<InfixExprContext *>(_localctx)->op = _errHandler->recoverInline(this);
          }
          else {
            _errHandler->reportMatch(this);
            consume();
          }
          setState(104);
          dynamic_cast<InfixExprContext *>(_localctx)->rhs = expr(5);
          break;
        }

        case 5: {
          auto newContext = _tracker.createInstance<MemberAccessContext>(_tracker.createInstance<ExprContext>(parentContext, parentState));
          _localctx = newContext;
          newContext->obj = previousContext;
          pushNewRecursionContext(newContext, startState, RuleExpr);
          setState(105);

          if (!(precpred(_ctx, 8))) throw FailedPredicateException(this, "precpred(_ctx, 8)");

          setState(106);
          match(YlangParser::T__5);
          setState(107);
          dynamic_cast<MemberAccessContext *>(_localctx)->name = match(YlangParser::ID);
          break;
        }

        } 
      }
      setState(112);
      _errHandler->sync(this);
      alt = getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 13, _ctx);
    }
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }
  return _localctx;
}

//----------------- FnatomContext ------------------------------------------------------------------

YlangParser::FnatomContext::FnatomContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t YlangParser::FnatomContext::getRuleIndex() const {
  return YlangParser::RuleFnatom;
}

void YlangParser::FnatomContext::copyFrom(FnatomContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- CallExprContext ------------------------------------------------------------------

tree::TerminalNode* YlangParser::CallExprContext::ID() {
  return getToken(YlangParser::ID, 0);
}

std::vector<YlangParser::ExprContext *> YlangParser::CallExprContext::expr() {
  return getRuleContexts<YlangParser::ExprContext>();
}

YlangParser::ExprContext* YlangParser::CallExprContext::expr(size_t i) {
  return getRuleContext<YlangParser::ExprContext>(i);
}

YlangParser::CallExprContext::CallExprContext(FnatomContext *ctx) { copyFrom(ctx); }

antlrcpp::Any YlangParser::CallExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<YlangVisitor*>(visitor))
    return parserVisitor->visitCallExpr(this);
  else
    return visitor->visitChildren(this);
}
//----------------- AtomAtomContext ------------------------------------------------------------------

YlangParser::AtomContext* YlangParser::AtomAtomContext::atom() {
  return getRuleContext<YlangParser::AtomContext>(0);
}

YlangParser::AtomAtomContext::AtomAtomContext(FnatomContext *ctx) { copyFrom(ctx); }

antlrcpp::Any YlangParser::AtomAtomContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<YlangVisitor*>(visitor))
    return parserVisitor->visitAtomAtom(this);
  else
    return visitor->visitChildren(this);
}
YlangParser::FnatomContext* YlangParser::fnatom() {
  FnatomContext *_localctx = _tracker.createInstance<FnatomContext>(_ctx, getState());
  enterRule(_localctx, 8, YlangParser::RuleFnatom);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(123);
    _errHandler->sync(this);
    switch (getInterpreter<atn::ParserATNSimulator>()->adaptivePredict(_input, 15, _ctx)) {
    case 1: {
      _localctx = dynamic_cast<FnatomContext *>(_tracker.createInstance<YlangParser::CallExprContext>(_localctx));
      enterOuterAlt(_localctx, 1);
      setState(113);
      dynamic_cast<CallExprContext *>(_localctx)->fname = match(YlangParser::ID);
      setState(114);
      match(YlangParser::T__2);
      setState(116); 
      _errHandler->sync(this);
      _la = _input->LA(1);
      do {
        setState(115);
        dynamic_cast<CallExprContext *>(_localctx)->args = expr(0);
        setState(118); 
        _errHandler->sync(this);
        _la = _input->LA(1);
      } while ((((_la & ~ 0x3fULL) == 0) &&
        ((1ULL << _la) & ((1ULL << YlangParser::T__2)
        | (1ULL << YlangParser::T__18)
        | (1ULL << YlangParser::T__20)
        | (1ULL << YlangParser::T__23)
        | (1ULL << YlangParser::T__24)
        | (1ULL << YlangParser::ID)
        | (1ULL << YlangParser::NUMBER)
        | (1ULL << YlangParser::STRING))) != 0));
      setState(120);
      match(YlangParser::T__3);
      break;
    }

    case 2: {
      _localctx = dynamic_cast<FnatomContext *>(_tracker.createInstance<YlangParser::AtomAtomContext>(_localctx));
      enterOuterAlt(_localctx, 2);
      setState(122);
      dynamic_cast<AtomAtomContext *>(_localctx)->a = atom();
      break;
    }

    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

//----------------- AtomContext ------------------------------------------------------------------

YlangParser::AtomContext::AtomContext(ParserRuleContext *parent, size_t invokingState)
  : ParserRuleContext(parent, invokingState) {
}


size_t YlangParser::AtomContext::getRuleIndex() const {
  return YlangParser::RuleAtom;
}

void YlangParser::AtomContext::copyFrom(AtomContext *ctx) {
  ParserRuleContext::copyFrom(ctx);
}

//----------------- NumberContext ------------------------------------------------------------------

tree::TerminalNode* YlangParser::NumberContext::NUMBER() {
  return getToken(YlangParser::NUMBER, 0);
}

YlangParser::NumberContext::NumberContext(AtomContext *ctx) { copyFrom(ctx); }

antlrcpp::Any YlangParser::NumberContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<YlangVisitor*>(visitor))
    return parserVisitor->visitNumber(this);
  else
    return visitor->visitChildren(this);
}
//----------------- BoolContext ------------------------------------------------------------------

YlangParser::BoolContext::BoolContext(AtomContext *ctx) { copyFrom(ctx); }

antlrcpp::Any YlangParser::BoolContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<YlangVisitor*>(visitor))
    return parserVisitor->visitBool(this);
  else
    return visitor->visitChildren(this);
}
//----------------- StringContext ------------------------------------------------------------------

tree::TerminalNode* YlangParser::StringContext::STRING() {
  return getToken(YlangParser::STRING, 0);
}

YlangParser::StringContext::StringContext(AtomContext *ctx) { copyFrom(ctx); }

antlrcpp::Any YlangParser::StringContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<YlangVisitor*>(visitor))
    return parserVisitor->visitString(this);
  else
    return visitor->visitChildren(this);
}
//----------------- VariableContext ------------------------------------------------------------------

tree::TerminalNode* YlangParser::VariableContext::ID() {
  return getToken(YlangParser::ID, 0);
}

YlangParser::VariableContext::VariableContext(AtomContext *ctx) { copyFrom(ctx); }

antlrcpp::Any YlangParser::VariableContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<YlangVisitor*>(visitor))
    return parserVisitor->visitVariable(this);
  else
    return visitor->visitChildren(this);
}
//----------------- ParenExprContext ------------------------------------------------------------------

YlangParser::ExprContext* YlangParser::ParenExprContext::expr() {
  return getRuleContext<YlangParser::ExprContext>(0);
}

YlangParser::ParenExprContext::ParenExprContext(AtomContext *ctx) { copyFrom(ctx); }

antlrcpp::Any YlangParser::ParenExprContext::accept(tree::ParseTreeVisitor *visitor) {
  if (auto parserVisitor = dynamic_cast<YlangVisitor*>(visitor))
    return parserVisitor->visitParenExpr(this);
  else
    return visitor->visitChildren(this);
}
YlangParser::AtomContext* YlangParser::atom() {
  AtomContext *_localctx = _tracker.createInstance<AtomContext>(_ctx, getState());
  enterRule(_localctx, 10, YlangParser::RuleAtom);
  size_t _la = 0;

  auto onExit = finally([=] {
    exitRule();
  });
  try {
    setState(133);
    _errHandler->sync(this);
    switch (_input->LA(1)) {
      case YlangParser::T__23:
      case YlangParser::T__24: {
        _localctx = dynamic_cast<AtomContext *>(_tracker.createInstance<YlangParser::BoolContext>(_localctx));
        enterOuterAlt(_localctx, 1);
        setState(125);
        dynamic_cast<BoolContext *>(_localctx)->val = _input->LT(1);
        _la = _input->LA(1);
        if (!(_la == YlangParser::T__23

        || _la == YlangParser::T__24)) {
          dynamic_cast<BoolContext *>(_localctx)->val = _errHandler->recoverInline(this);
        }
        else {
          _errHandler->reportMatch(this);
          consume();
        }
        break;
      }

      case YlangParser::ID: {
        _localctx = dynamic_cast<AtomContext *>(_tracker.createInstance<YlangParser::VariableContext>(_localctx));
        enterOuterAlt(_localctx, 2);
        setState(126);
        dynamic_cast<VariableContext *>(_localctx)->name = match(YlangParser::ID);
        break;
      }

      case YlangParser::NUMBER: {
        _localctx = dynamic_cast<AtomContext *>(_tracker.createInstance<YlangParser::NumberContext>(_localctx));
        enterOuterAlt(_localctx, 3);
        setState(127);
        dynamic_cast<NumberContext *>(_localctx)->num = match(YlangParser::NUMBER);
        break;
      }

      case YlangParser::STRING: {
        _localctx = dynamic_cast<AtomContext *>(_tracker.createInstance<YlangParser::StringContext>(_localctx));
        enterOuterAlt(_localctx, 4);
        setState(128);
        dynamic_cast<StringContext *>(_localctx)->s = match(YlangParser::STRING);
        break;
      }

      case YlangParser::T__2: {
        _localctx = dynamic_cast<AtomContext *>(_tracker.createInstance<YlangParser::ParenExprContext>(_localctx));
        enterOuterAlt(_localctx, 5);
        setState(129);
        match(YlangParser::T__2);
        setState(130);
        dynamic_cast<ParenExprContext *>(_localctx)->e = expr(0);
        setState(131);
        match(YlangParser::T__3);
        break;
      }

    default:
      throw NoViableAltException(this);
    }
   
  }
  catch (RecognitionException &e) {
    _errHandler->reportError(this, e);
    _localctx->exception = std::current_exception();
    _errHandler->recover(this, _localctx->exception);
  }

  return _localctx;
}

bool YlangParser::sempred(RuleContext *context, size_t ruleIndex, size_t predicateIndex) {
  switch (ruleIndex) {
    case 3: return exprSempred(dynamic_cast<ExprContext *>(context), predicateIndex);

  default:
    break;
  }
  return true;
}

bool YlangParser::exprSempred(ExprContext *_localctx, size_t predicateIndex) {
  switch (predicateIndex) {
    case 0: return precpred(_ctx, 7);
    case 1: return precpred(_ctx, 6);
    case 2: return precpred(_ctx, 5);
    case 3: return precpred(_ctx, 4);
    case 4: return precpred(_ctx, 8);

  default:
    break;
  }
  return true;
}

// Static vars and initialization.
std::vector<dfa::DFA> YlangParser::_decisionToDFA;
atn::PredictionContextCache YlangParser::_sharedContextCache;

// We own the ATN which in turn owns the ATN states.
atn::ATN YlangParser::_atn;
std::vector<uint16_t> YlangParser::_serializedATN;

std::vector<std::string> YlangParser::_ruleNames = {
  "code", "line", "defin", "expr", "fnatom", "atom"
};

std::vector<std::string> YlangParser::_literalNames = {
  "", "'def'", "'external'", "'('", "')'", "'\n'", "'->'", "'^'", "'*'", 
  "'/'", "'%'", "'+'", "'-'", "'=='", "'!='", "'<'", "'>'", "'<='", "'>='", 
  "'if'", "'else'", "'let'", "'='", "'in'", "'true'", "'false'"
};

std::vector<std::string> YlangParser::_symbolicNames = {
  "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", "", 
  "", "", "", "", "", "", "", "", "ID", "NUMBER", "STRING", "WS"
};

dfa::Vocabulary YlangParser::_vocabulary(_literalNames, _symbolicNames);

std::vector<std::string> YlangParser::_tokenNames;

YlangParser::Initializer::Initializer() {
	for (size_t i = 0; i < _symbolicNames.size(); ++i) {
		std::string name = _vocabulary.getLiteralName(i);
		if (name.empty()) {
			name = _vocabulary.getSymbolicName(i);
		}

		if (name.empty()) {
			_tokenNames.push_back("<INVALID>");
		} else {
      _tokenNames.push_back(name);
    }
	}

  _serializedATN = {
    0x3, 0x608b, 0xa72a, 0x8133, 0xb9ed, 0x417c, 0x3be7, 0x7786, 0x5964, 
    0x3, 0x1f, 0x8a, 0x4, 0x2, 0x9, 0x2, 0x4, 0x3, 0x9, 0x3, 0x4, 0x4, 0x9, 
    0x4, 0x4, 0x5, 0x9, 0x5, 0x4, 0x6, 0x9, 0x6, 0x4, 0x7, 0x9, 0x7, 0x3, 
    0x2, 0x6, 0x2, 0x10, 0xa, 0x2, 0xd, 0x2, 0xe, 0x2, 0x11, 0x3, 0x2, 0x3, 
    0x2, 0x3, 0x3, 0x3, 0x3, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 
    0x4, 0x3, 0x4, 0x3, 0x4, 0x7, 0x4, 0x1f, 0xa, 0x4, 0xc, 0x4, 0xe, 0x4, 
    0x22, 0xb, 0x4, 0x3, 0x4, 0x3, 0x4, 0x7, 0x4, 0x26, 0xa, 0x4, 0xc, 0x4, 
    0xe, 0x4, 0x29, 0xb, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 0x4, 0x3, 
    0x4, 0x3, 0x4, 0x7, 0x4, 0x31, 0xa, 0x4, 0xc, 0x4, 0xe, 0x4, 0x34, 0xb, 
    0x4, 0x3, 0x4, 0x3, 0x4, 0x5, 0x4, 0x38, 0xa, 0x4, 0x3, 0x4, 0x3, 0x4, 
    0x7, 0x4, 0x3c, 0xa, 0x4, 0xc, 0x4, 0xe, 0x4, 0x3f, 0xb, 0x4, 0x5, 0x4, 
    0x41, 0xa, 0x4, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x5, 0x5, 0x47, 
    0xa, 0x5, 0x3, 0x5, 0x3, 0x5, 0x5, 0x5, 0x4b, 0xa, 0x5, 0x3, 0x5, 0x3, 
    0x5, 0x5, 0x5, 0x4f, 0xa, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 
    0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x5, 0x5, 0x59, 0xa, 0x5, 0x3, 
    0x5, 0x3, 0x5, 0x3, 0x5, 0x5, 0x5, 0x5e, 0xa, 0x5, 0x3, 0x5, 0x3, 0x5, 
    0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 
    0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x3, 0x5, 0x7, 0x5, 
    0x6f, 0xa, 0x5, 0xc, 0x5, 0xe, 0x5, 0x72, 0xb, 0x5, 0x3, 0x6, 0x3, 0x6, 
    0x3, 0x6, 0x6, 0x6, 0x77, 0xa, 0x6, 0xd, 0x6, 0xe, 0x6, 0x78, 0x3, 0x6, 
    0x3, 0x6, 0x3, 0x6, 0x5, 0x6, 0x7e, 0xa, 0x6, 0x3, 0x7, 0x3, 0x7, 0x3, 
    0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x3, 0x7, 0x5, 0x7, 0x88, 
    0xa, 0x7, 0x3, 0x7, 0x2, 0x3, 0x8, 0x8, 0x2, 0x4, 0x6, 0x8, 0xa, 0xc, 
    0x2, 0x6, 0x3, 0x2, 0xa, 0xc, 0x3, 0x2, 0xd, 0xe, 0x3, 0x2, 0xf, 0x14, 
    0x3, 0x2, 0x1a, 0x1b, 0x2, 0x9b, 0x2, 0xf, 0x3, 0x2, 0x2, 0x2, 0x4, 
    0x15, 0x3, 0x2, 0x2, 0x2, 0x6, 0x40, 0x3, 0x2, 0x2, 0x2, 0x8, 0x5d, 
    0x3, 0x2, 0x2, 0x2, 0xa, 0x7d, 0x3, 0x2, 0x2, 0x2, 0xc, 0x87, 0x3, 0x2, 
    0x2, 0x2, 0xe, 0x10, 0x5, 0x4, 0x3, 0x2, 0xf, 0xe, 0x3, 0x2, 0x2, 0x2, 
    0x10, 0x11, 0x3, 0x2, 0x2, 0x2, 0x11, 0xf, 0x3, 0x2, 0x2, 0x2, 0x11, 
    0x12, 0x3, 0x2, 0x2, 0x2, 0x12, 0x13, 0x3, 0x2, 0x2, 0x2, 0x13, 0x14, 
    0x7, 0x2, 0x2, 0x3, 0x14, 0x3, 0x3, 0x2, 0x2, 0x2, 0x15, 0x16, 0x5, 
    0x6, 0x4, 0x2, 0x16, 0x5, 0x3, 0x2, 0x2, 0x2, 0x17, 0x18, 0x7, 0x3, 
    0x2, 0x2, 0x18, 0x19, 0x7, 0x4, 0x2, 0x2, 0x19, 0x1a, 0x7, 0x1c, 0x2, 
    0x2, 0x1a, 0x1b, 0x7, 0x1c, 0x2, 0x2, 0x1b, 0x20, 0x7, 0x5, 0x2, 0x2, 
    0x1c, 0x1d, 0x7, 0x1c, 0x2, 0x2, 0x1d, 0x1f, 0x7, 0x1c, 0x2, 0x2, 0x1e, 
    0x1c, 0x3, 0x2, 0x2, 0x2, 0x1f, 0x22, 0x3, 0x2, 0x2, 0x2, 0x20, 0x1e, 
    0x3, 0x2, 0x2, 0x2, 0x20, 0x21, 0x3, 0x2, 0x2, 0x2, 0x21, 0x23, 0x3, 
    0x2, 0x2, 0x2, 0x22, 0x20, 0x3, 0x2, 0x2, 0x2, 0x23, 0x27, 0x7, 0x6, 
    0x2, 0x2, 0x24, 0x26, 0x7, 0x7, 0x2, 0x2, 0x25, 0x24, 0x3, 0x2, 0x2, 
    0x2, 0x26, 0x29, 0x3, 0x2, 0x2, 0x2, 0x27, 0x25, 0x3, 0x2, 0x2, 0x2, 
    0x27, 0x28, 0x3, 0x2, 0x2, 0x2, 0x28, 0x41, 0x3, 0x2, 0x2, 0x2, 0x29, 
    0x27, 0x3, 0x2, 0x2, 0x2, 0x2a, 0x2b, 0x7, 0x3, 0x2, 0x2, 0x2b, 0x2c, 
    0x7, 0x1c, 0x2, 0x2, 0x2c, 0x2d, 0x7, 0x1c, 0x2, 0x2, 0x2d, 0x32, 0x7, 
    0x5, 0x2, 0x2, 0x2e, 0x2f, 0x7, 0x1c, 0x2, 0x2, 0x2f, 0x31, 0x7, 0x1c, 
    0x2, 0x2, 0x30, 0x2e, 0x3, 0x2, 0x2, 0x2, 0x31, 0x34, 0x3, 0x2, 0x2, 
    0x2, 0x32, 0x30, 0x3, 0x2, 0x2, 0x2, 0x32, 0x33, 0x3, 0x2, 0x2, 0x2, 
    0x33, 0x35, 0x3, 0x2, 0x2, 0x2, 0x34, 0x32, 0x3, 0x2, 0x2, 0x2, 0x35, 
    0x37, 0x7, 0x6, 0x2, 0x2, 0x36, 0x38, 0x7, 0x7, 0x2, 0x2, 0x37, 0x36, 
    0x3, 0x2, 0x2, 0x2, 0x37, 0x38, 0x3, 0x2, 0x2, 0x2, 0x38, 0x39, 0x3, 
    0x2, 0x2, 0x2, 0x39, 0x3d, 0x5, 0x8, 0x5, 0x2, 0x3a, 0x3c, 0x7, 0x7, 
    0x2, 0x2, 0x3b, 0x3a, 0x3, 0x2, 0x2, 0x2, 0x3c, 0x3f, 0x3, 0x2, 0x2, 
    0x2, 0x3d, 0x3b, 0x3, 0x2, 0x2, 0x2, 0x3d, 0x3e, 0x3, 0x2, 0x2, 0x2, 
    0x3e, 0x41, 0x3, 0x2, 0x2, 0x2, 0x3f, 0x3d, 0x3, 0x2, 0x2, 0x2, 0x40, 
    0x17, 0x3, 0x2, 0x2, 0x2, 0x40, 0x2a, 0x3, 0x2, 0x2, 0x2, 0x41, 0x7, 
    0x3, 0x2, 0x2, 0x2, 0x42, 0x43, 0x8, 0x5, 0x1, 0x2, 0x43, 0x44, 0x7, 
    0x15, 0x2, 0x2, 0x44, 0x46, 0x5, 0x8, 0x5, 0x2, 0x45, 0x47, 0x7, 0x7, 
    0x2, 0x2, 0x46, 0x45, 0x3, 0x2, 0x2, 0x2, 0x46, 0x47, 0x3, 0x2, 0x2, 
    0x2, 0x47, 0x48, 0x3, 0x2, 0x2, 0x2, 0x48, 0x4a, 0x5, 0x8, 0x5, 0x2, 
    0x49, 0x4b, 0x7, 0x7, 0x2, 0x2, 0x4a, 0x49, 0x3, 0x2, 0x2, 0x2, 0x4a, 
    0x4b, 0x3, 0x2, 0x2, 0x2, 0x4b, 0x4c, 0x3, 0x2, 0x2, 0x2, 0x4c, 0x4e, 
    0x7, 0x16, 0x2, 0x2, 0x4d, 0x4f, 0x7, 0x7, 0x2, 0x2, 0x4e, 0x4d, 0x3, 
    0x2, 0x2, 0x2, 0x4e, 0x4f, 0x3, 0x2, 0x2, 0x2, 0x4f, 0x50, 0x3, 0x2, 
    0x2, 0x2, 0x50, 0x51, 0x5, 0x8, 0x5, 0x5, 0x51, 0x5e, 0x3, 0x2, 0x2, 
    0x2, 0x52, 0x53, 0x7, 0x17, 0x2, 0x2, 0x53, 0x54, 0x7, 0x1c, 0x2, 0x2, 
    0x54, 0x55, 0x7, 0x18, 0x2, 0x2, 0x55, 0x56, 0x5, 0x8, 0x5, 0x2, 0x56, 
    0x58, 0x7, 0x19, 0x2, 0x2, 0x57, 0x59, 0x7, 0x7, 0x2, 0x2, 0x58, 0x57, 
    0x3, 0x2, 0x2, 0x2, 0x58, 0x59, 0x3, 0x2, 0x2, 0x2, 0x59, 0x5a, 0x3, 
    0x2, 0x2, 0x2, 0x5a, 0x5b, 0x5, 0x8, 0x5, 0x4, 0x5b, 0x5e, 0x3, 0x2, 
    0x2, 0x2, 0x5c, 0x5e, 0x5, 0xa, 0x6, 0x2, 0x5d, 0x42, 0x3, 0x2, 0x2, 
    0x2, 0x5d, 0x52, 0x3, 0x2, 0x2, 0x2, 0x5d, 0x5c, 0x3, 0x2, 0x2, 0x2, 
    0x5e, 0x70, 0x3, 0x2, 0x2, 0x2, 0x5f, 0x60, 0xc, 0x9, 0x2, 0x2, 0x60, 
    0x61, 0x7, 0x9, 0x2, 0x2, 0x61, 0x6f, 0x5, 0x8, 0x5, 0x9, 0x62, 0x63, 
    0xc, 0x8, 0x2, 0x2, 0x63, 0x64, 0x9, 0x2, 0x2, 0x2, 0x64, 0x6f, 0x5, 
    0x8, 0x5, 0x9, 0x65, 0x66, 0xc, 0x7, 0x2, 0x2, 0x66, 0x67, 0x9, 0x3, 
    0x2, 0x2, 0x67, 0x6f, 0x5, 0x8, 0x5, 0x8, 0x68, 0x69, 0xc, 0x6, 0x2, 
    0x2, 0x69, 0x6a, 0x9, 0x4, 0x2, 0x2, 0x6a, 0x6f, 0x5, 0x8, 0x5, 0x7, 
    0x6b, 0x6c, 0xc, 0xa, 0x2, 0x2, 0x6c, 0x6d, 0x7, 0x8, 0x2, 0x2, 0x6d, 
    0x6f, 0x7, 0x1c, 0x2, 0x2, 0x6e, 0x5f, 0x3, 0x2, 0x2, 0x2, 0x6e, 0x62, 
    0x3, 0x2, 0x2, 0x2, 0x6e, 0x65, 0x3, 0x2, 0x2, 0x2, 0x6e, 0x68, 0x3, 
    0x2, 0x2, 0x2, 0x6e, 0x6b, 0x3, 0x2, 0x2, 0x2, 0x6f, 0x72, 0x3, 0x2, 
    0x2, 0x2, 0x70, 0x6e, 0x3, 0x2, 0x2, 0x2, 0x70, 0x71, 0x3, 0x2, 0x2, 
    0x2, 0x71, 0x9, 0x3, 0x2, 0x2, 0x2, 0x72, 0x70, 0x3, 0x2, 0x2, 0x2, 
    0x73, 0x74, 0x7, 0x1c, 0x2, 0x2, 0x74, 0x76, 0x7, 0x5, 0x2, 0x2, 0x75, 
    0x77, 0x5, 0x8, 0x5, 0x2, 0x76, 0x75, 0x3, 0x2, 0x2, 0x2, 0x77, 0x78, 
    0x3, 0x2, 0x2, 0x2, 0x78, 0x76, 0x3, 0x2, 0x2, 0x2, 0x78, 0x79, 0x3, 
    0x2, 0x2, 0x2, 0x79, 0x7a, 0x3, 0x2, 0x2, 0x2, 0x7a, 0x7b, 0x7, 0x6, 
    0x2, 0x2, 0x7b, 0x7e, 0x3, 0x2, 0x2, 0x2, 0x7c, 0x7e, 0x5, 0xc, 0x7, 
    0x2, 0x7d, 0x73, 0x3, 0x2, 0x2, 0x2, 0x7d, 0x7c, 0x3, 0x2, 0x2, 0x2, 
    0x7e, 0xb, 0x3, 0x2, 0x2, 0x2, 0x7f, 0x88, 0x9, 0x5, 0x2, 0x2, 0x80, 
    0x88, 0x7, 0x1c, 0x2, 0x2, 0x81, 0x88, 0x7, 0x1d, 0x2, 0x2, 0x82, 0x88, 
    0x7, 0x1e, 0x2, 0x2, 0x83, 0x84, 0x7, 0x5, 0x2, 0x2, 0x84, 0x85, 0x5, 
    0x8, 0x5, 0x2, 0x85, 0x86, 0x7, 0x6, 0x2, 0x2, 0x86, 0x88, 0x3, 0x2, 
    0x2, 0x2, 0x87, 0x7f, 0x3, 0x2, 0x2, 0x2, 0x87, 0x80, 0x3, 0x2, 0x2, 
    0x2, 0x87, 0x81, 0x3, 0x2, 0x2, 0x2, 0x87, 0x82, 0x3, 0x2, 0x2, 0x2, 
    0x87, 0x83, 0x3, 0x2, 0x2, 0x2, 0x88, 0xd, 0x3, 0x2, 0x2, 0x2, 0x13, 
    0x11, 0x20, 0x27, 0x32, 0x37, 0x3d, 0x40, 0x46, 0x4a, 0x4e, 0x58, 0x5d, 
    0x6e, 0x70, 0x78, 0x7d, 0x87, 
  };

  atn::ATNDeserializer deserializer;
  _atn = deserializer.deserialize(_serializedATN);

  size_t count = _atn.getNumberOfDecisions();
  _decisionToDFA.reserve(count);
  for (size_t i = 0; i < count; i++) { 
    _decisionToDFA.emplace_back(_atn.getDecisionState(i), i);
  }
}

YlangParser::Initializer YlangParser::_init;
