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
	{"color", HANDLER(ColorHandler)},
	{"cloak", HANDLER(CloakHandler)},
	{"move", HANDLER(MoveHandler)},
	{"turn", HANDLER(TurnHandler)}
};

RootHandler::RootHandler()
{
}

bool RootHandler::handle()
{
	auto ipt = Singleton<Interpreter>::getInstance();
	int nxt_p; Token token; 
	if (!ipt->getNextToken(token, nxt_p))
	{
		ipt->log.error("Invalid syntax");
		return false;
	}
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
			ipt->log.error("Unknown keyword");
			//ipt->getErrorLogger() << "Unknown keyword: " << token.s << " at Row" << r+1 << ", Column" << c+1;
			return false;
		}
	}
	else {
		ipt->log.error("Invalid identification");
		//ipt->getErrorLogger() << "Invalid token: " << token;
		return false;
	}
}

bool SetHandler::handle()
{
	auto ipt = Singleton<Interpreter>::getInstance();
	int nxt_p; Token token; 
	if (!ipt->getNextToken(token, nxt_p))
	{
		ipt->log.error("Invalid syntax");
		return false;
	}
	int r, c; ipt->getIR(r, c);
	Token res;
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
			ipt->log.error("Undefined variable");
			//ipt->getErrorLogger() << "Undefined variable name:" << token << " at Row" << r+1 << " Column" << c+1;
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
			ipt->log.error("Invalid identification");
			//ipt->getErrorLogger() << "Missing notation \"=\" " << " at Row" << r + 1 << " Column" << c + 1;
			state = NAME;
			return false;
		}
	case VAL:
		if (ipt->transformVar(token, res) && res.data_type == Token::Int)
		{
			ipt->setVal(name, res.i);
			state = NAME;
			ipt->setIRPoint(nxt_p);
			ipt->popHandler();
		}
		else
		{
			ipt->log.error("Invalid variable value, only int accepted");
			//ipt->getErrorLogger() << "Invalid identifer at Row" << r + 1 << " Column" << c + 1;
			state = NAME;
			return false;
		}
	}
}

bool ColorHandler::handle()
{
	auto ipt = Singleton<Interpreter>::getInstance();
	int nxt_p; Token token;
	if (!ipt->getNextToken(token, nxt_p))
	{
		ipt->log.error("Invalid syntax");
		return false;
	}
	int r, c; ipt->getIR(r, c);

	if (cnt > 2) {
		ipt->log.error("Too many argument");
		//ipt->getErrorLogger() << "color: Too many argument";
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
			ipt->log.error("Invalid data type for, expect int");
			//ipt->getErrorLogger() << "color: Invalid data type for, expect int";
			return false;
		}
	}
	else if (token.type == Token::Var)
	{
		if (!ipt->getVal(token.s, v))
		{
			ipt->log.error("Undefined variable");
			//ipt->getErrorLogger() << "Undefined variable";
			return false;
		}
	}
	else {
		ipt->log.error("Invalid identification");
		//ipt->getErrorLogger() << "color: Invalid identification";
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

bool CloakHandler::handle()
{
	auto ipt = Singleton<Interpreter>::getInstance();
	int nxt_p; Token token;
	if (!ipt->getNextToken(token, nxt_p))
	{
		ipt->log.error("Invalid syntax");
		return false;
	}
	if (token.data_type == Token::Str && token.type == Token::Lit)
	{
		if (token.s == "on") ipt->turtle.is_cloak = true;
		else if (token.s == "off") ipt->turtle.is_cloak = false;
		else {
			ipt->log.error("Invalid identification, only on/off accepted");  return false;
		}
		ipt->setIRPoint(nxt_p);
		ipt->popHandler();
		return true;
	}
	else {
		ipt->log.error("Invalid identification, only on/off accepted");
		//ipt->getErrorLogger() << "cloak: Invalid identification, only on/off accepted";
		return false;
	}
}

bool MoveHandler::handle()
{
	auto ipt = Singleton<Interpreter>::getInstance();
	int nxt_p; Token token; 
	if (!ipt->getNextToken(token, nxt_p))
	{
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
			ipt->log.error("Invalid data type");
			//ipt->getErrorLogger() << "move: Invalid data type";
			return false;
		}
	}
	else if (token.type == Token::Var)
	{
		if (!ipt->getVal(token.s, v))
		{
			ipt->log.error("Undefined variable");
			//ipt->getErrorLogger() << "Undefined variable";
			return false;
		}
	}
	else {
		ipt->log.error("Invalid identification");
		//ipt->getErrorLogger() << "move: Invalid identification";
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
	ipt->popHandler();

	return true;
}

bool TurnHandler::handle()
{
	auto ipt = Singleton<Interpreter>::getInstance();
	int nxt_p; Token token;
	if (!ipt->getNextToken(token, nxt_p))
	{
		ipt->log.error("Invalid syntax");
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
			ipt->log.error("Invalid data type");
			//ipt->getErrorLogger() << "turn: Invalid data type";
			return false;
		}
	}
	else if (token.type == Token::Var)
	{
		if (!ipt->getVal(token.s, v))
		{
			ipt->log.error("Undefined variable");
			//ipt->getErrorLogger() << "Undefined variable";
			return false;
		}
	}
	else {
		ipt->log.error("Invalid identification");

		//ipt->getErrorLogger() << "turn: Invalid identification";
		return false;
	}
	auto& turtle = ipt->turtle;
	turtle.angle += v;
	ipt->setIRPoint(nxt_p);
	ipt->popHandler();
	return true;
}
