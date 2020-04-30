# TOP K问题
原文链接：https://blog.csdn.net/qq_28114615/article/details/86231822  
Top K是很常见的一种问题，是指在N个数的无序序列中找出最大的K个数，而其中的N往往都特别大，
对于这种问题，最容易想到的办法当然就是先对其进行排序，然后直接取出最大的K的元素就行了，
但是这种方法往往是不可靠的，不仅时间效率低而且空间开销大，排序是对所有数都要进行排序，
而实际上，这类问题只关心最大的K个数，并不关心序列是否有序，因此，排序实际上是浪费了的很多资源都是没必要的。  
1. 快速排序  
在快速排序中，每一轮都会将序列一分为二，左子区间的数都小于基准数，右子区间的数都大于基准数，快速排序解决TopK问题就是基于此的。
N个数经过一轮快速排序之后，如果基准数的位置被换到了i，那么区间[0,N-1]就被分为了[0,i-1]和[i+1,N-1]，也就是说，此时有N-1-i个数比基准数大，i个数比基准数小，
假设N-1-i=X那么就有以下几种情况：  
1）X=K。说明比基准数大的有K个，其他的都比基准数小，那么就说明这K个比基准数大的数就是TopK了。  
2）X<K。说明比基准数大的数不到K个，但是这X肯定是属于TopK中的TopX，剩下的K-X就在[0,i]之间，此时就应当在[0,i]中找到Top(K-X)，转换为了TopK子问题，递归解决。  
3）X>K。说明比基准数大的数超过K个，那么就说明TopK位于[i+1,N-1]中，此时应该继续在[i+1,N-1]找TopK，成了TopK的一个子问题，可以用递归解决。
````cpp
int getIndex(vector<int>& nums,int left,int right)  //快排获取相遇点（基准数被交换后的位置）
{
    int base=nums[left];
    int start=left;
    while(left<right)
    {
        while(left<right&&nums[right]>=base)right--;
        while(left<right&&nums[left]<=base)left++;
 
        int temp=nums[right];
        nums[right]=nums[left];
        nums[left]=temp;
    }
 
    nums[start]=nums[left];
    nums[left]=base;
 
    return left;
}
int findTopKthIndex(vector<int>&nums,int k,int left,int right)
{
    int index=getIndex(nums,left,right);    //获取基准数位置
    int NumOverBase=right-index;  //比基准数大的数的个数
 
    if(NumOverBase==k)
        return index;  //比基准数大的刚好有K个
    //比基准数大的多于K个，就在右边子区间寻找TopK
    else if(NumOverBase>k)
        return findTopKthIndex(nums,k,index+1,right);
        
     //比基准数大的少于K个，就在左边找剩下的
    return findTopKthIndex(nums,k-NumOverBase,left,index);
}
vector<int> TopKInQuick(vector<int>& nums,int k,int len)
{
    if(len==k)return nums;
 
    vector<int>res;
    vector<int>temp(nums.begin(),nums.end());  //TopK不对原数组改变
    int index=findTopKthIndex(temp,k,0,len-1);  //通过快排找到第K+1大的数的位置
 
    for(int i=len-1;i>index;i--)
        res.push_back(temp[i]);  //取出TopK返回
 
    return res;
}
````
复杂度分析：O(N)+O(N/2)+O(N/4)+…… < O(2N)，因此时间复杂度为O(N)，虽然时间复杂度较低，但是需要提前将N个数读入，空间开销是一笔负担，而且对于动态数据的放入是比较死板的。  

2. 堆排序法  
堆排序是通过维护大顶堆或小顶堆来实现的。思路是：先随机取出N个数中的K个数，将这N个数构造为小顶堆，那么堆顶的数肯定就是这K个数中最小的数，
然后再将剩下的N-K个数与堆顶的进行比较，如果大于堆顶，那么说明该数有机会成为TopK，就更新堆顶为该数，此时由于小顶堆的性质可能被破坏，需要调整堆；
否则说明这个数最多只能成为TopK+1th，因此不用管它。然后将下一个数与当前堆顶的数作比较，直到N-K个数都遍历完，此时还在堆中的K个数就是TopK了。
````cpp
void adjustMinHeap(vector<int>& nums,int root,int len) //小顶堆结点调整
{
    int lch=2*root+1;  //左子结点
    int rch=lch+1;   //右子结点
    int index=root;  //较大结点
 
    if(rch<len&&nums[rch]<nums[index])index=rch; 
 
    if(lch<len&&nums[lch]<nums[index])index=lch;
 
    if(index!=root) //当前结点非最小结点
    {
        swap(nums[index],nums[root]);
        adjustMinHeap(nums,index,len);
    }
    return;
}
 
vector<int> TopKInHeap(vector<int>& nums,int k,int len)
{
    vector<int> res(nums.begin(),nums.begin()+k); //取出前k个数
 
    for(int i=k/2-1;i>=0;i--)  //根据前K个数建立一个小顶堆
    {
        adjustMinHeap(res,i,k);
    }
 
    //将剩下的数与堆顶做比较
    for(int i=k;i<len;i++)
    {
        if(nums[i]>res[0])  //当前数比堆顶数大
        {
            res[0]=nums[i]; //将堆顶更新为该数
            adjustMinHeap(res,0,k); //重新调整堆
        }
    }
 
    return res;
}
````
复杂度分析：在该方法中，首先需要对K个元素进行建堆，时间复杂度为O(K)，然后对剩下的N-K个数对堆顶进行比较及更新，
最好的情况是都不需要调整，那么时间复杂度就是遍历这N-K个数的O(N-K)，这样总体的时间复杂度就是O(K)。
在最坏情况下，N-K个数都需要更新堆顶，每次调整堆的时间复杂度为logK，因此总体的时间复杂度就是O(K)+O(NlogK)≈O(NlogK)，空间复杂度是1。
堆排序法只需要提前读入K个数据即可，可以实现来一个数据更新一次，能够实现数据动态读入并找出TopK。

## 347. 前k个高频元素
利用priority_queue构建小根堆，数据类型为pair<int, int>。
````cpp
class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        unordered_map<int, int> mp;
        for (int num : nums)
            mp[num]++;
        int n = mp.size();

        priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;  //> >之间一定要有空格，不然就成了右移运算符
        for (auto iter = mp.begin(); iter != mp.end(); iter++) {
            if (pq.size() == k) {   //队列已满
                if (iter->second > pq.top().first) {  //iter->second和pq.top().first都是数字的个数
                    pq.pop();
                    pq.push(make_pair(iter->second, iter->first));
                }
            }
            else {
                pq.push(make_pair(iter->second, iter->first));
            }
        }

        vector<int> result;
        while (!pq.empty()) {
            result.push_back(pq.top().second);
            pq.pop();
        }
        return result;
        
    }
};
````
## 215. 数组中的第K个最大元素
### 快速排序法
````cpp
class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        int low = 0, high = nums.size() - 1, mid = 0;
        while (low <= high) {
            mid = partition(nums, low, high);
            if (mid == nums.size() - k)     //第k大的元素，在数组中的索引为nums.size()-k
                return nums[mid];
            else if(mid > nums.size() - k)
                high = mid - 1;
            else
                low = mid + 1;
        }
        return -1;
    }

    int partition(vector<int> &nums, int low, int high) {
        int pivotkey = nums[(low + high)/2];    //选择位于中间的数作为基数
        swap(nums[low], nums[(low + high)/2]);  //选好后，和最左边的数交换一下
        while (low < high) {
            while (low < high && nums[high] >= pivotkey)
                high--;
            swap(nums[low], nums[high]);
            while (low < high && nums[low] <= pivotkey)
                low++;
            swap(nums[low], nums[high]);
        }
        return low;
    }
};
````
