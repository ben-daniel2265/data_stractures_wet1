//
// Created by yonat on 18/11/2022.
//

#ifndef WET1_AVL_TREE_H
#define WET1_AVL_TREE_H

#include <algorithm>
#include <iostream>
using namespace std;
using std::cout;

template <typename T>
class AVL_Tree
{
    struct node
    {
        T* data;
        node* left;
        node* right;
        int height;
    };

    node* root;

    void makeEmpty(node* t)
    {
        if(t == nullptr)
            return;
        makeEmpty(t->left);
        makeEmpty(t->right);
        delete t;
    }

    T* find(node* t, T* x, bool(*func)(const T&, const T&)){
        if (t == nullptr){
            return nullptr;
        }

        else if (!func(*x, *t->data) && !func(*t->data, *x)){
            return t->data;
        }

        else if (func(*t->data, *x)){
            return find(t->right, x, func);
        }else
            return find(t->left, x, func);
    }

    node* insert(T* x, node* t, bool(*func)(const T&, const T&))
    {
        if(t == nullptr){
            t = new node{x, nullptr, nullptr, 0};
        }
        else if(func(*x, *t->data))
        {
            t->left = insert(x, t->left, func);
            if(height(t->left) - height(t->right) == 2){
                if(func(*x, *t->left->data))
                    t = singleRightRotate(t);
                else
                    t = doubleRightRotate(t);
            }
        }
        else if(func(*t->data, *x))
        {
            t->right = insert(x, t->right, func);
            if(height(t->right) - height(t->left) == 2)
            {
                if(func(*t->right->data, *x))
                    t = singleLeftRotate(t);
                else
                    t = doubleLeftRotate(t);
            }
        }

        t->height = max(height(t->left), height(t->right))+1;
        return t;
    }

    node* singleRightRotate(node* t)
    {
        node* u = t->left;
        t->left = u->right;
        u->right = t;
        t->height = max(height(t->left), height(t->right))+1;
        u->height = max(height(u->left), t->height)+1;
        return u;
    }

    node* singleLeftRotate(node* t)
    {
        node* u = t->right;
        t->right = u->left;
        u->left = t;
        t->height = max(height(t->left), height(t->right))+1;
        u->height = max(height(u->right), t->height)+1 ; // was t->right instead of u->right.
        return u;
    }

    node* doubleLeftRotate(node* t)
    {
        t->right = singleRightRotate(t->right);
        return singleLeftRotate(t);
    }

    node* doubleRightRotate(node* t)
    {
        t->left = singleLeftRotate(t->left);
        return singleRightRotate(t);
    }

    node* findMin(node* t)
    {
        if(t == nullptr)
            return nullptr;
        else if(t->left == nullptr)
            return t;
        else
            return findMin(t->left);
    }

    node* findMax(node* t)
    {
        if(t == nullptr)
            return nullptr;
        else if(t->right == nullptr)
            return t;
        else
            return findMax(t->right);
    }

    node* remove(node* t, T* x, bool(*func)(const T&, const T&))
    {
        node* temp;

        // Element not found
        if(t == nullptr)
            return nullptr;

            // Searching for element
        else if(func(*x, *t->data))
            t->left = remove(t->left, x, func);
        else if (func(*t->data, *x))
            t->right = remove(t->right, x, func);

            // Element found
            // With 2 children
        else if(t->left && t->right)
        {
            temp = findMin(t->right);
            t->data = temp->data;
            t->right = remove(t->right, x, func);
        }
            // With one or zero child
        else
        {
            temp = t;
            if(t->left == nullptr)
                t = t->right;
            else if(t->right == nullptr)
                t = t->left;
            delete temp;
        }
        if(t == nullptr)
            return t;

        t->height = max(height(t->left), height(t->right))+1;

        // If list is unbalanced
        // If left list is deleted, right case
        if(height(t->left) - height(t->right) == 2)
        {
            // right right case
            if(height(t->left->left) - height(t->left->right) == 1)
                return singleLeftRotate(t);
                // right left case
            else
                return doubleLeftRotate(t);
        }
            // If right list is deleted, left case
        else if(height(t->right) - height(t->left) == 2)
        {
            // left left case
            if(height(t->right->right) - height(t->right->left) == 1)
                return singleRightRotate(t);
                // left right case
            else
                return doubleRightRotate(t);
        }
        return t;
    }

    T* getParentOf(const T& x, node* t, bool(*func)(const T&, const T&)){
        if (t == nullptr){
            return nullptr;
        }
        else if ((t->left != nullptr && !func(x, t->left->data) && !func(t->left->data, x)) ||
                (t->right != nullptr && !func(x, t->right->data) && !func(t->right->data, x)))
        {
            return &t->data;
        }
        getParentOf(x, t->left, func);
        getParentOf(x, t->right, func);
    }

    int height(node* t)
    {
        return (t == nullptr ? -1 : t->height);
    }

    int getBalance(node* t)
    {
        if(t == nullptr)
            return 0;
        else
            return height(t->left) - height(t->right);
    }

    void inorder(int* p, node* t, void(*func)(const T&, int*)){
        if (t == nullptr){
            return;
        }
        inorder(p, t->left, func);
        func(*t->data, p);
        p++;
        inorder(p, t->right, func);
    }

    void inorderFillArray(T* p, node* t){
        if (t == nullptr){
            return;
        }
        inorderFillArray(p, t->left());
        *p++ = t->data;
        inorderFillArray(p, t->right());
    }

    T* getDesignatedParent(T* x, bool(*func)(const T&, const T&), node* t){
        if (t == nullptr){
            return nullptr;
        }
        if ((func(*x, *t->data) && t->left == nullptr ) || (func(*t->data, *x) && t->right == nullptr)){
            return t->data;
        }
        else if (func(*x, *t->data) && t->left != nullptr){
            return getDesignatedParent(x, func, t->left);
        }
        else
            return getDesignatedParent(x, func, t->right);
    }

public:
    AVL_Tree()
    {
        root = nullptr;
    }

    ~AVL_Tree(){
        makeEmpty(root);
    }

    void insert(T* x, bool(*func)(const T&, const T&))
    {
        root = insert(x, root, func);
    }

    void remove(T* x, bool(*func)(const T&, const T&))
    {
        root = remove(root, x, func);
    }

    T* find(T* x, bool(*func)(const T&, const T&)){
        return find(root, x, func);
    }

    T* findMax(){
        node* temp = findMax(root);
        return temp->data;
    }

    T* findMin(){
        node* temp = findMin(root);
        return temp->data;
    }

    T* getParentOf(const T& x,bool(*func)(const T&, const T&)){
        return getParentOf(x, root, func);
    }

    T* getDesignatedParent(T* x, bool(*func)(const T&, const T&)){
        return getDesignatedParent(x, func, root);
    }

    node* getRight(){
        return this->root->right();
    }

    node* getLeft(){
        return this->root->left();
    }

    void inorderFillArray(T* p){
        inorderFillArray(p, root);
    }

    void inorder(int* p, void(*func)(const T&, int*)) {
        inorder(p, root, func);
    }
};

#endif //WET1_AVL_TREE_H
