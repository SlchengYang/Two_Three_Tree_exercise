#include"TTTree.h"
#include<iostream>
using namespace std;
int main()
{
	int n;
	cin >> n;
	TTTree<int> tree;
	while (n--)
	{
		int tem;
		cin >> tem;
		tree.insert(tem);
	}
	tree.see();
	tree.inorder_walk(tree.get_root());
}