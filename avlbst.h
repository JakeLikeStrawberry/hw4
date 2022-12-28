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
    //AVLNode<Key, Value>* insertFix (const std::pair<const Key, Value> &new_item);
    void insertFix (AVLNode<Key, Value>* p, AVLNode<Key, Value>* n);
    void removeFix (AVLNode<Key,Value>* n, int8_t diff);
    void calc_balance(AVLNode<Key, Value> *n);
    int height(AVLNode<Key, Value> *n);
    //void attach(AVLNode<Key, Value> *oldNode, AVLNode<Key, Value> *newNode);
    void rotateLeft(AVLNode<Key, Value>* r);
    void rotateRight(AVLNode<Key, Value>* r);
    bool isRightChild(AVLNode<Key,Value>* n);
    bool isLeftChild(AVLNode<Key,Value>* n);
    void removeHelper(AVLNode<Key, Value>* current);
};

/*
 * Recall: If key is already in the tree, you should 
 * overwrite the current value with the updated value.
 */

template<class Key, class Value>
void AVLTree<Key, Value>::rotateLeft(AVLNode<Key,Value>* r)
{
  if (r == NULL){
    return;
  }

  AVLNode<Key,Value>* c = r->getRight();
  AVLNode<Key, Value>* p = r->getParent();

  if (c == NULL){
    return;
  }


  r->setRight(c->getLeft());

  if (c->getLeft() != nullptr){
    c->getLeft()->setParent(r);         //new root child to old root
  }
  if (p == NULL){
    BinarySearchTree<Key, Value>::root_ = c;
  }

  else if(p->getRight() == r){
    p->setRight(c);
  }
  else if(p->getLeft() == r){
    p->setLeft(c);
  }

  c->setParent(p);
  c->setLeft(r);
  r->setParent(c);
  return;

}

template<class Key, class Value>
void AVLTree<Key, Value>::rotateRight(AVLNode<Key,Value>* r)
{
  if (r == NULL){
    return;
  }

  AVLNode<Key,Value>* c = r->getLeft();
  AVLNode<Key, Value>* p = r->getParent();

  if (c == NULL){
    return;
  }


  r->setLeft(c->getRight());

  if (c->getRight() != nullptr){
    c->getRight()->setParent(r);         //new root child to old root
  }
  if (p == NULL){
    BinarySearchTree<Key, Value>::root_ = c;
  }

  else if(p->getLeft() == r){
    p->setLeft(c);
  }
  else if(p->getRight() == r){
    p->setRight(c);
  }

  c->setParent(p);
  c->setRight(r);
  r->setParent(c);
  return;

}

template<class Key, class Value>
void AVLTree<Key, Value>::insert (const std::pair<const Key, Value> &new_item)
{
    // TODO
    
    if (BinarySearchTree<Key,Value>::empty()) {
        this->root_ = new AVLNode<Key, Value>(new_item.first, new_item.second, NULL);
        AVLNode<Key, Value>* n = (AVLNode<Key, Value>*)this->root_;
        n->setBalance(0);
        n->setRight(NULL);
        n->setLeft(NULL);
    }

    // else if (internalFind(new_item.first) != NULL) {  
    //     AVLNode<Key, Value>* changeNode = (AVLNode<Key, Value>*)BinarySearchTree<Key, Value>::internalFind(new_item.first); //if it exists in the tree change the value
    //     changeNode->setValue(new_item.second);
    // }
    
else{
      AVLNode<Key, Value>* n = (AVLNode<Key, Value>*)this->root_;
      AVLNode<Key, Value>* parent = (AVLNode<Key, Value>*)this->root_;

    while (n != NULL){

            if (n->getKey() == new_item.first){
              n->setValue(new_item.second);
              return;
            }
            else if (n->getKey() < new_item.first){
                parent = n;
                n = n->getRight();
            }
            else{
                parent = n;
                n = n->getLeft();
            }
    }

    AVLNode<Key, Value>* addNode = new AVLNode<Key, Value>(new_item.first, new_item.second, parent);
    addNode->setBalance(0);

    if (parent->getKey() > new_item.first){
        parent->setLeft(addNode);
        

    }
    else if (parent->getKey() < new_item.first){
        parent->setRight(addNode);
    
    }
    addNode->setLeft(NULL);
    addNode->setRight(NULL);
    //"If balance" part of insert
    if (parent->getBalance() == -1){
        parent->setBalance(0);
        return;
    }
    else if (parent->getBalance() == 1){
        parent->setBalance(0);
        return;
    }

    else if (parent->getBalance() == 0){
        if (isLeftChild(addNode)){
            parent->setBalance(-1);
        }
        else{
            parent->setBalance(1);
        }

        insertFix(parent, addNode);

    }

    // while (n != NULL){
    //     calc_balance(n);
    //     n = n->getParent();
    // }

    
}
}
template <class Key, class Value>
void AVLTree<Key, Value>::insertFix(AVLNode<Key, Value>* p, AVLNode<Key, Value>* n){

    if (p == NULL || p->getParent() == NULL){
      return;
    }
    AVLNode<Key, Value>* g = p->getParent();

    //insert update balance thing here
    if (g == NULL){
      return;
    }
    //if left child
    if (isLeftChild(p)){
        g->updateBalance(-1);
        
      if(g->getBalance() == 0){
      return;
      }
    
      else if (g->getBalance() == -1){
      //insert note on slide here
      insertFix(g, p);
      }
    
      // else if (g->getBalance()== 1){
      // //insert note on slide here
      // insertFix(g, p);
      // }
    else if (g->getBalance() == -2){
        if (p->getRight() == n){
          rotateLeft(p);
          rotateRight(g);

          if(n->getBalance() == -1){
            p->setBalance(0);
            g->setBalance(1);
            n->setBalance(0);
          }
          else if (n->getBalance() == 0){
            p->setBalance(0);
            g->setBalance(0);
            n->setBalance(0);
          }
          else if(n->getBalance() == 1){
            p->setBalance(-1);
            g->setBalance(0);
            n->setBalance(0);
          }

          
        }
        else{
          rotateRight(g);
          p->setBalance(0);
          g->setBalance(0);
        }
    }

    }
    //right child
    else{
      g->updateBalance(1);
      if (g->getBalance() == 0){
        return;
      }
      else if(g->getBalance() == 1){
        insertFix(g, p);
      }
      else if(g->getBalance() == 2){
          //zig zag
        if (p->getLeft() == n){
          rotateRight(p);
          rotateLeft(g);
          if (p->getRight() == n){
            rotateLeft(p);
            rotateRight(g);
            if (n->getBalance() == -1){
              p->setBalance(0);     //was 1, 0
              g->setBalance(1);                         //CHANGED THIS PART, see if passes segfault
              n->setBalance(0);
            }

            else if(n->getBalance() == 0){
              p->setBalance(0);
              g->setBalance(0);
              n->setBalance(0);
            }

            else if (n->getBalance() == 1){
              p->setBalance(-1);      //was 0, -1 i think
              g->setBalance(0);                     //ALSO CHANGED THIS
              n->setBalance(0);
            }
          }
        }
        else{
          rotateLeft(g);
          p->setBalance(0);
          g->setBalance(0);
        }

      }
    }


}


//these two helpers calculate bal/height respectively
template<class Key, class Value>
void AVLTree<Key, Value>::calc_balance(AVLNode<Key, Value>* newNode){


    if (newNode == NULL){
        return;                                         //do nothing
    }
    int heightCalc = height(newNode->getLeft()) - height(newNode->getRight());
    newNode->setBalance(heightCalc);
}
    

template<class Key, class Value>
int AVLTree<Key, Value>::height(AVLNode<Key, Value>* newNode){

        if (newNode == NULL){                                               //pretty much same as height calc helper function I used in BST
            return 0;
        }
        return 1 + std::max(height(newNode->getLeft()),height(newNode->getRight()));
}


template<class Key, class Value>
bool AVLTree<Key, Value>::isRightChild(AVLNode<Key,Value>* n)
{
  AVLNode<Key,Value>* p = n->getParent();
  return p->getRight() == n;

}

template<class Key, class Value>
bool AVLTree<Key, Value>::isLeftChild(AVLNode<Key,Value>* n)
{
  if (n == NULL){
    return false;
  }
  if (n->getParent() != NULL){
    AVLNode<Key,Value>* p = n->getParent();
    if (p->getLeft() != NULL){
    if (p->getLeft() == n){
      return true;
    }
  }
  }
  return false;

}


// template<class Key, class Value>
// void AVLTree<Key, Value>::attach(AVLNode<Key, Value> *old, AVLNode<Key, Value> *new){
//     if(old==nullptr) return;
//     AVLNode<Key, Value>* p = old->getParent();
//     if(p==nullptr){
//         BinarySearchTree<Key, Value>::root_ = New;
//         if(new!=nullptr)
//             new->setParent(nullptr);
//     }else{
//         if(p->getLeft()==old){
//             p->setLeft(new);
//         }else{
//             p->setRight(new);                
//         }
//         if(new!=nullptr)
//             new->setParent(p);
//     }
//     old->setParent(nullptr);
// }
    

/*
 * Recall: The writeup specifies that if a node has 2 children you
 * should swap with the predecessor and then remove.
 */
template<class Key, class Value>
void AVLTree<Key, Value>::remove(const Key& key)
{
    // TODO
    AVLNode<Key, Value>* n = (AVLNode<Key, Value>*)BinarySearchTree<Key, Value>::internalFind(key);
    int8_t diff;
    if (n == NULL){
      return;
    }

    if (isLeftChild(n)){
          diff = 1;                                   //swap with in order successor or predecessor
    }
    else{
      diff = -1;
    }
    AVLNode<Key, Value>* p = n->getParent();
    removeHelper(n);
    removeFix(p, diff);
    
}

template<class Key, class Value>
void AVLTree<Key, Value>::removeHelper(AVLNode<Key,Value>* current){
    //if leaf node

    if (current->getRight()== NULL &&current->getLeft() == NULL){
      if (current->getParent() == NULL){
        this->root_ = NULL;
      }
      else if(current->getParent()->getLeft() == current){
        current->getParent()->setLeft(NULL);
      }
      else if(current->getParent()->getRight() == current){
        current->getParent()->setRight(NULL);
      }
      delete current;
    }

else if((current->getRight() == NULL && current->getLeft() != NULL) || (current->getLeft() == NULL && current->getRight() != NULL)){
  //promote child
  AVLNode<Key, Value>* newNode;
  if(current->getRight() == NULL){
    newNode = current->getLeft();
    AVLTree<Key, Value>::nodeSwap(newNode, current);
  }
  else if(current->getLeft() == NULL){
    newNode = current->getRight();
    AVLTree<Key, Value>::nodeSwap(newNode, current);
  }
  if(this->root_ == current){
    this->root_ = newNode;
  }
removeHelper(current);
}

else{
  AVLNode<Key, Value>* newNode = (AVLNode<Key,Value>*) BinarySearchTree<Key,Value>::predecessor(current);
  if (newNode != NULL && current != NULL)
  AVLTree<Key, Value>::nodeSwap(newNode, current);
  removeHelper(current);
}


}

template<class Key, class Value>
void AVLTree<Key, Value>::removeFix(AVLNode<Key, Value>* n, int8_t diff){

int8_t ndiff = 0;
if (n == NULL){
  return;
}  

  AVLNode<Key, Value>* p = n->getParent();

  if (p != NULL){
    
    if (isLeftChild(n)){
      ndiff += 1;
    }
    else{
      ndiff -= 1;
    }
  }

  //diff = -1 case
AVLNode<Key, Value>* g;
AVLNode<Key, Value>* c;
int8_t balanceG;
if (diff == -1){

  if ((n->getBalance() + diff) == -2){
    c = n->getLeft();

    if (c->getBalance() == -1) {        //zig-zig
      rotateRight(n);
      n->setBalance(0);
      c->setBalance(0);
      removeFix(p, ndiff);
    }
    else if (c->getBalance() == 0){        //zig-zig 
      rotateRight(n);
      n->setBalance(-1);
      c->setBalance(1);       //done
      
    }

    else if (c->getBalance() == 1){     //zig zag
      g = c->getRight();
      balanceG = g->getBalance();
      rotateLeft(c);
      rotateRight(n);
      if(balanceG == 1){
        n->setBalance(0);
        c->setBalance(-1);
        g->setBalance(0);
      }
      else if(balanceG == 0){
        n->setBalance(0);
        c->setBalance(0);
        g->setBalance(0);
      }
      else if (balanceG == -1){
        n->setBalance(1);
        c->setBalance(0);
        g->setBalance(0);
      }
      removeFix(p, ndiff);

    }


  }
      //2nd case
  
  else if ((n->getBalance() + diff) == -1){
    n->setBalance(-1);
    //done 
  }
    //3rd case
  else if ((n->getBalance() + diff) == 0) {
    n->setBalance(0);
    removeFix(p, ndiff);
  }

}



//diff = 1 case

else if(diff == 1){

  //1st case

  if((n->getBalance() + diff) == 2){
    c = n->getRight();

    if (c->getBalance() == 1){          //zig-zig
      rotateLeft(n);
      n->setBalance(0);
      c->setBalance(0);
      removeFix(p, ndiff);
    }

    else if (c->getBalance() == 0){   //zig-zig
      rotateLeft(n);
      n->setBalance(1);
      c->setBalance(-1);      //done
    }

    else if(c->getBalance() == -1){   //zig-zag
      g = c->getLeft();
      balanceG = g->getBalance();
      rotateRight(c);
      rotateLeft(n);
      if(balanceG == -1){                         //maybe switch this to 1 and other to -1
        n->setBalance(0);
        c->setBalance(1);
        g->setBalance(0);
      }
      else if(balanceG == 0){
        n->setBalance(0);
        c->setBalance(0);
        g->setBalance(0);
      }
      else if(balanceG == 1){
        n->setBalance(-1);
        c->setBalance(0);
        g->setBalance(0);
      }
      removeFix(p, ndiff);
    }
  }
  //2nd case
  else if ((n->getBalance() + diff) == 1){
    n->setBalance(1); //done
  }

  else if((n->getBalance() + diff) == 0){
    n->setBalance(0);
    removeFix(p, ndiff);
  }
}

}


template<class Key, class Value>
void AVLTree<Key, Value>::nodeSwap(AVLNode<Key,Value>* n1, AVLNode<Key,Value>* n2)
{
    BinarySearchTree<Key, Value>::nodeSwap(n1, n2);
    int8_t tempB = n1->getBalance();
    n1->setBalance(n2->getBalance());
    n2->setBalance(tempB);
}


#endif
