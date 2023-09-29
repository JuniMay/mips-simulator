    .text
main:
    addi $t0, $zero, 15     # 设置t0 = 15
    addi $t1, $zero, 3      # 设置t1 = 3
    div $t0, $t1            # $t0 / $t1
    mflo $t2                # $t2 = 商
    mfhi $t3                # $t3 = 余数
    li   $v0, 0xa           # 系统调用，退出
    syscall