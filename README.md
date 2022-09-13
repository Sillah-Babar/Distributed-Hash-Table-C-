# Circular Distributed Hash Table in C++:

***

# Implementation:

## Properties of Ring DHT
*  Ring DHT uses 160-bit circular identifier space using SHA-1. Figure 3 shows only a 4-bit identifier space to simplify the explanation.
*  Randomly chosen identifiers (ids in short) are assigned to machines (also called nodes in the
  following): H(Node Address) -> Node Id or Machine ID
  Figure 3 shows 5 machines with ids 1, 4, 7, 12 and 15.
*  Randomly chosen identifiers are assigned to data, i.e. (key, value) pairs: H(key)  Data Id
*  Data with identifier e is stored on the node/machine with the smallest identifier p ≥ e. This
  node/machine is called successor of e, denoted as succ(e). For example, Figure 3 depicts that
  machine with id 12 is responsible for storing the data with ids 8,9,10,11 and 12, because all these
  ids are less than or equal to the id of machine 12. The data with all these ids might not be available
  for storage but if such a data arrives this machine, i.e., machine with id 12, will be used for the
  storage. Likewise, machine with id 4 is responsible for storing data with ids 2, 3 and 4 (if
  available). 
* Each machine stores the data in the form of an AVL tree. Kindly note that the example in Figure 3
 is very simplistic, however, in reality a machine might be required to store millions of keys, value
 pairs. Once the machine (on which a data is stored) is identified, AVL tree searching can be used to
retrieve the actual data. 
![dht](https://user-images.githubusercontent.com/90323797/189929731-b8df6712-9de1-432e-ae1d-a7a73ff77185.jpg)
## Search Algorithm
The request to search a data, i.e., given a key search for the value, can arrive on any machine. The
machine will first determine whether the data is locally stored, i.e., id e of the data is less than or equal
to the id p of the machine (p ≥ e). If data is not locally found, the search request is forwarded to the
next node in the circular linked list and so on. This very simple search algorithm is not very efficient
with a complexity of O(N) where N is the number of machines in the distributed system. Why?
To efficiently resolve data identifier e to machine succ(e), each machine p contains routing table FTp
of at most O(log N) entries, where N is the number of machines in the distributed system. Each
routing table entry i contains the shortcut/pointer to the distant node, i.e., FTp[i]=succ(p + 2i-1),
where i=1,...,l≤log(N). Figure 4 shows a 5-bit identifier space. The FT of machine 1 includes
the links (also called shortcuts or pointers) to the machines who are responsible, i.e., succ, for the ids
(1+1), (1+2), (1+4), (1+8), (1+16).
![search_DHT](https://user-images.githubusercontent.com/90323797/189930204-e34da43d-cadd-488c-b513-49c0219864ee.jpg)

## How It Works

***

Given the properly maintained routing tables, the search query will be routed as follows:
* A machine p receives a key, i.e., H(key) = e and starts the process to search for the
corresponding value.
* Machine p receiving search(e) considers following cases:
* *  p=e, i.e. value is stored on the same machine. Use AVL search to find the data and return
the results.
* * p < e and e ≤ FTp [1]. In this case, the search request is forwarded to the machine
FTp[1], i.e., first entry of routing table entry.
* * FTp[j] < e ≤ FTp[ j+1]. In this case the search request is forwarded to the machine
FTp[j].

The above search algorithm using routing table will result in O(log N) lookups, where N is the
number of machines in the system. Why? Figure 5 shows the working of above algorithm, where the
search query for a data with H(key) = 12 is originated at node 28. 

![search-1](https://user-images.githubusercontent.com/90323797/189930833-0d76e22d-c8e2-49a3-baaa-8dd5e7dee437.jpg)


![search-2](https://user-images.githubusercontent.com/90323797/189930876-6aacbf09-98d6-44ef-9c69-57b2742e8bfa.jpg)
