#include <iostream>
#include <cstdio>
#include <iostream>
#include "lexer.h"
#include "interpreter.h"
using namespace std;

int main()
{
	Lexer lexer;
	Token token;
	lexer.setSource("set myname=bbpumpkin");
	while (lexer.getToken(token)) {
		token.print(cout); cout << ' ';
	}
	cout << endl;

	lexer.setSource("set myid=19351023");
	while (lexer.getToken(token)) {
		token.print(cout); cout << ' ';
	}
	cout << endl;
	lexer.setSource("set $myname=aacucumber");
	while (lexer.getToken(token)) {
		token.print(cout); cout << ' ';
	}
	cout << endl;
	auto ipt = Singleton<Interpreter>::getInstance();
	cout << ipt->interprete("set myid=19351023") << endl;
	return 0;
}