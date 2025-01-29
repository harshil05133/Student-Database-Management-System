//
// Created by ratho on 9/30/2024.
//

#ifndef PROJECT1_AVL_H
#define PROJECT1_AVL_H

#include <iostream>
#include <queue>
#include <string>
#include <vector>
using namespace std;

struct StudentNode{
    string name;
    string ufid;
    int height;
    StudentNode* left;
    StudentNode* right;

    StudentNode(string& name, string& ufid){
        this->name = name;
        this->ufid = ufid;
        this->left = nullptr;
        this->right = nullptr;
        height = 0;
    }
};

//function prototypes
class AVLTree{
private:
    StudentNode* root;
public:
    AVLTree(){
        root = nullptr;
    }
    ~AVLTree(){
        //use a postorder traversal to delete a node since each node
        //will only get visited once with a postorder traversal
        deleteTree(root);
    }

    //got from dsa lecture/videos from amanpreet kapoor
    StudentNode* leftRotate(StudentNode* node){
        if(node == nullptr or node->right == nullptr){
            return node;
        }
        else{
            StudentNode* grandchild = node->right->left;
            StudentNode* newParent = node->right;
            newParent->left = node;
            node->right = grandchild;

            //recalculate height of the nodes
            node->height = 1 + max((node->left == nullptr?0 : node->left->height), (node->right == nullptr? 0: node->right->height));
            newParent->height = 1 + max((newParent->left == nullptr ? 0 : newParent->left->height),(newParent->right == nullptr ? 0 : newParent->right->height));

            return newParent;
        }
    }

    //got from dsa lecture/videos from amanpreet kapoor
    StudentNode* rightRotate(StudentNode* node){
        if(node == nullptr or node->left == nullptr){
            return node;
        }
        else{
            StudentNode* grandchild = node->left->right;
            StudentNode* newParent = node->left;
            newParent->right = node;
            node->left = grandchild;

            //recalculate height of the nodes
            node->height = 1 + max((node->left == nullptr?0 : node->left->height), (node->right == nullptr? 0: node->right->height));
            newParent->height = 1 + max((newParent->left == nullptr ? 0 : newParent->left->height),(newParent->right == nullptr ? 0 : newParent->right->height));

            return newParent;
        }
    }

    StudentNode* leftRightRotate(StudentNode* node){
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    StudentNode* rightLeftRotate(StudentNode* node){
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    //got from dsa lecture/videos from amanpreet kapoor/lisha zhou
    //what does double inverted commas mean?
    int calculateHeight(StudentNode* rootNode) {
        if(rootNode == nullptr){
            return 0;
        }
        rootNode->height = 1 + max(calculateHeight(rootNode->left), calculateHeight(rootNode->right));
        return rootNode->height;
    }

    //cleaner way to calculate balance
    int calculateBalance(StudentNode* rootNode) {
        if(rootNode == nullptr){
            return 0;
        }
        return calculateHeight(rootNode->left) - calculateHeight(rootNode->right);
    }

    StudentNode* insert(StudentNode* rootNode, string name, string ufid) {
        //checks if id and name are valid before inserting
        if (checkIfValidID(ufid) == false or checkIfValidName(name) == false) {
            cout << "unsuccessful" << endl;
            return nullptr;
        }
        if(rootNode == nullptr){
            cout << "successful" << endl;
            return new StudentNode(name, ufid);
        }
        //check if id already exists bc you cant have duplicates
        if(ufid == rootNode->ufid){
            cout << "unsuccessful" << endl;
            return rootNode;
        }

        //recursively go until nullptr
        if(ufid < rootNode->ufid){
            rootNode->left = insert(rootNode->left, name, ufid);
        }
        else{
            rootNode->right = insert(rootNode->right, name, ufid);
        }
        //update hieghts
        rootNode->height = 1 + max(calculateHeight(rootNode->left), calculateHeight(rootNode->right));

        //ok made an easier and cleaner way to do this.
        int balanceFactor = calculateBalance(rootNode);

        if(balanceFactor <= -2){
            if(calculateBalance(rootNode->right) >= 1){
                rootNode = rightLeftRotate(rootNode);
            }
            else{
                rootNode = leftRotate(rootNode);
            }
        }
        else if(balanceFactor >= 2){
            if(calculateBalance(rootNode->left) <= -1){
                rootNode = leftRightRotate(rootNode);
            }
            else{
                rootNode = rightRotate(rootNode);
            }
        }
        return rootNode;

        //if tree is right heavy, look at right subtree and see if its right or left heavy
        //else if tree is left heavy then look at left subtree and see if its right or left heavy
        if(((rootNode->left == nullptr? 0 : rootNode->left->height) - (rootNode->right == nullptr? 0: rootNode->right->height)) <= -2){
            if(((rootNode->right->left == nullptr? 0: rootNode->right->left->height) - (rootNode->right->right == nullptr? 0: rootNode->right->right->height)) >= 1){
                rootNode = rightLeftRotate(rootNode);
            }
            else{
                rootNode = leftRotate(rootNode);
            }
        }
            //theres got to be an easier way to do this
        else if(((rootNode->left == nullptr? 0: rootNode->left->height) - (rootNode->right == nullptr? 0: rootNode->right->height)) >= 2){
            if(((rootNode->left->left == nullptr? 0: rootNode->left->left->height) - (rootNode->left->right == nullptr? 0: rootNode->left->right->height)) <= -1){
                rootNode = leftRightRotate(rootNode);
            }
            else{
                rootNode = rightRotate(rootNode);
            }
        }
        return rootNode;
    }

// how to add the 0s for leading 0s? nvm switching it to a string
//we supposed to cite isdigit and isalpha but like i already knew them so...
    bool checkIfValidID(string ufid){
        if(ufid.length() != 8){
            return false;
        }
        for(int i = 0; i < int(ufid.length()); i++){
            if(!isdigit(ufid[i])){
                return false;
            }
        }
        return true;
    }

    bool checkIfValidName(string name){
        for(int i = 0; i < int(name.length()); i++){
            if(!isalpha(name[i]) and name[i] != ' '){
                return false;
            }
        }
        return true;
    }

    //actual searchID and return name function
    void searchID(StudentNode* rootNode, string& ufid){
        queue<StudentNode*> idQueue;
        if (rootNode == nullptr){
            cout << "unsuccessful" << endl;
            return;
        }
        idQueue.push(rootNode);
        while(!idQueue.empty()){
            StudentNode* currentID = idQueue.front();
            idQueue.pop();
            if(currentID->ufid == ufid){
                cout << currentID->name << endl;
                return;
            }
            if(currentID->right != nullptr){
                idQueue.push(currentID->right);
            }
            if(currentID->left != nullptr){
                idQueue.push(currentID->left);
            }
        }
        cout << "unsuccessful" << endl;
    }

    //extra searchID function that retuns true or false for existence to see if it exists before removing
    bool searchIDForDeletion(StudentNode* rootNode, string& ufid){
        queue<StudentNode*> idQueue;
        idQueue.push(rootNode);
        while(!idQueue.empty()){
            StudentNode* currentIDNode = idQueue.front();
            idQueue.pop();
            if(currentIDNode->ufid == ufid){
                return true;
            }
            if(currentIDNode->right != nullptr){
                idQueue.push(currentIDNode->right);
            }
            if(currentIDNode->left != nullptr){
                idQueue.push(currentIDNode->left);
            }
        }
        return false;
    }

    // make queue for bfs
    void searchName(StudentNode* rootNode, string& name){
        queue<StudentNode*> nameQueue;
        vector<StudentNode*>vectorForMatchedNames;
        if(rootNode == nullptr){
            cout << "unsuccessful" << endl;
            return;
        }
        nameQueue.push(rootNode);
        while(!nameQueue.empty()){
            StudentNode* currentName = nameQueue.front();
            nameQueue.pop();
            if(currentName->name == name){
                vectorForMatchedNames.push_back(currentName); //put the node with matching name into vector so it can be sorted in preorder fasion
            }
            if(currentName->right != nullptr){
                nameQueue.push(currentName->right);
            }
            if(currentName->left != nullptr){
                nameQueue.push(currentName->left);
            }
        }
        //rip it no exist
        if (vectorForMatchedNames.empty()){
            cout << "unsuccessful" << endl;
            return;
        }
        printPreOrder(rootNode, vectorForMatchedNames);
        //what does the parsing thing mean?
    }

    //different type of search function for preorder search
    void searchNameWithDFS(StudentNode* rootNode, string& name, vector<StudentNode*>& vectNodes){
        if(rootNode == nullptr){
            return;
        }
        if(rootNode->name == name){
            vectNodes.push_back(rootNode);
        }
        searchNameWithDFS(rootNode->left, name, vectNodes);
        searchNameWithDFS(rootNode->right, name, vectNodes);
    }
    //helper for it
    void searchName(string name){
        vector<StudentNode*> vectNodes;
        searchNameWithDFS(root, name, vectNodes);
        if(vectNodes.empty()){
            cout << "unsuccessful" << endl;
            return;
        }
        for(int i = 0; i < int(vectNodes.size()); i++){
            cout << vectNodes[i]->ufid << endl;
        }
    }

    StudentNode* removeID(StudentNode* rootNode, string ufid){
        if(rootNode == nullptr){
            cout << "unsuccessful" << endl;
            return rootNode;
        }
        //see if it exists, if it does then its unsuccessful
        if(searchIDForDeletion(rootNode, ufid) == false){
            cout << "unsuccessful" << endl;
            return rootNode;
        }
        //yay we found it
        if(ufid == rootNode->ufid){
            //no children lucky ducky
            if(rootNode->left == nullptr and rootNode->right == nullptr){
                delete rootNode;
                rootNode = nullptr;
                cout << "successful" << endl;
            }
                //one child on the right
            else if(rootNode->left == nullptr and rootNode->right != nullptr){
                StudentNode* tempNode = rootNode;
                rootNode = rootNode->right;
                delete tempNode;
                cout << "successful" << endl;
            }
                //one child on the left
            else if(rootNode->left != nullptr and rootNode->right == nullptr){
                StudentNode* tempNode = rootNode;
                rootNode = rootNode->left;
                delete tempNode;
                cout << "successful" << endl;
            }
            else{ //two children so get inorder successor (smallest number or closest number to the right of the node)
                StudentNode* tempNode = rootNode->right;
                while(tempNode->left != nullptr){
                    tempNode = tempNode->left;
                }
                rootNode->ufid = tempNode->ufid;
                rootNode->name = tempNode->name;
                rootNode->right = removeID(rootNode->right, tempNode->ufid);
            }
        }
        //recursively try to find the id
        else if(ufid < rootNode->ufid){
            rootNode->left = removeID(rootNode->left, ufid);
        }
        else{
            rootNode->right = removeID(rootNode->right, ufid);
        }
        //if rootNode is nullptr bc it got deleted it needs to be checked again
        if(rootNode == nullptr){
            return rootNode;
        }
        //update height bc we have to after deletion but thank god no rotation stuff again
        rootNode->height = 1 + max((rootNode->left == nullptr?0 : rootNode->left->height), (rootNode->right == nullptr? 0: rootNode->right->height));

        return rootNode;
    }

    //last function yay
    void removeInorderN(StudentNode* rootNode, int n){
        if(rootNode == nullptr){
            cout << "unsuccessful" << endl;
            return;
        }
        vector<string> idVect;
        inOrderVector(rootNode, idVect);

        //check if theres even that many nodes in the tree before accessing or else bad things will happen
        if(n >= int(idVect.size())){
            cout << "unsuccessful" << endl;
            return;
        }
        removeID(rootNode, idVect[n]);
    }

    //left -> node -> right
    void inOrderVector(StudentNode* rootNode, vector<string>& idVect){
        if(rootNode == nullptr){
            return;
        }
        inOrderVector(rootNode->left, idVect);
        idVect.push_back(rootNode->ufid);
        inOrderVector(rootNode->right, idVect);
    }

    void printInOrder(StudentNode* rootNode, vector<StudentNode*>& vectNodes){
        if(rootNode != nullptr) {
            printInOrder(rootNode->left, vectNodes);
            vectNodes.push_back(rootNode);
            printInOrder(rootNode->right, vectNodes);
        }
    }

    //Node -> left ->right
    void printPreOrder(StudentNode* rootNode, vector<StudentNode*>& vectNodes){
        if(rootNode != nullptr) {
            vectNodes.push_back(rootNode);
            printPreOrder(rootNode->left, vectNodes);
            printPreOrder(rootNode->right, vectNodes);
        }
    }

    //left->right->node
    void printPostOrder(StudentNode* rootNode, vector<StudentNode*>& vectNodes){
        if(rootNode != nullptr) {
            printPostOrder(rootNode->left, vectNodes);
            printPostOrder(rootNode->right, vectNodes);
            vectNodes.push_back(rootNode);;
        }
    }


    //prints height in 1based height
    void printLevelCount(StudentNode* rootNode){
        if(rootNode == nullptr){
            cout << 0 << endl;
        }
        else{
            cout << (rootNode->height) << endl;
        }
    }



    //helper functions to use in main
    StudentNode* insert(string name, string ufid) {
        root = insert(root, name, ufid);
        return root;
    }

    void searchID(string ufid){
        searchID(root, ufid);
    }


    StudentNode* removeID(string ufid){
        root = removeID(root, ufid);
        return root;
    }

    void removeInorderN(int n){
        removeInorderN(root, n);
    }

    void printInOrder() {
        vector<StudentNode*> vectNodes;
        printInOrder(root, vectNodes);
        for(int i = 0; i < int(vectNodes.size() - 1); i++){
            cout << vectNodes[i]->name << ", ";
        }
        cout << vectNodes[vectNodes.size()-1]->name << endl;
    }

    void printPreOrder(){
        vector<StudentNode*> vectNodes;
        printPreOrder(root, vectNodes);
        for(int i = 0; i < int(vectNodes.size() - 1); i++){
            cout << vectNodes[i]->name << ", ";
        }
        cout << vectNodes[vectNodes.size()-1]->name << endl;
    }

    void printPostOrder(){
        vector<StudentNode*> vectNodes;
        printPostOrder(root, vectNodes);
        for(int i = 0; i < int(vectNodes.size() - 1); i++){
            cout << vectNodes[i]->name << ", ";
        }
        cout << vectNodes[vectNodes.size()-1]->name << endl;
    }

    void printLevelCount(){
        printLevelCount(root);
    }

    void inOrderVector(vector<string>& vect){
        inOrderVector(root, vect);
    }

    void deleteTree(StudentNode* rootNode){
        if(rootNode == nullptr){
            return;
        }
        deleteTree(rootNode->left);
        deleteTree(rootNode->right);
        delete rootNode;
    }

};

#endif //PROJECT1_AVL_H
