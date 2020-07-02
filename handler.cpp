#include "handler.h"
#include "utils.h"
#define HANDLER(name) make_shared<name>()
map<string, int> notation_table = {
			{"=", 1},
			{"(", 2 },
			{")", 3}
};
map<string, HandlerPtr> RootHandler::handler_map = {
	{"set", HANDLER(SetHandler)},
};

RootHandler::RootHandler()
{
}

bool RootHandler::handle(const Token& token, int nxt_p)
{
	auto ipt = Singleton<Interpreter>::getInstance();
	if (token && token.type == Token::Keywd) {
		int r, c; ipt->getIR(r, c);
		if (handler_map.count(token.s)) {
			auto handler = handler_map[token.s]->clone();
			ipt->pushHandler(handler);
			if (nxt_p >= ipt->getRowSize())
				ipt->setIR(r + 1, 0);
			else
				ipt->setIR(r, nxt_p);
			return true;
		}
		else {
			ipt->getErrorLogger() << "Unknown keyword: " << token.s << " at Row" << r+1 << ", Column" << c+1;
			return false;
		}
	}
	else {
		ipt->getErrorLogger() << "Invalid token: " << token;
		return false;
	}
}

bool SetHandler::handle(const Token& token, int nxt_p)
{
	auto ipt = Singleton<Interpreter>::getInstance();
	int r, c; ipt->getIR(r, c);
	switch (state)
	{
	case ERR:
		return false;
	case NAME:
		if (token.data_type == Token::Str && token.type == Token::Lit) {
			name = token.s;
			state = EQ;
			ipt->setIR(r, nxt_p);
			return true;
		}
		else {
			ipt->getErrorLogger() << "Undefined variable name:" << token << " at Row" << r+1 << " Column" << c+1;
			state = ERR;
			return false;
		}
	case EQ:
		if (token.type == Token::Nota && token.i == notation_table["="]) {
			state = VAL;
			ipt->setIR(r, nxt_p);
			return true;
		}
		else {
			ipt->getErrorLogger() << "Missing notation \"=\" " << " at Row" << r + 1 << " Column" << c + 1;
			state = NAME;
			return false;
		}
	case VAL:
		if (token.type == Token::Lit)
		{
			if (token.data_type == Token::Int) {
				ipt->setVal(name, token.i);
				state = NAME;
				ipt->setIR(r, nxt_p);
				ipt->popHandler();
				return true;
			}
			else {
				ipt->getErrorLogger() << "Invalid data type of variable at Row" << r + 1 << " Column" << c + 1;
				state = NAME;
				return false;
			}
		}
		else if (token.type == Token::Var) //variable
		{
			int v; 
			if (!ipt->getVal(token.s, v)) {
				ipt->getErrorLogger() << "Invalid variable name at Row" << r + 1 << " Column" << c + 1;
				state = NAME;
				return false;
			}
			ipt->setVal(name, v);
			state = NAME;
			ipt->popHandler();
			return true;
		}
		else
		{
			ipt->getErrorLogger() << "Invalid identifer at Row" << r + 1 << " Column" << c + 1;
			state = NAME;
			return false;
		}
	}
}

bool ColorHandler::handle(const Token& token, int nxt_p)
{
	auto ipt = Singleton<Interpreter>::getInstance();
	int r, c; ipt->getIR(r, c);
	if (cnt > 2) {
		ipt->getErrorLogger() << "color: Too many argument";
		return false;
	}
	int v = 0;
	if (token.type == Token::Lit)
	{
		if (token.data_type == Token::Int)
		{
			v = token.i;
		}
		else {
			ipt->getErrorLogger() << "color: Invalid data type for, expect int";
			return false;
		}
	}
	else if (token.type == Token::Var)
	{
		if (!ipt->getVal(token.s, v))
		{
			ipt->getErrorLogger() << "Undefined variable";
			return false;
		}
	}
	else {
		ipt->getErrorLogger() << "color: Invalid identification";
		return false;
	}
	color[cnt++] = v;
	if (cnt == 3)
	{
		ipt->turtle.color = color;
		ipt->popHandler();
	}
	ipt->setIRPoint(nxt_p);
	return true;
}

bool CloakHandler::handle(const Token& token, int nxt_p)
{
	auto ipt = Singleton<Interpreter>::getInstance();
	if (token.data_type == Token::Str && token.type == Token::Lit)
	{
		ipt->turtle.is_cloak = (token.s == "on");
		ipt->setIRPoint(nxt_p);
		return true;
	}
	else {
		ipt->getErrorLogger() << "cloak: Invalid identification, only on/off accepted";
		return false;
	}
}

bool MoveHandler::handle(const Token& token, int nxt_p)
{
	auto ipt = Singleton<Interpreter>::getInstance();
	int v = 0;
	if (token.type == Token::Lit)
	{
		if (token.data_type == Token::Int)
		{
			v = token.i;
		}
		else {
			ipt->getErrorLogger() << "move: Invalid data type";
			return false;
		}
	}
	else if (token.type == Token::Var)
	{
		if (!ipt->getVal(token.s, v))
		{
			ipt->getErrorLogger() << "Undefined variable";
			return false;
		}
	}
	else {
		ipt->getErrorLogger() << "move: Invalid identification";
		return false;
	}
	auto& turtle = ipt->turtle;
	int len = v;
	auto st = turtle.pos;
	auto ed = move_len(st, turtle.angle, len);
	if (!turtle.is_cloak)
		ipt->canvas->drawLine(Point{ st[0], st[1] }, Point{ ed[0], ed[1] }, turtle.color);
	turtle.pos = ed;
	ipt->setIRPoint(nxt_p);
	return true;
}

bool TurnHandler::handle(const Token& token, int nxt_p)
{
	auto ipt = Singleton<Interpreter>::getInstance();
	int v = 0;
	if (token.type == Token::Lit)
	{
		if (token.data_type == Token::Int)
		{
			v = token.i;
		}
		else {
			ipt->getErrorLogger() << "turn: Invalid data type";
			return false;
		}
	}
	else if (token.type == Token::Var)
	{
		if (!ipt->getVal(token.s, v))
		{
			ipt->getErrorLogger() << "Undefined variable";
			return false;
		}
	}
	else {
		ipt->getErrorLogger() << "turn: Invalid identification";
		return false;
	}
	auto& turtle = ipt->turtle;
	turtle.angle += v;
	ipt->setIRPoint(nxt_p);
	return true;
}
