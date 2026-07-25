# LeetCode Sync

自动将 LeetCode 提交记录同步到此 GitHub 仓库。

## 工作原理

使用 [LeetCode Sync](https://github.com/joshcai/leetcode-sync) GitHub Action，每天自动将 LeetCode 的提交记录同步到本仓库。

## 设置步骤

### 1. 获取 LeetCode Cookies

1. 在浏览器中登录 [LeetCode](https://leetcode.com)（国际站）或 [LeetCode 中文站](https://leetcode.cn)
2. 打开浏览器开发者工具（F12）
3. 切换到 **Application**（应用程序）标签
4. 在左侧菜单找到 **Cookies** → 选择 LeetCode 域名
5. 找到并复制以下两个 Cookie 值：

| Cookie 名称 | 用途 |
|------------|------|
| `LEETCODE_SESSION` | 你的登录会话 |
| `csrftoken` | CSRF 安全令牌 |

### 2. 配置 GitHub Secrets

在 GitHub 仓库页面：
1. 进入 **Settings** → **Secrets and variables** → **Actions**
2. 点击 **New repository secret**
3. 添加以下两个 Secrets：

| Secret 名称 | 值（粘贴你在上一步复制的 Cookie） |
|-------------|--------------------------------|
| `LEETCODE_SESSION` | 你的 `LEETCODE_SESSION` Cookie 值 |
| `LEETCODE_CSRF_TOKEN` | 你的 `csrftoken` Cookie 值 |

### 3. 启用 Workflow

1. 进入仓库的 **Actions** 标签
2. 如果看到 "Workflows aren't being run…"，点击 **"I understand my workflows, go ahead and enable them"**
3. 可以点击 **"Sync LeetCode Submissions"** → **"Run workflow"** 手动触发第一次同步

### 4. 自动同步

配置完成后，GitHub Action 会在每天 UTC 8:00（北京时间 16:00）自动运行，也可以随时手动触发。

> **注意：** LeetCode Session Cookie 会过期（通常几周到几个月）。如果同步失败并出现 401 错误，需要重新获取 Cookie 并更新 GitHub Secrets。

## 项目结构

```
leetcode-sync/
├── .github/
│   └── workflows/
│       └── leetcode-sync.yml   # GitHub Action 工作流配置
├── README.md                    # 本说明文件
└── *.md / *.sql / *.java ...   # 同步的 LeetCode 提交文件（自动生成）
```

## 参考链接

- [LeetCode Sync GitHub Action](https://github.com/marketplace/actions/leetcode-sync)
- [源代码仓库](https://github.com/joshcai/leetcode-sync)
