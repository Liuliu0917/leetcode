"""
LeetCode CN 提交同步脚本
通过 leetcode.cn GraphQL API 获取提交记录并保存到本地文件
"""
import json
import os
import sys
import time
from pathlib import Path

import requests

LEETCODE_GRAPHQL = "https://leetcode.cn/graphql/"

HEADERS = {
    "Content-Type": "application/json",
    "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36",
    "Origin": "https://leetcode.cn",
    "Referer": "https://leetcode.cn/",
}

# 语言 → 文件扩展名映射
LANG_EXT = {
    "cpp": "cpp",
    "c++": "cpp",
    "java": "java",
    "python": "py",
    "python3": "py",
    "c": "c",
    "c#": "cs",
    "javascript": "js",
    "typescript": "ts",
    "ruby": "rb",
    "swift": "swift",
    "golang": "go",
    "go": "go",
    "scala": "scala",
    "kotlin": "kt",
    "rust": "rs",
    "php": "php",
    "racket": "rkt",
    "erlang": "erl",
    "elixir": "ex",
}


def get_recent_submissions(session: str, csrf: str, limit: int = 50) -> list:
    """获取最近的提交记录"""
    cookies = {"LEETCODE_SESSION": session, "csrftoken": csrf}
    headers = {**HEADERS, "x-csrftoken": csrf}

    query = """
    query recentSubmissions($userSlug: String!) {
      recentSubmissionList(userSlug: $userSlug) {
        id
        title
        titleSlug
        timestamp
        statusDisplay
        lang
      }
    }
    """

    payload = {"query": query, "variables": {"userSlug": ""}}

    resp = requests.post(
        LEETCODE_GRAPHQL, json=payload, headers=headers, cookies=cookies, timeout=30
    )
    resp.raise_for_status()
    data = resp.json()

    if "errors" in data:
        print(f"GraphQL 错误: {data['errors']}", file=sys.stderr)
        return []

    submissions = data.get("data", {}).get("recentSubmissionList", [])
    # 只保留 Accepted 的
    return [s for s in submissions if s.get("statusDisplay") == "Accepted"]


def get_submission_code(session: str, csrf: str, submission_id: int) -> dict | None:
    """获取某次提交的代码详情"""
    cookies = {"LEETCODE_SESSION": session, "csrftoken": csrf}
    headers = {**HEADERS, "x-csrftoken": csrf}

    query = """
    query submissionDetails($submissionId: Int!) {
      submissionDetail(submissionId: $submissionId) {
        code
        lang {
          name
        }
        question {
          questionId
          title
          titleSlug
          translatedTitle
        }
      }
    }
    """

    payload = {"query": query, "variables": {"submissionId": submission_id}}

    resp = requests.post(
        LEETCODE_GRAPHQL, json=payload, headers=headers, cookies=cookies, timeout=30
    )
    resp.raise_for_status()
    data = resp.json()

    if "errors" in data:
        print(f"获取代码错误 (id={submission_id}): {data['errors']}", file=sys.stderr)
        return None

    return data.get("data", {}).get("submissionDetail")


def load_synced_ids(sync_file: Path) -> set:
    """加载已同步的提交 ID"""
    if sync_file.exists():
        return set(json.loads(sync_file.read_text(encoding="utf-8")))
    return set()


def save_synced_ids(sync_file: Path, ids: set):
    """保存已同步的提交 ID"""
    sync_file.parent.mkdir(parents=True, exist_ok=True)
    sync_file.write_text(json.dumps(list(ids), ensure_ascii=False), encoding="utf-8")


def sanitize_filename(name: str) -> str:
    """清理文件名中的非法字符"""
    return name.replace("/", "-").replace("\\", "-").replace(":", "-").strip()


def main():
    session = os.environ.get("LEETCODE_SESSION", "").strip()
    csrf = os.environ.get("LEETCODE_CSRF_TOKEN", "").strip()
    output_dir = Path(os.environ.get("OUTPUT_DIR", "solutions"))
    sync_file = output_dir / ".synced_ids.json"

    if not session or not csrf:
        print("错误: 请设置 LEETCODE_SESSION 和 LEETCODE_CSRF_TOKEN 环境变量", file=sys.stderr)
        sys.exit(1)

    # 获取已同步的 ID
    synced_ids = load_synced_ids(sync_file)
    print(f"已同步 {len(synced_ids)} 条记录")

    # 获取最近提交
    print("正在获取最近的提交记录...")
    submissions = get_recent_submissions(session, csrf)
    print(f"获取到 {len(submissions)} 条 Accepted 提交")

    new_count = 0

    for sub in submissions:
        sub_id = sub["id"]
        if str(sub_id) in synced_ids:
            continue  # 已同步过，跳过

        title = sub["title"]
        title_slug = sub["titleSlug"]
        lang = sub["lang"]
        ext = LANG_EXT.get(lang.lower(), lang.lower())

        print(f"  获取解题代码: {title} ({lang})")

        detail = get_submission_code(session, csrf, int(sub_id))
        if not detail or not detail.get("code"):
            print(f"    跳过: 无法获取代码", file=sys.stderr)
            continue

        code = detail["code"]
        question = detail.get("question", {})
        question_id = question.get("questionId", "0000")
        problem_title = question.get("translatedTitle") or title

        # 文件名格式: 题号-题目名.扩展名
        filename = sanitize_filename(f"{question_id}-{title_slug}.{ext}")
        filepath = output_dir / filename
        filepath.parent.mkdir(parents=True, exist_ok=True)

        # 添加注释头
        header = f"// 题目: {problem_title}\n// 题号: {question_id}\n// 语言: {lang}\n// 提交时间: {time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(sub['timestamp']))}\n\n"

        filepath.write_text(header + code, encoding="utf-8")
        synced_ids.add(str(sub_id))
        new_count += 1
        print(f"    ✓ 已保存: {filepath}")

        # 避免请求过快
        time.sleep(1)

    # 保存同步状态
    save_synced_ids(sync_file, synced_ids)
    print(f"\n同步完成: 新增 {new_count} 条记录")

    if new_count == 0:
        print("没有新的提交需要同步")


if __name__ == "__main__":
    main()
