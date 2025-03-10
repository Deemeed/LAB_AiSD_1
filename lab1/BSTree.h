#include <iostream>
#include <algorithm>

using namespace std;

#pragma once

struct Node {
	int _key;
	Node* _left { nullptr };
	Node* _right { nullptr };

	Node(int key) : _key(key) {}
};

class BSTree {
	Node* _root;

	bool erase(Node* &node, int key);
	bool contains(Node* node, int key) const;
	bool insert(Node* &node, int key);
	void print(Node* node) const;
	void destroy(Node* node);
	void copyTree(Node*& node, const Node* other_node);
	Node* findMin(Node* node);
public:
	BSTree() : _root(nullptr) {};

	BSTree(const BSTree& other);
	BSTree operator=(const BSTree& other);
	~BSTree();

	void print() const;
	bool insert(int key);
	bool contains(int key) const;
	bool erase(int key);
};

void BSTree::copyTree(Node*& node, const Node* other_node) {
	if (!other_node) node = nullptr;
	else {
		node = new Node(other_node->_key);
		copyTree(node->_left, other_node->_left);
		copyTree(node->_right, other_node->_right);
	}
}

BSTree::BSTree(const BSTree& other) : _root(nullptr) {
	copyTree(_root, other._root);
}

BSTree BSTree::operator=(const BSTree& other) {
	if (this != &other) {
		destroy(_root);
		copyTree(_root, other._root);
	}

	return *this;
}

void BSTree::destroy(Node* node) {
	if (!node) return;
	destroy(node->_left);
	destroy(node->_right);
	delete node;
}

BSTree::~BSTree() {
	destroy(_root);
}

void BSTree::print(Node* node) const {
	if (!node) return;
	print(node->_left);
	cout << node->_key << "\n";
	print(node->_right);
}

void BSTree::print() const {
	print(_root);
}

bool BSTree::insert(Node* &node, int key) {
	if (node == nullptr) {
		node = new Node(key);
		return true; // Успешная вставка
	}
	if (key == node->_key) {
		return false; // Ключ уже существует
	}
	if (key < node->_key) {
		return insert(node->_left, key);
	}
	else {
		return insert(node->_right, key);
	}
}

bool BSTree::insert(int key) {
	return insert(_root, key);
}

bool BSTree::contains(Node* node, int key) const {
	if (!node) return false;
	if (node->_key == key) return true;
	if (key < node->_key) {
		return contains(node->_left, key);
	}
	else {
		return contains(node->_right, key);
	}
}

bool BSTree::contains(int key) const {
	return contains(_root, key);
}

Node* BSTree::findMin(Node* node) {
	while (node->_left != nullptr) {
		node = node->_left;
	}

	return node;
}

bool BSTree::erase(Node* &node, int key) {
	if (!node) return false;
	if (key < node->_key) {
		return erase(node->_left, key);
	}
	else if (key > node->_key) {
		return erase(node->_right, key);
	}
	else {
		if (!node->_left) {
			Node* tmp = node->_right;
			delete node;
			node = tmp;
			return true;
		}
		else if (!node->_right) {
			Node* tmp = node->_left;
			delete node;
			node = tmp;
			return true;
		}
		else {
			Node* tmp = findMin(node->_right);
			node->_key = tmp->_key;
			return erase(node->_right, tmp->_key);
		}
	}
}

bool BSTree::erase(int key) {
	return erase(_root, key);
}