#pragma once
#ifndef _HANDLER_H
#define _HANDLER_H
#include <memory>
#include <string>
#include <map>
#include "interpreter.h"
#include "lexer.h"
using namespace std;

/*
 * Handler �������Ļ���
 * �����﷨�����ʹ�����ɽ���ִ�е��߼�
 * ����token
 * ʵ����Prototypeģʽ
 * ������ΪStrategyģʽ
 */
class Handler
{
public:
	virtual bool handle(const Token& token, int nxt_p) = 0;
	virtual shared_ptr<Handler> clone() = 0;
};
using HandlerPtr = shared_ptr<Handler>;

/*
 * ��handler�����𰴹ؼ��ֵ���Handler
 * ����Strategyģʽ
 */
class RootHandler : public Handler
{
public:
	static map<string, HandlerPtr> handler_map;
	RootHandler();
	virtual HandlerPtr clone() {
		return make_shared<RootHandler>();
	}
	virtual bool handle(const Token& token, int nxt_p);
private:
};
class SetHandler: public Handler
{
public:
	SetHandler() : state(NAME) {
	}
	virtual bool handle(const Token& token, int nxt_p);
	virtual shared_ptr<Handler> clone() {
		return make_shared<SetHandler>();
	}
private:
	enum State{NAME, EQ, VAL, ERR} state;
	string name;
};
#endif