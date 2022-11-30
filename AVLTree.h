#ifndef AVLTREE
#define AVLTREE

template <class T>
class AVLTree {
    public:

    AVLTree();

    void insert(T* node, bool(*cmp_func)(T* t1, T* t2));
    void remove(T* node, bool(*cmp_func)(T* t1, T* t2));
    T* find(T* node, bool(*cmp_func)(T* t1, T* t2));
};

#endif //AVLTREE