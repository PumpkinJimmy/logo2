#pragma once
#ifndef _HANDLER_H
#define _HANDLER_H
#include <memory>
#include <string>
#include <map>
#include <array>
#include "interpreter.h"
#include "lexer.h"
using namespace std;

/*
 * Handler 处理器的基类
 * 负责语法分析和处理，完成解释执行的逻辑
 * 输入token
 * 实现了Prototype模式
 * 功能上为Strategy模式
 */
class Handler
{
public:
	virtual bool handle() = 0;
	virtual string getName() const = 0;
	virtual shared_ptr<Handler> clone() = 0;
};
using HandlerPtr = shared_ptr<Handler>;

/*
 * 根handler，负责按关键字调用Handler
 * 基于Strategy模式
 */
class RootHandler : public Handler
{
public:
	static map<string, HandlerPtr> handler_map;
	RootHandler();
	virtual string getName() const { return "root"; }
	virtual HandlerPtr clone() {
		return make_shared<RootHandler>();
	}
	virtual bool handle();
private:
};
class SetHandler: public Handler
{
public:

	SetHandler() : state(NAME) {
	}
	virtual bool handle();
	virtual string getName() const { return "set"; }
	virtual shared_ptr<Handler> clone() {
		return make_shared<SetHandler>();
	}
private:
	enum State{NAME, EQ, VAL, ERR} state;
	string name;
};

class ColorHandler : public Handler
{
public:
	ColorHandler():cnt(0) {
	}
	virtual bool handle();
	virtual string getName() const { return "color"; }
	virtual shared_ptr<Handler> clone() {
		return make_shared<ColorHandler>();
	}
private:
	int cnt;
	array<int, 3> color;
};

class CloakHandler : public Handler
{
public:
	CloakHandler() {}
	virtual bool handle();
	virtual string getName() const { return "cloak"; }
	virtual shared_ptr<Handler> clone() {
		return make_shared<CloakHandler>();
	}
};
class MoveHandler : public Handler
{
public:
	MoveHandler() {}
	virtual bool handle();
	virtual string getName() const { return "move"; }
	virtual shared_ptr<Handler> clone() {
		return make_shared<MoveHandler>();
	}
};
class TurnHandler : public Handler
{
public:
	TurnHandler() {}
	virtual bool handle();
	virtual string getName() const { return "turn"; }
	virtual shared_ptr<Handler> clone() {
		return make_shared<TurnHandler>();
	}
};
#endif