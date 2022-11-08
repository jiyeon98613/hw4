#include "equal-paths.h"
#include <iostream>
using namespace std;


// You may add any prototypes of helper functions here


bool equalPaths(Node * root)
{

    int leafHeight = -1;
    return helper_equalPaths(root, 0, leafHeight);
}

bool helper_equalPaths(Node* root, int currHeight, int& leafHeight)
{
    // ++height;
    if (!root) {
        return true;
    }

    //base case: when succeed: when they hit the leaf node
    if(root->left == nullptr && root->right == nullptr)
    {
        if (leafHeight == -1) {
            leafHeight = currHeight;
        }
        return currHeight == leafHeight;
    }    

    bool valLeft = helper_equalPaths(root->left, currHeight+1, leafHeight);
    bool valRight = helper_equalPaths(root->right, currHeight+1, leafHeight);

    return valLeft && valRight;

    // else if(root->left == nullptr && root->right != nullptr)
    // {
    //     //if this node have right node
    //     return helper_equalPaths(root->right, height);

    // }
    // else if(root->left != nullptr && root->right == nullptr)
    // {
    //     //if this node have left node
    //     return helper_equalPaths(root->left, height);


    // }
    // else if(root->left != nullptr && root->right != nullptr)
    // {
    //     //if this node have left node
    //     int& l = helper_equalPaths(root->left, height);
    //     int& r = helper_equalPaths(root->right, height);
    //     //compare two heights and return the bigger int
    //     int m = -1;
    //     if(l != r){return m;}
    //     else {return l;}

    // }
    // else {cout << "height: "<< height<<", but this should not be printed out."<<endl;}


}

