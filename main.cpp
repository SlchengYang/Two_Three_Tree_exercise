#include"TTTree.h"
#include<iostream>
using namespace std;
int main()
{
	Node<int> a(true, 1, 2);
	int arr[2] = { 4,5 };
	Node<int> b(false, arr);
	a.seenode();
	cout << endl;
	b.seenode();
}