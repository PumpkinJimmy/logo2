#include <iostream>
#include <cstdio>
#include <iostream>
#include "lexer.h"
#include "interpreter.h"
#include "CLI.h"
using namespace std;
int main()
{
	Lexer lexer;
	Token token;
	//lexer.setSource("set myname=bbpumpkin");
	//while (lexer.popFront(token)) {
	//	token.print(cout); cout << ' ';
	//}
	//cout << endl;

	//lexer.setSource("set myid=19351023");
	//while (lexer.popFront(token)) {
	//	token.print(cout); cout << ' ';
	//}
	//cout << endl;
	//lexer.setSource("set $myname=aacucumber");
	//while (lexer.popFront(token)) {
	//	token.print(cout); cout << ' ';
	//}
	//cout << endl;
	//auto ipt = Singleton<Interpreter>::getInstance();
	//ipt->log.addHook("error", [](const string& msg) {cout << msg << endl; });
	//cout << ipt->interprete("set myid=19351023") << endl;
	//cout << ipt->interprete("color 255 255 $myid") << endl;
	//cout << ipt->interprete("turn 30") << endl;
	//cout << ipt->interprete("move 20") << endl;
	//for (auto s : ipt->getErrorInfos()) {
	//	cout << s << endl;
	//}
	CLI cli;
	cli.run();
	return 0;
}