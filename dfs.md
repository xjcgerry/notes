# 深度优先搜索
相关题目
## 46. 全排列
### 思路
全排列问题，可以用dfs解决。  用的这个里面的解法：https://www.bilibili.com/video/BV1c7411h7EW  
dfs可以用来解决两类问题。permutation和combination。78题子集是combination问题。
````cpp
class Solution {
public:
    void dfs(vector<int>& nums, int pos, vector<int> &list, vector<vector<int>> &result, vector<int> &visited) {
        if(pos == nums.size()) {  //停止条件
            result.push_back(list);
            return;   //相当于break
        }
        for (int i = 0; i < nums.size(); i++) {
            if(!visited[i]) {
                list.push_back(nums[i]);
                visited[i] = 1;
                dfs(nums, pos + 1, list, result, visited);
                visited[i] = 0;   //回退
                list.pop_back();
            }
        }
    }

    vector<vector<int>> permute(vector<int>& nums) {
        vector<vector<int>> result;
        vector<int> list;
        vector<int> visited(nums.size(),0);   //用来记录nums中的数是否被访问过
        dfs(nums, 0, list, result, visited);  //dfs的模板
        return result;
    }
};
````
### 回溯算法
参考：https://leetcode-cn.com/problems/permutations/solution/hui-su-suan-fa-xiang-jie-by-labuladong-2/  
解决一个回溯问题，实际是一个决策树的遍历过程，需要思考三个问题：
* 路径：已做出的选择
* 选择列表：当前可以做出的选择
* 结束条件：也就是到达决策树底层，无法再做选择的条件。
回溯算法的框架：
````python
result = []
def backtrack(路径, 选择列表):
    if 满足结束条件:
        result.add(路径)
        return
    
    for 选择 in 选择列表:
        做选择
        backtrack(路径, 选择列表)
        撤销选择
````
所以本题的答案如下
````cpp
class Solution {
public:
    vector<vector<int>> result;

    void backtrack(vector<int> &nums, vector<int> &track) {
        if(track.size() == nums.size()) {
            result.push_back(track);
            return;
        }
        for (int i = 0; i < nums.size(); i++) {
            if (find(track.begin(), track.end(), nums[i]) != track.end())   //find函数，如果没有找到，就返回nums尾部的迭代器。否则就是找到了
                continue;
            track.push_back(nums[i]);
            backtrack(nums, track);
            track.pop_back();
        }
    }

    vector<vector<int>> permute(vector<int>& nums) {
        vector<int> track;
        backtrack(nums, track);
        return result;
    }
};
````

## 112. 路径总和
````cpp
/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     TreeNode *left;
 *     TreeNode *right;
 *     TreeNode(int x) : val(x), left(NULL), right(NULL) {}
 * };
 */
class Solution {
public:
    bool flag = false;  //先定义出结果
    void dfs(TreeNode *root, int sum, int result) {
        if(!root) //结束条件1：到达叶子节点
            return;
        if(root->left == NULL && root->right == NULL) {
            if (sum == result + root->val) {  //结束条件2：到达叶子节点，且result = sum
                flag = true;
            }
            return;
        }
        dfs(root->left, sum, result + root->val);
        dfs(root->right, sum, result + root->val);
    }
    bool hasPathSum(TreeNode* root, int sum) {
        int result = 0;
        dfs(root, sum, result);
        return flag;
    }
};
````
## 113. 路径总和 II
用B站视频里看到的方法做，先定义结果和每次的路径值，以及当前的和cnt，然后借助dfs函数进行递归。
````cpp
class Solution {
public:
    void dfs(TreeNode* root, int sum, int cnt, vector<vector<int>> &result, vector<int> &track) {
        if (!root)
            return;
        track.push_back(root->val);
        if (root->left == NULL && root->right == NULL) {
            if (cnt + root->val == sum) {
                result.push_back(track);  //这一步下面不需要跳出，因为上面已经有退出条件了
            }
        }
        dfs(root->left, sum, cnt+root->val, result, track);
        dfs(root->right, sum, cnt+root->val, result, track);
        track.pop_back();   //只需要pop一次
    }
    vector<vector<int>> pathSum(TreeNode* root, int sum) {
        vector<vector<int>> result;
        vector<int> track;
        int cnt = 0;
        dfs(root, sum, cnt, result, track);
        return result;
    }
};
````
# 410. 分割数组的最大值
给定一个非负整数数组和一个整数 m，你需要将这个数组分成 m 个非空的连续子数组。设计一个算法使得这 m 个子数组各自和的最大值最小。
## 思路
用深度优先搜索生成所有的分割方案。对于书读中的每个元素，我们可以把它附加到之前的子数组上或者以这个数开始一个新的子数组（如果数组的长度不超过m）。同时更新每个子数组的和。

````cpp
class Solution {
public:
    int ans;
    int n, count;   //count记录子数组的个数
    void dfs(vector<int>& nums, int i, int cntSubarray, int curSum, int curMax) {
        if (i == n && cntSubarray == count) {
            ans = (ans, curMax);
            return;
        }
        if (i == n)
            return;
        if (i > 0)
            dfs(nums, i + 1, cntSubarray, curSum + nums[i], max(curMax, curSum + nums[i]));
        if (count < m)
            dfs(nums, i + 1, cntSubarray + 1, nums[i], max(curMax, nums[i]));
    }
    
    int splitArray(vector<int> &nums, int m) {
        ans = INT_MAX;
        n = nums.size();
        count = m;
        dfs(nums, 0, 0, 0, 0);
        return ans;
    }
};
````

# 面试题.字符串的全排列
可能有重复的字母
````cpp
#include <bits/stdc++.h>

using namespace std;

class Solution {
public:
	void dfs(string &array, string &cur, vector<bool> &used, vector<string> &res)
	{
		if (cur.size() == array.size())
		{
			res.push_back(cur);
			return;
		}
		for (int i = 0; i < array.size(); i++)
		{
			if (used[i] || (i > 0 && !used[i - 1] && array[i] == array[i - 1]))	//剪枝
				continue;
			cur.push_back(array[i]);
			used[i] = true;
			dfs(array, cur, used, res);
			cur.pop_back();
			used[i] = false;
		}
	}

	int permuteUnique(string &array)
	{
		int len = array.size();
		string cur;
		int count = 0;
		vector<string> res;
		if (len == 0)
			return 0;

		sort(array.begin(), array.end());
		vector<bool> used(len,false);
		dfs(array, cur, used, res);
		return res.size();
	}
};

int main()
{
	Solution ss;
	string array = "AABBCC";
	int result = ss.permuteUnique(array);
	system("pause");
	return 0;
}
````

# 39. 组合总和
给定一个无重复元素的数组 candidates 和一个目标数 target ，找出 candidates 中所有可以使数字和为 target 的组合。  
candidates 中的数字可以无限制重复被选取。  
说明：
* 所有数字（包括 target）都是正整数。
* 解集不能包含重复的组合。 
示例1：  
输入: candidates = [2,3,6,7], target = 7,  
所求解集为:  
[  
  [7],  
  [2,2,3]  
]  
## 解法
可以用target减去candidates中的数，直到target=0，找到了符合要求的集合；target<0，不符合要求；
或者用加法来做，从0开始加candidates中的数，直到sum=target；
````cpp
class Solution {
private:
    vector<int> candidates;
    vector<vector<int>> res;
    vector<int> cur;

public:
    void DFS(int start, int target) {
        if (target == 0) {
            res.push_back(cur);
            return;
        }
        for (int i = start; i < candidates.size() && target - candidates[i] >= 0; i++) {
            cur.push_back(candidates[i]);
            DFS(i, target - candidates[i]);
            cur.pop_back();
        }
    }

    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        sort(candidates.begin(), candidates.end());
        this->candidates = candidates;
        DFS(0, target);
        return res;
    }
};
````
````cpp
class Solution {
private:
	vector<int> candidates;
	vector<vector<int>> res;
	vector<int> path;
public:
	void DFS(int start, int sum, int target) {
		if (sum == target) {
			res.push_back(path);
			return;
		}
		for (int i = start; i < candidates.size() && sum + candidates[i] <= target; i++) {
			path.push_back(candidates[i]);
			DFS(i, sum + candidates[i], target);
			path.pop_back();
		}
	}

	vector<vector<int>> combinationSum(vector<int> &candidates, int target) {
		sort(candidates.begin(), candidates.end());
		this->candidates = candidates;
		DFS(0, 0, target);
		return res;
	}
};
````
