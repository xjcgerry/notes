#include <iostream>
#include <stack>
#include <vector>

using namespace std;

struct TreeNode{
	int val;
	TreeNode* left;
	TreeNode* right;
	TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

class Solution {
public:
	vector<int> preorderTraversal(TreeNode* root) {
		stack<TreeNode*> stk;			//调用栈
		vector<int> val;				//保存结果
		TreeNode* _root = root;
		while (_root || stk.size()) {
			while (_root) {
				stk.push(_root->right);	//右节点先压栈，最后处理
				val.push_back(_root->val);
				_root = _root->left;
			}
			_root = stk.top();
			stk.pop();
		}
		return val;
	}

	vector<int> ans;
	vector<int> preorderTraversal_re(TreeNode* root) {
		if (root != NULL) {
			ans.push_back(root->val);
			preorderTraversal_re(root->left);
			preorderTraversal_re(root->right);
		}
		return ans;
	}

	vector<int> inorderTraversal(TreeNode* root) {
		stack<TreeNode*> stk;
		vector<int> val;
		TreeNode* _root = root;
		while (_root || stk.size()) {
			while (_root) {
				stk.push(_root);		//根节点先压栈
				_root = _root->left;
			}
			_root = stk.top();
			stk.pop();
			val.push_back(root->val);
			_root = _root->right;
		}
		return val;
	}

	vector<int> inorderTraversal_re(TreeNode* root) {
		if (root != NULL) {
			inorderTraversal_re(root->left);
			ans.push_back(root->val);
			inorderTraversal_re(root->right);
		}
		return ans;
	}


	//https://leetcode-cn.com/problems/binary-tree-postorder-traversal/solution/a-li-mian-shi-ti-zhi-yong-zhan-qu-zuo-er-cha-shu-d/
	//仅利用栈判断该节点是否为父节点，每次在栈中压入父结点后压入NULL，之后再依次压入右子结点和左子结点
	vector<int> postorderTraversal(TreeNode* root) {
		if (root == NULL) return{};
		stack<TreeNode*> stk;
		vector<int> val;
		stk.push(root);
		while (!stk.empty()) {
			TreeNode* node = stk.top();
			if (node == NULL) {
				stk.pop();
				val.push_back(stk.top()->val);
				stk.pop();
				continue;
			}
			stk.push(NULL);
			if (node->right)
				stk.push(node->right);
			if (node->left)
				stk.push(node->left);
		}
		return val;
	}
	
	vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> res;

        if (root == NULL) return res;
        queue<TreeNode*> q;
        q.push(root);

        while(!q.empty()) {
            vector<int> level;
            int count = q.size();
            while (count--) {
                TreeNode* t = q.front();
                q.pop();
                level.push_back(t->val);
                if (t->left) q.push(t->left);
                if (t->right) q.push(t->right);
            }
            res.push_back(level);
        }
        return res;

    }

};

int main() {
	TreeNode* node1 = new TreeNode(1);
	TreeNode* node2 = new TreeNode(2);
	TreeNode* node3 = new TreeNode(3);
	TreeNode* node4 = new TreeNode(4);
	TreeNode* node5 = new TreeNode(5);
	TreeNode* node6 = new TreeNode(6);
	TreeNode* node7 = new TreeNode(7);

	node1->left = node2;
	node1->right = node3;
	node2->left = node4;
	node2->right = node5;
	node3->left = node6;
	node3->right = node7;

	vector<int> array;
	Solution ss;
	array = ss.inorderTraversal_re(node1);
	getchar();
	return 0;

}
