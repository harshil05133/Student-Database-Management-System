#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include "AVL.h"

using namespace std;

int main(){

    AVLTree tree;
    /*
    tree.insert("Alice", "17010000");
    tree.insert("Bob", "10010000");
    tree.insert("Charlie", "10200000");
    tree.insert("Harshil", "98765432");
    tree.insert("Emma", "69420000");
    tree.insert("Harshil", "10000012");
    tree.insert("Emshil", "69420000");
    tree.insert("Harma", "17220000");
    tree.insert("Banana", "00000000");
    tree.insert("Boxy", "00000001");
    tree.insert("Harshil", "11111111");


    //tree.inOrderVector();
    cout << "Printing inorder" << endl;
    tree.printInOrder();

    cout << boolalpha << tree.checkIfValidID("1878888");
    cout << boolalpha << tree.checkIfValidName("immyj ean") << endl;

    tree.searchID("69420001");

    tree.printLevelCount();

    tree.searchName("Harshil");

    cout<< endl;
    tree.removeID("27010000");
    cout << "Printing inorder" << endl;
    tree.printInOrder();

    cout << endl;
    tree.removeInorderN(10);
    tree.printInOrder();
*/
    /*
    cout << "Printing preorder" << endl;
    tree.printPreOrder();
    cout << endl << endl << endl;
    cout << "Printing inorder" << endl;
    tree.printInOrder();
    cout << endl << endl << endl;
    cout << "Printing postorder" << endl;
    tree.printPostOrder();
     */

    //start to get input
    string commandAmountInput;

    //counter for number of commands executed
    int counter = 0;
    //get number of commands
    getline(cin, commandAmountInput);

    //keep going until all comands are executed
    while(counter < stoi(commandAmountInput)){
        string currentFunction;

        getline(cin, currentFunction);
        istringstream functions(currentFunction);

        string function;
        getline(functions, function, ' ');

        if(function == "insert"){
            string next;
            getline(functions, next, '"');

            string name;
            getline(functions, name, '"');

            string next2;
            getline(functions, next, ' ');

            string ufid;
            getline(functions, ufid, ' ');

            tree.insert(name, ufid);
            counter++;
        }
        else if(function == "remove"){
            string ufid;
            getline(functions, ufid, ' ');
            tree.removeID(ufid);
            counter++;
        }
        else if(function == "search"){
            string nameOrID;
            getline(functions, nameOrID, ' ');
            if(nameOrID[0] == '"'){
                string name = nameOrID.substr(1, nameOrID.length() -2); //cpp reference
                tree.searchName(name);
            }
            else{
                tree.searchID(nameOrID);
            }
            counter++;
        }
        else if (function == "printInorder"){
            tree.printInOrder();
            counter++;
        }
        else if (function == "printPreorder"){
            tree.printPreOrder();
            counter++;
        }
        else if (function == "printPostorder"){
            tree.printPostOrder();
            counter++;
        }
        else if(function == "printLevelCount"){
            tree.printLevelCount();
            counter++;
        }
        else if(function == "removeInorder"){
            string number;
            getline(functions, number, ' ');
            tree.removeInorderN(stoi(number));
            counter++;
        }
        //if they type in wrong commands
        else{
            cout << "unsuccessful" << endl;
        }
    }

    return 0;
}
