    .text
main:
    addi $t0, $zero, 4      # 设置t0 = 4
    addi $t1, $zero, 10     # 设置t1 = 10
    slt  $t2, $t0, $t1      # 如果 $t0 < $t1，则$t2 = 1，否则$t2 = 0
    li   $v0, 0xa           # 系统调用，退出
    syscall