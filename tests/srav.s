    .text
main:
    addi $t0, $zero, 2      # 设置t0 = 2
    addi $t1, $zero, -16    # 设置t1 = -16
    srav $t2, $t1, $t0      # $t2 = $t1 >> $t0
    li   $v0, 0xa           # 系统调用，退出
    syscall