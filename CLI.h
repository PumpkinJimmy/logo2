#pragma once
#ifndef _CLI_H
#define _CLI_H
#include <string>
#include <functional>
#include "interpreter.h"
using namespace std;
class CLI
{
public:
	CLI() {
		auto ipt = Singleton<Interpreter>::getInstance();
		ipt->log.addHook("info", bind(&CLI::printInfo, this, placeholders::_1));
		ipt->log.addHook("warn", bind(&CLI::printWarn, this, placeholders::_1));
		ipt->log.addHook("error", bind(&CLI::printError, this, placeholders::_1));
	}
	void run();
	void printInfo(const string& msg)
	{
		cout << msg << endl;
	}
	void printWarn(const string& msg)
	{
		cout << msg << endl;
	}
	void printError(const string& msg)
	{
		cerr << msg << endl;
	}
};

#endif // !_CLI_H
