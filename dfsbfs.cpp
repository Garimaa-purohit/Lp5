#include <bits/stdc++.h>
#include <omp.h>

using namespace std;

class TreeNode {
public:
    int val;
    TreeNode *left, *right;

    TreeNode(int v) {
        val = v;
        left = right = NULL;
    }
};

class BinaryTree {
public:
    TreeNode *root;

    BinaryTree(int val) {
        root = new TreeNode(val);
    }

    // ================= SEQUENTIAL BFS =================
    void sequential_bfs() {

        cout << "Sequential BFS: ";

        queue<TreeNode*> q;
        q.push(root);

        while (!q.empty()) {

            TreeNode* node = q.front();
            q.pop();

            cout << node->val << " ";

            // Artificial workload
            for(long long int i = 0; i < 1000000; i++);

            if(node->left)
                q.push(node->left);

            if(node->right)
                q.push(node->right);
        }

        cout << endl; 
    }

    // ================= PARALLEL BFS =================
    void parallel_bfs() {

        cout << "Parallel BFS: ";

        queue<TreeNode*> q;
        q.push(root);

        while(!q.empty()) {

            int size = q.size();

#pragma omp parallel for
            for(int i = 0; i < size; i++) {

                TreeNode* node = NULL;

#pragma omp critical
                {
                    if(!q.empty()) {
                        node = q.front();
                        q.pop();

                        cout << node->val << " ";
                    }
                }

                if(node != NULL) {

                    // Artificial workload
                    for(long long int j = 0; j < 1000000; j++);

#pragma omp critical
                    {
                        if(node->left)
                            q.push(node->left);

                        if(node->right)
                            q.push(node->right);
                    }
                }
            }
        }

        cout << endl;
    }

    // ================= SEQUENTIAL DFS =================
    void sequential_dfs_util(TreeNode* node) {

        if(node == NULL)
            return;

        cout << node->val << " ";

        // Artificial workload
        for(long long int i = 0; i < 1000000; i++);

        sequential_dfs_util(node->left);
        sequential_dfs_util(node->right);
    }

    void sequential_dfs() {

        cout << "Sequential DFS: ";

        sequential_dfs_util(root);

        cout << endl;
    }

    // ================= PARALLEL DFS =================
    void parallel_dfs_util(TreeNode* node) {

        if(node == NULL)
            return;

#pragma omp critical
        {
            cout << node->val << " ";
        }

        // Artificial workload
        for(long long int i = 0; i < 1000000; i++);

#pragma omp parallel sections
        {

#pragma omp section
            parallel_dfs_util(node->left);

#pragma omp section
            parallel_dfs_util(node->right);
        }
    }

    void parallel_dfs() {

        cout << "Parallel DFS: ";

        parallel_dfs_util(root);

        cout << endl;
    }
};

int main() {

    BinaryTree tree(1);

    tree.root->left = new TreeNode(2);
    tree.root->right = new TreeNode(3);

    tree.root->left->left = new TreeNode(4);
    tree.root->left->right = new TreeNode(5);

    tree.root->right->left = new TreeNode(6);
    tree.root->right->right = new TreeNode(7);

    double start, end;

    // ================= SEQUENTIAL BFS =================
    start = omp_get_wtime();

    tree.sequential_bfs();

    end = omp_get_wtime();

    cout << "Time taken by Sequential BFS: "
         << end - start << " seconds" << endl << endl;

    // ================= PARALLEL BFS =================
    start = omp_get_wtime();

    tree.parallel_bfs();

    end = omp_get_wtime();

    cout << "Time taken by Parallel BFS: "
         << end - start << " seconds" << endl << endl;

    // ================= SEQUENTIAL DFS =================
    start = omp_get_wtime();

    tree.sequential_dfs();

    end = omp_get_wtime();

    cout << "Time taken by Sequential DFS: "
         << end - start << " seconds" << endl << endl;

    // ================= PARALLEL DFS =================
    start = omp_get_wtime();

    tree.parallel_dfs();

    end = omp_get_wtime();

    cout << "Time taken by Parallel DFS: "
         << end - start << " seconds" << endl << endl;

    return 0;
}