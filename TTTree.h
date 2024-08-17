#pragma once
#include<iostream>
#include<queue>
using namespace std;

template<typename T>
struct Node
{
	bool type;		//true for 3, false for 2
	T key[3];       //预留出4节点的中间态
	Node<T>* p;
	Node<T>* le;
	Node<T>* mi;
	Node<T>* ml;
	Node<T>* mr;
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
	void deepcpy(Node<T>*& to, Node<T>* from);
	void clear(Node<T>* root);
	void insert_adjust(Node<T>* cur);
public:
	TTTree();
	TTTree(Node<T>* root);
	~TTTree();
	Node<T>* get_root() { return root; }
	Node<T>* search(T k);
	Node<T>* maximum(Node<T>* ro);//以ro作为根节点向下查找最值
	Node<T>* minimum(Node<T>* ro);
	bool delete_index(T k);       //未完待续
	bool insert(T k);
	void inorder_walk(Node<T>* root);//中序遍历
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
inline void TTTree<T>::insert_adjust(Node<T>* cur)
{    
	if (cur->p == nullptr)
	{
		T middle = cur->key[1];
		T left = cur->key[0];
		T right = cur->key[2];
		Node<T>* l = cur->le;
		Node<T>* r = cur->ri;
		Node<T>* top = new Node<T>(false, middle);
		Node<T>* lef = new Node<T>(false, left, 0, top,l,nullptr,cur->ml);
		Node<T>* rig = new Node<T>(false, right, 0, top, cur->mr, nullptr, r);
		top->le = lef;
		top->ri = rig;
		if(l!=nullptr)
			l->p = lef;
		if (cur->ml != nullptr)
			cur->ml->p = lef;
		if (cur->mr != nullptr)
			cur->mr->p = rig;
		if(r!=nullptr)
			r->p = rig;
		root = top;
		return;
	}

	if (cur->p->type == false)//更改父节点为三节点 
	{
		Node<T>* tem = cur->p;
		if (tem->le == cur)//左支
		{
			tem->key[1] = tem->key[0];
			tem->key[0] = cur->key[1];
			tem->type = true;
			//分配新的子节点
			Node<T>* newmid = new Node<T>(false, cur->key[2], 0, tem, cur->mr, nullptr, cur->ri);
			cur->type = false;
			cur->ri = cur->ml;
			tem->mi = newmid;
		}
		else//右支
		{
			tem->key[1] = cur->key[1];
			tem->type = true;
			//分配新的子节点
			Node<T>* newmid = new Node<T>(false, cur->key[0], 0, tem, cur->le, nullptr, cur->ml);
			cur->type = false;
			cur->key[0] = cur->key[2];
			cur->le = cur->mr;
			tem->mi = newmid;
		}
		return;
	}

	else//将父节点变成四节点，递归 
	{
		Node<T>* tem = cur->p;
		if (tem->le == cur)
		{
			tem->key[2] = tem->key[1];
			tem->key[1] = tem->key[0];
			tem->key[0] = cur->key[1];
			//分配新的子节点
			Node<T>* newmid = new Node<T>(false, cur->key[2], 0, tem, cur->mr, nullptr, cur->ri);
			tem->mr = tem->mi;
			tem->ml = newmid;
			cur->ri = cur->ml;
			cur->type = false;
		}
		else if (tem->mi == cur)
		{
			tem->key[2] = tem->key[1];
			tem->key[1] = cur->key[1];
			//分配新的子节点
			Node<T>* newmid1 = new Node<T>(false, cur->key[0], 0, tem, cur->le, nullptr, cur->ml);
			Node<T>* newmid2 = new Node<T>(false, cur->key[2], 0, tem, cur->mr, nullptr, cur->ri);
			tem->ml = newmid1;
			tem->mr = newmid2;
		}
		else if (tem->ri == cur)
		{
			tem->key[2] = cur->key[1];
			//分配新的子节点
			Node<T>* newmid = new Node<T>(false, cur->key[0], 0, tem, cur->le, nullptr, cur->ml);
			tem->mr = newmid;
			tem->ml = tem->mi;
			cur->le = cur->mr;
			cur->key[0] = cur->key[2];
			cur->type = false;
		}
		insert_adjust(tem);
	}
}

template<typename T>
inline TTTree<T>::TTTree()
{
	root = nullptr;
}

template<typename T>
inline TTTree<T>::TTTree(Node<T>* r)
{
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
		tem->type = true;
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
		tem->ml = nullptr;
		tem->mr = nullptr;
		insert_adjust(tem);
		return true;
	}
}
template<typename T>
inline void TTTree<T>::inorder_walk(Node<T>* root)
{
	if (root == nullptr)
		return;
	if (root->type == true)
	{
		inorder_walk(root->le);
		cout << root->key[0] << ' ';
		inorder_walk(root->mi);
		cout << root->key[1] << ' ';
		inorder_walk(root->ri);
	}
	else
	{
		inorder_walk(root->le);
		root->seenode();
		cout << "   ";
		inorder_walk(root->ri);
	}
}

template<typename T>
inline void TTTree<T>::see()
{
	if (root == nullptr)
		return;
	queue<Node<T>*> walk;
	walk.push(root);
	walk.push(nullptr);  //means \n
	while (!walk.empty())
	{
		while (walk.front() != nullptr)
		{
			Node<T>* tem = walk.front();
			if (tem->type == true&&tem->le!=nullptr)
			{
				walk.push(tem->le);
				walk.push(tem->mi);
				walk.push(tem->ri);
			}
			else if(tem->type == false && tem->le != nullptr)
			{
				walk.push(tem->le);
				walk.push(tem->ri);
			}
			tem->seenode();
			cout << "   ";
			walk.pop();
		}
		cout << '\n';
		walk.pop();
		if (!walk.empty())
			walk.push(nullptr);
	}
}






