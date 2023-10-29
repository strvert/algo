# memorize.cpp

全自動メモ化再帰。 Memo を派生して body に再帰処理を書くだけで引数のセットに対してメモ化が行われる。
再帰呼び出しでは関数名の変わりに self() を呼ぶ。

## Example
https://atcoder.jp/contests/dp/tasks/dp_a

```cpp
struct Solve : Memo<Solve, i64(i64)> {
    Solve(const std::vector<i64>& hn)
        : hn(hn)
    {
    }
    i64 body(i64 i)
    {
        if (i == 0) {
            return 0;
        } else if (i == 1) {
            return cost(hn[0], hn[1]);
        }
        return std::min(self(i - 1) + cost(hn[i - 1], hn[i]), self(i - 2) + cost(hn[i - 2], hn[i]));
    }

    const std::vector<i64>& hn;
};

int main()
{
    const auto n = /* 足場の数 */;
    const auto hn = /* 足場の高さ */;
    const auto result = Solve(hn)(n - 1);
}
```
