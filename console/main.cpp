#include "system/Framework.hpp"

#include <cassert>
#include <string>

int main(int argc, char* argv[])
{
    str z = "AB👍C👍👍👍👍👍";
    c8 c = 'A';
    Int32 zz = 4;

    Pointer aaaa(&z);

    char x = c;
    bool ok = (c == 'A');

    CodePoint cp = 0x00E1;
    char32_t u = cp;
    bool ok2 = (cp == 0x00E1);
}