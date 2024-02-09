
#pragma once

#include <string>
#include <optional>
#include <vector>
#include <unordered_map>

enum class TokenType {
    // misc

    ident,
    unknown,

    // keywords

    exit,

    // operators / single character

    semi,
    open_paren,
    close_paren,
    eql,

    // literals

    int_lit,
    float_lit,
    char_lit,
    bool_lit,
    String_lit,
    i8_lit,
    i16_lit,
    i32_lit,
    i64_lit,
    i128_lit,
    isize_lit,
    u8_lit,
    u16_lit,
    u32_lit,
    u64_lit,
    u128_lit,
    usize_lit,
    f8_lit,
    f16_lit,
    f32_lit,
    f64_lit,
    f128_lit,
    fsize_lit,

   // modifiers for variable declaration

    _unsigned,
    _const,
    _static,
    _mutable
};

struct Token {
    TokenType type;
    std::optional<std::string> value {};
};

std::unordered_map<std::string, TokenType> literalMap = {
        {"i8", TokenType::i8_lit},
        {"i16", TokenType::i16_lit},
        {"i32", TokenType::i32_lit},
        {"i64", TokenType::i64_lit},
        {"i128", TokenType::i128_lit},
        {"isize", TokenType::isize_lit},
        {"u8", TokenType::u8_lit},
        {"u16", TokenType::u16_lit},
        {"u32", TokenType::u32_lit},
        {"u64", TokenType::u64_lit},
        {"u128", TokenType::u128_lit},
        {"usize", TokenType::usize_lit},
        {"f8", TokenType::f8_lit},
        {"f16", TokenType::f16_lit},
        {"f32", TokenType::f32_lit},
        {"f64", TokenType::f64_lit},
        {"f128", TokenType::f128_lit},
        {"fsize", TokenType::fsize_lit},
        {"char", TokenType::char_lit},
        {"bool", TokenType::bool_lit},
        {"String", TokenType::String_lit}
};

class Tokenizer {
public:
    inline explicit Tokenizer(std::string&& src) : m_src(std::move(src)) {

    }

    inline std::vector<Token> tokenize() {

        std::vector<Token> tokens;
        std::string buf{};
        while (peek().has_value()) {
            if (std::isalpha(peek().value())) {
                buf.push_back(consume());
                while (peek().has_value() && std::isalnum(peek().value())) {
                    buf.push_back(consume());
                }
                if (buf == "exit") {
                    tokens.push_back({.type = TokenType::exit});
                    buf.clear();
                    continue;
                } // check if buf is a literal
                auto it = literalMap.find(buf);
                if (it != literalMap.end()) {
                    tokens.push_back({.type = it->second});
                    buf.clear();
                }
                 else {
                     tokens.push_back({.type = TokenType::ident, .value = buf});
                     buf.clear();
                 }
            }
            else if (std::isdigit(peek().value())) {
                buf.push_back(consume());
                while (peek().has_value() && std::isdigit(peek().value())) {
                    buf.push_back(consume());
                }
                tokens.push_back({ TokenType::int_lit, buf });
                buf.clear();
            }
            else if (peek().value() == '(') {
                consume();
                tokens.push_back({ TokenType::open_paren });
            }
            else if (peek().value() == ')') {
                consume();
                tokens.push_back({ TokenType::close_paren });
            }
            else if (peek().value() == ';') {
                consume();
                tokens.push_back({ TokenType::semi });
            } else if (peek().value() == '=') {
                consume();
                tokens.push_back({ TokenType::eql });
            }
            else if (std::isspace(peek().value())) {
                consume();
            }
            else {
                std::cout << "bad code :(" << std::endl;
                exit(1);
            }
        }
        m_index = 0;
        return tokens;
    }

private:
    [[nodiscard]] inline std::optional<char> peek(int offset = 0) const
    {
        if (m_index + offset >= m_src.length()) {
            return {};
        }
        else {
            return m_src.at(m_index + offset);
        }
    }

    inline char consume()
    {
        return m_src.at(m_index++);
    }

    const std::string m_src;
    size_t m_index = 0;
};