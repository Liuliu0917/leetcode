"""
LeetCode CN 提交同步脚本 v4 - 自动探测正确的 API 字段
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
        return {"_error": f"HTTP {resp.status_code}", "_body": resp.text[:300]}
    data = resp.json()
    if "errors" in data:
        return {"_error": str(data["errors"])}
    return data.get("data", {})


def try_fields(session, csrf, user_slug):
    """尝试多种可能的 GraphQL 查询，找到可用的提交列表字段"""
    tests = [
        # 方案 A: submitted 页面使用的查询
        {
            "name": "submissionList (offset/limit)",
            "query": """
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
            """,
            "vars": {"offset": 0, "limit": 5},
        },
        # 方案 B: userProfileQuestions
        {
            "name": "recentSubmissionList (userSlug)",
            "query": """
            query recentSubmissionList($userSlug: String!) {
              recentSubmissionList(userSlug: $userSlug) {
                id
                title
                titleSlug
                timestamp
                statusDisplay
                lang
              }
            }
            """,
            "vars": {"userSlug": user_slug},
        },
        # 方案 C: progress 页面使用的
        {
            "name": "progressSubmissionList",
            "query": """
            query progressSubmissionList($userSlug: String!) {
              progressSubmissionList(userSlug: $userSlug, limit: 5, offset: 0) {
                submissions {
                  id
                  title
                  titleSlug
                  timestamp
                  statusDisplay
                  lang
                }
              }
            }
            """,
            "vars": {"userSlug": user_slug},
        },
        # 方案 D: 不带参数
        {
            "name": "submissionList (no args)",
            "query": """
            query {
              submissionList(offset: 0, limit: 5) {
                submissions {
                  id title titleSlug timestamp statusDisplay lang
                }
                hasNext
              }
            }
            """,
            "vars": {},
        },
        # 方案 E: 直接从 userStatus 里获取
        {
            "name": "userPublicProfile + recentSubmission",
            "query": """
            query userPublicProfile($userSlug: String!) {
              userProfilePublicProfile(userSlug: $userSlug) {
                submissionProgress {
                  totalSubmissions
                }
              }
            }
            """,
            "vars": {"userSlug": user_slug},
        },
    ]

    for test in tests:
        print(f"  尝试: {test['name']}...", end=" ")
        result = graphql_request(session, csrf, test["query"], test["vars"])
        if "_error" in result:
            print(f"❌ {result['_error'][:100]}")
        else:
            print(f"✓ 返回: {json.dumps(result, ensure_ascii=False)[:150]}")
        time.sleep(0.5)

    return []


def get_user_slug(session: str, csrf: str) -> str:
    query = """
    query globalData {
      userStatus {
        userSlug
        isSignedIn
        username
      }
    }
    """
    data = graphql_request(session, csrf, query, {})
    user_status = data.get("userStatus", {})
    if not user_status or not user_status.get("isSignedIn"):
        print("错误: 未登录或 Cookie 已过期", file=sys.stderr)
        sys.exit(1)
    slug = user_status.get("userSlug", "")
    print(f"  已登录: {user_status.get('username')} (slug: {slug})")
    return slug


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

    print("=== LeetCode CN 同步 v4 ===")

    # 1. 验证登录
    print("\n[1] 验证登录...")
    user_slug = get_user_slug(session, csrf)

    # 2. 探测可用 API
    print("\n[2] 探测可用 API 字段...")
    submissions = try_fields(session, csrf, user_slug)

    print(f"\n获取到 {len(submissions)} 条记录")
    print("\n=== 诊断完成 ===")


if __name__ == "__main__":
    main()
