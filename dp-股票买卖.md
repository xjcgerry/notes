# 309. 最佳买卖股票时机含冷冻期
## 思路
用dp[i]表示第i天结束之后的累计最大收益

* 我们目前持有一只股票，对应的累计最大收益为dp[i][0]
* 我们目前不持有股票，并且处于冷冻期中，对应的累计最大收益为dp[i][1]
* 我们目前不持有股票，并且不在冷冻期中，对应的累积最大收益为dp[i][2]

````cpp
class Solution {
public:
    int maxProfit(vector<int>& prices) {
        if (prices.empty()) {
            return 0;
        }
        int n = prices.size();
        vector<vector<int>> dp(n, vector<int>(3));
        dp[0][0] = -prices[0];
        //dp[i][0]:当前持有股票的最大收益
        //dp[i][1]:当前不持有股票，处于冷冻期中的累计最大收益；说明这一天把股票卖了
        //dp[i][2]:当前不持有股票，不在冷冻期中的累计最大收益；说明上一天把股票卖了，或者更早的时间
        for (int i = 1; i < n; i++) {
            dp[i][0] = max(dp[i - 1][0], dp[i - 1][2] - prices[i]);
            dp[i][1] = dp[i - 1][0] + prices[i];
            dp[i][2] = max(dp[i - 1][1], dp[i - 1][2]);
        }
        return max(dp[n - 1][1], dp[n - 1][2]);
    }
};
````
