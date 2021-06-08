#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <stack>
#include <string>
#include <deque>
#include <list>
#include <queue>

using namespace std;
namespace ariel{

    template<class T>
    class BinaryTree{

        struct Node {
            T data;
            Node *left, *right;
            Node(T data): data(data), left(nullptr), right(nullptr){}
        };

        Node *root;


        void copyTree(Node &newBT, const Node &oldBT){
            if (oldBT.left != nullptr){
                newBT.left = new Node(oldBT.left->data);
                copyTree(*newBT.left, *oldBT.left);
            }
            if (oldBT.right != nullptr){
                newBT.right = new Node(oldBT.right->data);
                copyTree(*newBT.right, *oldBT.right);
            }

            /** Recursive find **/
            /*
            T findParent(Node* p, const T& son){
                if(p->left->data == son){ return p->data;}
                if(p->right->data == son){return p->data;}
                if(p->left != nullptr){return findParent(p->left, son);}
                if(p->right != nullptr){return findParent(p->right, son);}
                throw invalid_argument{"no daddy for you"};
            }
             */


        }
    public:

        BinaryTree(){root = nullptr;}

        /** Copy constructor **/
        BinaryTree(const BinaryTree& bt){
            T val = bt.root->data;
            root = new Node(val);
            copyTree(*root, *bt.root);
        }

        /** Move constructor **/
        BinaryTree(BinaryTree &&bt) noexcept {
            root = bt.root;
            bt.root = nullptr;
        }

        BinaryTree& add_root(const T& data){
            if(root==nullptr){
                root = new Node(data);

            }
            else{
                root->data=data;
            }
            return *this;
        }

        BinaryTree& add(const T& parentData,const T& childData, bool isLeftFlag){
            Node *curr = root;
            std::stack<Node*> s;
            while (curr != nullptr || !s.empty()){
                while (curr !=  NULL){
                    s.push(curr);
                    curr = curr->right;
                }
                curr = s.top();
                s.pop();
                if(curr->data == parentData){
                    if(isLeftFlag){
                        if(curr->left == nullptr) {curr->left = new Node(childData);}
                        if(curr->left != nullptr) {curr->left->data = childData;}
                    }else {if(curr->right == nullptr) {curr->right = new Node(childData);}
                        if(curr->right != nullptr) {curr->right->data = childData;}
                    }
                    return *this;
                }
                curr = curr->left;
            }
            throw std::invalid_argument{"noParentException"};
        }

        BinaryTree& add_left(const T& patentData,const T& childData){
            if(root == nullptr){
                throw std::string{"noRootException"};
            }
            add(patentData, childData, true);
            return *this;
        }

        BinaryTree& add_right(const T& patentData,const T& childData){
            if(root == nullptr){
                throw std::string{"noRootException"};
            }
            add(patentData, childData, false);
            return *this;
        }

        BinaryTree &operator=(BinaryTree second){
            if(this == &second){
                return *this;
            }
            delete root;
            root = new Node(second.root->data);
            copyTree(*root, *second.root);
            return *this;
        }

        BinaryTree& operator=(BinaryTree &&tree) noexcept {
            if (root != nullptr) {
                delete root;
            }
            root = tree.root;
            tree.root = nullptr;
        }

        ~BinaryTree(){
            if(root != nullptr){
                delete root;
            }
        }

        class iterator{

        private:
            Node* pNode;
            int Order;
            std::stack<Node*> s1, s2;
            std::queue<Node*> q;

        public:
            iterator(int order, Node* ptr=nullptr):pNode(ptr) {
                Order = order;
                if(pNode != nullptr){
                    //** preorder **//
                    if(order == 0){
                        if(pNode->right != nullptr){
                            s1.push(pNode->right);
                        }
                        if(pNode->left!=nullptr){
                            s1.push(pNode->left);
                        }
                    }
                    //** inorder **//
                    if(order == 1){
                        Node* tmp = pNode;
                        while (tmp != nullptr || !s1.empty()) {
                            while (tmp !=  nullptr) {
                                s1.push(tmp);
                                tmp = tmp->left;
                            }
                            tmp = s1.top();
                            q.push(tmp);
                            s1.pop();
                            tmp = tmp->right;
                        }
                        pNode = q.front();
                        q.pop();
                    }
                    //** postorder **//
                    if(order == 2){
                        s1.push(pNode);
                        while (!s1.empty()){
                            Node* curr = s1.top();
                            s1.pop();
                            s2.push(curr);
                            if (curr->left) {
                                s1.push(curr->left);
                            }
                            if (curr->right) {
                                s1.push(curr->right);
                            }
                        }
                        pNode = s2.top();
                        s2.pop();
                    }
                }
            }

            /** prefix **/
            iterator& operator++() {
                if(!s1.empty() || !s2.empty() || !q.empty()){
                    //** preorder **//
                    if(Order == 0){
                        pNode = s1.top();
                        s1.pop();
                        if(pNode->right != nullptr){s1.push(pNode->right);}
                        if(pNode->left != nullptr){s1.push(pNode->left);}
                    }
                    //** inorder **//
                    if(Order == 1){
                        pNode = q.front();
                        q.pop();
                    }
                    //** postorder **//
                    if(Order == 2){
                        pNode = s2.top();
                        s2.pop();
                    }
                    return *this;
                }
                pNode = nullptr;
                return *this;
            }

            /** postfix **/;
            iterator operator++(int) {
                iterator tmp = *this;
                if(!s1.empty() || !s2.empty() || !q.empty()) {
                    //** preorder **//
                    if(Order == 0){
                        pNode = s1.top();
                        s1.pop();
                        if(pNode->right != nullptr){s1.push(pNode->right);}
                        if(pNode->left != nullptr){s1.push(pNode->left);}
                    }
                    //** inorder **//
                    if(Order == 1){
                        pNode = q.front();
                        q.pop();
                    }
                    //** postorder **//
                    if(Order == 2){
                        pNode = s2.top();
                        s2.pop();
                    }
                    return tmp;
                }
                pNode = nullptr;
                return tmp;
            }

            bool operator==(const iterator& other) const {return pNode == other.pNode;}
            bool operator!=(const iterator& other) const {return pNode != other.pNode;}
            T& operator*() const {return pNode->data;}
            T* operator->() const {return &(pNode->data);}
        };

        iterator begin_preorder(){return iterator(0, root);}
        iterator end_preorder(){return iterator(0, nullptr);}
        iterator begin_inorder(){return iterator(1, root);}
        iterator end_inorder(){return iterator(1, nullptr);}
        iterator begin_postorder(){return iterator(2, root);}
        iterator end_postorder(){return iterator(2, nullptr);}
        iterator begin(){return iterator(1, root);}
        iterator end(){return iterator(1, nullptr);}

        static void print(std::ostream& os, const std::string& prefix, const Node* node, bool isLeft){
            if( node != nullptr ){
                os << prefix;
                if(isLeft){os <<  "├──--";}
                else{os << "└──--";}
                os << node->data << std::endl;
                print(os, prefix + (isLeft ? "│   " : "    "), node->left, true);
                print(os, prefix + (isLeft ? "│   " : "    "), node->right, false);
            }
        }

        friend std::ostream& operator<<(std::ostream& os, const BinaryTree &bt){
            if (bt.root == nullptr){return os;}
            os << std::endl;
            print(os, bt.root);
            os << std::endl;
            return os;
        }
    };
}
