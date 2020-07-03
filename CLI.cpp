#include <string>
#include "CLI.h"
#include "interpreter.h"
void CLI::run()
{
	auto ipt = Singleton<Interpreter>::getInstance();
	string s;
	cout << "+-------------------------------+" << endl
		<<  "|                               |" << endl
		<<  "| Welcome to BBLogo Interpreter |" << endl
		<<  "|                               |" << endl
		<<  "+-------------------------------+" << endl;
	while (1)
	{
		cout << "\n>> ";
		getline(cin, s);
		ipt->interprete(s);
	}
}
