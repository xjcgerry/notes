在这里记录一下用到动态规划思想的题目
# 115. 不同的子序列
给定一个字符串 S 和一个字符串 T，计算在 S 的子序列中 T 出现的个数。  
一个字符串的一个子序列是指，通过删除一些（也可以不删除）字符且不干扰剩余字符相对位置所组成的新字符串。
（例如，"ACE" 是 "ABCDE" 的一个子序列，而 "AEC" 不是）  
题目数据保证答案符合 32 位带符号整数范围。  
* 示例1  
输入：S = "rabbbit", T = "rabbit"  
输出：3  
解释：  
如下图所示, 有 3 种可以从 S 中得到 "rabbit" 的方案。  
(上箭头符号 ^ 表示选取的字母)  
rabbbit  
^^^^^ ^  
rabbbit  
^^ ^^^^  
rabbbit  
^^^ ^^^  
## 思路
参考：https://www.jianshu.com/p/719954a411c0  
https://blog.csdn.net/qq874455953/article/details/83959093  
二维dp数组记录，dp[i][j]表示的是s的前j个字符有多少种方法可以包含t的前i个字符。  
dp[0][j]：t是空串，s不是空串，所以dp[0][j]=1  
dp[i][0]：t不是空串，s是空串，则肯定为0  
接下来：
* s[j]！=t[i]，s：“ABCD”，t：“AC”  
ABCD"匹配"AC"的方式至少有"ABC"匹配"AC"那么多种
* s[j]==t[i]，s：“ABCD”，t：“AC”  
还要加上dp[i - 1][j - 1]那么多种匹配方式
````cpp
class Solution {
public:
    int numDistinct(string s, string t) {
        vector<vector<long>> dp(t.size()+1, vector<long>(s.size()+1, 0));
        dp[0][0] = 1;
        for (int i = 1; i <= s.size(); i++)
            dp[0][i] = 1;
        for (int i = 1; i <= t.size(); i++)
            dp[i][0] = 0;

        for (int i = 1; i <= t.size(); i++) {
            for (int j = 1; j <= s.size(); j++) {
                if (s[j-1] != t[i-1])
                    dp[i][j] = dp[i][j-1];
                if (s[j-1] == t[i-1])
                    dp[i][j] = dp[i][j-1] + dp[i-1][j-1];
            }
        }
        return dp[t.size()][s.size()];
    }
};
````

# 322. 零钱兑换
给定不同面额的硬币 coins 和一个总金额 amount。编写一个函数来计算可以凑成总金额所需的最少的硬币个数。如果没有任何一种硬币组合能组成总金额，返回 -1。
````
输入: coins = [1, 2, 5], amount = 11
输出: 3 
解释: 11 = 5 + 5 + 1
````
## 思路
这是一道比较基本的动态规划问题。

凑成面值为11的最小硬币数可以由以下3者的最小值得到：
1. 凑成面值为10的最小硬币数+面值为1的这一枚硬币
2. 凑成面值为9的最小硬币数+面值为2的这一枚硬币
3. 凑成面值为6的最小硬币数+面值为5的这一枚硬币
即dp[11] = min(dp[10]+1, dp[9]+1, dp[6]+1)

定义状态 dp[i]：凑齐总价值i需要的最少硬币数，状态就是问的问题。  
状态转移方程：
````
 dp[amount] = min(1 + dp[amount - coin[i]]) for i in [0, len - 1] if coin[i] <= amount
 ````
 
 需要注意以下两点：
 1. 硬币的面值首先要小于等于当前要凑出来的面值
 2. 剩余的那个面值要能够凑出来，否则这个面值对应的状态应该设置为一个不可能的值
 
 ````cpp
 class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        vector<int> dp(amount + 1, amount + 1); //先把dp的值都设成不可能的值amount+1
        dp[0] = 0;

        for (int i = 1; i <= amount; i++) {
            for (int coin : coins) {
                if (i - coin >= 0 && dp[i - coin] != amount + 1)
                    dp[i] = min(dp[i], dp[i - coin] + 1);
            }
        }
        if (dp[amount] == amount + 1)
            dp[amount] = -1;
        return dp[amount];
    }
};
 ````

# 279. 完全平方数
跟零钱兑换一个思路

# 343. 整数拆分
来自：https://leetcode-cn.com/problems/integer-break/solution/bao-li-sou-suo-ji-yi-hua-sou-suo-dong-tai-gui-hua-/
## 暴力搜索
![image](https://github.com/xjcgerry/no-unemployment/blob/master/images/343-1.png)
容易得到递归表达式F(n) = max{i * F(n - i)}, i = 1, 2, ..., n - 1  
上述表达式是表明n-i要继续分解的情况，但是如果n-i比F(n-i)更大，就不用继续分解了。就比如3和F(3)。因此还需要比较i*(n-i)和i\*F(n-i)的大小。
````cpp
class Solution {
public:
    int integerBreak(int n) {
        if (n == 2)
            return 1;
        int res = -1;
        for (int i = 1; i <= n-1; i++)
            res = max(res, max(i * (n - i), i * integerBreak(n - i)));
        return res;
    }
};
````
暴力解法在n取到28时，就会超出时间限制，这是因为递归计算的过程中计算了很多重复值。
## 记忆化搜索（备忘录）
````cpp
class Solution {
public:
    vector<int> memory;
    int integerBreak(int n) {
        memory = vector<int>(n + 1);
        return integerBreakHelper(n);
    }
    int integerBreakHelper(int n) {
        if (n == 2)
            return 1;
        // memory的初始值为0，如果它不为0，说明已经计算过了，直接返回即可
        if (memory[n] != 0)
            return memory[n];
        
        int res = -1;
        for (int i = 1; i <= n - 1; i++) {
            res = max(res, max(i * (n - i), i * integerBreakHelper(n - i)));
        }
        
        //将每次计算的结果保存到备忘录数组中
        memory[n] = res;
        return res;
    }
};
````
## 动态规划
````cpp
class Solution {
public:
    int integerBreak(int n) {
        vector<int> dp(n+1, 0);
        dp[2] = 1;
        for (int i = 3; i <= n; i++) {
            for (int j = 1; j <= i - 1; j++) {
                dp[i] = max(dp[i], max(j * dp[i - j], j * (i - j)));
            }
        } 
        return dp[n];
    }
};
````

# 198. 打家劫舍
你是一个专业的小偷，计划偷窃沿街的房屋。每间房内都藏有一定的现金，影响你偷窃的唯一制约因素就是相邻的房屋装有相互连通的防盗系统，如果两间相邻的房屋在同一晚上被小偷闯入，系统会自动报警。

给定一个代表每个房屋存放金额的非负整数数组，计算你 不触动警报装置的情况下 ，一夜之内能够偷窃到的最高金额。

示例1
````
输入：[1,2,3,1]
输出：4
解释：偷窃 1 号房屋 (金额 = 1) ，然后偷窃 3 号房屋 (金额 = 3)。
     偷窃到的最高金额 = 1 + 3 = 4 。
````

示例2
````
输入：[2,7,9,3,1]
输出：12
解释：偷窃 1 号房屋 (金额 = 2), 偷窃 3 号房屋 (金额 = 9)，接着偷窃 5 号房屋 (金额 = 1)。
     偷窃到的最高金额 = 2 + 9 + 1 = 12 。
````
基础的dp，递推公式比较简单
````cpp
class Solution {
public:
    int rob(vector<int>& nums) {
        if (nums.size() == 0)
            return 0;
        vector<int> dp(nums.size() + 1);
        dp[0] = 0;
        dp[1] = nums[0];
        for (int i = 2; i <= nums.size(); i++)
            dp[i] = max(dp[i - 2] + nums[i - 1], dp[i - 1]);
        return dp[nums.size()];
    }
};
````
空间优化

对于小偷问题，我们发现，最后一步计算dp[n]的时候，实际上只用到了dp[n-1]和dp[n-2]的结果。n-3之前的子问题，实际上早就已经用不到了，那么可以只用两个变量保存两个子问题的结果。
````cpp
class Solution {
public:
    int rob(vector<int>& nums) {
        if (nums.size() == 0)
            return 0;
        
        int prev = 0;
        int cur = 0;

        for (int i = 0; i < nums.size(); i++) {
            int temp = max(cur, prev + nums[i]);
            prev = cur;
            cur = temp;
        }
        return cur;
    }
};
````

# 62. 不同路径
找出状态转移方程就可以解出来，通过画图能够很容易的推导出状态转移方程

![image](https://github.com/xjcgerry/no-unemployment/blob/master/images/62-1.png)
````cpp
class Solution {
public:
    int uniquePaths(int m, int n) {
        vector<vector<int>> dp(m, vector<int>(n));
        for (int i = 0; i < m; i++)
            dp[i][0] = 1;
        for (int j = 0; j < n; j++)
            dp[0][j] = 1;
        for (int i = 1; i < m; i++) {
            for (int j = 1; j < n; j++) {
                dp[i][j] = dp[i-1][j] + dp[i][j-1];
            }
        }
        return dp[m-1][n-1];
    }
};
````

# 63. 不同路径Ⅱ
与上一题思路一样的，只要该处有障碍，则状态值就是0。
````cpp
class Solution {
public:
    int uniquePathsWithObstacles(vector<vector<int>>& obstacleGrid) {
        vector<vector<int>> dp(obstacleGrid.size(), vector<int>(obstacleGrid[0].size()));   //int的默认值是0
        for (int i = 0; i < obstacleGrid.size(); i++) {
            if (obstacleGrid[i][0] == 0)    //只要没遇到障碍物，该处的状态就是1
                dp[i][0] = 1;
            else    //只要遇到障碍物了，后面的状态都是0。因为默认值是0，所以只要跳出不赋值就可以了
                break;
        }
        for (int j = 0; j < obstacleGrid[0].size(); j++) {  //同上
            if (obstacleGrid[0][j] == 0)
                dp[0][j] = 1;
            else
                break;
        }
        for (int i = 1; i < obstacleGrid.size(); i++) {
            for (int j = 1; j < obstacleGrid[0].size(); j++) {
                if (obstacleGrid[i][j] == 1)    //这里要注意，如果该处有障碍物，则状态值为0
                    dp[i][j] = 0;
                else
                    dp[i][j] = dp[i-1][j] + dp[i][j-1];
            }
        }
        return dp[obstacleGrid.size()-1][obstacleGrid[0].size()-1];
    }
};
````

# 718. 最长重复子数组
这题第一眼看到就意识到应该用动态规划来做，然而快要成功的时候放弃了，啊啊啊啊，恨呐。
首先应该把dp表列出来，dp表里面记录的是当前位置的子数组长度，状态转移方程就比较容易看出来了。
![image](https://github.com/xjcgerry/no-unemployment/blob/master/images/718-1.png)
````cpp
class Solution {
public:
    int findLength(vector<int>& A, vector<int>& B) {
        if (A.size() == 0 || B.size() == 0)
            return 0;
        int m = A.size();
        int n = B.size();
        int maxlen = 0;
        vector<vector<int>> dp(m + 1, vector<int>(n + 1));
        for (int i = 0; i <= m; i++)
            dp[i][0] = 0;
        for (int j = 0; j <= n; j++)
            dp[0][j] = 0;
        
        for (int i = 1; i <= m; i++) {
            for (int j = 1; j <= n; j++) {
                if (A[i - 1] == B[j - 1])
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                if (dp[i][j] > maxlen)
                    maxlen = dp[i][j];
            }
        }
        return maxlen;
    }
};
````
## 滑动窗口法
来自力扣官方题解：https://leetcode-cn.com/problems/maximum-length-of-repeated-subarray/solution/zui-chang-zhong-fu-zi-shu-zu-by-leetcode-solution/
![image](https://github.com/xjcgerry/no-unemployment/blob/master/images/718-2.gif)
````cpp
class Solution {
public:
    int maxLength(vector<int>& A, vector<int>& B, int addA, int addB, int len) {
        int res = 0, k = 0;
        for (int i = 0; i < len; i++) {
            if (A[addA + i] == B[addB + i]) {
                k++;
            }
            else {
                k = 0;
            }
            res = max(res, k);
        }
        return res;
    }
    
    int findLength(vector<int>& A, vector<int>& B) {
        int m = A.size();
        int n = B.size();
        int res = 0;
        for (int i = 0; i < m; i++) {
            int len = min(m - i, n);
            int maxlen = maxLength(A, B, i, 0, len);
            res = max(res, maxlen);
        }

        for (int i = 0; i < n; i++) {
            int len = min(m, n - i);
            int maxlen = maxLength(A, B, 0, i, len);
            res = max(res, maxlen);
        }
        return res;
    }
};
````

# 44. 通配符匹配
给定一个字符串 (s) 和一个字符模式 (p) ，实现一个支持 '?' 和 '*' 的通配符匹配。
````
'?' 可以匹配任何单个字符。
'*' 可以匹配任意字符串（包括空字符串）。
两个字符串完全匹配才算匹配成功。
````
说明:

* s 可能为空，且只包含从 a-z 的小写字母。
* p 可能为空，且只包含从 a-z 的小写字母，以及字符 ? 和 *。

## 思路
动态规划

状态转移方程

边界条件：  
dp[0][j]：只有\*才能匹配字符串，只要遇到不是\*号的，后面就都是false  
dp[i][0]：肯定无法匹配，就保持初始的false值就行了

````cpp
class Solution {
public:
    bool isMatch(string s, string p) {
        int len1 = s.size();
        int len2 = p.size();
        vector<vector<bool>> dp(len1 + 1, vector<bool>(len2 + 1));
        dp[0][0] = true;
        for (int i = 1; i <= len2; ++i) {
            if (p[i - 1] == '*') {
                dp[0][i] = true;
            }
            else {
                break;
            }
        }

        for (int i = 1; i <= len1; i++) {
            for (int j = 1; j <= len2; j++) {
                if (p[j - 1] == s[i - 1] || p[j - 1] == '?') {
                    dp[i][j] = dp[i - 1][j - 1];
                }
                else if (p[j - 1] == '*') {
                    dp[i][j] = dp[i][j - 1] | dp[i - 1][j]; //*可以匹配0或多个字符，对应的就是不使用或者使用这个*
                }
            }
        }
        return dp[len1][len2];
    }
};
````

# 120. 三角形最小路径和
给定三角形，每次只能移动到下一行中的相邻结点，求从顶点到底边的最小路径和。
````
[
[2],
[3,4],
[6,5,7],
[4,1,8,3]
]
相邻结点：与(i, j) 点相邻的结点为 (i + 1, j) 和 (i + 1, j + 1)。
````
## 思路
以自底向上的顺序，定义f(i,j)为点（i,j)到底边的最小路径和，那么递归求解式为  
f(i,j) = min(f(i+1,j),f(i+1,j+1))+triangle[i][j]  
1. 递归解法  
````cpp
class Solution {
public:
    int dfs(vector<vector<int>>& triangle, int i, int j) {
        if (i == triangle.size())
            return 0;
        return min(dfs(triangle, i + 1, j), dfs(triangle, i + 1, j + 1)) + triangle[i][j];
    }

    int minimumTotal(vector<vector<int>>& triangle) {
        return dfs(triangle, 0, 0);
    }
};
````

2. 动态规划  
dp[i][j]表示从点(i,j)到底边的最小路径和

状态转移方程为：dp[i][j] = min(dp[i + 1][j], dp[i + 1][j + 1]) + triangle[i][j]
````cpp
class Solution {
public:
    int minimumTotal(vector<vector<int>>& triangle) {
        int n = triangle.size();
        vector<vector<int>> dp(n + 1, vector<int>(n + 1));  //假设它有n+1行，第n+1行全都是0
        for (int i = n - 1; i >= 0; i--) {
            for (int j = 0; j <= i; j++) {  //因为第i行最多有i个数字，所以j<=i
                dp[i][j] = triangle[i][j] + min(dp[i + 1][j], dp[i + 1][j + 1]);
            }
        }
        return dp[0][0];
    }
};
````

3. 空间优化  
计算dp[i][j]时，只用到了下一行的dp[i+1][j]和dp[i+1][j+1]，因此dp数组不需要定义n行，只需要定义一行就行了。
````cpp
class Solution {
    int minimumTotal(vector<vector<int>>& triangle) {
        int n = triangle.size();
        vector<int> dp(n+1);
        for (int i = n - 1; i >= 0; i--) {
            for (int j = 0; j <= i; j++) {
                dp[i][j] = min(dp[i+1][j], dp[i+1][j+1])+triangle[i][j];
            }
        }
        return dp[0][0];
    }
};
````
