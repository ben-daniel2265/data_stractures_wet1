#ifndef AVLTREE
#define AVLTREE

template <class T>
class AVLTree {
    class node{
        T* value;
        int height;
        node* left;
        node* right;

        node(T* value){
            this->valuevalue = value;
            this->height = height;
            this->left = nullptr;
            this->right = nullptr;
        }
    }
    node * root;

    public:

    AVLTree();

    void insert(T* value, bool(*cmp_func)(T* t1, T* t2));
    void remove(T* value, bool(*cmp_func)(T* t1, T* t2));
    T* find(T* value, bool(*cmp_func)(T* t1, T* t2));
};

#endif //AVLTREE