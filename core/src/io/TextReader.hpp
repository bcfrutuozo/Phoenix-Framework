#pragma once
#include "Stream.hpp"
#include "types/Byte.hpp"
#include "types/Char.hpp"
#include "types/String.hpp"

#include <cstdlib>  // malloc, free
#include <cstring>  // memcpy

class TextReader {
	Stream* base;

public:
	explicit TextReader(Stream* s) noexcept : base(s) {}
	TextReader() noexcept : base(nullptr) {}

	bool IsValid() const noexcept {
		return base != nullptr && base->CanRead();
	}

	// Read up to count raw bytes into buffer. Returns number of bytes read.
	Stream::size_type ReadBytes(Byte* buffer, Stream::size_type count) noexcept {
		if (!IsValid()) return 0;
		return base->Read(buffer, count);
	}

	// Read line until '\n' (NOT including '\n'), return as UTF-8 String.
    // dentro do seu Stream Reader (ou onde estava)
    String ReadLine() noexcept {
        if (!IsValid()) return String();

        constexpr size_t INITIAL_CAPACITY = 1024;
        size_t capacity = INITIAL_CAPACITY;
        size_t count = 0;

        Char* buf = static_cast<Char*>(malloc(capacity));
        if (!buf) return String();

        Byte b = 0; // Stream::byte deve ser unsigned char

        while (true) {
            Stream::size_type r = base->Read(&b, 1);
            if (r == 0) break;          // EOF
            if (b == '\n') break;

            if (count + 1 >= capacity) {
                size_t newcap = capacity * 2;
                Char* newbuf = static_cast<Char*>(malloc(newcap));
                if (!newbuf) { free(buf); return String(); }
                memcpy(newbuf, buf, count);
                free(buf);
                buf = newbuf;
                capacity = newcap;
            }
            buf[count++] = b;
        }

        if (count == 0) { free(buf); return String(); }
        if (count > 0 && buf[count - 1] == '\r') --count;

        String s(buf, static_cast<String::size_type>(count));
        free(buf);
        return s;
    }
};