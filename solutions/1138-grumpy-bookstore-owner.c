// 题目: 爱生气的书店老板
// 题号: 1138
// 语言: c
// 提交时间: 2026-08-06 04:06:33

#define max(a, b) ((a > b) ? a : b)

int maxSatisfied(int* customers, int customersSize, int* grumpy, int grumpySize, int minutes) {
    int n = customersSize;
    //算不抑制情绪满意总数
    int Satisfied = 0;
    for(int i = 0; i < n; i++) {
        if(grumpy[i] == 0) {
            Satisfied += customers[i];
        }
    }
    //再算抑制情绪时增加满意人数最大值（定长滑动窗口）
    int addSatisfied = 0;
    int temp = 0;
    for(int i = 0; i < n; i++) {
        //1
        if(grumpy[i] == 1) {
            temp += customers[i];
        }
        int left = i - minutes + 1;
        if(left < 0) {
            continue;
        }

        //2
        addSatisfied = max(addSatisfied, temp);

        //3
        if(grumpy[left] == 1) {
            temp -= customers[left];
        }
    }
    //最终最大值为 Satisfied + addSatisfied
    return Satisfied + addSatisfied;
}