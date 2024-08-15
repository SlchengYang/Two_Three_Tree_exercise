#pragma once
#include<iostream>
using namespace std;

template<typename T>
struct Node
{
	bool type;		//true for 3, false for 2
	T key[3];       //预留出4节点的中间态
	Node<T>* p;
	Node<T>* le;
	Node<T>* mi;
	Node<T>* ri;
	Node(bool t, T k1, T k2=0, Node<T>* p_=nullptr, Node<T>* le_ = nullptr, Node<T>* mi_ = nullptr, Node<T>* ri_ = nullptr) :
		type(t), p(p_), le(le_), mi(mi_), ri(ri_)
	{
		key[0] = k1;
		key[1] = k2;
	}
	Node(bool t, T k[2], Node<T>* p_ = nullptr, Node<T>* le_ = nullptr, Node<T>* mi_ = nullptr, Node<T>* ri_ = nullptr) :
		type(t), p(p_), le(le_), mi(mi_), ri(ri_) 
	{
		key[0] = k[0];
		key[1] = k[1];
	}
	void seenode();
};
template<typename T>
inline void Node<T>::seenode()
{
	if (type == true)
		cout << key[0] << ' ' << key[1];
	else
		cout << key[0];
}






template<typename T>
class TTTree    //indexes must be different, 相同元素的查找尚未解决
{
private:
	Node<T>* root;  //if key==root, k belong to left tree. 
	int height;  //height==-1 means unknown ;
	void deepcpy(Node<T>*& to, Node<T>* from);
	void clear(Node<T>* root);
	void adjust(Node<T>* cur);
public:
	TTTree();
	TTTree(Node<T>* root);
	~TTTree();
	Node<T>* get_root() { return root; }
	int get_height();
	Node<T>* search(T k);
	Node<T>* maximum(Node<T>* ro);//以ro作为根节点向下查找最值
	Node<T>* minimum(Node<T>* ro);
	Node<T>* predecessor(Node<T>* k);
	Node<T>* successor(Node<T>* k);
	bool delete_index(T k);
	bool insert(T k);
	void inorder_walk();//中序遍历
	void see();//层序遍历
};

template<typename T>
inline void TTTree<T>::deepcpy(Node<T>*& to, Node<T>* from)
{
	if (from == nullptr)
		return;
	to = new Node<T>(from->type, from->key, from->p, from->le, from->mi, from->ri);
	deepcpy(to->le, from->le);
	deepcpy(to->mi, from->mi);
	deepcpy(to->ri, from->ri);
}    

template<typename T>
inline void TTTree<T>::clear(Node<T>* root)
{
	if (root == nullptr)
		return;
	clear(root->le);
	clear(root->mi);
	clear(root->ri);
	delete root;
}

template<typename T>
inline void TTTree<T>::adjust(Node<T>* cur)
{    
	if (cur->p == nullptr)
	{
		T middle = cur->key[1];
		T left = cur->key[0];
		T right = cur->key[2];
		Node<T>* l = cur->le;
		Node<T>* r = cur->ri;
		Node<T>* m = cur->mi;
		delete cur;
		Node<T>* top = new Node<T>(false, middle);
		Node<T>* lef = new Node<T>(false, left, 0, top,l,nullptr,m);
		Node<T>* rig = new Node<T>(false, right, 0, top, nullptr, nullptr, r);
		top->le = lef;
		top->ri = rig;
		l->p = lef;
		m->p = lef;
		r->p = rig;
		root = top;
		return;
	}

	if (cur->p->type == false)//更改父节点为三节点 
	{
		Node<T>* tem = cur->p;
		tem->key[1] = tem->key[0];
		tem->key[0] = cur->key[2];
	}

	else//将父节点变成四节点，递归 
	{

	}
}

template<typename T>
inline TTTree<T>::TTTree()
{
	root = nullptr;
	height = -1;
}

template<typename T>
inline TTTree<T>::TTTree(Node<T>* r)
{
	height = -1;
	deepcpy(root, r);
	root->p = nullptr;
}

template<typename T>
inline TTTree<T>::~TTTree()
{
	clear(root);
}

template<typename T>
inline Node<T>* TTTree<T>::search(T k)
{
	Node<T>* tem = root;
	while (tem != nullptr)
	{
		if (tem->type == false)
		{
			if (k < tem->key[0])
				tem = tem->le;
			else if (k > tem->key[0])
				tem = tem->ri;
			else
				return tem;
		}
		if (tem->type == true)
		{
			if (k == tem->key[0] || k == tem->key[1])
				return tem;
			else if (k < tem->key[0])
				tem = tem->le;
			else if (k > tem->key[1])
				tem = tem->ri;
			else
				tem = tem->mi;
		}
	}
	return tem;
}

template<typename T>
inline Node<T>* TTTree<T>::maximum(Node<T>* ro)
{
	if (ro == nullptr)
		return nullptr;
	Node<T>* tem = ro;
	while (tem->ri != nullptr)
	{
		tem = tem->ri;
	}
	return tem;
}

template<typename T>
inline Node<T>* TTTree<T>::minimum(Node<T>* ro)
{
	if (ro == nullptr)
		return nullptr;
	Node<T>* tem = ro;
	while (tem->le != nullptr)
	{
		tem = tem->le;
	}
	return tem;
}

template<typename T>
inline bool TTTree<T>::insert(T k)
{
	if (root == nullptr)
	{
		root = new Node<T>(false, k);
		return true;
	}

	Node<T>* tem = root;//找到树叶
	while (tem->le != nullptr || tem->ri != nullptr || tem->mi != nullptr)
	{
		if (tem->type == true)
		{
			if (k == tem->key[0] || k == tem->key[1])
				return false;
			else if (k < tem->key[0])
				tem = tem->le;
			else if (k > tem->key[1])
				tem = tem->ri;
			else
				tem = tem->mi;
		}
		else
		{
			if (k == tem->key[0])
				return false;
			else if (k < tem->key[0])
				tem = tem->le;
			else if (k > tem->key[0])
				tem = tem->ri;
		}
	}
	//插入
	if (tem->type == false)
	{
		if (tem->key[0] < k)
			tem->key[1] = k;
		else
		{
			tem->key[1] = tem->key[0];
			tem->key[0] = k;
		}
		return true;
	}
	else   //（四节点）
	{
		if (tem->key[0] > k)
		{
			tem->key[2] = tem->key[1];
			tem->key[1] = tem->key[0];
			tem->key[0] = k;
		}
		else if (tem->key[1] < k)
			tem->key[2] = k;
		else
		{
			tem->key[2] = tem->key[1];
			tem->key[1] = k;
		}
		//调整
		adjust(tem);
		return true;
	}
}






