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
