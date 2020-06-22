#pragma once
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <memory>
using namespace std;

/*
 * ×Ö¶ÎÊ÷½Úµã
 */
struct TrieNode
{
	shared_ptr<TrieNode> nxt[256];
	int val;
	bool is_leaf = false;
	TrieNode(int _val=0):val(_val) {
	}
	TrieNode(const TrieNode& other) {
		val = other.val;
		memcpy(nxt, other.nxt, sizeof(other.nxt));
	}
	TrieNode& operator=(const TrieNode& other) {
		val = other.val;
		for (int i = 0; i < 256; i++) {
			nxt[i] = other.nxt[i];
		}
	}
	
};
class Trie
{
public:
	Trie() { 
		root = make_shared<TrieNode>();
		cur_p = root; state = Matching;  
	}
	enum State { Matching, Matched, Mismatch } state;
	void addPattern(const string& pat, int val){
		shared_ptr<TrieNode> p = root, nxt;
		for (char c : pat) {
			nxt = p->nxt[c];
			if (!nxt) nxt = p->nxt[c] = make_shared<TrieNode>();
			p = nxt;
		}
		p->val = val;
		p->is_leaf = true;
	}
	
	int getVal() const {
		if (state != Matched) {
			throw "Try to get unmatched pattern value";
		}
		return cur_p->val;
	}
	bool fit(char c) {
		if (is_fail()) return false;
		cur_p = cur_p->nxt[c];
		if (!cur_p) {
			state = Mismatch;
			return false;
		}
		else if (cur_p->is_leaf) {
			state = Matched;
			return true;
		}
		else {
			state = Matching;
			return true;
		}
	}
	bool fit(const string& s) {
		for (char c : s) {
			if (!fit(c)) {
				return false;
			}
		}
		return true;
	}
	void reset() {
		cur_p = root;
		state = Matching;
	}
	bool is_match() { return state == Matched; }
	bool is_fail() { return state == Mismatch; }
	bool is_matching() { return state == Matching; }
private:
	shared_ptr<TrieNode> root;
	shared_ptr<TrieNode> cur_p;
};