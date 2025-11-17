/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode() : val(0), left(nullptr), right(nullptr) {}
 *     TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
 *     TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
 * };
 **/
    int maxDepth(TreeNode* root) {
        if (root == NULL)
            return 0;

        int leftDepth = maxDepth(root->left); //profundidade da subarvore esquerda
        int rightDepth = maxDepth(root->right); //profundidade da subarvore direita

        return (leftDepth > rightDepth ? leftDepth : rightDepth) + 1; //retorna a maior profundidade entre as duas subarvores + 1 (para o no atual)
    }
