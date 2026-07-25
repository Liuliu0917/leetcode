"""
LeetCode CN 提交同步脚本 v2
通过 leetcode.cn API 获取提交记录并保存到本地文件
"""
import json
import os
import sys
import time
from pathlib import Path

import requests

LEETCODE_GRAPHQL = "https://leetcode.cn/graphql"

HEADERS = {
    "Content-Type": "application/json",
    "User-Agent": "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/131.0.0.0 Safari/537.36",
    "Accept": "*/*",
    "Accept-Language": "zh-CN,zh;q=0.9,en;q=0.8",
    "Origin": "https://leetcode.cn",
    "Referer": "https://leetcode.cn/",
}

LANG_EXT = {
    "cpp": "cpp", "c++": "cpp", "java": "java", "python": "py",
    "python3": "py", "c": "c", "c#": "cs", "javascript": "js",
    "typescript": "ts", "ruby": "rb", "swift": "swift", "golang": "go",
    "go": "go", "scala": "scala", "kotlin": "kt", "rust": "rs",
    "php": "php", "racket": "rkt", "erlang": "erl", "elixir": "ex",
}


def make_request(session: str, csrf: str, query: str, variables: dict, operation_name: str = "") -> dict:
    """发送 GraphQL 请求到 leetcode.cn"""
    cookies = {"LEETCODE_SESSION": session, "csrftoken": csrf}
    headers = {**HEADERS, "x-csrftoken": csrf}

    payload = {"query": query, "variables": variables}
    if operation_name:
        payload["operationName"] = operation_name

    print(f"    请求 operation: {operation_name or '(无)'}")

    resp = requests.post(
        LEETCODE_GRAPHQL,
        json=payload,
        headers=headers,
        cookies=cookies,
        timeout=30,
    )

    print(f"    状态码: {resp.status_code}")

    if resp.status_code != 200:
        print(f"    响应体: {resp.text[:500]}", file=sys.stderr)

    resp.raise_for_status()
    data = resp.json()

    if "errors" in data:
        print(f"    GraphQL 错误: {data['errors']}", file=sys.stderr)
        return {}

    return data.get("data", {})


def get_user_slug(session: str, csrf: str) -> str:
    """获取当前用户的 userSlug"""
    query = """
    query globalData {
      userStatus {
        userSlug
        isSignedIn
        username
      }
    }
    """
    data = make_request(session, csrf, query, {}, "globalData")
    user_status = data.get("userStatus", {})
    if not user_status or not user_status.get("isSignedIn"):
        print("错误: 未登录或 Cookie 已过期，请重新获取", file=sys.stderr)
        sys.exit(1)

    slug = user_status.get("userSlug", "")
    username = user_status.get("username", "")
    print(f"  已登录: {username} (slug: {slug})")
    return slug


def get_recent_ac_submissions(session: str, csrf: str, user_slug: str, limit: int = 50) -> list:
    """获取最近的 Accepted 提交记录"""
    query = """
    query recentAcSubmissions($userSlug: String!, $limit: Int!) {
      recentAcSubmissionList(userSlug: $userSlug, limit: $limit) {
        id
        title
        titleSlug
        timestamp
        lang
      }
    }
    """
    data = make_request(
        session, csrf, query,
        {"userSlug": user_slug, "limit": limit},
        "recentAcSubmissions",
    )
    return data.get("recentAcSubmissionList", [])


def get_submission_code(session: str, csrf: str, submission_id: int) -> dict | None:
    """获取某次提交的代码详情"""
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
    data = make_request(
        session, csrf, query,
        {"submissionId": submission_id},
        "submissionDetails",
    )
    detail = data.get("submissionDetail")
    return detail


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
    return name.replace("/", "-").replace("\\", "-").replace(":", "-").strip()


def main():
    session = os.environ.get("LEETCODE_SESSION", "").strip()
    csrf = os.environ.get("LEETCODE_CSRF_TOKEN", "").strip()
    output_dir = Path(os.environ.get("OUTPUT_DIR", "solutions"))
    sync_file = output_dir / ".synced_ids.json"

    if not session or not csrf:
        print("错误: 请设置 LEETCODE_SESSION 和 LEETCODE_CSRF_TOKEN 环境变量", file=sys.stderr)
        sys.exit(1)

    print("=== LeetCode CN 同步开始 ===")
    print(f"Session 长度: {len(session)}, CSRF 长度: {len(csrf)}")

    # 1. 获取用户 slug
    print("\n[1] 验证登录状态...")
    user_slug = get_user_slug(session, csrf)

    # 2. 加载已同步记录
    synced_ids = load_synced_ids(sync_file)
    print(f"\n[2] 已同步 {len(synced_ids)} 条记录")

    # 3. 获取最近 Accepted 提交
    print(f"\n[3] 获取最近提交记录...")
    submissions = get_recent_ac_submissions(session, csrf, user_slug)
    print(f"    获取到 {len(submissions)} 条 Accepted 提交")

    # 4. 下载代码
    print(f"\n[4] 下载解题代码...")
    new_count = 0

    for sub in submissions:
        sub_id = sub["id"]
        if str(sub_id) in synced_ids:
            continue

        title = sub["title"]
        title_slug = sub["titleSlug"]
        lang = sub.get("lang", "unknown")
        ext = LANG_EXT.get(lang.lower(), lang.lower())

        print(f"    [{lang}] {title}")

        detail = get_submission_code(session, csrf, int(sub_id))
        if not detail or not detail.get("code"):
            print(f"      ⚠ 无法获取代码", file=sys.stderr)
            continue

        code = detail["code"]
        question = detail.get("question", {})
        question_id = question.get("questionId", "0000")
        problem_title = question.get("translatedTitle") or title

        filename = sanitize_filename(f"{question_id}-{title_slug}.{ext}")
        filepath = output_dir / filename
        filepath.parent.mkdir(parents=True, exist_ok=True)

        header = (
            f"// 题目: {problem_title}\n"
            f"// 题号: {question_id}\n"
            f"// 语言: {lang}\n"
            f"// 提交时间: {time.strftime('%Y-%m-%d %H:%M:%S', time.localtime(sub['timestamp']))}\n\n"
        )

        filepath.write_text(header + code, encoding="utf-8")
        synced_ids.add(str(sub_id))
        new_count += 1
        print(f"      ✓ → {filename}")

        time.sleep(1)  # 避免请求过快

    # 5. 保存同步状态
    save_synced_ids(sync_file, synced_ids)
    print(f"\n=== 同步完成: 新增 {new_count} 条 ===")


if __name__ == "__main__":
    main()
