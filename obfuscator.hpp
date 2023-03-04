/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2016 katursis
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef STR_OBFUSCATOR_HPP_
#define STR_OBFUSCATOR_HPP_

#ifdef _MSC_VER
#define forceinline __forceinline
#else
#ifdef __GNUC__
#define forceinline __attribute__((always_inline))
#endif
#endif

namespace detail {
    template <std::size_t index>
    struct encryptor {
        forceinline static constexpr void encrypt(char* dest, const char* str,
            char key) {
            dest[index] = str[index] ^ key;

            encryptor<index - 1>::encrypt(dest, str, key);
        }
    };

    template <>
    struct encryptor<0> {
        forceinline static constexpr void encrypt(char* dest, const char* str,
            char key) {
            dest[0] = str[0] ^ key;
        }
    };
};  // namespace detail

class cryptor {
public:
    template <std::size_t S>
    class string_encryptor {
    public:
        constexpr string_encryptor(const char str[S], int key)
            : _buffer{},
            _decrypted{ false },
            _key{ static_cast<const char>(key % 255) } {
            detail::encryptor<S - 1>::encrypt(_buffer, str, _key);
        }

#ifdef __GNUC__
        __attribute__((noinline))
#endif
            const char*
            decrypt() const {
            if (_decrypted) {
                return _buffer;
            }

            for (auto& c : _buffer) {
                c ^= _key;
            }

            _decrypted = true;

            return _buffer;
        }

    private:
        mutable char _buffer[S];
        mutable bool _decrypted;
        const char _key;
    };

    template <std::size_t S>
    static constexpr auto create(const char(&str)[S]) {
        return string_encryptor<S>{str, S};
    }
};

#endif  // STR_OBFUSCATOR_HPP_