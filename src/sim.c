#include <stdio.h>

#include "shell.h"

uint32_t extract_op(uint32_t inst) { return inst >> 26; }

uint32_t extract_rs(uint32_t inst) { return (inst >> 21) & 0x1f; }

uint32_t extract_rt(uint32_t inst) { return (inst >> 16) & 0x1f; }

uint32_t extract_rd(uint32_t inst) { return (inst >> 11) & 0x1f; }

uint32_t extract_target(uint32_t inst) { return inst & 0x3ffffff; }

uint32_t extract_imm(uint32_t inst) { return inst & 0xffff; }

uint32_t extract_shamt(uint32_t inst) { return (inst >> 6) & 0x1f; }

uint32_t extract_funct(uint32_t inst) { return inst & 0x3f; }

/// Sign extend the 16 bits immediate
uint32_t sign_ext(uint32_t imm) {
    int32_t signed_imm = *((int16_t*)&imm);
    uint32_t extended_imm = *((uint32_t*)&signed_imm);
    return extended_imm;
}

/// Sign extend a byte to 32 bits
uint32_t sign_ext_byte(uint8_t imm) {
    int32_t signed_imm = *((int8_t*)&imm);
    uint32_t extended_imm = *((uint32_t*)&signed_imm);
    return extended_imm;
}

uint32_t sign_ext_half(uint16_t imm) {
    int32_t signed_imm = *((int16_t*)&imm);
    uint32_t extended_imm = *((uint32_t*)&signed_imm);
    return extended_imm;
}

uint32_t zero_ext(uint32_t imm) { return imm; }

uint32_t zero_ext_byte(uint8_t imm) { return imm; }

uint32_t zero_ext_half(uint16_t imm) { return imm; }

void process_instruction() {
    /* execute one instruction here. You should use CURRENT_STATE and modify
     * values in NEXT_STATE. You can call mem_read_32() and mem_write_32() to
     * access memory. */
    uint32_t inst = mem_read_32(CURRENT_STATE.PC);

    printf("Instruction: 0x%08x\n", inst);

    uint32_t op = extract_op(inst);
    uint32_t rs = extract_rs(inst);
    uint32_t rt = extract_rt(inst);
    uint32_t imm = extract_imm(inst);
    uint32_t rd = extract_rd(inst);
    uint32_t shamt = extract_shamt(inst);
    uint32_t funct = extract_funct(inst);

    switch (op) {
        case 0x0: {
            switch (funct) {
                case 0x0: {
                    // SLL
                    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] << shamt;
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;
                }
                case 0x2: {
                    // SRL
                    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] >> shamt;
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;
                }
                case 0x3: {
                    // SRA
                    int32_t val = *((int32_t*)&CURRENT_STATE.REGS[rt]);
                    val = val >> shamt;
                    NEXT_STATE.REGS[rd] = val;
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;
                }
                case 0x4: {
                    // SLLV
                    uint32_t shamt = CURRENT_STATE.REGS[rs] & 0x1f;
                    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] << shamt;
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;
                }
                case 0x6: {
                    // SRLV
                    uint32_t shamt = CURRENT_STATE.REGS[rs] & 0x1f;
                    NEXT_STATE.REGS[rd] = CURRENT_STATE.REGS[rt] >> shamt;
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;
                }
                case 0x7: {
                    // SRAV
                    int32_t val = *((int32_t*)&CURRENT_STATE.REGS[rt]);
                    uint32_t shamt = CURRENT_STATE.REGS[rs] & 0x1f;
                    val = val >> shamt;
                    NEXT_STATE.REGS[rd] = val;
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;
                }
                case 0x8: {
                    // JR
                    NEXT_STATE.PC = CURRENT_STATE.REGS[rs];
                    break;
                }
                case 0x9: {
                    // JALR
                    NEXT_STATE.REGS[rd] = CURRENT_STATE.PC + 4;
                    NEXT_STATE.PC = CURRENT_STATE.REGS[rs];
                    break;
                }
                case 0xc: {
                    // SYSCALL
                    if (CURRENT_STATE.REGS[2] == 0x0a) {
                        RUN_BIT = FALSE;
                    } else {
                        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    }
                    break;
                }
                case 0x10: {
                    // MFHI
                    NEXT_STATE.REGS[rd] = CURRENT_STATE.HI;
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;
                }
                case 0x11: {
                    // MTHI
                    NEXT_STATE.HI = CURRENT_STATE.REGS[rs];
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;
                }
                case 0x12: {
                    // MFLO
                    NEXT_STATE.REGS[rd] = CURRENT_STATE.LO;
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;
                }
                case 0x13: {
                    // MTLO
                    NEXT_STATE.LO = CURRENT_STATE.REGS[rs];
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;
                }
                case 0x18: {
                    // MULT
                    int64_t lhs = *((int32_t*)&CURRENT_STATE.REGS[rs]);
                    int64_t rhs = *((int32_t*)&CURRENT_STATE.REGS[rt]);
                    int64_t product = lhs * rhs;
                    uint64_t uint_product = *((uint32_t*)&product);
                    NEXT_STATE.HI =
                        (uint32_t)((uint_product >> 32) & 0xffffffff);
                    NEXT_STATE.LO = (uint32_t)(uint_product & 0xffffffff);
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;
                }
                case 0x19: {
                    // MULTH
                    uint64_t lhs = CURRENT_STATE.REGS[rs];
                    uint64_t rhs = CURRENT_STATE.REGS[rt];
                    uint64_t product = lhs * rhs;

                    NEXT_STATE.HI = (uint32_t)((product >> 32) & 0xffffffff);
                    NEXT_STATE.LO = (uint32_t)(product & 0xffffffff);
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;
                }
                case 0x1a: {
                    // DIV
                    int32_t lhs = *((int32_t*)&CURRENT_STATE.REGS[rs]);
                    int32_t rhs = *((int32_t*)&CURRENT_STATE.REGS[rt]);
                    NEXT_STATE.LO = lhs / rhs;
                    NEXT_STATE.HI = lhs % rhs;
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;
                }
                case 0x1b: {
                    // DIVU
                    uint32_t lhs = CURRENT_STATE.REGS[rs];
                    uint32_t rhs = CURRENT_STATE.REGS[rt];
                    NEXT_STATE.LO = lhs / rhs;
                    NEXT_STATE.HI = lhs % rhs;
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;
                }
                case 0x20: {
                    // ADD
                    NEXT_STATE.REGS[rd] =
                        CURRENT_STATE.REGS[rs] + CURRENT_STATE.REGS[rt];
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;
                }
                case 0x21: {
                    // ADDU
                    NEXT_STATE.REGS[rd] =
                        CURRENT_STATE.REGS[rs] + CURRENT_STATE.REGS[rt];
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;
                }
                case 0x22: {
                    // SUB
                    NEXT_STATE.REGS[rd] =
                        CURRENT_STATE.REGS[rs] - CURRENT_STATE.REGS[rt];
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;
                }
                case 0x23: {
                    // SUBU
                    NEXT_STATE.REGS[rd] =
                        CURRENT_STATE.REGS[rs] - CURRENT_STATE.REGS[rt];
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;
                }
                case 0x24: {
                    // AND
                    NEXT_STATE.REGS[rd] =
                        CURRENT_STATE.REGS[rs] & CURRENT_STATE.REGS[rt];
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;
                }
                case 0x25: {
                    // OR
                    NEXT_STATE.REGS[rd] =
                        CURRENT_STATE.REGS[rs] | CURRENT_STATE.REGS[rt];
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;
                }
                case 0x26: {
                    // XOR
                    NEXT_STATE.REGS[rd] =
                        CURRENT_STATE.REGS[rs] ^ CURRENT_STATE.REGS[rt];
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;
                }
                case 0x27: {
                    // NOR
                    NEXT_STATE.REGS[rd] =
                        ~(CURRENT_STATE.REGS[rs] | CURRENT_STATE.REGS[rt]);
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;
                }
                case 0x2a: {
                    // SLT
                    int32_t lhs = *((int32_t*)&CURRENT_STATE.REGS[rs]);
                    int32_t rhs = *((int32_t*)&CURRENT_STATE.REGS[rt]);
                    NEXT_STATE.REGS[rd] = (lhs < rhs) ? 1 : 0;
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;
                }
                case 0x2b: {
                    // SLTU
                    NEXT_STATE.REGS[rd] =
                        CURRENT_STATE.REGS[rs] < CURRENT_STATE.REGS[rt] ? 1 : 0;
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    break;
                }
                default: {
                    printf("Unknown instruction: 0x%x\n", inst);
                    break;
                }
            }
            break;
        }
        case 0x8: {
            // ADDI
            NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] + sign_ext(imm);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        }
        case 0x9: {
            // ADDIU
            NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] + sign_ext(imm);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        }
        case 0xc: {
            // ANDI
            NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] & zero_ext(imm);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        }
        case 0xd: {
            // ORI
            NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] | zero_ext(imm);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        }
        case 0xe: {
            // XORI
            NEXT_STATE.REGS[rt] = CURRENT_STATE.REGS[rs] ^ zero_ext(imm);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        }
        case 0x4: {
            // BEQ

            uint32_t offset = sign_ext(imm) << 2;

            if (CURRENT_STATE.REGS[rs] == CURRENT_STATE.REGS[rt]) {
                NEXT_STATE.PC = CURRENT_STATE.PC + offset + 4;
            } else {
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            }
            break;
        }
        case 0x1: {
            uint32_t offset = sign_ext(imm) << 2;

            switch (rt) {
                case 0x0: {
                    // BLTZ
                    if ((CURRENT_STATE.REGS[rs] & 0x80000000) != 0) {
                        NEXT_STATE.PC = CURRENT_STATE.PC + offset + 4;
                    } else {
                        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    }
                    break;
                }
                case 0x10: {
                    // BLTZAL
                    NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
                    if ((CURRENT_STATE.REGS[rs] & 0x80000000) != 0) {
                        NEXT_STATE.PC = CURRENT_STATE.PC + offset + 4;
                    } else {
                        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    }
                    break;
                }
                case 0x1: {
                    // BGEZ
                    if ((CURRENT_STATE.REGS[rs] & 0x80000000) == 0) {
                        NEXT_STATE.PC = CURRENT_STATE.PC + offset + 4;
                    } else {
                        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    }
                    break;
                }
                case 0x11: {
                    // BGEZAL
                    NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
                    if ((CURRENT_STATE.REGS[rs] & 0x80000000) == 0) {
                        NEXT_STATE.PC = CURRENT_STATE.PC + offset + 4;
                    } else {
                        NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                    }
                    break;
                }
            }
            break;
        }
        case 0x5: {
            // BNE

            uint32_t offset = sign_ext(imm) << 2;

            printf("BNE: offset: %d, rs: %d, rt: %d\n", offset, rs, rt);

            printf("rs: 0x%08x\n", CURRENT_STATE.REGS[rs]);
            printf("rt: 0x%08x\n", CURRENT_STATE.REGS[rt]);

            if (CURRENT_STATE.REGS[rs] != CURRENT_STATE.REGS[rt]) {
                NEXT_STATE.PC = CURRENT_STATE.PC + offset + 4;
            } else {
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            }
            break;
        }
        case 0x6: {
            // BLEZ

            uint32_t offset = sign_ext(imm) << 2;

            if (rt == 0) {
                if ((CURRENT_STATE.REGS[rs] & 0x80000000) != 0 ||
                    CURRENT_STATE.REGS[rs] == 0) {
                    NEXT_STATE.PC = CURRENT_STATE.PC + offset + 4;
                } else {
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                }
            } else {
                // Illegal instruction
                printf("Illegal rt in BLEZ.\n");
            }
            break;
        }
        case 0x7: {
            // BGTZ
            uint32_t offset = sign_ext(imm) << 2;

            printf("BGTZ: offset: 0x%08x, rs: %d, rt: %d, pc: 0x%08x\n", offset,
                   rs, rt, CURRENT_STATE.PC);

            if (rt == 0) {
                if ((CURRENT_STATE.REGS[rs] & 0x80000000) == 0 &&
                    CURRENT_STATE.REGS[rs] != 0) {
                    NEXT_STATE.PC = CURRENT_STATE.PC + offset + (uint32_t)4;
                    printf("PC: 0x%08x\n", NEXT_STATE.PC);
                } else {
                    NEXT_STATE.PC = CURRENT_STATE.PC + 4;
                }
            } else {
                // Illegal instruction
                printf("Illegal rt in BGTZ.\n");
            }
            break;
        }
        case 0x2: {
            // J
            uint32_t target = extract_target(inst);
            NEXT_STATE.PC = (CURRENT_STATE.PC & 0xf0000000) | (target << 2);
            break;
        }
        case 0x3: {
            // JAL
            uint32_t target = extract_target(inst);
            NEXT_STATE.REGS[31] = CURRENT_STATE.PC + 4;
            NEXT_STATE.PC = (CURRENT_STATE.PC & 0xf0000000) | (target << 2);
            break;
        }
        case 0xf: {
            // LUI

            if (rs == 0) {
                NEXT_STATE.REGS[rt] = imm << 16;
                NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            } else {
                // Illegal instruction
            }
            break;
        }
        case 0x20: {
            // LB

            uint32_t addr = sign_ext(imm) + CURRENT_STATE.REGS[rs];

            uint8_t byte = mem_read_32(addr) & 0xff;

            NEXT_STATE.REGS[rt] = sign_ext_byte(byte);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        }
        case 0x24: {
            // LBU

            uint32_t addr = sign_ext(imm) + CURRENT_STATE.REGS[rs];

            uint8_t byte = mem_read_32(addr) & 0xff;

            NEXT_STATE.REGS[rt] = zero_ext_byte(byte);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        }
        case 0x21: {
            // LH

            uint32_t addr = sign_ext(imm) + CURRENT_STATE.REGS[rs];

            uint16_t half = mem_read_32(addr) & 0xffff;

            NEXT_STATE.REGS[rt] = sign_ext_half(half);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        }
        case 0x25: {
            // LHU

            uint32_t addr = sign_ext(imm) + CURRENT_STATE.REGS[rs];

            uint16_t half = mem_read_32(addr) & 0xffff;

            NEXT_STATE.REGS[rt] = zero_ext_half(half);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        }
        case 0x23: {
            // LW

            uint32_t addr = sign_ext(imm) + CURRENT_STATE.REGS[rs];

            NEXT_STATE.REGS[rt] = mem_read_32(addr);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        }
        case 0x28: {
            // SB

            uint32_t addr = sign_ext(imm) + CURRENT_STATE.REGS[rs];

            uint32_t val = (mem_read_32(addr) & 0xffffff00) |
                           (CURRENT_STATE.REGS[rt] & 0xff);

            mem_write_32(addr, val);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        }
        case 0x29: {
            // SH

            uint32_t addr = sign_ext(imm) + CURRENT_STATE.REGS[rs];

            uint32_t val = (mem_read_32(addr) & 0xffff0000) |
                           (CURRENT_STATE.REGS[rt] & 0xffff);
            mem_write_32(addr, val);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        }
        case 0x2b: {
            // SW
            uint32_t addr = sign_ext(imm) + CURRENT_STATE.REGS[rs];

            mem_write_32(addr, CURRENT_STATE.REGS[rt]);
            NEXT_STATE.PC = CURRENT_STATE.PC + 4;
            break;
        }
        default: {
            printf("unimplemented instruction: 0x%08x\n", inst);
            break;
        }
    }
}
