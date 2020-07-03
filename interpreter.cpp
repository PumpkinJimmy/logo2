#include <memory>
#include "interpreter.h"
#include "handler.h"

InterpreterPtr Interpreter::instance = shared_ptr<Interpreter>(new Interpreter);
Interpreter::Interpreter() :ir_row(0), ir_point(0), finished(false)
{
	val_contexts.push(VarContext());
	handlers.push(make_shared<RootHandler>());
	canvas = make_shared<DummyCanvas>();
}
int Interpreter::getRowCount() const
{
	return srcs.size();
}

int Interpreter::getRowSize() const
{
	return srcs[ir_row].size();
}

bool Interpreter::getNextToken(Token& token, int& nxt_p)
{
	if (!finished && lexer.popFront(token))
	{
		nxt_p = lexer.getPointer();
		return true;
	}
	else
		return false;
}

bool Interpreter::interprete(const string& s)
{
	finished = false;
	if (s.empty()) return false;
	srcs = split(s);
	ir_row = 0;
	ir_point = 0;
	lexer.setSource(srcs[0]);
	while (ir_row < srcs.size() && ir_point < srcs[ir_row].size())
	{
		if (!handlers.top()->handle())
		{
			while (handlers.size() > 1)
			{
				handlers.pop();
			}
			return false;
		}
	}
	return true;
}
void Interpreter::reset()
{
	ir_row = 0;
	ir_point = 0;
	err_infos.clear();
	err_ss.clear();
	while (!val_contexts.empty()) val_contexts.pop();
	while (!handlers.empty()) handlers.pop();

	val_contexts.push(VarContext());
	handlers.push(make_shared<RootHandler>());
}
void Interpreter::resetErrMsg()
{
	err_infos.clear();
	err_ss.clear();
}
void Interpreter::getIR(int& r, int& c) const {
	r = ir_row;
	c = ir_point;
}
void Interpreter::setIR(int r, int c) {
	if (r == srcs.size())
	{
		finished = true;
		return;
	}
	if (r > srcs.size() || c > srcs[r].size())
	{
		throw "Invalid ir setting: (" + to_string(r) + ", " + to_string(c) + ")";
	}
	else if (c == srcs[r].size()) {
		if (r == srcs.size() - 1) { ir_row = r; ir_point = c; return; }
		r++; c = 0;
	}
	if (r != ir_row) {
		lexer.setSource(srcs[r]);
	}
	lexer.setPointer(c);
	ir_row = r;
	ir_point = c;
}
void Interpreter::setIRPoint(int c)
{
	setIR(ir_row, c);
}
bool Interpreter::getVal(const string& name, int& val)
{
	auto& vctx = val_contexts.top();
	if (!vctx.count(name)) {
		return false;
	}
	else {
		val = vctx[name]; return true;
	}
}

void Interpreter::setVal(const string& name, int val)
{
	auto& vctx = val_contexts.top();
	vctx[name] = val;
}

bool Interpreter::transformVar(const Token& token, Token& res)
{
	if (token.type == Token::Var)
	{
		int val;
		if (getVal(token.s, val))
		{
			res.data_type = Token::Int;
			res.type = Token::Lit;
			res.i = val;
			return true;
		}
		else return false;
	}
	else if (token.type == Token::Lit)
	{
		res = token;
		return true;
	}
	else return false;
}


void Interpreter::pushHandler(shared_ptr<Handler> h)
{
	handlers.push(h);
}

void Interpreter::popHandler()
{
	handlers.pop();
}

void Interpreter::pushVarContext(VarContext ctx)
{
	val_contexts.push(ctx);
}

void Interpreter::popVarContext(VarContext& ctx)
{
	ctx = val_contexts.top();
	val_contexts.pop();
}

/** 多行切分为字符串数组 */
vector<string> split(const string& s)
{
	int i = 0, last_p = 0;
	vector<string> res;
	while (i < s.size())
	{
		while (i < s.size() && s[i] != '\n') i++;
		res.push_back(s.substr(last_p, i - last_p));
		if (i < s.size()) {
			last_p = ++i;
		}
	}
	return res;
}
const HandlerPtr Interpreter::getTopHandler() const
{
	return handlers.top();
}
void Interpreter::Logger::log(const string& level, const string& name, int row, int col, const string& msg)
{
	stringstream ss;
	ss << '[' << level << ']' << "<" << name << ">" << ": " << "Near row " << row+1 << ", column " << col+1 << ", " << msg;
	last_log = ss.str();
}
#define LOG_METHOD(lvl) \
void Interpreter::Logger::##lvl(const string& msg) \
{ \
	auto ipt = Singleton<Interpreter>::getInstance(); \
	int row, col; ipt->getIR(row, col); \
	log(#lvl, ipt->getTopHandler()->getName(), row, col, msg); \
	if (hooks.count(#lvl))\
	for (auto& callback: hooks[#lvl]) \
		callback(last_log); \
}
LOG_METHOD(error)
LOG_METHOD(warn)
LOG_METHOD(info)
