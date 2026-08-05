// 题目: 可获得的最大点数
// 题号: 1538
// 语言: c
// 提交时间: 2026-08-04 08:35:16

#define min(a, b) ((a < b) ? a : b)

int maxScore(int* cardPoints, int cardPointsSize, int k) {
    //逆向思维
    int n = cardPointsSize;
    int ans = 1000000000; //因为求最小值，所以ans一开始应该设置成最大值
    int sum = 0;
    int Sum = 0;
    //特殊情况：所有的牌都被拿起(k == cardPointsSize)
    //先求总点数
    for(int i = 0; i < n; i++){
        Sum += cardPoints[i];
    }
    if(k == n) {
        ans = 0;
    }else{
        for(int i = 0; i < n; i++) {
            //1
            sum += cardPoints[i];

            int left = i - (n - k) + 1;
            if(left < 0) {
                continue;
            }

            //2
            ans = min(ans, sum);

            //3
            sum -= cardPoints[left];
        }
    }

    printf("%d %d\n", Sum, ans);
    return Sum - ans;
}