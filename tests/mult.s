    .text
main:
    addi $t0, $zero, 5      # 设置t0 = 5
    addi $t1, $zero, 6      # 设置t1 = 6
    mult $t0, $t1           # HI, LO = $t0 * $t1
    mflo $t2                # $t2 = LO
    li   $v0, 0xa           # 系统调用，退出
    syscall