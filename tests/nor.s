    .text
main:
    addi $t0, $zero, 0xc        # 设置$t0 = 12 (二进制: 1100)
    addi $t1, $zero, 0xa        # 设置$t1 = 10 (二进制: 1010)
    nor  $t2, $t0, $t1          # $t2 = ~($t0 | $t1)
    li   $v0, 0xa               # 系统调用，退出
    syscall