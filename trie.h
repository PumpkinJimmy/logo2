/**
 * @file trie.h
 * @brief �ֵ���
 * @author ���ͽ�
 * @version 0.1
 * @details �ֵ���ʵ�֣�header-only
 */
#pragma once
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <memory>
using namespace std;

/**
 * @brief �ֶ����ڵ�
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
 * @brief �ֵ���
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
	 * @brief ���һ���ַ���ģʽ���ֵ���
	 * @param pat �ַ���ģʽ
	 * @param val ��Ӧģʽ��ֵ����ƥ��ɹ�ʱ���Եõ�
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
	 * @brief ��õ�ǰƥ��ֵ
	 * @details ��õ�ǰƥ��ֵ������δƥ��ɹ����׳��쳣
	 * @return val ƥ��ֵ
	 */
	int getVal() const {
		if (state != Matched) {
			throw "Try to get unmatched pattern value";
		}
		return cur_p->val;
	}
	/**
	 * @brief ƥ��һ���ַ�
	 * @param c ��ƥ���ַ�
	 * @return ƥ���Ƿ�ɹ�
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
	 * @brief ƥ��һ���ַ���
	 * @param s ��ƥ���ַ���
	 * @return ƥ���Ƿ�ɹ�
	 */
	bool fit(const string& s) {
		for (char c : s) {
			if (!fit(c)) {
				return false;
			}
		}
		return true;
	}
	/** ����ƥ�� */
	void reset() {
		cur_p = root;
		state = Matching;
	}
	/** ����ֵ��� */
	void clear() {
		root = make_shared<TrieNode>();
		cur_p = root; state = Matching;
	}
	/** �Ƿ�ƥ�䵽ģʽ */
	bool is_match() { return state == Matched; }
	/** �Ƿ�ʧ�� */
	bool is_fail() { return state == Mismatch; }
	/** �Ƿ�����ƥ�� */
	bool is_matching() { return state == Matching; }
private:
	shared_ptr<TrieNode> root;
	shared_ptr<TrieNode> cur_p;
};