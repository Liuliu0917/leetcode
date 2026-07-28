// 题目: 山脉数组的峰顶索引
// 题号: 882
// 语言: c
// 提交时间: 2026-07-28 12:30:44

int peakIndexInMountainArray(int* arr, int arrSize) {
    /*二分查找的核心思想：
    比较 arr[mid] 和 arr[mid + 1]
    如果 arr[mid] > arr[mid + 1]，说明 mid 在下降段（峰顶在 mid 或左侧），所以记录right = mid - 1 继续往左找
    如果 arr[mid] <= arr[mid + 1]，说明 mid 在上升段（峰顶在右侧），所以 left = mid + 1*/

    int n = arrSize;
    int left = 1, right = n - 2, ans = 0;
    int mid; 
    //3 <= arr.length
    //因为峰顶不可能在索引0（必须严格递增到峰顶），也不可能在索引 n-1（必须严格递减）,所以搜索范围缩小到 [1, n-2]
    while(left <= right) {
        mid = (left + right) / 2;
        if(arr[mid] > arr[mid + 1]) {
            ans = mid; //※ans 记录的是"目前找到的最靠左的候选峰顶"
            right = mid - 1;
        } else {
            left = mid + 1;
        }
    }
    return ans;
}