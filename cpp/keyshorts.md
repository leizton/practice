# mac
## mac升级后
vi ~/.ssh/config
Host *
  Ciphers aes256-ctr
  KexAlgorithms +diffie-hellman-group1-sha1
  PubkeyAcceptedKeyTypes +ssh-rsa
  HostkeyAlgorithms +ssh-rsa
ControlMaster auto
ControlPath ~/.ssh/master-%r@%h:%p

# arc
cmd+T “dup”  copy当前tab出新tab
cmd+k  切换tab
cmd+s  隐藏左边栏
cmd+shift+L  split view
cmd+[  go back
cmd+Y  通过历史恢复关闭的tab
cmd+shift+X  恢复最后一个关闭的tab
shift+点击链接  在当前标签页打开链接

# notion
行内公式  实现公式左对齐
[[  引用其他page
((  引用page里的行