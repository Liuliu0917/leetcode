// 题目: 判断根结点是否等于子结点之和
// 题号: 2384
// 语言: c
// 提交时间: 2025-12-19 11:03:30

/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */
bool checkTree(struct TreeNode* root) {
    if((root->left)->val + (root->right)->val == root->val){
        return true;
    }else
    {
        return false;
    }                                       
}