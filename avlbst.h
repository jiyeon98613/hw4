#ifndef RBBST_H
#define RBBST_H

#include <iostream>
#include <exception>
#include <cstdlib>
#include <cstdint>
#include <algorithm>
#include "bst.h"

struct KeyError { };

/**
* A special kind of node for an AVL tree, which adds the balance as a data member, plus
* other additional helper functions. You do NOT need to implement any functionality or
* add additional data members or helper functions.
*/
template <typename Key, typename Value>
class AVLNode : public Node<Key, Value>
{
public:
    // Constructor/destructor.
    AVLNode(const Key& key, const Value& value, AVLNode<Key, Value>* parent);
    virtual ~AVLNode();

    // Getter/setter for the node's height.
    int8_t getBalance () const;
    void setBalance (int8_t balance);
    void updateBalance(int8_t diff);

    // Getters for parent, left, and right. These need to be redefined since they
    // return pointers to AVLNodes - not plain Nodes. See the Node class in bst.h
    // for more information.
    virtual AVLNode<Key, Value>* getParent() const override;
    virtual AVLNode<Key, Value>* getLeft() const override;
    virtual AVLNode<Key, Value>* getRight() const override;

protected:
    int8_t balance_;    // effectively a signed char
};

/*
  -------------------------------------------------
  Begin implementations for the AVLNode class.
  -------------------------------------------------
*/

/**
* An explicit constructor to initialize the elements by calling the base class constructor and setting
* the color to red since every new node will be red when it is first inserted.
*/
template<class Key, class Value>
AVLNode<Key, Value>::AVLNode(const Key& key, const Value& value, AVLNode<Key, Value> *parent) :
    Node<Key, Value>(key, value, parent), balance_(0)
{

}

/**
* A destructor which does nothing.
*/
template<class Key, class Value>
AVLNode<Key, Value>::~AVLNode()
{

}

/**
* A getter for the balance of a AVLNode.
*/
template<class Key, class Value>
int8_t AVLNode<Key, Value>::getBalance() const
{
    return balance_;
}

/**
* A setter for the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::setBalance(int8_t balance)
{
    balance_ = balance;
}

/**
* Adds diff to the balance of a AVLNode.
*/
template<class Key, class Value>
void AVLNode<Key, Value>::updateBalance(int8_t diff)
{
    balance_ += diff;
}

/**
* An overridden function for getting the parent since a static_cast is necessary to make sure
* that our node is a AVLNode.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getParent() const
{
    return static_cast<AVLNode<Key, Value>*>(this->parent_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getLeft() const
{
    return static_cast<AVLNode<Key, Value>*>(this->left_);
}

/**
* Overridden for the same reasons as above.
*/
template<class Key, class Value>
AVLNode<Key, Value> *AVLNode<Key, Value>::getRight() const
{
    return static_cast<AVLNode<Key, Value>*>(this->right_);
}


/*
  -----------------------------------------------
  End implementations for the AVLNode class.
  -----------------------------------------------
*/


template <class Key, class Value>
class AVLTree : public BinarySearchTree<Key, Value>
{
public:
    virtual void insert (const std::pair<const Key, Value> &new_item); // TODO
    virtual void remove(const Key& key);  // TODO
protected:
    virtual void nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2);

    // Add helper functions here
    bool isBalanced(); 
    int helper_isBalanced(AVLNode<Key, Value>* ptr, bool bal);
    void removeFix(AVLNode<Key, Value>* ptr);
    void rotate(AVLNode<Key, Value>* grand, AVLNode<Key, Value>* parent, AVLNode<Key, Value>* child);
    void zigZig(AVLNode<Key, Value>* grandparent, AVLNode<Key, Value>* parent, AVLNode<Key, Value>* child);
    void zigZag(AVLNode<Key, Value>* grandparent, AVLNode<Key, Value>* parent, AVLNode<Key, Value>* child);

};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */
// TODO
template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    //logic:
    //1. add Node to the tree
    AVLNode<Key,Value>* newNode = new AVLNode<Key,Value>(new_item.first, new_item.second, nullptr);
    newNode->setBalance(0);   

    if(this->empty())
    {
        this->root_ = newNode;
        return;
    }

    //if the key already exists
    AVLNode<Key,Value>*searchNode = static_cast<AVLNode<Key,Value>*> (this->internalFind(new_item.first));
    if (searchNode) {
        searchNode->setValue(new_item.second);
        return;
    }

    //else: find the spot to add the node
    AVLNode<Key,Value> *curr = static_cast<AVLNode<Key,Value>*> (this->root_);
    AVLNode<Key,Value> *parent = nullptr;
    AVLNode<Key, Value>* grand = nullptr; 
    while (curr) {
        parent = curr;
        grand = parent->getParent();
        if (new_item.first < curr->getKey()) {
            curr = curr->getLeft();
        } else {
            curr = curr->getRight();
        }
    }
    
    //adding the node
    if(new_item.first < parent->getKey())
    {
        parent->setLeft(newNode);
        ++(parent->balance_);
    }
    else
    {
        parent->setRight(newNode);
        --(parent->balance_);
    }
    
    else{++(grand->balance_);}

    //2. check the balance
    //3. call the rotate function call if the balance is off
    //4. update the balance
    //5. check if there's any node for the rotation
    //once the loop is done, the tree is a completed AVL
    while(!isBalanced())
    {
        if(abs(grand->getBalance())>1)
        {
            rotate(grand, parent, curr);
        }
        
        //move up to find the unbalanced grand parent node
        if(grand->getParent())
        {
            curr = parent;
            parent = grand;
            grand = grand->getParent();
        }

    }
    


}

//TODO

template<class Key, class Value>
void AVLTree<Key, Value>::rotate (AVLNode<Key, Value>* grand, AVLNode<Key, Value>* parent, AVLNode<Key, Value>* child)
{
    //call zigzig
    if((grand->getLeft() == parent && parent->getLeft() == child)||(grand->getRight() == parent && parent->getRight() == child))
    {
        zigZig(grand, parent, child);
    }
    //or call zigzag
    else {zigZag(grand, parent, child);}
}

template<class Key, class Value>
void AVLTree<Key, Value>::zigZig(AVLNode<Key, Value>* g, AVLNode<Key, Value>* p, AVLNode<Key, Value>* c)
{
    

    AVLNode<Key,Value> *buffer = nullptr;
    if(p->getRight() == c)
    {
        if(p->getLeft() && p->getLeft() != c)
        {
            p->getLeft()->setParent(buffer);
            buffer->setRight(p->getLeft());
        }
        p->setParent(g->getParent());
        p->setLeft(g);
        g->setRight(nullptr);
        g->setParent(p);
        g->setRight(buffer->getRight());
    }
     else 
     {
        if(p->getRight() && p->getRight() != c)
        {
            p->getRight()->setParent(buffer);
            buffer->setLeft(p->getRight());
        }
        p->setParent(g->getParent());
        p->setRight(g);
        g->setLeft(nullptr);
        g->setParent(p);
        g->setLeft(buffer->getLeft());
     }
}


template<class Key, class Value>
void AVLTree<Key, Value>::zigZag(AVLNode<Key, Value>* g, AVLNode<Key, Value>* p, AVLNode<Key, Value>* c
)
{
    
    /**
     * logic of the rotoation- zigzag:
     * this is recursion function.
     * 1) In the g, p, and c dynamic, rotate the p and c either zigzig or zigzag way
     *  - if the c->balance_ is positive: do zigzag rotation
     *      -
     *  -else if the c-> balance is negative: do zigzig rotation
     *  -else do zigzig (cus it's simpler)
     * 2) Then do the rotation - speciifically zigzig - of in order of g, c , and p.  
     * 
     */

    //1)
    AVLNode<Key,Value> *buffer = nullptr;
    if(p->getRight() == c) 
    {
        //if c.balance is negative
        if (c->balance_ <0)
        {
            //if c has right child
            if(c->getRight())
            {
                //store the descendents in the buffer node ptr
                c->getRight()->setParent(buffer);
                buffer->setLeft(c->getRight());
            }

            //call the zigzag function
            zigZag(p, c, c->getLeft());
        }

        //else if c.balance is positive
        else if(c->balance_ >0)
        {
            //if c has left child
            if(c->getLeft())
            {
                c->getLeft()->setParent(buffer);
                buffer->setRight(c->getLeft());
            }

            //do zigzig
            zigZig(p, c, c->getRight());
        }
        //else:
        else{zigZig(p, c, c->getRight());}
    } else
    {
        //if c.balance is negative
        if (c->balance_ <0)
        {
            //if c has right child
            if(c->getRight())
            {
                //store the descendents in the buffer node ptr
                c->getRight()->setParent(buffer);
                buffer->setLeft(c->getRight());
            }

            //call the zigzag function
            zigZig(p, c, c->getLeft());
        }

        //else if c.balance is positive
        else if(c->balance_ >0)
        {
            //if c has left child
            if(c->getLeft())
            {
                c->getLeft()->setParent(buffer);
                buffer->setRight(c->getLeft());
            }

            //do zigzig
            zigZag(p, c, c->getRight());
        }
        //else:
        else{zigZig(p, c, c->getLeft());}
    }

    //2)
    zigZig(g,c,p);
    
}

template<class Key, class Value>
bool AVLTree<Key, Value>::isBalanced()
{
    bool isB= true;
    helper_isBalanced(static_cast<AVLNode<Key, Value>*>(this->root_),  isB);
    return isB;
}


template<typename Key, typename Value>
int AVLTree<Key, Value>::helper_isBalanced(AVLNode<Key, Value>* curr, bool isB)
{
    //if the tree is empty
    if(BinarySearchTree<Key, Value>::empty()){return 0;}
    //if we hit the leaf node
    if(!(curr->getLeft()) && !(curr->getRight()))
    {
        curr->setBalance(0);
        return 1;
    }
    
    int bufferL = 0, bufferR = 0; 
    //if 
    if(curr->getLeft())
    {
        bufferL =  helper_isBalanced(curr->getLeft(), isB);
    }
    if(curr->getRight())
    {
        bufferR =  helper_isBalanced(curr->getRight(), isB);
    }
    if(abs(bufferL-bufferR)>1)
    {
        isB = false;
    }
    curr->setBalance(bufferL-bufferR);
    if(bufferL > bufferR){return bufferL+1;}
    else {return bufferR+1;}
}

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
// TODO
template<class Key, class Value>
void AVLTree<Key, Value>:: remove(const Key& key)
{
    //if the tree is empty
    if(this->empty()) {return;}
    
    //variable for the node we are searching
    AVLNode<Key,Value> *searchNode = static_cast<AVLNode <Key, Value>*> (this->internalFind(key));

    //if the searching key DNE
    if (!searchNode) {return;}

    // A var for the parent of the searchNode
    AVLNode<Key,Value> *parent = searchNode->getParent();

    //if searchNode does not have any child
    if(!searchNode->getLeft() && !searchNode->getRight())
    {
        //if this tree has one node 
        if(!searchNode->getParent())
        {
            delete searchNode; 
            return;
        }
        if(searchNode->getKey() < parent->getKey())
        {
            parent->setLeft(nullptr);
            ++(parent->balance_);
        }
        else
        {
            parent->setRight(nullptr);
            --(parent->balance_);
        }
        
    }

    //if searchNode have only one child
    else if(!searchNode->getLeft() || !searchNode->getRight())
    {
        //promote the child node
        AVLNode<Key,Value> *childNode;
        if(searchNode->getLeft()) //if the child node is left child
        {
            childNode = searchNode->getLeft();
            if(searchNode->getParent())
            {
                childNode->setParent(searchNode->getParent());
                //update searchNode's parent node
                if(childNode->getKey() < parent->getKey())
                {
                    parent->setLeft(childNode);
                    ++(parent->balance_);
                    childNode->setBalance(0);
                }
                else
                {
                    parent->setRight(childNode);
                    --(parent->balance_);
                    childNode->setBalance(0);
                
                }
            }
            else // if the node is root_
            {
                childNode->setParent(nullptr);
                root_ = childNode;
            }
        }
        if(searchNode->getRight()) //if the child node is right child
        {
            childNode = searchNode->getRight();
            if(searchNode->getParent())
            {
                childNode->setParent(searchNode->getParent());
                //update searchNode's parent node
                if(childNode->getKey() < parent->getKey())
                {
                    parent->setLeft(childNode);
                    ++(parent->balance_);
                    childNode->setBalance(0);
                }
                else
                {
                    parent->setRight(childNode);
                    --(parent->balance_);
                    childNode->setBalance(0);
                
                }
            }
            else // if the node is root_
            {
                childNode->setParent(nullptr);
                root_ = childNode;
            }
        }

    } 

    //if searchNode has two children
    else 
    {
        AVLNode<Key, Value>* nodeToBeFixed = static_cast<AVLNode<Key,Value>*>(this->predecessor(searchNode));
        //swap with the predecessor
        nodeSwap(searchNode, nodeToBeFixed);
        parent = searchNode->getParent();
        
        //update parent node
        if(searchNode->getParent()->getLeft() == searchNode){parent->setLeft(nullptr);}
        else{parent->setRight(nullptr);}

    }
    //delete the node with the given Key
    delete searchNode;
    removeFix(parent);

    // //check the balance
    // while(!isBalanced())
    // {
    //     if(abs(grand->getBalance())>1)
    //     {
    //         rotate(grand, parent, curr);
    //     }
        
    //     //move up to find the unbalanced grand parent node
    //     if(grand->getParent())
    //     {
    //         curr = parent;
    //         parent = grand;
    //         grand = grand->getParent();
    //     }

    // }
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* ptr)
{
    AVLNode<Key, Value>* g = ptr;
    AVLNode<Key, Value>* p;
    AVLNode<Key, Value>* c;

    //while g exists
    while (g)
    {
        isBalanced();
        if (g->balance_ > 2)
        {
            p = g->getRight();
            if (p->balance_ <0)
            {
                c= p->getLeft();
                zigZag(g,p,c);
            }
            else{
                c= p->getRight();
                zigZig(g,p,c);
            }
            isBalanced();
        }
        else if(g->balance_ < -2)
        {
            p = g->getLeft();
            if (p->balance_>0)
            {
                c = p->getRight();
                zigZag(g,p,c);
            }
            else
            {
                c = p->getLeft();
                zigZig(g,p,c);
            }
            isBalanced();
        }
        
        g = g->getParent();
    }
    
}

template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap( AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
