#include "system/Framework.hpp"

#include <cassert>
#include <string>

int main(int argc, char* argv[])
{
    Char c = 'A';

    char x = c;
    bool ok = (c == 'A');

    CodePoint cp = 0x00E1;
    char32_t u = cp;
    bool ok2 = (cp == 0x00E1);
}