#include "system/Framework.hpp"

#include <cassert>
#include <string>

int main(int argc, char* argv[])
{
    const char* p = "ABCDEF";
    String oooo = p;

    String x = "\xF0\x9F\x91\x8D\xF0\x9F\x91\x8D Bruno Corrêa Frutuozo \xF0\x9F\x91\x8D\xF0\x9F\x91\x8D";
    String y = "UTF8: " + x.ToHex();
    String w = "UTF16: " + x.ToHex(Encoding::UTF16);
    String z = "UTF32: " + x.ToHex(Encoding::UTF32);

    Console::WriteLine(x);
    Console::WriteLine(y);
    Console::WriteLine(w);
    Console::WriteLine(z);

    String ooo = 'A';
    ooo.Contains(u8"ABCDEF");

    Console::WriteLine(ooo == "acb");
}