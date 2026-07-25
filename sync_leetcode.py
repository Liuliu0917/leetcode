"""
LeetCode CN 提交同步脚本 v5 - 最终版
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


def graphql_request(session: str, csrf: str, query: str, variables: dict) -> dict:
    cookies = {"LEETCODE_SESSION": session, "csrftoken": csrf}
    headers = {**HEADERS, "x-csrftoken": csrf}
    resp = requests.post(LEETCODE_GRAPHQL, json={"query": query, "variables": variables},
                         headers=headers, cookies=cookies, timeout=30)
    if resp.status_code != 200:
        print(f"    HTTP {resp.status_code}: {resp.text[:300]}", file=sys.stderr)
        resp.raise_for_status()
    data = resp.json()
    if "errors" in data:
        print(f"    GraphQL 错误: {data['errors']}", file=sys.stderr)
        return {}
    return data.get("data", {})


def verify_login(session: str, csrf: str) -> str:
    data = graphql_request(session, csrf, """
    query globalData {
      userStatus { userSlug isSignedIn username }
    }
    """, {})
    u = data.get("userStatus", {})
    if not u or not u.get("isSignedIn"):
        print("错误: 未登录或 Cookie 已过期", file=sys.stderr)
        sys.exit(1)
    print(f"  已登录: {u.get('username')}")
    return u.get("userSlug", "")


def get_all_submissions(session: str, csrf: str, limit: int = 20) -> list:
    """获取最近的 Accepted 提交"""
    query = """
    query submissionList($offset: Int!, $limit: Int!) {
      submissionList(offset: $offset, limit: $limit) {
        submissions {
          id
          title
          titleSlug
          timestamp
          statusDisplay
          lang
        }
        hasNext
      }
    }
    """
    data = graphql_request(session, csrf, query, {"offset": 0, "limit": limit})
    sl = data.get("submissionList", {})
    subs = sl.get("submissions", [])
    return [s for s in subs if s.get("statusDisplay") == "Accepted"]


def get_submission_code(session: str, csrf: str, submission_id: int) -> dict | None:
    query = """
    query submissionDetails($submissionId: Int!) {
      submissionDetail(submissionId: $submissionId) {
        code
        lang { name }
        question { questionId title titleSlug translatedTitle }
      }
    }
    """
    data = graphql_request(session, csrf, query, {"submissionId": submission_id})
    return data.get("submissionDetail")


def load_synced_ids(sync_file: Path) -> set:
    if sync_file.exists():
        return set(json.loads(sync_file.read_text(encoding="utf-8")))
    return set()


def save_synced_ids(sync_file: Path, ids: set):
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
        print("错误: 缺少环境变量", file=sys.stderr)
        sys.exit(1)

    print("=== LeetCode CN 同步 v5 ===")

    print("\n[1] 验证登录...")
    user_slug = verify_login(session, csrf)

    synced_ids = load_synced_ids(sync_file)
    print(f"[2] 已同步 {len(synced_ids)} 条记录")

    print("\n[3] 获取最近提交...")
    submissions = get_all_submissions(session, csrf)
    print(f"    获取到 {len(submissions)} 条 Accepted 提交")

    print("\n[4] 下载解题代码...")
    new_count = 0

    for sub in submissions:
        sub_id = sub["id"]
        if str(sub_id) in synced_ids:
            continue

        title = sub["title"]
        lang = sub.get("lang", "unknown")
        ext = LANG_EXT.get(lang.lower(), lang.lower())
        print(f"    [{lang}] {title}")

        detail = get_submission_code(session, csrf, int(sub_id))
        if not detail or not detail.get("code"):
            print(f"      ⚠ 无法获取代码，跳过")
            continue

        code = detail["code"]
        q = detail.get("question", {}) or {}
        question_id = q.get("questionId", "0000")
        problem_title = q.get("translatedTitle") or title

        filename = sanitize_filename(f"{question_id}-{sub['titleSlug']}.{ext}")
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
        time.sleep(1)

    save_synced_ids(sync_file, synced_ids)
    print(f"\n=== 同步完成: 新增 {new_count} 条 ===")


if __name__ == "__main__":
    main()
