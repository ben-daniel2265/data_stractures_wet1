#include<iostream>
using namespace std;

template<class T>
class node{
    public:
        T key;
        int height;
        node * left;
        node * right;
        node(T k){
            height = 1;
            key = k;
            left = NULL;
            right = NULL;
        }
};



template <typename T>
class AVL{
    public:
        class node{
            public:
                T key;
                int height;
                node * left;
                node * right;
                node(T k){
                    height = 1;
                    key = k;
                    left = NULL;
                    right = NULL;
                }
        };
        node * root = NULL;

        void insert(T x){
            root=insertUtil(root, x);
        }
        void remove(T x){
            root=removeUtil(root, x);
        }
        node * search(T x){
            return searchUtil(root,x);
        }
        void inorder(){
            inorderUtil(root);
            cout<<endl;
        }
    private:
        int height(node * head){
            if(head==NULL) return 0;
            return head->height;
        }
        node * rightRotation(node * head){
            node * newhead = head->left;
            head->left = newhead->right;
            newhead->right = head;
            head->height = 1+max(height(head->left), height(head->right));
            newhead->height = 1+max(height(newhead->left), height(newhead->right));
            return newhead;
        }


        node * leftRotation(node * head){
            node * newhead = head->right;
            head->right = newhead->left;
            newhead->left = head;
            head->height = 1+max(height(head->left), height(head->right));
            newhead->height = 1+max(height(newhead->left), height(newhead->right));
            return newhead;
        }

        void inorderUtil(node * head){
            if(head==NULL) return ;
            inorderUtil(head->left);
            cout<<head->key<<"-"<<head->height<<" ";
            inorderUtil(head->right);
        }

        node * insertUtil(node * head, T x){
            if(head==NULL){
                node * temp = new node(x);
                return temp;
            }
            if(x < head->key) head->left = insertUtil(head->left, x);
            else if(x > head->key) head->right = insertUtil(head->right, x);
            head->height = 1 + max(height(head->left), height(head->right));
            int bal = height(head->left) - height(head->right);
            if(bal>1){
                if(x < head->left->key){
                    return rightRotation(head);
                }else{
                    head->left = leftRotation(head->left);
                    return rightRotation(head);
                }
            }else if(bal<-1){
                if(x > head->right->key){
                    return leftRotation(head);
                }else{
                    head->right = rightRotation(head->right);
                    return leftRotation(head);
                }
            }
            return head;
        }
        node * removeUtil(node * head, T x){
            if(head==NULL) return NULL;
            if(x < head->key){
                head->left = removeUtil(head->left, x);
            }else if(x > head->key){
                head->right = removeUtil(head->right, x);
            }else{
                node * r = head->right;
                if(head->right==NULL){
                    node * l = head->left;
                    delete(head);
                    head = l;
                }else if(head->left==NULL){
                    delete(head);
                    head = r;
                }else{
                    while(r->left!=NULL) r = r->left;
                    head->key = r->key;
                    head->right = removeUtil(head->right, r->key);
                }
            }
            if(head==NULL) return head;
            head->height = 1 + max(height(head->left), height(head->right));
            int bal = height(head->left) - height(head->right);
            if(bal>1){
                if(height(head->left) >= height(head->right)){
                    return rightRotation(head);
                }else{
                    head->left = leftRotation(head->left);
                    return rightRotation(head);
                }
            }else if(bal < -1){
                if(height(head->right) >= height(head->left)){
                    return leftRotation(head);
                }else{
                    head->right = rightRotation(head->right);
                    return leftRotation(head);
                }
            }
            return head;
        }
        node * searchUtil(node * head, T x){
            if(head == NULL) return NULL;
            T k = head->key;
            if(k == x) return head;
            if(k > x) return searchUtil(head->left, x);
            if(k < x) return searchUtil(head->right, x);
        }
};

void print2DUtil(AVL<float>::node* root, int space)
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
        cout << root -> key << "\n";

        // Process left child
        print2DUtil(root->left, space);
    }

int main(){
    AVL<float> t;
    t.insert(1.3);
    t.insert(2.4);
    t.insert(3.5);
    t.insert(4.6);
    t.insert(5.7);
    t.insert(6.8);
    t.insert(7.9);
    t.inorder();

    print2DUtil(t.root, 20);
    t.remove(5.7);
    t.remove(6.8);
    t.remove(7.9);
    t.inorder();

    print2DUtil(t.root, 20);
}


