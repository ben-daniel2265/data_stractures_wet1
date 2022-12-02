#ifndef AVLTREE
#define AVLTREE

#include <math.h>
#include <iostream>

using namespace std;

template <class T>
class AVLTree {
    public:
    class Node{
        public:
            T* value;
            int height;
            Node* left;
            Node* right;

            Node(T* value){
                this->value = value;
                this->height = 1;
                this->left = nullptr;
                this->right = nullptr;
            }
    };

    Node* root;

    AVLTree(){
        this->root = nullptr;
    };

    void insertValue(T* value, int(*cmp_func)(T* t1, T* t2)){
        this->root = insert(this->root, value, cmp_func);
    }

    void removeValue(T* value, int(*cmp_func)(T* t1, T* t2)){
        this->root = remove(this->root, value, cmp_func);
    }

    Node* findValue(T* value, int(*cmp_func)(T* t1, T* t2)){
        return find(root, value, cmp_func);
    }


    private: 
    int height(Node* head){
        if(head == nullptr) return 0;
        return head->height;
    }

    Node* rotateRight(Node* head){
        Node* newHead = head->left;
        head->left = newHead->right;
        newHead->right = head;

        head->height = max(height(head->left), height(head->right)) + 1;
        newHead->height = max(height(newHead->left), height(newHead->right)) + 1;

        return newHead;
    }

    Node* rotateLeft(Node* head){
        Node* newHead = head->right;
        head->right = newHead->left;
        newHead->left = head;

        head->height = max(height(head->left), height(head->right)) + 1;
        newHead->height = max(height(newHead->left), height(newHead->right)) + 1;

        return newHead;
    }
    
    Node * insert(Node* head, T* value, int(*cmp_func)(T* t1, T* t2)){
        if(head == nullptr){
            Node* temp = new Node(value);
            return temp;
        }

        int compare_result = cmp_func(value, head->value);

        cout << compare_result << endl;
        if(compare_result < 0){
            head->left = insert(head->left, value, cmp_func);
        }
        else if(compare_result > 0){
            head->right = insert(head->right, value, cmp_func);
        }

        head->height = max(height(head->left), height(head->right)) + 1;

        int balance = height(head->left) - height(head->right);
    
        if(balance > 1){
            if(cmp_func(value, head->left->value) < 0){
                return rotateRight(head);
            }
            else{
                head->left = rotateLeft(head->left);
                return rotateRight(head);
            }
        }
        else if(balance < -1){
            if(cmp_func(value, head->right->value) > 0){
                cout << balance << endl;
                return rotateLeft(head);
            }
            else{
                head->right = rotateRight(head->right);
                return rotateLeft(head);
            }
        }

        return head;
    }

    Node* remove(Node* head, T* value, int(*cmp_func)(T* t1, T* t2)){
        if(head == nullptr) return nullptr;

        int compare_result = cmp_func(value, head->value);
        
        if(compare_result < 0){
            head->left = remove(head->left, value, cmp_func);
        }
        else if(compare_result > 0){
            head->right = remove(head->right, value, cmp_func);
        }
        else{
            Node * temp_left = head->left;
            Node * temp_right = head->right;

            if(temp_left == nullptr){
                delete head;
                head = temp_right;
            }
            else if(temp_right == nullptr){
                delete head;
                head = temp_left;
            }
            else{
                while(temp_left->right != nullptr){
                    temp_left = temp_left->right;
                }
                head->value = temp_left->value;
                head->left = remove(head->left, temp_left->value, cmp_func);
            }
        }

        if(head == nullptr) return nullptr;

        head->height = max(height(head->left), height(head->right)) + 1;

        int balance = height(head->left) - height(head->right);

        if(balance > 1){
            if(height(head->left) >= height(head->right)){
                return rotateRight(head);
            }
            else{
                head->left = rotateLeft(head->left);
                return rotateRight(head);
            }
        }
        else if(balance < -1){
            if(height(head->right) >= height(head->left)){
                return rotateLeft(head);
            }
            else{
                head->right = rotateRight(head->right);
                return rotateLeft(head);
            }
        }

        return head;
    }

    Node* find(Node* head, T* value, int(*cmp_func)(T* t1, T* t2)){
        if(head == nullptr) return nullptr;

        int compare_result = cmp_func(value, head->value);
    
        if(compare_result == 0){
            return head;
        }
        else if(compare_result < 0){
            return find(head->left, value, cmp_func);
        }
        else{
            return find(head->right, value, cmp_func);
        }
    }

    Node* findDad(Node* head, T* value, int(*cmp_func)(T* t1, T* t2)){
        if(head == nullptr) return nullptr;

        int compare_result = cmp_func(value, head->value);
        Node* temp;
    
        if(compare_result == 0){
            return nullptr;
        }

        else if(compare_result < 0){
            temp = find(head->left, value, cmp_func);
            if(temp == nullptr) return head;
            return temp;
        }
        else{
            temp = find(head->right, value, cmp_func);
            if(temp == nullptr) return head;
            return temp;
        }
    }


};

/*
int cmp(int* x, int* y){
    return *x - *y;
}


void print2DUtil(AVLTree<int>::Node* root, int space)
    {
        // Base case
        if (root == NULL)
            return;

        // Increase distance between levels
        space += 10;

        // Process right child first
        print2DUtil(root->right, space);

        // Print current node after space
        // count

        cout << endl;
        for (int i = 10; i < space; i++)
            cout << " ";
        cout << *(root -> value) << "\n";

        // Process left child
        print2DUtil(root->left, space);
    }


int main(){
    AVLTree<int> t;

    int x = 1;
    int y = 2;
    int x1 = 3;
    int x2 = 4;
    int x3 = 5;
    int y2 = 6;
    int x4 = 7;
    int x5 = 8;
    int b = 2;

    int p = 4;

    t.insertValue(&x, cmp);
    t.insertValue(&y, cmp);
    t.insertValue(&x2, cmp);
    t.insertValue(&y2, cmp);
    t.insertValue(&x4, cmp);
    t.insertValue(&x3, cmp);

    print2DUtil(t.root, 20);

    t.removeValue(&p, cmp);

    t.insertValue(&x5, cmp);
    t.insertValue(&x1, cmp);

    print2DUtil(t.root, 20);
    
    t.removeValue(&x3, cmp);
    t.removeValue(&x1, cmp);
    t.removeValue(&x, cmp);

    print2DUtil(t.root, 20);

    
    AVLTree<int>::Node* t2 = t.findValue(&b, cmp);
    

    if(t2 != nullptr){
        cout << *(t2->value) << endl;
    }

    
    return 0;
}*/





#endif //AVLTREE