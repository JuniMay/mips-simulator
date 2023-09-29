    .text
main:
    addi $t0, $zero, 3      # 设置t0 = 3
    addi $t1, $zero, 4      # 设置t1 = 4
    sllv $t2, $t1, $t0      # $t2 = $t1 << $t0
    li   $v0, 0xa           # 系统调用，退出
    syscall