/**
 * @file trie.h
 * @brief 字典树
 * @author 邓贤杰
 * @version 0.1
 * @details 字典树实现，header-only
 */
#pragma once
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <memory>
using namespace std;

/**
 * @brief 字段树节点
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

/**
 * @brief 字典树
 */
class Trie
{
public:
	Trie() { 
		root = make_shared<TrieNode>();
		cur_p = root; state = Matching;  
	}
	enum State { Matching, Matched, Mismatch } state;
	/**
	 * @brief 添加一个字符串模式到字典树
	 * @param pat 字符串模式
	 * @param val 对应模式的值，在匹配成功时可以得到
	 */
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
	
	/**
	 * @brief 获得当前匹配值
	 * @details 获得当前匹配值，若尚未匹配成功则抛出异常
	 * @return val 匹配值
	 */
	int getVal() const {
		if (state != Matched) {
			throw "Try to get unmatched pattern value";
		}
		return cur_p->val;
	}
	/**
	 * @brief 匹配一个字符
	 * @param c 待匹配字符
	 * @return 匹配是否成功
	 */
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
	/**
	 * @brief 匹配一个字符串
	 * @param s 待匹配字符串
	 * @return 匹配是否成功
	 */
	bool fit(const string& s) {
		for (char c : s) {
			if (!fit(c)) {
				return false;
			}
		}
		return true;
	}
	/** 重置匹配 */
	void reset() {
		cur_p = root;
		state = Matching;
	}
	/** 清空字典树 */
	void clear() {
		root = make_shared<TrieNode>();
		cur_p = root; state = Matching;
	}
	/** 是否匹配到模式 */
	bool is_match() { return state == Matched; }
	/** 是否失配 */
	bool is_fail() { return state == Mismatch; }
	/** 是否正在匹配 */
	bool is_matching() { return state == Matching; }
private:
	shared_ptr<TrieNode> root;
	shared_ptr<TrieNode> cur_p;
};