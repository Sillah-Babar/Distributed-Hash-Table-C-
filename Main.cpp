#include"CircularDHT.h"
//#include"FileHandling.cpp"
//#include <boost/multiprecision/cpp_int.hpp>
//int MachineIDChoice()
//{
//	char choice;
//	cout << "To manually assign IDs to the machines press Y." << endl;
//	cout << "To automatically assign IDs to the machines press N." << endl;
//	cin >> choice;
//
//	if (choice == 'y' || choice == 'Y') {		//ids assigned manually
//		return	1;
//	}
//	else if (choice == 'n' || choice == 'N') {		//ids automatically assigned
//		return 0;
//	}
//	return 8;
//}

int main()
{
	int bits, num;
	char choice;
	string id;

	cout << "Specify the number of machines you want to add: ";
	cin >> num;

	cout << "Speicify the size of the identifier space in bits: ";
	cin >> bits;

	DHTRing<string> ring(bits, num);

	cout << "To manually assign IDs to the machines press Y." << endl;
	cout << "To automatically assign IDs to the machines press N." << endl;
	cin >> choice;

	if (choice == 'y' || choice == 'Y') {		//ids assigned by user
		do {
			id;
			cout << "Enter Machine IP Address: ";
			cin >> id;

			id = Hashing(id, ring.identifierSpace);

			DHTNode<string>* temp = ring.AddMachine(id);
			if (temp != NULL)
				num--;
		} while (num != 0);
	}
	else if (choice == 'n' || choice == 'N') {		//ids automatically assigned
		do {
			cout << "Enter Machine ID : ";
			cin >> id;
			while (intcheck(id) == false)
			{
				cout << "Enter Valid numerical Machine ID : ";
				cin >> id;

			}
			DHTNode<string>* temp = ring.AddMachine(id);
			if (temp != NULL)
				num--;
		} while (num != 0);
	}


	char option = '0';
	while (option != 'x' && option != 'X')
	{
		cout << "1. Print the Routing Table of any machine." << endl;
		cout << "2. Add new machine." << endl;
		cout << "3. Remove existing machine." << endl;
		cout << "4. Print the AVL Tree." << endl;
		cout << "5. Delete Data from any machine" << endl;
		cout << "6. Insert Data in any Machine" << endl;
		cout << "7. Retrieve Data from any Machine" << endl;
		cout << "8. Display Machine ID's and respective AVL Trees" << endl;
		cout << endl << "Enter the option number to choose." << endl;
		cout << "Press X to exit menu." << endl;		//27 ascii of esc

		cin >> option;

		if (option == '1') {		//print routing table
			cout << "Enter Machine ID to display the Routing Table: " << endl;
			cin >> id;
			string saveid = id;
			if (choice == 'y' || choice == 'Y')
			{
				id = Hashing(id, ring.identifierSpace);
			}
			else
			{
				while (intcheck(id) == false)
				{
					cout << "Enter Valid numerical Machine ID : ";
					cin >> id;

				}
			}
			DHTNode<string>* temp = ring.findMachine(id);

			while (temp == NULL) {
				cout << "Enter valid Machine ID: ";
				cin >> id;
				if (choice == 'y' || choice == 'Y')
				{
					id = Hashing(id, ring.identifierSpace);
				}
				else
				{
					while (intcheck(id) == false)
					{
						cout << "Enter Valid numerical Machine ID : ";
						cin >> id;

					}
				}
				temp = ring.findMachine(id);
			}

			cout << "------------------------------------------" << endl;
			//DHTNode<string>* machine = ring.findMachine(id);
			if (temp != NULL && (choice == 'n' or choice == 'N'))
			{
				temp->PrintRoutingTable();
			}
			else
			{
				cout << "Original Machine ID: " << saveid << endl;
				temp->PrintRoutingTable();

			}
		}
		else if (option == '2') {		//add new machine



			if (choice == 'y' || choice == 'Y') {		//ids assigned by user
				string id;
				cout << "Enter new Machine's IP Address: ";
				cin >> id;
				id = Hashing(id, ring.identifierSpace);


				ring.AddMachine(id);
			}
			else if (choice == 'n' || choice == 'N') {		//ids automatically assigned
				string id;
				cout << "Enter new Machine's ID: ";
				cin >> id;

				while (intcheck(id) == false)
				{
					cout << "Enter Valid numerical Machine ID : ";
					cin >> id;

				}

				ring.AddMachine(id);
			}

		}
		else if (option == '3') {		//remove machine
			cout << "Enter the ID of the Machine you want to delete." << endl;
			cin >> id;
			if (choice == 'y' || choice == 'Y') {
				id = Hashing(id, ring.identifierSpace);
			}
			else
			{
				while (intcheck(id) == false)
				{
					cout << "Enter Valid numerical Machine ID : ";
					cin >> id;

				}
			}
			DHTNode<string>* temp = ring.deleteMachine(id);

			while (temp == NULL) {
				cout << "Enter valid Machine ID: ";
				cin >> id;
				if (choice == 'y' || choice == 'Y') {
					id = Hashing(id, ring.identifierSpace);
				}
				else
				{
					while (intcheck(id) == false)
					{
						cout << "Enter Valid numerical Machine ID : ";
						cin >> id;

					}
				}
				temp = ring.deleteMachine(id);
			}
		}
		else if (option == '4') {		//print AVL Tree
			cout << "Enter the ID of the Machine to display AVL Tree" << endl;
			cin >> id;
			string saveid = id;
			if (choice == 'y' || choice == 'Y') {
				id = Hashing(id, ring.identifierSpace);
			}
			else
			{
				while (intcheck(id) == false)
				{
					cout << "Enter Valid numerical Machine ID : ";
					cin >> id;

				}
			}
			DHTNode<string>* temp = ring.findMachine(id);

			while (temp == NULL) {
				cout << "Enter valid Machine ID: ";
				cin >> id;
				if (choice == 'y' || choice == 'Y') {
					id = Hashing(id, ring.identifierSpace);
				}
				else
				{
					while (intcheck(id) == false)
					{
						cout << "Enter Valid numerical Machine ID : ";
						cin >> id;

					}
				}
				temp = ring.findMachine(id);
			}

			cout << "------------------------------------------" << endl;
			if (temp != NULL && (choice == 'n' or choice == 'N'))
			{
				temp->DisplayInOrderTree_atMachine();
			}
			else
			{
				cout << "Original Machine ID: " << saveid << endl;
				temp->DisplayInOrderTree_atMachine();

			}

		}
		else if (option == '5')		//delete data from a machine
		{
			string key, UserMachine;

			cout << "Enter the Machine ID from which you want to delete data" << endl;
			cin >> UserMachine;
			if (choice == 'y' || choice == 'Y') {
				UserMachine = Hashing(UserMachine, ring.identifierSpace);
			}
			else
			{
				while (intcheck(UserMachine) == false)
				{
					cout << "Enter Valid numerical Machine ID : ";
					cin >> UserMachine;

				}
			}

			cout << "Enter the key for the data you want to delete" << endl;
			cin >> key;

			string val = ring.DeleteData(key, UserMachine);
			cout << "Value Deleted is : " << val << endl;

		}
		else if (option == '6')		//insert data in a machine
		{
			string key, Value;

			cout << "Enter the Machine ID from which you want to Insert data:" << endl;
			cin >> id;
			if (choice == 'y' || choice == 'Y') {
				id = Hashing(id, ring.identifierSpace);
			}
			else
			{
				while (intcheck(id) == false)
				{
					cout << "Enter Valid numerical Machine ID : ";
					cin >> id;

				}
			}
			cout << "Enter the key for the data you want to insert" << endl;
			cin >> key;

			cout << "Enter the Value for the data you want to insert" << endl;
			cin >> Value;


			DHTNode<string>* machine = ring.InsertData(id, key, Value);

			if (machine != NULL)
				cout << "Data is inserted on Machine with ID: " << machine->MachineID << endl;

			while (machine == NULL) {
				cout << "Enter valid Machine ID: ";
				cin >> id;
				if (choice == 'y' || choice == 'Y') {
					id = Hashing(id, ring.identifierSpace);
				}
				else
				{
					while (intcheck(id) == false)
					{
						cout << "Enter Valid numerical Machine ID : ";
						cin >> id;

					}
				}
				machine = ring.InsertData(id, key, Value);
			}

			if (machine != NULL)
				cout << "Data is inserted on Machine with ID: " << machine->MachineID << endl;

		}
		else if (option == '7')		//get data from a machine
		{
			string key;

			cout << "Enter the Machine ID you want to retrieve data from" << endl;
			cin >> id;
			if (choice == 'y' || choice == 'Y') {
				id = Hashing(id, ring.identifierSpace);
			}
			else
			{
				while (intcheck(id) == false)
				{
					cout << "Enter Valid numerical Machine ID : ";
					cin >> id;

				}
			}
			cout << "Enter the key for the data you want to retrieve" << endl;
			cin >> key;

			string val = ring.RetrieveData(key, id);

			cout << "Value Retrieved is : " << val << endl;



		}
		else if (option == '8') {	//display all machine ids along with their avl trees
			ring.DisplayMachineids();
		}
	}

}