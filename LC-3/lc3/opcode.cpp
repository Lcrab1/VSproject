#include"opcode.h"


void op_add(uint16_t instr) {
    /* destination register (DR) */
    uint16_t r0 = (instr >> 9) & 0x7;
    /* first operand (SR1) */
    uint16_t r1 = (instr >> 6) & 0x7;
    /* whether we are in immediate mode */
    uint16_t imm_flag = (instr >> 5) & 0x1;

    if (imm_flag)
    {
        uint16_t imm5 = sign_extend(instr & 0x1F, 5);
        reg[r0] = reg[r1] + imm5;
    }
    else
    {
        uint16_t r2 = instr & 0x7;
        reg[r0] = reg[r1] + reg[r2];
    }

    update_flags(r0);
}

void op_and(uint16_t instr) {
    uint16_t r0 = (instr >> 9) & 0x7;
    uint16_t r1 = (instr >> 6) & 0x7;
    uint16_t imm_flag = (instr >> 5) & 0x1;

    if (imm_flag)
    {
        uint16_t imm5 = sign_extend(instr & 0x1F, 5);
        reg[r0] = reg[r1] & imm5;
    }
    else
    {
        uint16_t r2 = instr & 0x7;
        reg[r0] = reg[r1] & reg[r2];
    }
    update_flags(r0);
}

void op_not(uint16_t instr) {
    uint16_t r0 = (instr >> 9) & 0x7;
    uint16_t r1 = (instr >> 6) & 0x7;

    reg[r0] = ~reg[r1];
    update_flags(r0);
}


//读取输入信号wasd
void trap_getc() {
    reg[R_R0] = (uint16_t)getchar();
}


void trap_out() {
    //putc可以将数据传递给文件的缓冲区
    putc((char)reg[R_R0], stdout);
    //刷新输出缓冲区，让数据立即写入到文件中
    fflush(stdout);
}

//输出Unicode类型的字符串
void trap_puts() {
    //字符串位于寄存器R_RO所存储的地址中
    uint16_t* c = memory + reg[R_R0];
    //while循环来输出字符
    while (*c)
    {
        //将内存中数据传递至控制台的输出缓冲区
        putc((char)*c, stdout);
        ++c;
    }
    //清空输出缓冲区
    fflush(stdout);
}

// 输出y/n? 让用户能够看到自己的输入结果,并显示在输出界面

void trap_in() {
    printf("请输入一个字符: ");
    //接收字符并将字符放在输出缓冲区中
    char c = getchar();
    putc(c, stdout);
    //修改，加入了清空输出缓冲区的代码
    fflush(stdout);
    reg[R_R0] = (uint16_t)c;
}

//按照ASCII码来进行输出
void trap_putsp() {
    uint16_t* c = memory + reg[R_R0];

    while (*c)
    {
        //先输出低四位
        char char1 = (*c) & 0xFF;
        putc(char1, stdout);
        //再输出高四位
        char char2 = (*c) >> 8;
        if (char2) { 
            putc(char2, stdout); 
        }
        ++c;
    }
    fflush(stdout);
}

void trap_halt() {
    puts("HALT");
    fflush(stdout);
}