#pragma once
#ifndef _LEXER_H
#define _LEXER_H
#include <iostream>
#include <string>
#include <cctype>
#include <map>
#include "trie.h"
#define CLEAR_SPACE while (p < src.size() && isspace(src[p])) p++;\
if (p >= src.size()) {\
	p = raw_p;\
	return false;\
}\
using namespace std;
/*
 * Token令牌为词法分析的输出结果，供解释器执行
 * type: 0:literal 1:keyword 2:variable 3:notation
 */
struct Token
{
	enum Type { Null, Str, Int, Float};
	enum {Lit=0, Keywd=1, Var=2, Nota=3};
	string s;
	int i;
	double f;
	Type data_type;
	int type;
	Token() :i(0), f(0), data_type(Null),type(0) {}
	Token(const string& _s) : s(_s), i(0), f(0), data_type(Str),type(0) {}
	Token(int _i) : i(_i), f(0), data_type(Int),type(0) {}
	Token(double _f) : i(0), f(_f), data_type(Float),type(0) {}
	Token(const Token& other) = default;
	Token& operator=(const string& _s) { s = _s; data_type = Str;  return *this; }
	Token& operator=(int _i) { i = _i; data_type = Int; return *this; }
	Token& operator=(double _f) { f = _f; data_type = Float; return *this; }
	Token& operator=(const Token& other) = default;
	operator bool() const{
		return data_type != Null;
	}
	void reset() { data_type = Null; }
	ostream& print(ostream& out) const {
		switch (data_type)
		{
		case Token::Null:
			out << "<Null>"; break;
		case Token::Str:
			out << "<" << type << ":Str \"" << s << "\">"; break;
		case Token::Int:
			out << "<" << type << ":Int " << i << ">";  break;
		case Token::Float:
			out << "<" << type << ":Float " << f << ">"; break;
		default:
			out << "<Unknow>";
		}
		return out;
	}
	friend ostream& operator<<(ostream&, const Token& token);
};
inline ostream& operator<<(ostream& out, const Token& token) { return token.print(out); }
/*
 * 词法分析器，分析逻辑是硬编码的
 */
class Lexer
{
public:
	Lexer(): p(0){
		map<string, int> keyword_table = {
			{"set", 1},
			{"move", 2},
			{"turn", 3},
			{"color", 4},
			{"cloak", 5},
			{"print", 6},
			{"loop", 7},
			{"end", 8},
			{"def", 9},
			{"call", 10}
			};
		map<string, int> notation_table = {
			{"=", 1},
			{"(", 2 },
			{")", 3}
			};
		for (auto& kp : keyword_table) {
			kw_trie.addPattern(kp.first, kp.second);
		}
		for (auto& np : notation_table) {
			notation_trie.addPattern(np.first, np.second);
		}
	}
	void setSource(const string& _src) {
		src = _src;
		p = 0;
	}
	int getPointer() {
		return p;
	}
	void setPointer(int new_p) {
		if (new_p >= src.size()) throw "Lexer Pointer Over the boundary";
		p = new_p;
	}
	bool getToken(Token& token) {
		if (p >= src.size()) return false;
		int raw_p = p;
		while (p < src.size())
		{
			char c = src[p];
			if (isspace(c)) { p++; continue;  }
			else if (isdigit(c)) {
				return getInt(token);
			}
			else if (c == '$') {
				return getVariable(token);
			}
			else if (isalpha(c)) {
				return getStr(token);
			}
			else {
				return getNotation(token);
			}
		}
		p = raw_p;
		return false;
	}
	bool getInt(Token& token)
	{
		int res = 0;
		
		while (p < src.size() && isdigit(src[p])) {
			res = res * 10 + src[p] - '0';
			p++;
		}
		token = res;
		token.type = 0;
		return true;
	}
	bool getStr(Token& token)
	{
		int raw_p = p;
		while (p < src.size() && isspace(src[p])) p++;
		if (p >= src.size()) {
			p = raw_p;
			return false;
		}
		int st_p = p;
		kw_trie.reset();
		if (!isalpha(src[p])) {
			p = raw_p;  return false;
		}
		while (p < src.size() && isalnum(src[p])) {
			kw_trie.fit(src[p]);
			p++;
		}
		token = src.substr(st_p, p - st_p);
		if (kw_trie.is_match()) {
			token.type = 1;
		}
		else token.type = 0;
		return true;
	}
	bool getVariable(Token& token)
	{
		int raw_p = p;
		while (p < src.size() && isspace(src[p])) p++;
		if (p >= src.size()) {
			p = raw_p;
			return false;
		}
		if (p < src.size() && src[p] == '$') {
			p++;
			bool ok = getStr(token);
			if (ok) {
				if (token.type == 1) { //keyword
					p = raw_p;
					return false;
				}
				token.type = 2;
				return true;
			}
			else {
				p = raw_p;
				return false;
			}
		}
		else return false;
	}
	bool getNotation(Token& token) {
		int raw_p = p;
		while (p < src.size() && isspace(src[p])) p++;
		if (p >= src.size()) {
			p = raw_p;
			return false;
		}
		notation_trie.reset();
		token.reset();
		int last_p = p;
		while (p < src.size() && !notation_trie.is_fail())
		{
			notation_trie.fit(src[p]);
			if (notation_trie.is_fail()) {
				p = last_p;
			}
			else {
				p++;
				if (notation_trie.is_match())
				{
					token = notation_trie.getVal();
					token.type = 3;
					last_p = p;
				}
			}
		}
		if (!token) {
			p = raw_p;
			return false;
		}
		else {
			return true;
		}
	}
private:
	string src;
	int p;
	Trie kw_trie;
	Trie notation_trie;
	//keywords trie
};

#endif