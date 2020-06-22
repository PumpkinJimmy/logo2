#pragma once
#ifndef _INTERPRETER_H
#define _INTERPERTER_H
#include <memory>
#include <string>
#include <map>
#include <sstream>
#include <stack>
#include <vector>
#include "lexer.h"
using namespace std;

/*
 * 单例类模版
 */
template <typename T>
class Singleton
{
public:
	static shared_ptr<T> getInstance() {
		return T::instance;
	}
};

/*
 * 解释器类
 * 运行主循环，总管所有Handler和Context
 * 向前端开放的解释执行的接口类
 * 单例模式，全局访问
 */
using VarContext = map<string, int>;
class Handler;
vector<string> split(const string& s);
class Interpreter
{
public:
	friend Singleton<Interpreter>;
	bool interprete(const string& s);
	void reset();
	void resetErrMsg();
	void getIR(int& r, int& c) const;
	void setIR(int r, int c);
	bool getVal(const string& name, int& val);
	void setVal(const string& name, int val);
	void pushHandler(shared_ptr<Handler> h);
	void switchHandler(shared_ptr<Handler> h);
	void popHandler();
	void pushVarContext(VarContext ctx);
	void popVarContext(VarContext& ctx);
	int getRowCount() const;
	int getRowSize() const;
	void setErrorInfo(const string& s) {
		err_infos.push_back(s);
	}
	const vector<string>& getErrorInfos() {
		if (!err_ss.empty()) {
			for (auto& ss : err_ss) {
				err_infos.push_back(ss.str());
			}
			err_ss.clear();
		}
	}
	void clearErrorInfo() {
		err_infos.clear();
		err_ss.clear();
	}
	stringstream& getErrorLogger() {
		err_ss.push_back(stringstream());
		return err_ss[err_ss.size() - 1];
	}
private:
	int ir_row;
	int ir_point;
	Lexer lexer;
	vector<string> srcs;
	vector<string> err_infos;
	vector<stringstream> err_ss;
	stack<shared_ptr<Handler> > handlers;
	stack<VarContext> val_contexts;
	Interpreter();
	static shared_ptr<Interpreter> instance;
};
using InterpreterPtr = shared_ptr<Interpreter>;
#endif // !_INTERPRETER_H
