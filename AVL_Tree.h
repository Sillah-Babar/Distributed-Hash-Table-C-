#pragma once
#include"LinkedList.h"

/// 
/// contains all data of a machine
/// balances data according to hashed keys sent for insertion in the tree
/// 
template<typename T>
class AVL_Tree
{
public:
	AVLNode<T>* Root;			//root of AVL Tree changes
	AVL_Tree()
	{
		Root = nullptr;
	}

	// checks if there is no AVL node in the tree
	//returns true/false
	bool isEmpty()
	{
		if (Root == nullptr)
			return true;
		else
			return false;
	}

	void  TransferWholeTree(AVLNode<T>* treeNode, RTLinkedList<AVLNode, T>& transferTo){
		if (treeNode != nullptr)
		{
			TransferWholeTree(treeNode->left, transferTo);
			//	cout << "treeNode: " << treeNode->nodeID << endl;
			transferTo.insert(treeNode);
			TransferWholeTree(treeNode->right, transferTo);
		}

	}
	AVLNode<T>* SearchAndNullifyNodes(T Key, AVLNode<T>* node)
	{
		//cout << "hkshdkshdfs" << endl;
		AVLNode<T>* temp = nullptr;
		/*The following function is required to collect addresses of the nodes in the  tree which are <= machineID*/
		if (node == NULL)// if root is ==NULL return the root
			return node;

		if (comparison(Key, node->nodeID) == -5)//else compare with the key < node id the move towards the left of the tree
			node->left = SearchAndNullifyNodes(Key, node->left);

		else if (comparison(Key, node->nodeID) == 5)
			node->right = SearchAndNullifyNodes(Key, node->right);

		else
		{

			//cout << "node->Value.Length: " << node->Values.Length << endl;

			if ((node->left == NULL) or (node->right == NULL))
			{
				temp = node->left;
				if (temp)
				{
					temp = node->left;
				}
				else
					temp = node->right;

				// zero child 
				if (temp == NULL)
				{
					temp = node;//store the node in temp
					node = NULL;//assign null to node


					temp->left = NULL;
					temp->right = NULL;
					temp->height = 1;
					//cout << "no child: " << temp->nodeID << endl;
				}
				else // One child   
				{
					AVLNode<T>* current = node;
					node = temp; //copy data
					//cout << "node: " << node->nodeID << endl;
					temp = current;
					temp->left = NULL;
					temp->right = NULL;
					temp->height = 1;
					//cout << "one child: " << temp->nodeID << endl;
				}

			}
			else
			{
				//node with two children
				AVLNode<T>* temp = node->right;
				while (temp->left != NULL)
				{
					temp = temp->left;
				}


				temp->left = node->left;
				temp = node;
				node = node->right;
				temp->left = NULL;
				temp->right = NULL;
				temp->height = 1;
				//cout << "temp two children: " << temp->nodeID << endl;
				//delete temp;

			}
			//cout << "temp inside: " << temp->nodeID << endl;
		}
		//tree only got one node
		if (node == NULL)//when the node had zero children so it was deleted , in the case that the tree only had one node
			return node;// the returned value be null as the tree no longer has any node means it has no root


		node->height = 1 + maxNum(getHeight(node->left), getHeight(node->right));
		//cout << "node: " << node->nodeID<< endl;
		//cout << "node height: " << node->height << endl;
		int b = BalanceRatio(node);

		// LL  case
		if (b > 1 && BalanceRatio(node->left) >= 0)
			return RR_Rotation(node);

		// LR case
		if (b > 1 && BalanceRatio(node->left) < 0)
		{
			node->left = LL_Rotation(node->left);
			return RR_Rotation(node);
		}

		// RR Case  
		if (b < -1 && BalanceRatio(node->right) <= 0)
			return LL_Rotation(node);

		//RL rotation
		if (b < -1 && BalanceRatio(node->right) > 0)
		{
			node->right = RR_Rotation(node->right);
			return LL_Rotation(node);
		}


		return node;
	}

	AVLNode<T>* insertAddresses(AVLNode<T>* node, AVLNode<T>* addressNode)//now the value node has the key and the filename and the line number
	{
		if (node == NULL)
		{
			//cout << "addressNode: " << addressNode->nodeID << endl;
			node = addressNode;
			return node;
		}

		if (comparison(addressNode->nodeID, node->nodeID) == -5)
			node->left = insertAddresses(node->left, addressNode);
		else if (comparison(addressNode->nodeID, node->nodeID) == 5)
			node->right = insertAddresses(node->right, addressNode);
		else
			return node;
		//updating heights
		node->height = 1 + maxNum(getHeight(node->left), getHeight(node->right));


		int b = BalanceRatio(node);

		// LL Case  
		if (b > 1 && (comparison(addressNode->nodeID, node->left->nodeID) == -5))
			return RR_Rotation(node);

		//RR Case  
		if (b < -1 && (comparison(addressNode->nodeID, node->right->nodeID) == 5))
			return LL_Rotation(node);

		// LR Case  
		if (b > 1 && (comparison(addressNode->nodeID, node->left->nodeID) == 5))
		{
			node->left = LL_Rotation(node->left);
			return RR_Rotation(node);
		}

		// RL Case  
		if (b < -1 && (comparison(addressNode->nodeID, node->right->nodeID) == -5))
		{
			node->right = RR_Rotation(node->right);
			return LL_Rotation(node);
		}


		return node;
	}
	void storeNodes(T start, T end, AVLNode<T>* treeNode, RTLinkedList<AVLNode, T>& Transfer)
	{
		if (treeNode != nullptr)
		{
			storeNodes(start, end, treeNode->left, Transfer);
			//treeNodeID>=start and treeNodeID<end
			if ((comparison(treeNode->nodeID, start) == 5 or comparison(treeNode->nodeID, start) == 0) && comparison(treeNode->nodeID, end) == -5)
			{
				Transfer.insert(treeNode);
			}
			storeNodes(start, end, treeNode->right, Transfer);
		}

	}

	//parameters ---	1. hashed key (identifier of AVL node/machine inserted)		 2. a ValueNode object (value node has the data associated to the machine / AVL node that is inserted)
//inserts new AVL node in the tree
//returns Root of the AVL Tree
	AVLNode<T>* insert(T hashedID, ValueNode<T>& vNode)//now the value node has the key and the filename and the line number
	{
		AVLNode<T>* FoundNode = search(hashedID);
		if (FoundNode != NULL)//if the hashed id has been found then
		{
			FoundNode->insert(vNode);
			//cout << "Displaying linked list at AVL node:-" << endl;
			//oundNode->Display();
		}
		else
		{
			Root = insertNode(Root, hashedID, vNode);
		}
		return Root;
	}


	/// parameters ---	1. first integer   2. second integer
	/// compares two integers to check which one is greater in value
	/// returns the greater number
	int maxNum(int num1, int num2)
	{
		if (num1 > num2)
			return num1;
		else
			return num2;
	}

	/// parameters ---	1. pointer to an AVL Node (which acts as head for the subtree of which height is to be found)
/// calculates height of the tree/subtree
/// returns integer height of the tree/subtree
	int getHeight(AVLNode<T>* tmp)
	{
		if (tmp == nullptr)
			return 0;
		return tmp->height;
	}

	/// parameters ---	1. pointer to an AVL Node (the node for which balance factor is to be found)
/// calculates the balance factor of an AVL Node in the tree. Needed upon insertion
/// returns integer balnce factor of AVL node
	int BalanceRatio(AVLNode<T>* NODE)
	{
		if (NODE == NULL)
			return 0;
		return getHeight(NODE->left) - getHeight(NODE->right);
	}

	/// parameters ---	1. hashed machine Id	 2. a value Node
/// functions creates an AVL Node from the sent hashed machineId, and valueNode, and then adds it to the system (Hashed key MAY BE A DUPLICATE, hence it is added to the Values linked list (caters for need of chaining).)
/// returns pointer to the AVL node created
	AVLNode<T>* CreateNode(T k, ValueNode<T> Vn)
	{
		AVLNode<T>* node = new AVLNode<T>;
		node->Values.insert(Vn);
		node->height = 1;
		node->left = NULL;
		node->right = NULL;
		node->nodeID = k;


		return node;
	}

	/// parameters ---	1. pointer to an AVL Node which is to be rotated
/// rotates the tree if right to right imbalance occurs.
/// returns the updated Root of the AVL Tree
	AVLNode<T>* RR_Rotation(AVLNode<T>* n1)
	{
		AVLNode<T>* n2 = n1->left;
		AVLNode<T>* subtree = n2->right;


		n2->right = n1;
		n1->left = subtree;

		// fixing height AGAIN
		n1->height = maxNum(getHeight(n1->left), getHeight(n1->right)) + 1;
		n2->height = max(getHeight(n2->left), getHeight(n2->right)) + 1;


		return n2;//returning updates ROOt
	}

	/// parameters ---	1. pointer to an AVL Node which is to be rotated
/// rotates the tree if left to left imbalance occurs.
/// returns the updated Root of the AVL Tree
	AVLNode<T>* LL_Rotation(AVLNode<T>* n1)
	{
		AVLNode<T>* n2 = n1->right;
		AVLNode<T>* subtree = n2->left;
		n2->left = n1;
		n1->right = subtree;
		//Fixing the heights again
		n1->height = maxNum(getHeight(n1->left), getHeight(n1->right)) + 1;
		n2->height = maxNum(getHeight(n2->left), getHeight(n2->right)) + 1;
		return n2;//returning updates ROOt
	}

	/// parameters ---	1. AVLNode<T>* node:- pointer to the root of the tree/subtree at which AVL node is to be inserted in the system		2. T key:- machine key(to identify on which machine AVL node is to be stored)		3. ValueNode<T>& vNode:- ValueNode object that contains the data to be stored on machine
/// creates an AVL node using the passed ValueNode on the given machineId, and inserts it into the correct machineID using the reference of the tree/subtree's root pointer. This function also maintains the AVL Tree height balance by performing the required rotations.
/// returns updated root of the machine/AVL Tree.
	AVLNode<T>* insertNode(AVLNode<T>* node, T key, ValueNode<T>& vNode)
	{

		if (node == NULL)
			return CreateNode(key, vNode);

		if (comparison(key, node->nodeID) == -5)
			node->left = insertNode(node->left, key, vNode);
		else if (comparison(key, node->nodeID) == 5)
			node->right = insertNode(node->right, key, vNode);
		else
			return node;
		//updating heights
		node->height = 1 + maxNum(getHeight(node->left), getHeight(node->right));


		int b = BalanceRatio(node);

		// LL Case  
		if (b > 1 && (comparison(key, node->left->nodeID) == -5))
			return RR_Rotation(node);

		//RR Case  
		if (b < -1 && (comparison(key, node->right->nodeID) == 5))
			return LL_Rotation(node);

		// LR Case  
		if (b > 1 && (comparison(key, node->left->nodeID) == 5))
		{
			node->left = LL_Rotation(node->left);
			return RR_Rotation(node);
		}

		// RL Case  
		if (b < -1 && (comparison(key, node->right->nodeID) == -5))
		{
			node->right = RR_Rotation(node->right);
			return LL_Rotation(node);
		}


		return node;
	}

	/// parameters ---	1. T Key:- Hashed key (machineId) to identify the node to be deleted	2. AVLNode<T>* node:- pointer to the root of the tree/subtree at which AVL node is to be deleted from the system	3. T OriginalKey:- original key to search for the exact data element in a machine of the tree.
	/// sets the updated node pointer after deletion of a node from the machine (refer to the function below) as root of the AVL Tree.
	void  deleteNode(T Key, AVLNode<T>* node, T OriginalKey)
	{
		this->Root = deleteAVLNode(Key, node, OriginalKey);
	}

	/// parameters ---	1. T Key:- Hashed key (machineId) to identify the node to be deleted	2. AVLNode<T>* node:- pointer to the root of the tree/subtree at which AVL node is to be deleted from the system	3. T OriginalKey:- original key to search for the exact data element in a machine of the tree.
/// uses machineId to reach to the correct machine in the DHT Ring(system) using comparisons with the hashed key and node ID, finds the original key for one single data element on every AVL Node of that machine's AVL tree, and checks the if after deleting the data element from the linked list ("Values" list) of data elements clustered on the same AVL node, the "Values" list is deleted. Finally, it performs rotations if any height balancing is required.
/// returns updated root of the AVL Tree.
	AVLNode<T>* deleteAVLNode(T Key, AVLNode<T>* node, T OriginalKey)		//calls delete function at the right position, and maintains height balance after deletion
	{
		if (node == NULL)// if root is ==NULL return the root
			return node;

		if (comparison(Key, node->nodeID) == -5)//else compare with the key < node id the move towards the left of the tree
			node->left = deleteAVLNode(Key, node->left, OriginalKey);

		else if (comparison(Key, node->nodeID) == 5)
			node->right = deleteAVLNode(Key, node->right, OriginalKey);

		else
		{
			ValueNode<T>* found = node->Values.Search(OriginalKey);

			while (found != NULL)// that means that on a particular node of an avl tree we have found the key
			{
				//cout << found->key << endl;
				node->Values.deleteNode(found);// the value id deleted from that node
				found = node->Values.Search(OriginalKey);
			}

			if (node->Values.Length == 0)// in the case that there is no linklist of values in that node then it must be deleted
			{
				//cout << "node->Value.Length: " << node->Values.Length << endl;

				if ((node->left == NULL) or (node->right == NULL))
				{
					AVLNode<T>* temp = node->left;
					if (temp)
					{
						temp = node->left;
					}
					else
						temp = node->right;

					// zero child 
					if (temp == NULL)
					{
						temp = node;//store the node in temp
						node = NULL;//assign null to node
					}
					else // One child   
						*node = *temp; //copy data  
					delete temp;
				}
				else
				{
					//node with two children
					AVLNode<T>* temp = node->right;
					while (temp->left != NULL)
					{
						temp = temp->left;
					}


					temp->left = node->left;
					temp = node;
					node = node->right;
					delete temp;

				}
				//tree only got one node
				if (node == NULL)//when the node had zero children so it was deleted , in the case that the tree only had one node
					return node;// the returned value be null as the tree no longer has any node means it has no root

				node->height = 1 + maxNum(getHeight(node->left), getHeight(node->right));

				int b = BalanceRatio(node);

				// LL  case
				if (b > 1 && BalanceRatio(node->left) >= 0)
					return RR_Rotation(node);

				// LR case
				if (b > 1 && BalanceRatio(node->left) < 0)
				{
					node->left = LL_Rotation(node->left);
					return RR_Rotation(node);
				}

				// RR Case  
				if (b < -1 && BalanceRatio(node->right) <= 0)
					return LL_Rotation(node);

				//RL rotation
				if (b < -1 && BalanceRatio(node->right) > 0)
				{
					node->right = RR_Rotation(node->right);
					return LL_Rotation(node);
				}
			}
		}
		return node;
		//allRotations(root, node);
	}







	/// parameters --- 1. AVLNode<T>* tmp:- takes pointer to the root of an AVL Tree
/// displays the AVL Tree in postOrder traversals.
	void postOrderDB(AVLNode<T>* tmp)
	{
		if (tmp != nullptr)
		{
			postOrderDB(tmp->left);
			postOrderDB(tmp->right);
			tmp->Display();
		}

	}

	/// parameters --- 1. AVLNode<T>* tmp:- takes pointer to the root of an AVL Tree
/// displays the AVL Tree in inOrder traversals.
	void inOrderDB(AVLNode<T>* tmp)
	{
		if (tmp != nullptr)
		{
			inOrderDB(tmp->left);
			tmp->Display();
			inOrderDB(tmp->right);
		}
	}

	/// parameters --- 1. AVLNode<T>* tmp:- takes pointer to the root of an AVL Tree
/// displays the AVL Tree in preOrder traversals.
	void printPreOrderAVL_T(AVLNode<T>* tmp)
	{
		if (tmp == nullptr)
		{
			return;
		}
		tmp->Display();
		printPreOrderAVL_T(tmp->left);
		printPreOrderAVL_T(tmp->right);
	}

	AVLNode<T>* search(T id)//id of the node will be given to be found
	{
		AVLNode<T>* result = nullptr;
		AVLNode<T>* tmp = Root;
		while (tmp != nullptr)
		{
			int Val = comparison(id, tmp->nodeID);
			if (Val == -5)
				tmp = tmp->left;
			else if (Val == 5)
				tmp = tmp->right;
			else
			{
				//cout << "Node found!" << endl;
				result = tmp;
				return result;
			}
		}
		return result;
	}


	~AVL_Tree()
	{
		/*
		if (Root != NULL)
			delete Root;
			*/
	}

};
