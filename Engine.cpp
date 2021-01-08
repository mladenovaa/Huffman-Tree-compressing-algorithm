#include <iostream>
#include "Engine.h"
using namespace std;

//help function to read the data from input file
void Engine::readfromfile(string filename)
{
    ifstream myfile(filename);
    if (myfile.is_open())
    {
        getline (myfile, this->info);
        myfile.close();
    }

}

//help function to make the frequency table
void Engine::makeFrequencyTable()
{
    string temp = this->info;
    int size = temp.length();

    while(!temp.empty())
    {
        char symbol = temp.front();
        int freq = 0; 

        for(int i = 0; i < size; i++)
        {
            if(symbol == temp.at(i))
            {
                freq++;
                temp.erase(i,1);
                size--;
                i--;
            }
        }

        pair<char, int> topush(symbol, freq);
        this->frequency_table.push_back(topush);
        
    }
}

//help function to make the coding table
void Engine::makeCodingTable(Tree t)
{
    this->coding_table = t.makeCodingTable();
}

string Engine::getCharCode(char c)
{
    for(int i = 0; i < this->coding_table.size(); i++)
    {
        if(c == this->coding_table.at(i).first)
        {
            return this->coding_table.at(i).second;
        }
    }

    return "";
}

//help function to make the binary string (the compressed data)
void Engine::makeBinaryString()
{
    this->binary_string = getCharCode(this->info.at(0));

    for(int i = 1; i < this->info.size(); i++)
    {
        this->binary_string += getCharCode(this->info.at(i));
    }
}

//help function to write the binary string (the compressed data) to output file
void Engine::writetofile(string filename)
{
    ofstream myfile;
    myfile.open(filename);
    myfile << this->binary_string;
    myfile.close();
} 

//help function to write the coding table in dataFile for later decompression
void Engine::writeTable(ofstream &dataFile)
{
    dataFile << endl;
    for(int i = 0; i < this->coding_table.size(); i++)
    {
        dataFile << this->coding_table.at(i).first << this->coding_table.at(i).second ;
    }
}

void Engine::calculatePercentage()
{
    int symbolsize = this->info.size()*8;
    int binarysize = this->binary_string.size();

    this->percentage = (binarysize * 100) / symbolsize;
}

Engine::Engine()
{
    this->info = "";
    this->binary_string = "";
    this->percentage = 0;
}

void Engine::printInfo()
{
    cout << this->info <<endl;
}

void Engine::printFrequencyTable()
{
    for(int i = 0; i < this->frequency_table.size(); i++)
    {
        cout << this->frequency_table.at(i).first << ", " << this->frequency_table.at(i).second << endl;
    }
}

void Engine::printTree()
{
    this->huffman_tree.print(this->huffman_tree.getRoot());
    cout << endl;
}

void Engine::printCodingTable()
{
    for(int i = 0; i < this->coding_table.size(); i++)
    {
        cout << this->coding_table.at(i).first << ", " << this->coding_table.at(i).second << endl;
    }
}

void Engine::printBinaryString()
{
    cout << this->binary_string << endl;
}

void Engine::printPercentage()
{
    cout << "Percentage volume of the compressed information: " << this->percentage << "%" << endl;
}

void Engine::compress(string inputfile, string outputfile)
{
    readfromfile(inputfile);
    makeFrequencyTable();
    Tree huffmantree(this->frequency_table);
    this->huffman_tree = huffmantree;
    makeCodingTable(huffmantree);

    //write needed data in file for later decompression
    ofstream dataFile;
    dataFile.open("dataFile.txt");
    huffmantree.writeTree(huffmantree.getRoot(), dataFile);
    writeTable(dataFile);
    dataFile.close();
   
    makeBinaryString();
    writetofile(outputfile);

    //we calculate and print the compressed data volume 
    calculatePercentage();
    printPercentage();
}

void Engine::decompress(string outputfile, string dataFile, string decompressionFile)
{
    //reading the compressed information
    string binary_string;
    ifstream myfile(outputfile);
    if(myfile.is_open())
    {
        getline (myfile, binary_string);
        myfile.close();
    }

    //reading the tree and table information for decompress 
    string tree_string; 
    string table_string;
    ifstream decfile(dataFile);
    if(decfile.is_open())
    {
        getline(decfile, tree_string);
        getline(decfile, table_string);

        decfile.close();
    }

    //making the tree 
    Tree dec_tree(tree_string, 1, tree_string.length() - 2);
    this->huffman_tree = dec_tree;


    //initializing the table 
    vector<pair<string, string>> v;
    for(int i = 0; i < table_string.size(); i++)
    {
        if(table_string[i] != '1' && table_string[i] != '0')
        {
            string symbol;
            symbol.push_back(table_string[i]);
            string code = "";
            int j = i + 1;

            while(table_string[j] == '1' || table_string[j] == '0')
            {
                code += table_string[j];
                j++;
            }

            pair<string, string> pairtopush(symbol,code);
            v.push_back(pairtopush);
        }
    }

    //restoring the data
    string res = dec_tree.result(binary_string, v);

    ofstream dout;
    dout.open(decompressionFile);
    dout << res; 
    dout.close();

}

void Engine::decompressDebug(string inputfile)
{
    //reading the compressend data
    string binaryString;
    ifstream myfile(inputfile);
    if(myfile.is_open())
    {
        getline (myfile, binaryString);
        myfile.close();
    }

    string result = "";
    int decimal;
    int base;

    //calculating blocks of data with size 8
    while(binaryString.size() >= 8)
    {
        decimal = 0;
        base = 1;
        for(int i = 7; i >= 0; i--)
        {
            if(binaryString[i] == '1')
            {
                decimal += base;
            }
            
            base = base * 2;
        }

        result += to_string(decimal);
        result += " ";
        binaryString.erase(0,8);
    }

    //calculating the last block with size smaller or even to 8
    decimal = 0;
    base = 1;
    for(int i = binaryString.size() - 1; i >= 0; i--)
    {
            if(binaryString[i] == '1')
            {
                decimal += base;
            }
            
            base = base * 2;
    }

    result += to_string(decimal);

    cout << result << endl;
}