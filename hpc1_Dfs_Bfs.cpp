#include<iostream>
#include<stdlib.h>
#include<stack>
#include<queue>
#include<omp.h>

using namespace std;

class node {
public:
    node *left, *right;
    int data;
};

node *insert(node *root, int data) {
    if (!root) {
        root = new node;
        root->left = NULL;
        root->right = NULL;
        root->data = data;
        return root;
    }
    queue<node *> q;
    q.push(root);

    while (!q.empty()) {
        node *temp = q.front();
        q.pop();
        if (temp->left == NULL) {
            temp->left = new node;
            temp->left->left = NULL;
            temp->left->right = NULL;
            temp->left->data = data;
            return root;
        } else {
            q.push(temp->left);
        }
        if (temp->right == NULL) {
            temp->right = new node;
            temp->right->left = NULL;
            temp->right->right = NULL;
            temp->right->data = data;
            return root;
        } else {
            q.push(temp->right);
        }
    }
    return root;
}

void bfs(node *head) {
    queue<node*> q;
    q.push(head);
    int qSize;

    while (!q.empty()) {
        qSize = q.size();
        #pragma omp parallel for
        for (int i = 0; i < qSize; i++) {
            node* currNode;
            #pragma omp critical
            {
                currNode = q.front();
                q.pop();
                cout << "\t" << currNode->data;
            }
            if (currNode->left) {
                #pragma omp critical
                {
                    q.push(currNode->left);
                }
            }
            if (currNode->right) {
                #pragma omp critical
                {
                    q.push(currNode->right);
                }
            }
        }
    }
}

void dfs(node *root) {
    stack<node*> s;
    s.push(root);

    while (!s.empty()) {
        node* currNode;
        #pragma omp critical
        {
            currNode = s.top();
            s.pop();
            cout << "\t" << currNode->data;
        }
        if (currNode->right) {
            #pragma omp critical
            {
                s.push(currNode->right);
            }
        }
        if (currNode->left) {
            #pragma omp critical
            {
                s.push(currNode->left);
            }
        }
    }
}

int main() {
    node *root = NULL;
    int data;
    char ans;
    do {
        cout << "\nEnter data: ";
        cin >> data;
        root = insert(root, data);
        cout << "Do you want to insert one more node? (y/n): ";
        cin >> ans;
    } while (ans == 'y' || ans == 'Y');

    cout << "\nBFS Traversal:";
    bfs(root);

    cout << "\nDFS Traversal:";
    dfs(root);

    return 0;
}
