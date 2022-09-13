#pragma once
#include<iostream>
using namespace std;
#include<string>

bool intcheck(string a)
{
	int Size = a.size();
	for (int i = 0; i < Size; i++)
	{
		if (a[i] < 48 or a[i]>57)
			return false;
	}
	return true;
}

string subStr(string a, string b) {

	string result = "";

	//we will reassign strings if necessary (a will be the one with more digits or the one with greater value)
	//the smaller length string will always be b
	//append ' ' char at the beginning of b to make the lengths of both strings equal

	if (b.length() > a.length()) {
		string temp = a;
		a = b;
		b = temp;

		int diff = a.length() - b.length();

		temp = "";

		do {
			temp += ' ';
			diff--;
		} while (diff);

		temp += b;
		b = temp;
	}
	else if (b.length() < a.length()) {
		int diff = a.length() - b.length();

		string temp = "";

		do {
			temp += ' ';
			diff--;
		} while (diff);

		temp += b;
		b = temp;
	}
	else if (a.length() == b.length()) {		//cuz a-b
		if (b > a) {
			string temp = a;
			a = b;
			b = temp;
		}
	}

	//both string are ready to be subtracted
	for (int i = a.length() - 1; i >= 0; i--) {

		string temp = result;

		if (b[i] != ' ') {

			int charA = a[i] - 48;
			int charB = b[i] - 48;

			if ((charA - charB) < 0) {		//if the differnce is less than 0, carry must be taken.
				for (int j = i - 1; j >= 0; j--) {
					int tempA = a[j] - 48;
					bool* carry = new bool[i - j];
					if (tempA > 0) {
						a[j] = ((tempA - 1) + 48);
						break;
					}
					else {
						a[j] = 9 + 48;
					}
				}
				charA += 10;
				//cout << "charA is: " << charA << endl;
				result = ((charA - charB) + 48);
				//cout << result << endl;
				result += temp;
			}
			else if ((charA - charB) >= 0) {		//if the differnce is greater than equal to 0, directly subtract.
				result = ((charA - charB) + 48);
				result += temp;
			}
		}
		else {		//if there is ' ' character in b[i] then copy a[i] as it is 
			result = a[i];
			result += temp;
		}
	}

	return result;
}

string addStr(string a, string b) {
	string result = "";

	//we will reassign strings if necessary (a will be the one with more digits or the one with greater value)
	//the smaller length string will always be b
	//append '0' char at the beginning of b to make the lengths of both strings equal

	if (b.length() > a.length()) {
		string temp = a;
		a = b;
		b = temp;

		int diff = a.length() - b.length();

		temp = "";

		do {
			temp += '0';
			diff--;
		} while (diff);

		temp += b;
		b = temp;
	}
	else if (b.length() < a.length()) {
		int diff = a.length() - b.length();

		string temp = "";

		do {
			temp += '0';
			diff--;
		} while (diff);

		temp += b;
		b = temp;
	}
	else if (a.length() == b.length()) {		//cuz a-b
		if (b > a) {
			string temp = a;
			a = b;
			b = temp;
		}
	}

	//cout << endl << endl;

	//cout << "a is" << a << endl;
	//cout << "b is" << b << endl;

	string temp = "";
	bool carry = 0;

	//both string are ready to be added
	for (int i = a.length() - 1; i >= 0; i--) {

		temp = result;

		int charA = a[i] - 48;
		int charB = b[i] - 48;


		if ((charA + charB + carry) <= 9) {
			result = (charA + charB + carry) + 48;
			result += temp;
			carry = 0;
		}
		else {
			result = ((charA + charB + carry) - 10) + 48;
			//cout << "calculation: " << ((charA + charB + carry) - 10) + 48 << endl;
			result += temp;
			//cout << "result is: " << result << endl;
			carry = 1;
		}

		if (carry == 1 && i == 0) {
			temp = result;
			result = "1";
			result += temp;
		}

	}

	//cout << "a is" << a << endl;
	//cout << "result is" << result << endl;

	return result;
}

int comparison(string a, string b) {		//return: 5 if a>b, -5 if b>a, 0 if a==b.

	//if length of both string is equal directly use the relational operators to compare values of both strings.
	//if there is a length mismatch append 0's at the beginning of the smaller string

	if (a.size() == b.size())
	{
		if (a > b)
			return 5;
		else if (a < b)
			return -5;
		else
			return 0;
	}
	else if (a.size() > b.size())
	{
		long int diff = a.size() - b.size();
		for (int i = 0; i < diff; i++)
		{
			b = "0" + b;
		}
		//cout << "b: " << b << endl;
		//cout << "a: " << a << endl;
		if (a > b)
			return 5;
		else if (a < b)
			return -5;
		else
			return 0;
	}
	else if (a.size() < b.size())
	{
		long int diff = b.size() - a.size();
		for (int i = 0; i < diff; i++)
		{
			a = "0" + a;

		}
		//cout << "b: " << b << endl;
		//cout << "a: " << a << endl;
		if (a > b)
			return 5;
		else if (a < b)
			return -5;
		else
			return 0;
	}
	//return 8;
}

template <template<class> class Node, class T>
struct PointerNode{
	Node<T>* ptr;
	PointerNode<Node, T>* Next;
	PointerNode<Node, T>* Previous;

	/// default constructor
	PointerNode() {
		ptr = NULL;
		Next = NULL;
		Previous = NULL;
	}

	/// setter function
	void setAddress(Node<T>* obj) {
		ptr = obj;
		Next = NULL;
		Previous = NULL;
	}

	//calls Display() of the user-defined object type
	void Display() {
		if (this->ptr != NULL) {
			//cout << "key at address: " << endl;
			this->ptr->Display();
		}
	}
};

template <template<class> class Node, class T>
class RTLinkedList {
public:
	PointerNode<Node, T>* Head;		//head of routing table linked list
	int Length;

	/// default constructor
	RTLinkedList() {
		Length = 0;
		Head = nullptr;
	}

	//parameters --- 1. Node<T>* obj:- gives pointer to any user-defined object to insert in linked list.  
	//sending address of the node which has to be inserted in the Routing Table's linked list
	void insert(Node <T>* obj) {

		PointerNode<Node, T>* it = Head;
		PointerNode<Node, T>* temp = new PointerNode<Node, T>;
		temp->setAddress(obj);

		if (it == NULL) {
			//cout << "Head" << endl;
			Head = temp;
		}
		else {
			while (it->Next != nullptr) {
				it = it->Next;
			}
			it->Next = temp;
			temp->Previous = it;
		}
		Length++;
	}

	//display the routing table of the calling object (which will be a machine node)
	void Display() {
		PointerNode<Node, T>* RTPtr = this->Head;

		if (Head == NULL) {
			cout << "The LinkedList is empty" << endl;
		}
		else {
			while (RTPtr != nullptr) {
				RTPtr->Display();
				RTPtr = RTPtr->Next;
			}
			cout << endl;
		}
	}

	/// destructor
	void deleteLinkedlist() {

		while (this->Head != NULL) {
			PointerNode<Node, T>* it = Head->Next;
			delete Head;
			Head = it;
		}
		Head = NULL;
	}
};

template<typename U>
struct ValueNode {
public:
	U key;
	int LineNo;
	U FileName;
	ValueNode<U>* Next;
	ValueNode<U>* Previous;

	/// default constructor 
	ValueNode() {
		key = " ";
		LineNo = 0;
		FileName = " ";
		Next = NULL;
		Previous = NULL;
	}

	/// overloaded constructor
	ValueNode(U k, U file, int line) {
		key = k;
		LineNo = line;
		FileName = file;
		Next = NULL;
		Previous = NULL;
	}

	/// setter function
	void setAttributes(U k, U file, int line) {
		key = k;
		LineNo = line;
		FileName = file;
		Next = NULL;
		Previous = NULL;
	}

	//copy constructor
	void setValue(ValueNode<U> copy) {
		this->key = copy.key;
		this->FileName = copy.FileName;
		this->LineNo = copy.LineNo;
		this->Next = NULL;
		this->Previous = NULL;

	}

	//displays ValueNode object
	void Display() {
		cout << "key:" << key << endl;
		cout << "File Name:" << FileName << endl;
		cout << "Line Number: " << LineNo << endl;

	}

	//checks if sent key belongs to the ValueNode object
	//returns true/false
	bool ValueFound(U k) {
		if (key == k)
			return true;
		return false;
	}
};

template <template<class> class Node, class T>
class LinkedList {
public:
	int Length = 0;
	Node<T>* Head;

	LinkedList() {
		Length = 0;
		Head = NULL;
	}

	/// paramters --- 1. Node<T> obj:- gives any user-defined object to insert in linked list.
	/// inserts Node<T> type object in doubly linked list
	void insert(Node<T> obj) {

		Node<T>* it = Head;
		Node<T>* temp = new Node<T>;
		temp->setValue(obj);

		if (it == NULL) {
			Head = temp;
		}
		else {
			while (it->Next != nullptr)
			{
				it = it->Next;
			}
			it->Next = temp;
			temp->Previous = it;

		}
		Length++;
	}

	/// parameters --- T key
	/// searches for the data associated to the searched node's key in the linked list.
	///  If key is present in linked list, function returns the Node pointer corresponding to the key, otherwise it returns a null pointer.
	Node<T>* Search(T key) {
		Node<T>* it = Head;

		while (it != NULL) {
			if (it->ValueFound(key) == true)
				return it;

			it = it->Next;
		}
		//cout << endl;
		return nullptr;
	}

	void Display() {
		Node<T>* it = Head;

		if (Head == NULL) {
			//out << "the list is empty" << endl;
		}
		else {
			while (it != NULL) {
				it->Display();
				it = it->Next;
			}
			cout << endl;
		}
	}

	//Function to return length of LinkedList
	int getLength() {
		return Length;
	}

	// deletes each node of the linked list, and sets the Head pointer of the linked list to nullptr
	void deleteLinkedlist() {

		while (this->Head != NULL) {
			Node<T>* it = Head->Next;
			delete Head;
			Head = it;
		}
		Head = NULL;
	}

	//deletes one node in the linked list, and updates the linkages in the list
	void deleteNode(Node<T>* del) {
		if (del != NULL) {
			if (del == Head) {
				Node<T>* Nextnode = Head->Next;
				delete del;
				Head = Nextnode;

				if (Head != NULL) {
					Head->Previous = NULL;
				}
			}
			else {
				Node<T>* PreviousNode = del->Previous;
				Node<T>* NextNode = del->Next;
				delete del;
				PreviousNode->Next = NextNode;

				if (NextNode != NULL) {
					NextNode->Previous = PreviousNode;
				}
			}
			Length--;
		}
	}
};

template<typename T>
struct AVLNode {
public:
	T nodeID;
	int height;
	LinkedList<ValueNode, T> Values;		//values with duplicate ids stored in a linked list here

	AVLNode<T>* left, * right;

	AVLNode() {
		nodeID = " ";
		left = nullptr;
		right = nullptr;
		height = 0;
	}

	// parameters --- 1. T value:- AVL nodeId (hashed ID)
	// overloaded constructor
	AVLNode(T value) {
		nodeID = value;
		left = nullptr;
		right = nullptr;
		height = 0;
	}


	/// parameters --- 1. ValueNode<T>& vN:- the valueNode that has to stored in the AVLNode's ValueNodes linked list.
	/// adds ValueNode object to "Values" list instead of simply making it the AVLNode itself, because it might be possible that the hashed key of two different keys may be identical.
	void insert(ValueNode<T>& vN) {
		Values.insert(vN);
	}

	//displays all data elements that are present on the AVL Node 
	void Display() {
		//cout << endl << "///////////////////	AVL node display	//////////////////" << endl;
		cout << "Displaying linked list at node ID: " << nodeID << ":-" << endl;
		Values.Display();
	}
};