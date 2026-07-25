# LeetCode CN Sync

> 🔄 自动将力扣（LeetCode 中文站）的提交记录同步到 GitHub 仓库

[![GitHub Workflow Status](https://img.shields.io/github/actions/workflow/status/Liuliu0917/leetcode/.github%2Fworkflows%2Fleetcode-sync.yml?label=sync&logo=leetcode)](https://github.com/Liuliu0917/leetcode/actions)
[![License: MIT](https://img.shields.io/badge/license-MIT-green.svg)](./LICENSE)

## ✨ 特点

- 🎯 **专为力扣中文站（leetcode.cn）设计** — 现有开源方案大多只支持国际站
- 🐍 **纯 Python 实现** — 零依赖第三方 Action，透明可控
- ⏰ **每日自动同步** — GitHub Actions 定时运行，无需手动操作
- 📝 **完整代码存档** — 下载解题代码原文，标注题号、语言、提交时间
- 🔁 **增量同步** — 已同步的题目不会重复下载

## 🚀 快速开始（3 分钟）

### 第 1 步：Fork 或创建新仓库

点击右上角 **Fork** 本仓库，或者按以下结构创建新仓库：

```
your-repo/
├── .github/workflows/leetcode-sync.yml
├── sync_leetcode.py
├── .gitignore
└── README.md
```

> 💡 如果想从零开始，只需复制 `.github/workflows/leetcode-sync.yml` 和 `sync_leetcode.py` 两个文件即可。

### 第 2 步：获取 LeetCode Cookies

1. 在浏览器中登录 [力扣（leetcode.cn）](https://leetcode.cn)
2. 按 `F12` 打开开发者工具
3. 切换到 **Application**（应用程序）标签页
   - 如果找不到该标签，看标签栏末尾是否有 `»` 箭头，点击展开
   - 或者用 **Network**（网络）标签：刷新页面后点任意请求，在 Request Headers 里找 Cookie
4. 左侧菜单 **Cookies** → `leetcode.cn`
5. 复制以下两个值：

| Cookie 名称 | 示例 | 说明 |
|-------------|------|------|
| `LEETCODE_SESSION` | `eyJ0eXAiOiJKV...`（一串长字符） | 登录会话凭证 |
| `csrftoken` | `xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx`（32位） | CSRF 安全令牌 |

### 第 3 步：配置 GitHub Secrets

1. 进入仓库 **Settings** → **Secrets and variables** → **Actions**
2. 点击 **New repository secret**，分别添加：

| Name | Value |
|------|-------|
| `LEETCODE_SESSION` | 粘贴上一步复制的 `LEETCODE_SESSION` 值 |
| `LEETCODE_CSRF_TOKEN` | 粘贴上一步复制的 `csrftoken` 值 |

### 第 4 步：启用并运行

1. 确保 **Settings** → **Actions** → **General** → Workflow permissions 选择 **Read and write permissions**
2. 进入 **Actions** 标签 → 点击 **Sync LeetCode Submissions**
3. 点击 **Run workflow** → **Run workflow** 手动触发首次同步

稍等片刻，你的 `solutions/` 目录下就会出现 LeetCode 提交代码了！

## 📂 目录结构

提交同步后，仓库的 `solutions/` 目录会自动生成：

```
solutions/
├── .synced_ids.json          # 已同步记录（避免重复）
├── 1-two-sum.c               # 1. 两数之和
├── 2383-add-two-integers.cpp
├── 2491-smallest-even-multiple.c
└── ...
```

每个文件顶部自动添加：

```c
// 题目: 两数之和
// 题号: 1
// 语言: c
// 提交时间: 2026-07-26 10:30:00

/* 原始代码... */
```

## ⚙️ 自定义配置

### 修改同步频率

编辑 `.github/workflows/leetcode-sync.yml` 中的 `cron` 表达式：

```yaml
on:
  schedule:
    - cron: '0 8 * * *'   # 每天 UTC 8:00（北京时间 16:00）
```

| Cron 表达式 | 说明 |
|------------|------|
| `0 */6 * * *` | 每 6 小时 |
| `0 0,12 * * *` | 每天 0 点和 12 点（UTC） |
| `30 2 * * *` | 每天 UTC 2:30（北京时间 10:30） |

### 修改代码存放目录

编辑 `.github/workflows/leetcode-sync.yml`，添加环境变量：

```yaml
- name: Sync LeetCode submissions
  env:
    LEETCODE_SESSION: ${{ secrets.LEETCODE_SESSION }}
    LEETCODE_CSRF_TOKEN: ${{ secrets.LEETCODE_CSRF_TOKEN }}
    OUTPUT_DIR: my-solutions   # 自定义输出目录
```

## 🔧 工作原理

```
┌─────────────────┐     ┌──────────────────┐     ┌─────────────────┐
│  GitHub Actions  │────▶│  sync_leetcode   │────▶│  leetcode.cn    │
│  定时/手动触发    │     │  .py (Python)    │     │  GraphQL API    │
└─────────────────┘     └────────┬─────────┘     └─────────────────┘
                                 │
                                 │ 下载代码
                                 ▼
                        ┌─────────────────┐
                        │  solutions/     │
                        │  保存 + git push │
                        └─────────────────┘
```

1. GitHub Actions 定时（或手动）触发 Workflow
2. `sync_leetcode.py` 通过 leetcode.cn 的 GraphQL API 获取最近 Accepted 提交
3. 调用 `submissionDetail` 查询获取每道题的完整代码
4. 将代码按 `题号-题目.{ext}` 格式保存到 `solutions/` 目录
5. 自动 `git commit` 和 `git push` 推送到仓库

### LeetCode CN API 踩坑笔记

| API 字段 | 是否可用 | 说明 |
|----------|---------|------|
| `recentAcSubmissionList` | ❌ | leetcode.com 专用，cn 站不存在 |
| `recentSubmissionList` | ❌ | leetcode.com 专用，cn 站不存在 |
| `submissionList` | ✅ | cn 站正确字段，无需 userSlug 参数 |
| `submissionDetail` | ✅ | `submissionId` 类型为 `ID!`（非 `Int!`）；`lang` 为 String（非对象） |

## ❓ 常见问题

### Cookie 过期了怎么办？

LEETCODE_SESSION 通常**几周到几个月**后会过期。同步失败时：

1. 重新登录 leetcode.cn → F12 → 获取新的 Cookie
2. 去 Settings → Secrets and variables → Actions → 更新对应 Secret

### 国际站（leetcode.com）能用吗？

本方案专为 leetcode.cn 设计。国际站用户建议使用 [LeetSync Chrome 扩展](https://chromewebstore.google.com/detail/leetsync-leetcode-to-gith/ppkbejeolfcbaomanmbpjdbkfcjfhjnd)。

### 想同步所有历史提交怎么办？

修改 `sync_leetcode.py` 中 `get_all_submissions` 的 `limit` 参数，或者循环分页获取 `hasNext` 为 true 为止。

## 📄 License

MIT © [Liuliu0917](https://github.com/Liuliu0917)

---

⭐ 如果这个项目对你有帮助，欢迎 Star！
