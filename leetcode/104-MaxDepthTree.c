#include <stdlib.h>
#include <stdio.h>

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */
int maxDepth(struct TreeNode* root) {
    if (root == NULL)
        return 0;

    int leftDepth = maxDepth(root->left); //profundidade da subarvore esquerda
    int rightDepth = maxDepth(root->right); //profundidade da subarvore direita

    return (leftDepth > rightDepth ? leftDepth : rightDepth) + 1;
}