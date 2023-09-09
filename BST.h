//
//  BST.h
//  235 Lab 09 - Pokemon
//
//  Created by James Wade on 3/30/20.
//  Copyright © 2020 James Wade. All rights reserved.
//

#ifndef BST_h
#define BST_h

#include "BSTInterface.h"
#include <iostream>
#include <string>
#include <sstream>

template <typename T>
class BST : BSTInterface<T> {
private:
    struct Node {
        Node() : parent(NULL), left(NULL), right(NULL) {}
        Node(T data, Node* parent = NULL, Node* left = NULL, Node* right = NULL) : data(data), parent(parent), left(left), right(right) {}
        ~Node() {
            parent = NULL;
            left = NULL;
            right = NULL;
        }
        T data;
        Node* parent;
        Node* left;
        Node* right;
    };
    Node* root;
    size_t treeSize;
    
    /** Add node to tree recursively */
    bool addNode(BST<T>::Node*& current, BST<T>::Node*& parent, const T& item);
    
    /** Output nodes at a given level */
    bool outLevel(Node* root, int level, std::stringstream& os) const;
    
    /** Recursive function to clear tree */
    void clearTree(Node* current) {
        if (current == NULL) return;
        clearTree(current->left);
        clearTree(current->right);
        delete current;
        return;
    }
    
    /** Recursive function to erase a node from tree */
    bool erase(const T& data, Node*& current);
    
    /** Called by 'erase' to remove a node with two children */
    void replaceParent(Node*& oldRoot, Node*& localRoot) {
        if (localRoot->right != NULL) {
            replaceParent(oldRoot, localRoot->right);
        }
        else {
            oldRoot->data = localRoot->data;
            oldRoot = localRoot;
            localRoot = localRoot->left;
        }
    }
    
public:
    BST() : root(NULL), treeSize(0) {}
    virtual ~BST() { clearTree(); }
    
    class Iterator {
    private:
        Node* node;
    public:
        Iterator(Node* node = NULL) : node(node) {}
        ~Iterator() { node = NULL; };
        
        /** Compares two iterators */
        bool operator!=(const Iterator& rhs) const {
            bool notEqual;
            (node == rhs.node) ? (notEqual = false) : (notEqual = true);
            return notEqual;
        }
        
        /** Increments iterator */
        Iterator& operator++();
        
        /** Returns data referenced by iterator */
        T& operator*() const {
            return node->data;
        }
    };
    
    /** Grabs iterator at beginning of in-order chain */
    Iterator begin() const {
        Node* start = root;
        if (start == NULL) {
            return BST<T>::Iterator(NULL);
        }
        while (start->left != NULL) {
            start = start->left;
        }
        return BST<T>::Iterator(start);
    }
    
    /** Grabs an iterator to the position just beyond last in-order node */
    Iterator end() const { return BST<T>::Iterator(NULL); }
    
    /** Uses Iterators to print an in-order reading of the tree */
    std::string treeInOrder() {
        if (root == NULL) {
            return "empty";
        }
        std::ostringstream os;
        for (Iterator iter = begin(); iter != end(); ++iter) {
            os << *iter << " ";
        }
        return os.str();
    }
    
    /** Return number of nodes in tree */
    size_t size() const { return treeSize; }
    
    /** Return true if node added to BST, else false */
    virtual bool addNode(const T& item) {
        return addNode(root, root->parent, item);
    }
    
    /** Return true if node removed from BST, else false */
    virtual bool removeNode(const T& data) { return erase(data, root); }
    
    /** Find a node based off of it's data */
    bool find(T data) {
        for (Iterator iter = begin(); iter != end(); ++iter) {
            if (*iter == data) {
                return true;
            }
        }
        return false;
    }

    /** Return true if BST cleared of all nodes, else false */
    virtual bool clearTree() {
        clearTree(root);
        root = NULL;
        treeSize = 0;
        return true;
    }

    /** Return a level order traversal of a BST as a string */
    std::string toString() const;
    
    friend std::ostream& operator<<(std::ostream& os, BST<T> bst) {
        return os << bst.toString();
    }
};

/** Add node to tree */
template <typename T>
bool BST<T>::addNode(BST<T>::Node*& current, BST<T>::Node*& parent, const T& item) {
    if (root == NULL) {
        root = new Node(item);
        treeSize++;
        return true;
    }
    else if (current == NULL) {
        current = new Node(item, parent);
        treeSize++;
        return true;
    }
    else if (current->data == item) { return false; }
    else if (item < current->data) {
        return addNode(current->left, current, item);
    }
    else {
        return addNode(current->right, current, item);
    }
}

/** Recursive function to erase a node from tree */
template <typename T>
bool BST<T>::erase(const T& data, Node*& current) {
    if (current == NULL) {
        return false;
    }
    else if (data < current->data) {
        return erase(data, current->left);
    }
    else if (data > current->data) {
        return erase(data, current->right);
    }
    else {
        Node* temp = current;
        if (current->left == NULL) {
            current = current->right;
        }
        else if (current->right == NULL) {
            current = current->left;
        }
        else {
            replaceParent(temp, temp->left);
        }
        treeSize--;
        delete temp;
        return true;
        
    }
}

/** Output nodes at a given level */
template <typename T>
bool BST<T>::outLevel(Node* root, int level, std::stringstream& os) const {
   if (root == NULL) return false;
   if (level == 1) {
      os << " " << root->data;
      if ((root->left != NULL) || (root->right != NULL)) return true;
      return false;
   }
   if ((level == 2) && !root->left && root->right) os << " _";
   bool left = outLevel(root->left, level - 1, os);
   bool right = outLevel(root->right, level - 1, os);
   if ((level == 2) && root->left && !root->right) os << " _";
   return left || right;
}

/** Return a level order traversal of a BST as a string */
template <typename T>
std::string BST<T>::toString() const {
   std::stringstream os;
   if (root == NULL) os << "empty";
   else {
      int level = 0;
      do {
          os << std::endl << "  " << ++level << ":";
      } while (outLevel(root, level, os));
   }
   return os.str();
}

/** Increments iterator */
template <typename T>
typename BST<T>::Iterator& BST<T>::Iterator::operator++() {
    if (node->right != NULL) {
        node = node->right;
        while (node->left != NULL) {
            node = node->left;
        }
    }
    else if (node->right == NULL) {
        Node* prev = node;
        node = node->parent;
        while (node != NULL && (node->data < prev->data)) {
            prev = node;
            node = node->parent;
        }
    }
    return *this;
}

#endif /* BST_h */

