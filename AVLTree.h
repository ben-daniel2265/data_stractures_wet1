#ifndef AVLTREE
#define AVLTREE

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
                this->height = height;
                this->left = nullptr;
                this->right = nullptr;
            }
    };

    Node* root;

    AVLTree();

    void insert(T* value, int(*cmp_func)(T* t1, T* t2));
    void remove(T* value, int(*cmp_func)(T* t1, T* t2));
    T* find(T* value, int(*cmp_func)(T* t1, T* t2));


    private: 

    int height(Node* head){
        if(head == nullptr) return -1;
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
            if(cmp_func(value, head->left->value) > 0){
                return rotateLeft(head);
            }
            else{
                head->left = rotateRight(head->right);
                return rotateLeft(head);
            }
        }

        return head;
    }


    Node* remove(Node* head, T* value, int(*cmp_func)(T* t1, T* t2)){

        
    }
};

#endif //AVLTREE