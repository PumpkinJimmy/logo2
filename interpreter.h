#pragma once
#ifndef _INTERPRETER_H
#define _INTERPERTER_H
#include <memory>
#include <string>
#include <map>
#include <sstream>
#include <stack>
#include <vector>
#include <queue>
#include <functional>
#include "lexer.h"
#include "context.h"
#include "canvas.h"
using namespace std;

/*
 * ������ģ��
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
 * @brief ��������
 * ������ѭ�����ܹ�����Handler��Context
 * ��ǰ�˿��ŵĽ���ִ�еĽӿ���
 * ����ģʽ��ȫ�ַ���
 */
using VarContext = map<string, int>;
class Handler;
vector<string> split(const string& s);
class Interpreter
{
public:
	friend Singleton<Interpreter>;
	struct Logger
	{
		using Callback = function<void(const string&)>;
		void log(const string& level, const string& name, int row, int col, const string& msg);
		void info(const string& msg);
		void warn(const string& msg);
		void error(const string& msg);
		void addHook(const string& level, Callback callback)
		{
			hooks[level].push_back(callback);
		}
		map<string, vector<Callback> > hooks;
		string last_log;
	} log;
	/**
	 * @brief ����ִ�д���
	 * @param s ִ�еĴ���
	 * @return ִ���Ƿ�ɹ�
	 */
	bool interprete(const string& s);
	void reset();
	void resetErrMsg();
	void getIR(int& r, int& c) const;
	void setIR(int r, int c);
	void setIRPoint(int c);
	bool getVal(const string& name, int& val);
	void setVal(const string& name, int val);
	bool transformVar(const Token& token, Token& res);
	void pushHandler(shared_ptr<Handler> h);
	void switchHandler(shared_ptr<Handler> h);
	const shared_ptr<Handler> getTopHandler() const;
	void popHandler();
	void pushVarContext(VarContext ctx);
	void popVarContext(VarContext& ctx);
	int getRowCount() const;
	int getRowSize() const;
	bool getNextToken(Token& token, int& nxt_p);
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
		return err_infos;
	}
	void clearErrorInfo() {
		err_infos.clear();
		err_ss.clear();
	}
	stringstream& getErrorLogger() {
		err_ss.push_back(stringstream());
		return err_ss[err_ss.size() - 1];
	}
	Turtle turtle;
	CanvasPtr canvas;
private:
	int ir_row; ///< ����ִ�е���
	int ir_point; ///< ����ִ�е�λ��
	bool finished;
	Lexer lexer; ///< �ʷ�������
	vector<string> srcs;
	vector<string> err_infos;
	vector<stringstream> err_ss;
	stack<shared_ptr<Handler> > handlers;
	stack<VarContext> val_contexts;
	Interpreter();
	static shared_ptr<Interpreter> instance;
	queue<Token> ex_tokens;
};
using InterpreterPtr = shared_ptr<Interpreter>;
#endif // !_INTERPRETER_H
