# netlinkを使ったアプリの流れ
ソケットから送信して、その後同じソケットからlistenしているものが多い

# 送信
ソケットを開いて、データとなるバイナリを送る
## nlmsghdr
データの先頭にはまずnlmsghdr構造体が入る
構成要素としては
-     __u32 nlmsg_len;    /* ヘッダーを含むメッセージの長さ */
-     __u16 nlmsg_type;   /* メッセージの内容のタイプ */
-     __u16 nlmsg_flags;  /* 追加フラグ */
-     __u32 nlmsg_seq;    /* シーケンス番号 */
-     __u32 nlmsg_pid;    /* 送信者のポート ID */ }; 
seqはアプリ側で管理(好きに使って良い)
pidは宛先がkernelのときは0
<br>
バイトストリームには、標準の NLMSG_* マクロでアクセスする

# タイプ別ヘッダ
rtmsgとかifaddrmsgとかの構造体。
<br>
nlmsghdrのtypeが何かによって、ここに入る構造体が変わってくる。


# RTNETLINK
カーネルのルーティングテーブルの読み書き等のためのメッセージ。
socketの第三引数にNETLINK_ROUTEと入れれば、このメッセージが送れる。

## rtattr
メッセージタイプによって変わる。
要は、RTNETLINKで送りたい情報の中身(経路追加の場合はIPアドレスとか)。
RTA_*マクロによってデータを操作する。
rtaヘッダのあとに、そのtypeに合わせてrta_dataを配置する。(配置とかも全部マクロがやってくれる)
<br>
rtattrが配置される場所はタイプ別ヘッダの後ろ。
また、rtattrは複数並べて使える。

# sockaddr_nl構造体
sendto(),recvfrom()などの引数となるsockaddrとして、sockaddr_nl構造体が利用される