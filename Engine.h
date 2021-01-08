#ifndef __ENGINE_H
#define __ENGINE_H

#include <iostream>
#include "Tree.cpp"
using namespace std;

class Engine
{
    private:
    string info;
    vector<pair<char, int>> frequency_table;
    Tree huffman_tree;
    vector<pair<char, string>> coding_table;
    string binary_string;
    int percentage;

    //help function for compression
    void readfromfile(string filename);
    void makeFrequencyTable();
    void makeCodingTable(Tree t);
    string getCharCode(char c); //help function for makeBinaryString()
    void makeBinaryString();
    void writetofile(string filename);
    void writeTable(ofstream &dataFile);
    void calculatePercentage();

    public: 

    Engine();
    void printInfo();
    void printFrequencyTable();
    void printTree();
    void printCodingTable();
    void printBinaryString();
    void printPercentage();

    void compress(string inputfile, string outputfile);
    void decompressDebug(string inputfile);
    void decompress(string outputfile, string dataFile, string decompressionFile);

};

#endif