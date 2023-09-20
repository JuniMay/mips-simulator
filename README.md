# MIPS Simulator

NKU computer architecture lab. An instruction level mips simulator.

## Implementation

First, a simple decoder to extract fields from instructions is implemented by several functions.

```c
uint32_t extract_op(uint32_t inst) { return inst >> 26; }

uint32_t extract_rs(uint32_t inst) { return (inst >> 21) & 0x1f; }

uint32_t extract_rt(uint32_t inst) { return (inst >> 16) & 0x1f; }

uint32_t extract_rd(uint32_t inst) { return (inst >> 11) & 0x1f; }

uint32_t extract_target(uint32_t inst) { return inst & 0x3ffffff; }

uint32_t extract_imm(uint32_t inst) { return inst & 0xffff; }

uint32_t extract_shamt(uint32_t inst) { return (inst >> 6) & 0x1f; }

uint32_t extract_funct(uint32_t inst) { return inst & 0x3f; }
```

All the fields are extracted before the instruction is executed.

```c
uint32_t op = extract_op(inst);
uint32_t rs = extract_rs(inst);
uint32_t rt = extract_rt(inst);
uint32_t imm = extract_imm(inst);
uint32_t rd = extract_rd(inst);
uint32_t shamt = extract_shamt(inst);
uint32_t funct = extract_funct(inst);
```

And some helper functions for sign/zero extensions. Because of the variety of load instructions, extensions for word, half word and byte are implemented.

```c
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
```

Then in `process_instruction`, an instruction is firstly fetched from memory using program counter.

```c
uint32_t inst = mem_read_32(CURRENT_STATE.PC);
```

Then a large `switch` statement is adopted to handle all the different types of instructions. and `funct` is `switch`-ed if necessary.

Note that to handle the branch instruction in the simulator without the delay slot, the `PC` is incremented by an additional 4.

```c
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
```

And if the branch is not taken, the `PC` is incremented by 4 as normal.
