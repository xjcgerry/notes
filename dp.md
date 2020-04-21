# 动态规划
在这里记录一下用到动态规划思想的题目
## 115. 不同的子序列
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
### 思路
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
