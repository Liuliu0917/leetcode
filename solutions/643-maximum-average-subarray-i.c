// 题目: 子数组最大平均数 I
// 题号: 643
// 语言: c
// 提交时间: 2026-08-02 07:44:21

#define Max(a, b) ((a > b) ? (a) : (b))

double findMaxAverage(int* nums, int numsSize, int k) {
    double sum = -100000 * 10000, temp = 0.0;
    for(int i = 0; i < numsSize; i++){ //i代表右端点
        //1
        temp += nums[i];
        int left = i - k + 1;
        if(left < 0){
            continue;
        }
        
        //2
        sum = Max(sum, temp);
        
        //3
        temp -= nums[left];
    }
    
    double ave = sum / k;
    return ave;
}