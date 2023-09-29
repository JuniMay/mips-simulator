    .text
main:
    addi $t0, $zero, 4          # 设置$t0 = 4
    addi $t1, $zero, 10         # 设置$t1 = 10
    sltu $t2, $t0, $t1          # 如果 $t0 < $t1 (无符号比较)，则$t2 = 1，否则$t2 = 0
    li   $v0, 0xa               # 系统调用，退出
    syscall