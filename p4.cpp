#include <iostream>
#include <string>
#include <string.h>
#include <fstream>
#include <sstream>
#include <list>
#include <vector>

const int T_S = 8;
int slot_of_key;
using namespace std;
string orig_slot_key;



unsigned long djb2(string str) {
    const char *ptr = str.c_str();
    unsigned long hash = 5381;
    int c;
    while ((c = *ptr++)) {
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}


struct Node{
    public:
        string value;
        vector<string> vec;
        Node* right;
};

int setSlot(string key, Node* header);

class HashTable {    
    public:
    int size;
    vector<vector<vector<string> > > ht;

    HashTable(int size) {
        this->size = size; 
        ht.resize(size);
    }

    void Insert(string key, vector<string> vec){
        unsigned long place = djb2(key) % this->size;
        ht[place].push_back(vec);
        return;
    }
    // TODO: go set slot_of_key to arg
    void Print() {
        for(int i = 0; i < size; i++) {
            if(ht.at(i).size() > 0) {
                    cout << i << ": " << ht.at(i).at(0).at(slot_of_key) << " (" << ht.at(i).size() << "),\n";
            }
        }
    }

    void LookUp(Node* header, string key) {
        bool gate = true;
        bool check = false;
        Node* temp = header;

        for(int i = 0; i < size; i++) {
            for(int j = 0; j < ht.at(i).size(); j++) {
                if(ht.at(i).at(j).at(slot_of_key)== key) {
                    if(!check) {
                        check = true;
                        while(gate) {
                            if(temp->right != nullptr) {
                                cout << temp->value << ",";
                                temp = temp->right;
                            }
                            else {
                                gate = false;
                                cout << temp->value << endl;
                            }
                        }
                    }
                    for(int z = 0; z < 8; z++) {
                        cout << ht.at(i).at(j).at(z) << ",";
                    }  
                }
            }
        }
        if(!check) cout << "No results\n";
        int i = setSlot(orig_slot_key, header);
        return;
    }
};

void getVal(istream &is, string &str) {
    char ch;
    string line;

    is >> ch;
    getline(is, line);

    str = ch + line;
}


bool isValidKey(const string &key) {
    string validKeys[8] = { "Id", "FirstName", "LastName", "Email", "Phone", "City", "State", "PostalCode" };
    for (int i = 0; i < 8; i++) {
        if (key == validKeys[i]) {
            return true;
        }
    }
    return false;
}


int setSlot(string key, Node* header) {

    Node* temp = new Node;
    temp = header;
    bool gate = true;
    int index = 0;
    while(gate) {
        if(temp->value == key) {
            slot_of_key = index;
            return 1;
        } else {
            if(temp->right != nullptr) temp = temp->right;
            else return 0;
        }
        index++;
    }

    return 2;

}

void getKey(istream &is, Node* header) {
    string temp;
    getline(is,temp,' ');
    // lookup
    getline(is, temp, ' ');
    int i = setSlot(temp, header);
}

int main(int argc, char **argv) {
    if (argc != 4) {
        cout << "Usage: ./a.out filename.txt table_size key" << endl;
        return 1;
    }

    string filename = argv[1];
    int tableSize = stoi(argv[2]);
    string key = argv[3];
    

    unsigned long newKey = djb2(key) % tableSize;

    //start this hoe
    //HashTable HT(tableSize);
    HashTable HT(tableSize);
    



    ifstream file(filename);
    if (!file) {
        cout << "Unable to open " << filename << endl;
        return 2;
    }

    if (!isValidKey(key)) {
        cout << "Invalid key: " << key << endl;
        return 3;
    }
   
    // This is an example of how to retreive the tokens from the input file
    // You will need to modify this to fit your needs to build the hash table
    string line, token;
    Node* header = new Node;
    Node* temp = new Node;
    getline(file,line);
    istringstream iss(line);
    getline(iss,token,'\t');
    header->value = token;
    temp = header;

    // while loop to set the first line nodes
    while(getline(iss,token,'\t')){
        Node* newtemp = new Node;
        temp->right = newtemp;
        newtemp->value = token;
        temp = temp->right;
    }
    temp = header;

    orig_slot_key = key;
    int check = setSlot(orig_slot_key, header);
    if(check == 0) {
        cout << "error, typo in passing key";
        return -2;
    }


    // while loop to set the remaining lines
    int rowCount = 0;
    while (getline(file, line)) {
        istringstream iss(line);
        temp = header;
        vector<string> keyvector;

        while (getline(iss, token, '\t')) {
           temp->vec.push_back(token);
           keyvector.push_back(token);
           if(temp->right != nullptr) temp = temp->right;
        }

        HT.Insert(keyvector.at(slot_of_key), keyvector);
        rowCount++;
    } 
    temp = header;
    
    cout << "Commands:" << endl << "\tprint" << endl << "\tlookup <key>" << endl << "\tquit" << endl;
    string cmd, val;
    while (1) {
        cout << endl << "Enter a command:" << endl;
        cin >> cmd;
        if (cmd == "quit") {
            return 0;
            break;
        }
        else if (cmd == "print") {
            HT.Print();
            // print code here
        }
        else if (cmd == "lookup") {
            getVal(cin, val);
            HT.LookUp(header, val);

            // lookup code here
        }
        else {
            getline(cin, val);
            cout << "Invalid command" << endl;
        }
    }

    return 0;
}
