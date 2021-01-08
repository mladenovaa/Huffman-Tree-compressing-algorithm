#ifndef __TREE_H
#define __TREE_H

#include "node.cpp"
#include <vector>
#include <fstream>

class Tree
{
    private:
    node* root;
    
    //functions for constructor and deconstructor
    void copy(node*& self, node* other);
    void erase(node* self);

    //help functions
    void makeCodingTableHelper(node* root, string code, vector<pair<char, string>>& coding_table);
    int findIndex(string str, int start, int end);
    node* fromStringToTreeHelper(string tree_string, int start, int end);

    public: 

    Tree();
    Tree(pair<char,int> const _data, node* _left, node* _right);
    Tree(const Tree& t);
    Tree& operator=(const Tree& other);
    ~Tree();
    
    //construct the huffman tree from the frequency table when compression
    Tree(vector<pair<char, int>> frequency_table);

    //construct a tree from decoded data in dataFile when decompression
    Tree(string tree_string, int start, int end);

    //getters
    int getSecond() const;
    char getFirst() const;
    node* getRoot() const;

    //tree function used when compressing and decompressing 
    vector<pair<char,string>> makeCodingTable();
    void writeTree(node* t, ofstream &dataFile);
    string getSymbol(vector<pair<string,string>> v, string s); //help function for result function
    string result(string binary_string, vector<pair<string, string>> v);
    void print(node* root);

};


#endif