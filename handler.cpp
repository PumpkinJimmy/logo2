#include "handler.h"
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
	if (token && token.type == 1) {
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
		if (token.data_type == Token::Str && token.type == 0) {
			name = token.s;
			state = EQ;
			ipt->setIR(r, nxt_p);
			return true;
		}
		else {
			ipt->getErrorLogger() << "Invalid variable name:" << token << " at Row" << r+1 << " Column" << c+1 << endl;
			state = ERR;
			return false;
		}
	case EQ:
		if (token.type == 3 && token.i == notation_table["="]) {
			state = VAL;
			ipt->setIR(r, nxt_p);
			return true;
		}
		else {
			ipt->getErrorLogger() << "Missing notation \"=\" " << " at Row" << r + 1 << " Column" << c + 1 << endl;
			state = NAME;
			return false;
		}
	case VAL:
		if (token.type == 0)
		{
			if (token.data_type == Token::Int) {
				ipt->setVal(name, token.i);
				state = NAME;
				ipt->setIR(r, nxt_p);
				ipt->popHandler();
				return true;
			}
			else {
				ipt->getErrorLogger() << "Invalid data type of variable at Row" << r + 1 << " Column" << c + 1 << endl;
				state = NAME;
				return false;
			}
		}
		else if (token.type == 2) //variable
		{
			int v; 
			if (!ipt->getVal(token.s, v)) {
				ipt->getErrorLogger() << "Invalid variable name at Row" << r + 1 << " Column" << c + 1 << endl;
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
			ipt->getErrorLogger() << "Invalid identifer at Row" << r + 1 << " Column" << c + 1 << endl;
			state = NAME;
			return false;
		}
	}
}