// 题目: 大小为 K 且平均值大于等于阈值的子数组数目
// 题号: 1445
// 语言: c
// 提交时间: 2026-08-02 08:10:39

int numOfSubarrays(int* arr, int arrSize, int k, int threshold) {
    int num = 0;
    double sum = 0.0;
    for(int i = 0; i < arrSize; i++) { // right == i
        //1
        sum += arr[i];
        int left = i - k + 1;
        if(left < 0) {
            continue;
        }

        //2
        double ave = sum / k;
        if(ave >= threshold) {
            num++;
        }

        //3
        sum -= arr[left];
    }
    return num;
}