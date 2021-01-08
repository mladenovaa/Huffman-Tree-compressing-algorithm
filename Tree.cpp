#include <iostream>
#include "Tree.h"
#include <string>
using namespace std;

void Tree::copy(node*& self, node* other)
{
	if (other == nullptr)
	{
		self = nullptr;
		return;
	}

	self = new node{ other->data, nullptr, nullptr };
	copy(self->left, other->left);
	copy(self->right, other->right);
}

void Tree::erase(node* self)
{
	if (self == nullptr)
	{
		return;
	}
	node* save = self;

	if(self->left != nullptr)
	{
		erase(self->left);
	}
	if(self->right != nullptr)
	{
		erase(self->right);
	}
	delete save;
}

//help function to make the coding table from the huffmantree
void Tree::makeCodingTableHelper(node* root, string code, vector<pair<char, string>>& coding_table)
{
	if (root == nullptr)
	{
		return;
	}

	if (root->left == nullptr)
	{
		pair<char, string> pairtopush(root->data.first, code);
		coding_table.push_back(pairtopush);
		return;
	}

	makeCodingTableHelper(root->left, code + "0", coding_table);
	makeCodingTableHelper(root->right, code + "1", coding_table);
}

//help function when decoding the tree from dataFile
//it finds the index of a corresponding closing bracket
int Tree::findIndex(string str, int start, int end)
{
	if (start > end)
	{
		return -1;
	}

	int brackets = 0;
	for (int i = start; i <= end; i++)
	{
		if (str[i] == '(')
		{
			brackets++;
		}
		else if (str[i] == ')')
		{
			brackets--;
		}

		if (brackets == 0)
		{
			return i;
		}
	}

	return -1;
}

//help function to decode the tree from dataFile
node* Tree::fromStringToTreeHelper(string tree_string, int start, int end)
{
	if (start > end)
	{
		return nullptr;
	}

	pair<char, int> pairtopush(tree_string[start], 0);
	node* t = new node{pairtopush, nullptr, nullptr};

	int index = -1;

	if (start + 1 <= end && tree_string[start + 1] == '(')
	{
		index = findIndex(tree_string, start + 1, end);
	}

	if (index != -1)
	{
		t->left = fromStringToTreeHelper(tree_string, start + 2, index - 1);
		t->right = fromStringToTreeHelper(tree_string, index + 2, end - 1);
	}

	return t; 
}

Tree::Tree()
{
	this->root = nullptr;
}

Tree::Tree(pair<char, int> const _data, node* _left, node* _right)
{
	this->root = new node{ _data, nullptr, nullptr };
	copy(root->left, _left);
	copy(root->right, _right);
}

Tree::Tree(const Tree& t)
{
	this->root = new node{ t.root->data, nullptr, nullptr };
	copy(root->left, t.root->left);
	copy(root->right, t.root->right);
}

Tree& Tree::operator=(const Tree& other)
{
	if (this != &other)
	{
		this->erase(this->root);
		this->copy(this->root, other.root);
	}
	return *this;
}

Tree::~Tree()
{
	erase(this->root);
}

Tree::Tree(vector<pair<char, int>> frequency_table)
{
	//create a vector of trees which represents
	//the leaves of the Huffman tree
	vector<Tree*> treeLeaves;
	for (int i = 0; i < frequency_table.size(); i++)
	{
		Tree* leave = new Tree(frequency_table.at(i), nullptr, nullptr);
		treeLeaves.push_back(leave);
	}

	//until we have created the whole tree 
	while (treeLeaves.size() != 1)
	{
		//find the tree with minimum int in its root
		Tree* min1 = treeLeaves.front();
		int position = 0;

		for (int i = 0; i < treeLeaves.size(); i++)
		{
			if (min1->getSecond() > treeLeaves.at(i)->getSecond())
			{
				min1 = treeLeaves.at(i);
				position = i;
			}
		}

		//delete it from the vector so we can find the second minimum
		treeLeaves.erase(treeLeaves.begin() + position);

		//find the second
		Tree* min2 = treeLeaves.front();
		position = 0;

		for (int i = 0; i < treeLeaves.size(); i++)
		{
			if (min2->getSecond() > treeLeaves.at(i)->getSecond())
			{
				min2 = treeLeaves.at(i);
				position = i;
			}
		}

		//delete the second minimum from the vector
		treeLeaves.erase(treeLeaves.begin() + position);

		//create new tree from the two minimums and add it to the vector
		int mergeddata = min1->getSecond() + min2->getSecond();
		pair<char, int> pushpair(' ', mergeddata);
		Tree* mergednode = new Tree(pushpair, min1->getRoot(), min2->getRoot());
		treeLeaves.push_back(mergednode);
		
		delete min1;
		delete min2;
	}

	//now we have the final tree in vector[0] and we initialize the tree
	this->root = new node{treeLeaves.at(0)->getRoot()->data, nullptr, nullptr};
	copy(root->left, treeLeaves.at(0)->getRoot()->left);
	copy(root->right, treeLeaves.at(0)->getRoot()->right);

	delete treeLeaves[0];
}

Tree::Tree(string tree_string, int start, int end)
{
	this->root = fromStringToTreeHelper(tree_string, start, end);
}

int Tree::getSecond() const
{
	return this->root->data.second;
}

char Tree::getFirst() const
{
	return this->root->data.first;
}

node* Tree::getRoot() const
{
	return this->root;
}

//making the coding table from the huffman tree
vector<pair<char, string>> Tree::makeCodingTable()
{
	vector<pair<char, string>> coding_table;
	makeCodingTableHelper(this->root, "", coding_table);

	return coding_table;
}

//when we compress we write the tree in dataFile for later decoding
void Tree::writeTree(node* t, ofstream& dataFile)
{
	if (t == nullptr)
	{
		return;
	}

	dataFile << "(";

	if (t->left == nullptr && t->right == nullptr)
	{
		dataFile << t->data.first;
	}
	else
	{
		dataFile << "*";
	}
	writeTree(t->left, dataFile);
	writeTree(t->right, dataFile);
	dataFile << ")";
}

//help function for decompressing the data
string Tree::getSymbol(vector<pair<string, string>> v, string s)
{
	for (int i = 0; i < v.size(); i++)
	{
		if (v[i].second == s)
		{
			return v[i].first;
		}
	}

	return "";
}

//function used in decompression
//it finds the original data using the huffman tree, the decoded binary data and frequency of every symbol
string Tree::result(string binary_string, vector<pair<string, string>> v)
{
	node* temp = this->root;
	string result;
	int start = 0;
	for (int i = 0; i <= binary_string.size(); i++)
	{
		//if we reached a leaf
		if (temp->left == nullptr && temp->right == nullptr)
		{
			string code = binary_string.substr(start, i - start);
			result = result + getSymbol(v, code);
			start = i;
			
			//restart the search 
			temp = this->root;
		}

		if (binary_string[i] == '0')
		{
			temp = temp->left;
		}

		if (binary_string[i] == '1')
		{
			temp = temp->right;
		}
	}

	return result;
}

void Tree::print(node* root)
{
	if (root == nullptr)
	{
		return;
	}

	cout << "(";

	if (root->left == nullptr && root->right == nullptr)
	{
		cout << root->data.first << ", " << root->data.second;
	}
	else
	{
		cout << root->data.second;
	}
	print(root->left);
	print(root->right);
	cout << ")";
}