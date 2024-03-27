# 課題メモ
与えられたバイナリファイルは
- 選択ソート
- バブルソート
- マージソート
のいずれか。
それぞれの特徴としては

- 選択ソート
    - O(n^2)
    - i番目から末尾まで線形に探索して一番小さい要素をi番目にもってくる
    - 毎回i番目から末尾までを探索するので一番遅い気がする
- バブルソート
    - O(n^2)
    - 0番目からi番目までで一番大きい要素がi番目に移動される
    - スワップの回数分選択ソートよりも遅くなりがち（らしい）
- マージソート
    - 他の2つに比べて高速

# 結果
timeコマンドとともに実行してみて`user`の値を比べれば各sortの速さの比較ができそう。

```sh
 time ./sort1 random50000.txt
 # real    0m5.184s
 # user    0m5.117s
 # sys     0m0.001s
 ```
```sh
 time ./sort2 random50000.txt
 # real    0m0.702s
 # user    0m0.023s
 # sys     0m0.291s
 ```
```sh
 time ./sort3 random50000.txt
 # real    0m2.345s
 # user    0m1.745s
 # sys     0m0.275s
 ```

 ということでsort2 -> sort3 -> sort1の順に早かったので
sort1: バブルソート
sort2: マージソート
sort3: 選択ソート
だと思われる。

また、選択ソートはソート済みのテキストに対してもO(n^2)の計算量が必要。
実際にsort1(Bubble sort)とsort3(Selection sort)を比較すると3の方が遅い。

```sh
lab3/ $ time ./sort1 sorted50000.txt 
# real    0m0.763s
# user    0m0.023s
# sys     0m0.282s
```

```sh
lab3/ $ time ./sort3 sorted50000.txt 
# real    0m2.468s
# user    0m1.860s
# sys     0m0.287s
```