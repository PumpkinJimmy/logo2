#include <memory>
#include "interpreter.h"
#include "handler.h"

InterpreterPtr Interpreter::instance = shared_ptr<Interpreter>(new Interpreter);
Interpreter::Interpreter() :ir_row(0), ir_point(0)
{
	val_contexts.push(VarContext());
	handlers.push(make_shared<RootHandler>());
}
int Interpreter::getRowCount() const
{
	return srcs.size();
}

int Interpreter::getRowSize() const
{
	return srcs[ir_row].size();
}

bool Interpreter::interprete(const string& s)
{
	if (s.empty()) return false;
	srcs = split(s);
	ir_row = 0;
	ir_point = 0;
	lexer.setSource(srcs[0]);
	Token token;
	while (ir_row < srcs.size() && ir_point < srcs[ir_row].size() && lexer.getToken(token))
	{
		if (!handlers.top()->handle(token, lexer.getPointer()))
		{
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
	if (r >= srcs.size() || c > srcs[r].size())
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

void Interpreter::pushHandler(shared_ptr<Handler> h)
{
	handlers.push(h);
}

void Interpreter::popHandler()
{
	handlers.pop();
}

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
