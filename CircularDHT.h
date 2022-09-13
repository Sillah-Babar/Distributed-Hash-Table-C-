#pragma once
#include<cmath>
#include"AVL_Tree.h"
#include"SHA1.h"
#include"fstream"
#include<iostream>
#include<sstream>
#include<boost/multiprecision/cpp_int.hpp>
using namespace boost::multiprecision;
#include<boost/lexical_cast.hpp>
using namespace std;
template<typename T>
struct DHTNode;

/// parameters --- ValueNode<T> & vn
/// deletes the actual key value pair using the file name and line number given as valueNode member variables, and replaces it with "xxx,xxx" to show which data has been deleted from the file.
template<typename T>
T delVNode(ValueNode<T>& vn){
	T result;
	int letters = 0;
	int lcount = 1;
	
	string tempLine = "";
	
	ifstream inFile(vn.FileName, ios::in);
	ofstream outCFile("copy" + vn.FileName, ios::out);

	while (!inFile.eof()){
		getline(inFile, tempLine);
		if (lcount == vn.LineNo){
			result = tempLine;
			outCFile << "xxx,xxx" << endl;
		}
		else {
			outCFile << tempLine << endl;
		}
		lcount++;
	}
	outCFile.close();
	inFile.close();

	char* fName = new char[vn.FileName.length()];
	for (int i = 0; i < vn.FileName.length(); i++)
		fName[i] = '\0';
	//cout << "Length = " << vn.FileName.length() << endl;
	strcpy_s(fName, vn.FileName.length() + 1, vn.FileName.c_str());
	//cout << "fName = " << fName << endl;

	//rewrite original file
	ifstream inCFile("copy" + vn.FileName, ios::in);
	ofstream outFile(vn.FileName, ios::out);
	//	outFile.seekp(0, ios::beg);
	
	while (!inCFile.eof()){
		getline(inCFile, tempLine);
		outFile << tempLine << endl;
	}

	inCFile.close();
	outFile.close();
	return result;
}

/// parameters --- 1. ValueNode<T> &vn
/// uses the file name and line number given as member variables of vNode to reach to the actual key, value pair in the system's files.
/// returns the value corresponding to the vNode file name and line number.
template <typename T>
string retrieveFromFile(ValueNode<T>& vNode){
	//cout << "Entered" << endl;
	int lNum = vNode.LineNo;
	int lcount = 1;

	string fName = vNode.FileName;
	
	ifstream inFile(fName, ios::in);
	
	string tempLine = "";
	string retValue = "";

	while (lcount != lNum){
		//cout << lcount << endl;
		lcount++;
		getline(inFile, tempLine);
	}

	getline(inFile, tempLine);
	int spaces = 0;

	for (int i = 0; i < tempLine.length(); i++){
		if (tempLine[i] == ','){
			//cout << tempLine;
			spaces++;
			i++;
		}
		if (spaces > 0){
			retValue += tempLine[i];
		}
		//cout << "VALUE =  " << retValue << endl;
	}
	return tempLine;
}

/// parameters --- 1. string Key :- actual key		 2. string Value
/// writes the key, value pair at the end of file until 100 lines(or key, value pairs) have been entered, after which a new file is generated.
/// Each file is named in its order of creation for maintaining clarity. 
/// This function also creates a ValueNode object that is assigned the key value pair's fileName and line number.
/// returns the created valueNode object
ValueNode<string> FileWriting(string Key, string Value){
	static int fileNum = 1;
	static int lineNum = 0;

	bool newFile = false;
	
	string fileName = to_string(fileNum) + ".csv";
	
	ofstream outFile(fileName, ios::app | ios::out);
	
	outFile << Key;
	outFile << ',';
	outFile << Value;
	outFile << endl;

	lineNum++;
	
	if (lineNum > 100){
		fileNum++;
		lineNum = 1;
	}
	ValueNode<string> vNode(Key, fileName, lineNum);
	return vNode;
}

string ComputeHashedVal(string hashed);

string Hashing(string key, string identifierSpace){
	unsigned char hash[20]; char hexstring[41];
	long int s = key.size();
	const void* pointerToString = key.c_str();

	sha1::calc(pointerToString, s, hash);
	// 10 is the length of the string
	sha1::toHexString(hash, hexstring);
	
	string hashed = "";

	for (int i = 0; i < 41; i++){
		hashed += hexstring[i];
	}
	//cout << "hashed<<: " << hashed << endl;
	string num = ComputeHashedVal(hashed);

	istringstream convert(num);
	int256_t hashedValue;
	int256_t space;
	convert >> hashedValue;
	istringstream ss(identifierSpace);
	ss >> space;
	int256_t result = hashedValue % space;

	string res = boost::lexical_cast<string>(result);

	return res;
}

//this function is used by subStr inorder to remvove decimal point char
string RemoveDecimal(string h){
	string result;

	for (int i = 0; i < h.size(); i++){
		if (h[i] != '.'){
			result += h[i];
		}
		else{
			i = h.size();
		}
	}
	return result;
}

string ComputeHashedVal(string hashed){
	string Summation;
	string result;
	int power = 0;
	//cout << "hashedsize: " << hashed.size() << endl;
	//cout << "hashed: " << hashed[hashed.size() - 1] << endl;
	for (int i = hashed.size() - 1 - 1; i >= 0; i--){
		int digit = hashed[i];
		result = "";
		//cout << "digit: " << digit - 48 << endl;
		if (digit == 'a'){
			result = to_string(10 * pow(16, power));//since a =10
		}
		else if (digit == 'b'){
			result = to_string(11 * pow(16, power));//since b =11
		}
		else if (digit == 'c'){
			result = to_string(12 * pow(16, power));//since c =12
		}
		else if (digit == 'd'){
			result = to_string(13 * pow(16, power));//since d =13
		}
		else if (digit == 'e'){
			result = to_string(14 * pow(16, power));//since e =14
		}
		else if (digit == 'f'){
			result = to_string(15 * pow(16, power));//since f =15
		}
		else{
			digit = digit - 48;
			result = to_string(digit * pow(16, power));
		}
		result = RemoveDecimal(result);
		//cout << result << endl;
		Summation = addStr(Summation, result);
		//cout << "Summation" << Summation << endl;
		power++;
	}
	return Summation;
}

template<typename T>
struct DHTNode {
	T MachineID; //since machine ids at most can go uptill 2^160 -1 WILL BE DOUBLE
	AVL_Tree<string> MachineTree;
	RTLinkedList <DHTNode, T> RoutingTable;
	DHTNode<T>* Next;
	DHTNode<T>* Previous;

	// default constructor
	DHTNode() {
		MachineID = "";
		Next = nullptr;
		Previous = nullptr;
	}

	// parameterised constructor
	DHTNode(T id) {
		MachineID = id;
		Next = nullptr;
		Previous = nullptr;
	}

	// displays the Machine ID of the calling object
	void Display() {
		cout << "Machine ptr: " << MachineID << endl;
	}

	//display the routing table of the calling object
	void PrintRoutingTable() {
		cout << "Machine ID: " << MachineID << endl;
		this->RoutingTable.Display();
	}

	// inorder traversal of the tree stored in the calling object
	void DisplayInOrderTree_atMachine(){
		cout << "Machine ID: " << MachineID << endl;
		cout << "------------------------------------------" << endl;
		this->MachineTree.inOrderDB(this->MachineTree.Root);
	}
};

template<typename T>
class DHTRing{
public:
	DHTNode<T>* Head;
	int NumOfMachines;
	T identifierSpace;
	int Bits;

	// default constructor
	DHTRing(){
		Head = NULL;
		NumOfMachines = 0;
		identifierSpace = 0;
		Bits = 0;
	}

	// parameterised constrcutor
	DHTRing(int bits, int Machines){
		Head = NULL;
		while (bits < 0){
			cout << "Number of bits below zero" << endl;
			cout << "Enter Again" << endl;
			cin >> bits;
		}

		Bits = bits;
		
		while (Machines < 0){
			cout << "Number of machines below zero" << endl;
			cout << "Enter Again" << endl;
			cin >> Machines;
		}

		T identifierspace = to_string(pow(2, bits) - 1);
		identifierspace = RemoveDecimal(identifierspace);

		while (comparison(to_string(Machines), identifierspace) == 5){
			cout << "Number of machines are greater than the identifier space" << endl;
			cout << "Enter Again" << endl;
			cin >> Machines;
		}

		NumOfMachines = 0;
		identifierSpace = identifierspace;
	}

	// setter function
	void setidentifierSpace(int bits){
		while (bits < 0){
			cout << "Number of bits below zero" << endl;
			cout << "Enter Again" << endl;
			cin >> bits;
		}

		Bits = bits;
		identifierSpace = to_string(pow(2, bits) - 1);
		identifierSpace = RemoveDecimal(identifierSpace);
	}

	//setter function
	void setNumOfMachines(int Machines){
		while (Machines < 0){
			cout << "Number of machines below zero" << endl;
			cout << "Enter Again" << endl;
			cin >> Machines;
		}

		while (comparison(to_string(Machines), identifierSpace) == 5){
			cout << "Number of machines are greater than the identifier space" << endl;
			cout << "Enter Again" << endl;
			cin >> Machines;
		}

		NumOfMachines = Machines;
	}

	// parameters --- 1. UserMachine
	//finds and returns the machine in the dht ring using the machine id passed in the parameter
	//if the machine of the specified id is not present in the dht ring it returns null
	DHTNode<T>* findMachine(T UserMachine){
		if (Head == nullptr){
			cout << "No machine exists" << endl;
			return NULL;
		}
		T id;	//call hash function to hash the key

		id = UserMachine;	// the ID of the machine the user is requesting data insertion from

		if (comparison(UserMachine, identifierSpace) == 5 or comparison(UserMachine, "0") == -5){
			cout << "you have entered a machine that does not exist." << endl;
			return NULL;
		}

		DHTNode<T>* Machine = this->SearchFromMachine(id, Head); //return the machine where the user is
		cout << "Machine ID: " << Machine->MachineID << endl;
		cout << "User Machine Path above " << endl;

		// UserMachine doesnt exit in ring or is out of the ring or userMachine <0
		if (Machine->MachineID != UserMachine){
			cout << "you have entered a machine that does not exist." << endl;
			return NULL;
		}
		return Machine;
	}

	T DeleteData(T key, T UserMachine){
		T result = " ";
		
		if (Head == nullptr){
			cout << "No machine exists thus no data exists" << endl;
			return " ";
		}

		T id;	//call hash function to hash the key

		id = UserMachine;// the machine user is requesting data  insertion from

		if (comparison(UserMachine, identifierSpace) == 5 or comparison(UserMachine, "0") == -5){
			cout << "you have entered a machine that does not exist." << endl;
			return result;
		}

		DHTNode<T>* Machine = this->SearchFromMachine(id, Head); //return the machine where the user is

		// UserMachine doesnt exit in ring or is out of the ring or userMachine <0
		if (Machine->MachineID != UserMachine){
			cout << "you have entered a machine that does not exist." << endl;
			return result;
		}
		
		cout << "User machine path above" << endl;

		//first hash the key then send below
		id = Hashing(key, identifierSpace);
		cout << "hashed key: " << id << endl;

		if (Head->Next == Head){
			Machine = Head;
		}
		else {
			Machine = this->SearchFromMachine(id, Machine);		//return the machine where key is present      
		}

		if (Machine != NULL){
			AVLNode<T>* avlnode = Machine->MachineTree.search(id);//searches the hashed id of the key to see which AVLnode it is present on

			if (avlnode == NULL){
				cout << "the data does not exist" << endl;
			}
			else{
				ValueNode<T>* Vnode = avlnode->Values.Search(key);// the valueNode on which the key is present 

				if (Vnode == nullptr){
					cout << "the data does not exist" << endl;
				}
				else{
					T deletedValue;
					deletedValue = delVNode(*Vnode);
					//cout << "reached the function" << endl;
					Machine->MachineTree.deleteNode(id, Machine->MachineTree.Root, key); //id id hashed key of machine and key is original key of the data
					//Machine->DisplayInOrderTree_atMachine();
					return deletedValue;
				}
			}
		}
		return result;
	}

	T RetrieveData(T key, T UserMachine){
		T result = " ";

		if (Head == nullptr){
			cout << "No machine exists thus no data exists" << endl;
			return " ";
		}

		T id;//call hash function to hash the key

		id = UserMachine;// the machine user is requesting data  insertion from
		if (comparison(UserMachine, identifierSpace) == 5 or comparison(UserMachine, "0") == -5){
			cout << "you have entered a machine that does not exist." << endl;
			return result;
		}

		DHTNode<T>* Machine = this->SearchFromMachine(id, Head); //return the machine where the user is
		// UserMachine doesnt exit in ring or is out of the ring or userMachine <0

		if (Machine->MachineID != UserMachine){
			cout << "you have entered a machine that does not exist." << endl;
			return result;
		}

		cout << "Machine ID: " << Machine->MachineID << endl;
		cout << "User machine path above" << endl;
		
		id = key; //first hash the key then send below
		id = Hashing(key, identifierSpace);
		
		cout << "hashed key: " << id << endl;
		
		if (Head->Next == Head){
			Machine = Head;
		}
		else {
			Machine = this->SearchFromMachine(id, Machine);//return the machine where key is Present
		}
		
		if (Machine != NULL){
			cout << "Machine ID: " << Machine->MachineID << endl;
			
			AVLNode<T>* avlnode = Machine->MachineTree.search(id);//searches the hashed id of the key to see which AVLnode it is present on
			
			if (avlnode == NULL){
				cout << "the data does not exist" << endl;
			}
			else{
				ValueNode<T>* Vnode = avlnode->Values.Search(key);// the valueNode on which the key is present 
				if (Vnode == nullptr){
					cout << "the data does not exist" << endl;
				}
				else {
					result = retrieveFromFile(*Vnode);
				}
			}
		}
		return result;
	}

	/*The InsertData function first arrives at the macchine from which the data insertion is requested.
	From that maachine it arrives at the machine where the hashed value of key is to be inserted.
	It inserts the key and returns the machine it is inserted on*/
	DHTNode<T>* InsertData(T UserMachine, T key, T value)
	{
		if (Head == nullptr){
			cout << "No machine exists to insert data on" << endl;
			return NULL;
		}
		T id;//call hash function to hash the key


		id = UserMachine;// the machine user is requesting data  insertion from
		if (comparison(UserMachine, identifierSpace) == 5 or comparison(UserMachine, "0") == -5){
			cout << "you have entered a machine that does not exist." << endl;
			return NULL;
		}

		cout << "User machine  id: " << id << endl;
		DHTNode<T>* Machine = this->SearchFromMachine(id, Head); //return the machine where the user is
		cout << "Machine ID: " << Machine->MachineID << endl;
		// UserMachine doesnt exit in ring or is out of the ring or userMachine <0
		if (Machine->MachineID != UserMachine)
		{
			cout << "you have entered a machine that does not exist." << endl;
			return NULL;

		}

		//cout <<"machine: " << Machine->MachineID << endl;
		cout << "User machine path above" << endl;
		id = key; //first hash the key then send below. since data will always be hashed therfore it will not return null
		id = Hashing(key, identifierSpace);
		cout << "hashed key: " << id << endl;
		if (Head->Next == Head)
		{
			Machine = Head;
		}
		else Machine = this->SearchFromMachine(id, Machine);//return the machine where key is to be inserted
		if (Machine == NULL)
		{
			cout << "The provided data cannot be stored on any machine" << endl;
		}
		else
		{

			//cout << "inserted" << endl;
			cout << "Machine ID where data will be inserted: " << Machine->MachineID << endl;
			ValueNode<T> Vnode = FileWriting(key, value);
			Machine->MachineTree.insert(id, Vnode);// the hashed id of the key and the ValueNode being sent
			//cout << "inorder traversal of the tree where the key has been inserted" << endl;
			//Machine->DisplayInOrderTree_atMachine();
		}
		return Machine;
	}

	// search machine id from current machine
	DHTNode<T>* SearchFromMachine(T id, DHTNode<T>* it)
	{
		if (it == this->Head)//in case the machine reurned is the head of circular linklist
		{
			//machineid > head->previous && machineid<=identifierspace
			if (comparison(id, this->Head->Previous->MachineID) == 5 && (comparison(id, identifierSpace) == -5 or comparison(id, identifierSpace) == 0))
			{
				return this->Head;
				//cout << "machine IDjj: " << it->MachineID << endl;
			}
			//or machineid>=0  && machineid <= this->Head
			if ((comparison(id, "0") == 5 or comparison(id, "0") == 0) && (comparison(id, Head->MachineID) == -5 or comparison(id, Head->MachineID) == 0))
			{


				return this->Head;
				//cout << "machine IDhkkhk: " << it->MachineID << endl;

			}
		}
		bool found = false;
		while (found == false)
		{
			cout << "machine ID: " << it->MachineID << endl;
			//cout << "id: " << id << endl;
			// the case where the id== machineID
			if (it->MachineID == id)
			{
				return it;
			}
			//else
			it = this->SearchRoutingTable(id, it);
			//cout << "it: " << it->MachineID << endl;
			if (it == this->Head)//in case the machine reurned is the head of circular linklist
			{
				//machineid > head->previous && machineid<=identifierspace
				if (comparison(id, this->Head->Previous->MachineID) == 5 && (comparison(id, identifierSpace) == -5 or comparison(id, identifierSpace) == 0))
				{
					found = true;
					//cout << "machine IDjj: " << it->MachineID << endl;
				}
				//or machineid>=0  && machineid <= this->Head
				if ((comparison(id, "0") == 5 or comparison(id, "0") == 0) && (comparison(id, Head->MachineID) == -5 or comparison(id, Head->MachineID) == 0))
				{


					found = true;
					//cout << "machine IDhkkhk: " << it->MachineID << endl;

				}
			}

			//id<=machineid id> machine->previous->machineiD
			else if ((comparison(id, it->MachineID) == -5 or comparison(id, it->MachineID) == 0) && comparison(id, it->Previous->MachineID) == 5)
			{
				// cout << "itmachine: " << it->MachineID << endl;
				found = true;

			}
		}
		return it;
		return NULL;
	}

	// searches the routing table of the current machine to get to the desired machine
	DHTNode<T>* SearchRoutingTable(T id, DHTNode<T>* start)
	{
		if (start->RoutingTable.Length > 0)
		{
			if (start == this->Head->Previous)//incase start==tail
			{
				//id>this->Head->Previous->MachineID && id<=identifierSpace
				if (comparison(id, this->Head->Previous->MachineID) == 5 && (comparison(id, identifierSpace) == -5 or comparison(id, identifierSpace) == 0))
				{
					//cout << "Tail" << endl;
					//returnLargest = true;
					return Head;
					// return start->RoutingTable.Head->Previous;
				}
				//id>=0 and id<=Head->MachineID
				if ((comparison(id, "0") == 5 or comparison(id, "0") == 0) && (comparison(id, this->Head->MachineID) == -5 or comparison(id, this->Head->MachineID) == 0))
				{
					//cout << "Tail" << endl;
					return Head;
					//returnLargest = true;

				}
			}
			PointerNode<DHTNode, T>* it = start->RoutingTable.Head;
			if (it != NULL && it->ptr != NULL)
			{
				if (comparison(id, start->MachineID) == 5 && (comparison(id, it->ptr->MachineID) == -5 or comparison(id, it->ptr->MachineID) == 0))
				{
					//cout << "first condition" << endl;
					return it->ptr;
				}
			}
			bool returnLargest = false;
			DHTNode<T>* largest = NULL;
			//DHTNode<T>* END = NULL;
			string co = "0";
			it = start->RoutingTable.Head;//FT[1=j]
			//cout << "ID" << id << endl;
			while (it != NULL)
			{
				//cout << "it:  " << it->ptr->MachineID << endl;
				//cout << "it->ptr: " << it->ptr->MachineID << endl;
				if (comparison(it->ptr->MachineID, co) == 5 or comparison(it->ptr->MachineID, co) == 0)
				{
					co = it->ptr->MachineID;
					largest = it->ptr;
					//cout << "largest: " << largest->MachineID << endl;
				}
				//END = it->ptr;
				if (it->Next != NULL)
				{

					if (returnLargest == false)
					{
						//id> it->ptr->MachineID && id<=it->Next->ptr->MachineID
						if (comparison(id, it->ptr->MachineID) == 5 && (comparison(id, it->Next->ptr->MachineID) == -5 or comparison(id, it->Next->ptr->MachineID) == 0))// to decide between which two machines the datanode should lie
						{
							//cout << "it->ptr: " << it->ptr->MachineID << endl;
							return it->ptr;
						}

						if (comparison(id, this->Head->Previous->MachineID) == 5 && (comparison(id, identifierSpace) == -5 or comparison(id, identifierSpace) == 0))
						{
							//cout << "return largest is true" << endl;
							returnLargest = true;
							// return start->RoutingTable.Head->Previous;
						}
						if ((comparison(id, "0") == 5 or comparison(id, "0") == 0) && (comparison(id, this->Head->MachineID) == -5 or comparison(id, this->Head->MachineID) == 0))
						{
							//cout << "return largest is true" << endl;
							returnLargest = true;

						}

					}
				}
				it = it->Next;
			}
			//cout << "arrived" << endl;
			//if (returnLargest == true)
			return largest;

		}
		return Head;
	}

	//assigns routing tables to all the machines in the dht ring
	void assignRTs()
	{

		DHTNode<T>* it = Head;
		if (Head != NULL)
		{
			do
			{
				this->insertRoutingTable(it, Bits, Head);
				//it->PrintRoutingTable();
				it = it->Next;
			} while (it != Head);
		}
	}

	//deletes the routing table of each and every machine in the dht ring
	void deleteRoutingTables()
	{
		DHTNode<T>* it = Head;
		if (Head != NULL)
		{
			do
			{
				it->RoutingTable.deleteLinkedlist();
				it = it->Next;
			} while (it != Head);
		}
	}

	// parameters --- 1. obj is the machine node that is being assigned the routing table
	// 2. idSpaceBits is the number of bits used to form the identifier space
	// 3. RingHead is the machine node that forms the head of the dht ring
	void insertRoutingTable(DHTNode<T>* obj, int idSpaceBits, DHTNode<T>* RingHead)
	{	                              //call this function for every node in DHTring
		T p = obj->MachineID;		//will be string
		int limit = log2(pow(2, idSpaceBits));
		//cout << "limit: " << limit << endl;
		for (int i = 1; i <= limit; i++)
		{
			string power = to_string(pow(2, i - 1));
			power = RemoveDecimal(power);
			//cout << "power to be added: " << power << endl;

			string machine = addStr(p, power);
			//cout << "machine: " << machine << endl;
			bool Found = false;
			bool machineExceeds = false;
			/*incase the machine exceeds the identifier space then subtract identifier space from it*/
			if (comparison(machine, identifierSpace) == 5)
			{
				string space = addStr(identifierSpace, "1");
				machine = subStr(machine, space);
			}
			//cout << "machine: " << machine << endl;
			//cout << "power: " << power << endl;
			DHTNode<T>* it = obj;
			do
			{
				//machine>Head->Previous->MachineID && machine<= Head->MachineID
				if (comparison(machine, Head->Previous->MachineID) == 5 && (comparison(machine, Head->MachineID) == -5 or comparison(machine, Head->MachineID) == 0))
				{
					//cout << "insertion:" << Head->MachineID << endl;
					obj->RoutingTable.insert(Head);
					Found = true;
				}
				else
				{
					if (comparison(it->MachineID, it->Next->MachineID) == 5)//if the machine id next is smaller than before
					{

						if (comparison(machine, it->MachineID) == 5 && (comparison(machine, identifierSpace) == -5 or comparison(machine, identifierSpace) == 0))
						{
							//cout << "insertion:" << it->Next->MachineID << endl;
							obj->RoutingTable.insert(it->Next);
							Found = true;
						}
					}
					else
					{
						/*
						cout << "found in else" << endl;
						cout<<"Head machin id: "<< Head->MachineID << endl;
						cout << "the machine: " << machine << endl;
						cout << "it: " << it->MachineID << endl;
						*/
						if (comparison(machine, Head->MachineID) == -5 or comparison(machine, Head->MachineID) == 0)
						{
							//cout << "it has happened" << endl;
							//cout << "insertion:" << Head->MachineID << endl;
							obj->RoutingTable.insert(Head);
							Found = true;
						}

						else if (comparison(machine, it->MachineID) == 5 && (comparison(machine, it->Next->MachineID) == -5 or comparison(machine, it->Next->MachineID) == 0))
						{
							//cout << "insertion:" << it->Next->MachineID << endl;
							obj->RoutingTable.insert(it->Next);

							Found = true;
							//it = obj;
						}
					}
				}
				it = it->Next;
			} while (/*it != obj &&*/ Found == false);

		}

	}

	// display the ids of all the machines present in the dht ring
	void DisplayMachineids()
	{
		cout << "num of machines specified at the start: " << NumOfMachines << endl;
		cout << "Identifier space: " << identifierSpace << endl;
		DHTNode<T>* it = Head;
		if (it != NULL)
		{
			do
			{
				//cout << it->MachineID << endl;
				it->DisplayInOrderTree_atMachine();
				it = it->Next;
			} while (it != Head);
		}

	}

	// jumps from one machine node to the other
	void Transfer_bw_Machine(DHTNode<T>* Machine, T start, T end, DHTNode<T>* newNode)
	{
		//cout << "start: " << start << endl;
		//cout << "end " << end << endl;
		RTLinkedList<AVLNode, T> Transfer;
		Machine->MachineTree.storeNodes(start, end, Machine->MachineTree.Root, Transfer);
		PointerNode<AVLNode, T>* it = Transfer.Head;
		while (it != NULL)
		{
			AVLNode<T>* temp = Machine->MachineTree.search(it->ptr->nodeID);//searches for the node with id start
			Machine->MachineTree.Root = Machine->MachineTree.SearchAndNullifyNodes(it->ptr->nodeID, Machine->MachineTree.Root);//nullifies that node from the other machine
			if (temp != NULL)//inserts that node  in the machine of the new tree
			{
				newNode->MachineTree.Root = newNode->MachineTree.insertAddresses(newNode->MachineTree.Root, temp);
			}
			//cout << "start: " << start << endl;
			//start = addStr(start, "1");
			it = it->Next;
		}

	}

	// remove a machine from the existing dht ring
	DHTNode<T>* deleteMachine(T Machinekey)//Machine key could be hashed or could not
	{

		//if id<0 or id>IdentifierSpace
		if (comparison(Machinekey, identifierSpace) == 5 or comparison(Machinekey, "0") == -5)
		{
			cout << "you have entered a machine that is out of bounds of identifier space." << endl;
			return NULL;

		}
		if (this->Head->Next == Head)//In the case that there is only one machine and it needs to be deleted
		{
			//cout << "what happened " << endl;
			delete Head;
			Head = NULL;
			NumOfMachines--;
			return NULL;
		}
		else
		{

			DHTNode<T>* Machine = this->SearchFromMachine(Machinekey, Head); //return the machine where machinekey should exist
			cout << "Machine ID: " << Machine->MachineID << endl;
			cout << "User Machine Path above" << endl;
			if (Machine->MachineID == Machinekey)//in the case that the machine ID is not repeated. MachineKey<Machine->MachineID always
			{
				//cout << "machine: " << Machine->MachineID << endl;

				DHTNode<T>* PreviousMachine = Machine->Previous;
				//cout << "previous machine: " << PreviousMachine->MachineID << endl;
				DHTNode<T>* TransferTo = Machine->Next;
				//cout << "transfer To: " << TransferTo->MachineID << endl;
				//TransferTo->DisplayInOrderTree_atMachine();

				//cout << ".....................machine................................................." << endl;
				//Machine->MachineTree.inOrderDB(Machine->MachineTree.Root);
				RTLinkedList<AVLNode, T> Transfer;
				Machine->MachineTree.TransferWholeTree(Machine->MachineTree.Root, Transfer);
				// cout << "Transfer Lenghth: " << Transfer.Length << endl;
				PointerNode<AVLNode, T>* it = Transfer.Head;
				while (it != NULL)
				{
					it->ptr->left = NULL;
					it->ptr->right = NULL;
					it->ptr->height = 1;
					TransferTo->MachineTree.Root = TransferTo->MachineTree.insertAddresses(TransferTo->MachineTree.Root, it->ptr);
					it = it->Next;
				}
				Transfer.deleteLinkedlist();
				//cout << "Root :  " << TransferTo->MachineTree.Root->nodeID << endl;
				//cout << "Transfer Root height: " << TransferTo->MachineTree.Root->height << endl;

				Machine->MachineTree.Root = NULL;
				PreviousMachine->Next = TransferTo;
				TransferTo->Previous = PreviousMachine;
				if (Machine == Head)
				{
					Head = TransferTo;
				}
				delete Machine;
				//cout << "Head: " << Head->MachineID << endl;
				//cout << "Head Previous: " << TransferTo->Previous->MachineID << endl;
				//cout << "Head Next: " << TransferTo->Next->MachineID << endl;
				NumOfMachines--;
				//this->DisplayMachineids();
				this->deleteRoutingTables();
				if (Head->Next != Head)
					this->assignRTs();
				return TransferTo;
			}
			else
			{
				cout << "Tha machine you entered doesn't exist" << endl;

			}
		}
		return NULL;
	}

	// add a new machine in the existing dht ring
	DHTNode<T>* AddMachine(T Machinekey)//Machine key could be hashed or could not
	{
		//if the number of machines has exceeded the identifier Space
		if (comparison(to_string(NumOfMachines), addStr(identifierSpace, "1")) == 5)
		{
			cout << "The number of machines has exceeded the identifierSpace" << endl;
			return NULL;
		}
		DHTNode<T>* newNode = NULL;
		//if id<0 or id>IdentifierSpace
		if (comparison(Machinekey, identifierSpace) == 5 or comparison(Machinekey, "0") == -5)
		{
			cout << "you have entered a machine that is out of bounds of identifier space." << endl;
			return NULL;

		}
		if (this->Head == nullptr)//adding the very first machine
		{
			newNode = new DHTNode<T>(Machinekey);
			Head = newNode;
			Head->Next = newNode;
			Head->Previous = newNode;
			NumOfMachines++;
		}
		else
		{
			DHTNode<T>* Machine = this->SearchFromMachine(Machinekey, Head); //return the machine where machinekey should exist
			cout << "Machine ID: " << Machine->MachineID << endl;
			cout << "User Machine Path above" << endl;
			if (Machine->MachineID != Machinekey)//in the case that the machine ID is not repeated. MachineKey<Machine->MachineID always
			{
				//cout << "machine: " << Machine->MachineID << endl;
				bool HeadCheck = false;
				T start;
				T end;
				DHTNode<T>* MachinePrevious = Machine->Previous;
				newNode = new DHTNode<T>(Machinekey);
				if (Machine == Head)
				{
					//if Machinekey<=idetifierspace && machinekey> Head->previous->MachineID
					if ((comparison(Machinekey, identifierSpace) == -5 or comparison(Machinekey, identifierSpace) == 0) && comparison(Machinekey, Head->Previous->MachineID) == 5)
					{
						start = Machine->Previous->MachineID;
						start = addStr(start, "1");
						end = addStr(Machinekey, "1");
						//cout << "machien is lesser than identifier space " << endl;
						Transfer_bw_Machine(Machine, start, end, newNode);
					}
					//Machinekey>= 0 && Machinekey<Head.MachineiD
					else if ((comparison(Machinekey, "0") == 5 or comparison(Machinekey, "0") == 0) && comparison(Machinekey, Head->MachineID) == -5)
					{
						start = Machine->Previous->MachineID;
						start = addStr(start, "1");
						end = addStr(identifierSpace, "1");
						Transfer_bw_Machine(Machine, start, end, newNode);

						start = "0";
						end = addStr(Machinekey, "1");
						Transfer_bw_Machine(Machine, start, end, newNode);
					}
					HeadCheck = true;
				}
				else
				{
					start = Machine->Previous->MachineID;// if insertion is between 9 and 14 then start is 10 and end is 15
					start = addStr(start, "1");

					end = Machinekey;
					end = addStr(end, "1");
					Transfer_bw_Machine(Machine, start, end, newNode);

				}
				//cout << "Where" << endl;
				//the newnode is between zero and the head of machine, then it becomes the new head
				if ((comparison(Machinekey, "0") == 0 or comparison(Machinekey, "0") == 5) && comparison(Machinekey, Head->MachineID) == -5)
				{
					MachinePrevious->Next = newNode;
					newNode->Previous = MachinePrevious;
					newNode->Next = Machine;
					Machine->Previous = newNode;

					Head = newNode;
					//cout << "Head id: " << Head->MachineID << endl;
				}
				else//Head remains the same
				{
					MachinePrevious->Next = newNode;
					newNode->Previous = MachinePrevious;
					newNode->Next = Machine;
					Machine->Previous = newNode;

				}
				NumOfMachines++;
				//cout << "nuumber of machines: " << NumOfMachines << endl;
				this->deleteRoutingTables();//updating routing tables
				this->assignRTs();
			}
			else{
				cout << "This machine ID already exits in the DHT ring" << endl;
			}
		}
		return newNode;
	}
};