#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

extern "C"
{
    #include "lib6502.h"
}
#define WRCH    0xFFEE

int wrch(M6502 *mpu, uint16_t address, uint8_t data)
{
    int pc;
    putchar(mpu->registers->a);
    pc  = mpu->memory[++mpu->registers->s + 0x100];
    pc |= mpu->memory[++mpu->registers->s + 0x100] << 8;
    return pc + 1;  /* JSR pushes next insn addr - 1 */
}

int done(M6502 *mpu, uint16_t address, uint8_t data)
{
    char buffer[64];
    M6502_dump(mpu, buffer);
    printf("\nBRK instruction\n%s\n", buffer);
    exit(0);
}

int main(int argc, char **argv)
{
    M6502    *mpu = M6502_new(0, 0, 0);
    unsigned  pc  = 0x1000;

    mpu->callbacks->call[WRCH] = wrch;     /* write character */
    mpu->callbacks->call[0000] = done;     /* reached after BRK */

# define gen1(X)        (mpu->memory[pc++] = (uint8_t)(X))
# define gen2(X,Y)      gen1(X); gen1(Y)
# define gen3(X,Y,Z)    gen1(X); gen2(Y,Z)




    gen2(0xA2, 'A'     );  /* LDX #'A'   */
    gen1(0x8A          );  /* TXA        */
    gen3(0x20,0xEE,0xFF);  /* JSR FFEE   */
    gen1(0xE8          );  /* INX        */
    gen2(0xE0, 'Z'+1   );  /* CPX #'Z'+1 */
    gen2(0xD0, -9      );  /* BNE 1002   */
    gen2(0xA9, '\n'    );  /* LDA #'\n'  */
    gen3(0x20,0xEE,0xFF);  /* JSR FFEE   */
    gen2(0x00,0x00     );  /* BRK        */

    {
        uint16_t ip = 0x1000;
        while (ip < pc)
        {
            char insn[64];
            ip += M6502_disassemble(mpu, ip, insn);
            printf("%04X %s\n", ip, insn);
        }
    }

    M6502_setVector(mpu, RST, 0x1000);

    M6502_reset(mpu);
    M6502_run(mpu);
    M6502_delete(mpu);

    return 0;
}