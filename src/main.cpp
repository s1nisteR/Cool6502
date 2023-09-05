#include <iostream>

extern "C" {
#include <lib6502.h>
}

int main()
{
    std::cout << "Welcome to Cool6502 | A 6502 Emulator" << std::endl;

    M6502* mProcessor = M6502_new(nullptr, NULL, nullptr);



    return 0;
}
