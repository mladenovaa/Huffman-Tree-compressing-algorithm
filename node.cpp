#include <utility>
using namespace std; 

struct node
{
    //pair of symbol and a number that represents the frequency of the symbol
    pair<char, int> data; 
    node* left; 
    node* right;
    
};