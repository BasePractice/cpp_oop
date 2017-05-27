
#include <string>
#include <memory>
#include <iostream>
#include <gtest/gtest.h>

const std::string test_expression = std::string("10e+10 + 5 * (45 - 78+ (     23*8 +sin    (1.5   ))) + (variable1 * 10 + (variable2 * 4))");

class parser;
class lexer final {
    int _column;
    const std::string _text;
public:
    enum token_type {
        NUMBER,
        STRING,
        CHARACTER
    };

    struct token {
        virtual const std::string to_string() const = 0;

        virtual bool is_type(token_type type) const = 0;
    };

    class number_token final : public token {
        const double _value;

        number_token(double value)
                : _value(value) {}

        friend class lexer;
        friend class parser;
    public:
        const std::string to_string() const override {
            return std::to_string(_value);
        }

        bool is_type(token_type type) const override {
            return type == NUMBER;
        }
    };

    class string_token final : public token {
        const std::string _value;

        string_token(std::string value)
                : _value(value) {}

        friend class lexer;
        friend class parser;
    public:
        const std::string to_string() const override {
            return _value;
        }

        bool is_type(token_type type) const override {
            return type == STRING;
        }
    };

    class character_token final : public token {
        const int _value;

        character_token(int character)
                : _value(character) {}

        friend class lexer;
        friend class parser;
    public:
        const std::string to_string() const override {
            return std::string("") + ((char) _value);
        }

        bool is_type(token_type type) const override {
            return type == CHARACTER;
        }
    };

public:
    lexer(const std::string &text)
            : _text(text), _column(0) {}

    bool eol() const {
        return _column < 0 || _column >= _text.size();
    }

    bool next() {
        ++_column;
        return !eol();
    }

	bool prev() {
		--_column;
		return !eol();
	}

    int current() const {
        if (eol())
            return -1;
        return _text.at((std::string::size_type) _column);
    }

	std::shared_ptr<token> current_token() const {
		return _current_token;
	}

    std::shared_ptr<token> next_token() {
		if (eol()) {
			_current_token  = std::shared_ptr<token>();
		} else {
			skip_ws();
			if (isdigit(current())) {
				_current_token = parse_number();
			}	else if (isalnum(current())) {
				_current_token = parse_string();
			}	else {
				int character = current();
				next();
				_current_token = std::shared_ptr<token>(new character_token(character));
			}
		}
		return current_token();
    }

private:
    char character() const {
        return (char) current();
    }

    void skip_ws() {
        while (!eol() && isspace(current())) {
            next();
        }
    }

    std::shared_ptr<token> parse_number() {
        std::string number;
        bool has_dot = false;
        while (!eol()) {
            if (isdigit(current())) {
                number += character();
            } else if (current() == '.') {
                number += character();
                if (has_dot)
                    std::cerr << "Ошибка в строке " << _column << std::endl;
                has_dot = true;
            } else if (current() == 'e') {
                number += "e";
                next();
                if (current() == '+' || current() == '-') {
                    number += character();
                    next();
                }
                while (!eol() && isdigit(current())) {
                    number += character();
                    next();
                }
                return std::shared_ptr<token>(new number_token(std::strtod(number.c_str(), nullptr)));
            } else {
                break;
            }
            next();
        }
        return std::shared_ptr<token>(new number_token(std::strtod(number.c_str(), nullptr)));
    }

    std::shared_ptr<token> parse_string() {
        std::string text;

        text += character();
        while (next()) {
            if (!isalnum(current()) && current() != '_' && !isdigit(current()))
                break;
            text += character();
        }
        return std::shared_ptr<token>(new string_token(text));
    }

	std::shared_ptr<token> _current_token;
};

std::ostream &
operator<<(std::ostream &output, const lexer::token &token) {
    output << token.to_string();
    return output;
}

struct expression;
class braces_expression;
class function_expression;
class number_expression;
class op_expression;
class variable_expression;

struct visitor {

	virtual void enter(const variable_expression * const) {

	}

    virtual void enter(const number_expression * const) {

    }

	virtual void enter(const braces_expression * const) {

	}

	virtual void exit(const braces_expression * const) {

	}

	virtual void enter(const function_expression * const) {

	}

	virtual void exit(const function_expression * const) {

	}

	virtual void enter(const op_expression * const) {

	}

	virtual void exit(const op_expression * const) {

	}

	virtual void op_next_expression(const op_expression * const) {

	}
};

/** TODO: equals */
struct expression {
	virtual std::unique_ptr<visitor> walk(std::unique_ptr<visitor> visitor) = 0;
};

struct parser_exception : public std::exception {
	const std::string _msg;
	parser_exception(const std::string &msg)
		: _msg(msg) {}
public:
	const char* what() const noexcept override {
		return _msg.c_str();
	}
};

class function_expression final : public expression {
	const std::string _name;
	std::unique_ptr<expression> _expression;

	function_expression(const std::string &name, std::unique_ptr<expression> expression)
		: _name(name), _expression(std::move(expression))
	{}

	friend class parser;
public:

	const std::string &name() const { return _name; }

	std::unique_ptr<visitor> walk(std::unique_ptr<visitor> visitor) override {
		visitor->enter(this);
		visitor = _expression->walk(std::move(visitor));
		visitor->exit(this);
		return visitor;
	}
};

class number_expression final : public expression {
	const double _value;

	number_expression(double value)
		: _value(value)
	{}

	friend class parser;
public:

	double value() const { return _value; }

	std::unique_ptr<visitor> walk(std::unique_ptr<visitor> visitor) override {
		visitor->enter(this);
		return visitor;
	}
};

/**
Variable table

variable -> expression
----------------------
Variable replace befor evaluate

expression -> expression
*/
class variable_expression final : public expression {
	const std::string _value;

	variable_expression(const std::string &value)
		: _value(value)
	{}

	friend class parser;
public:

	const std::string &value() const { return _value; }

	std::unique_ptr<visitor> walk(std::unique_ptr<visitor> visitor) override {
		visitor->enter(this);
		return visitor;
	}
};

class braces_expression final : public expression {
	std::unique_ptr<expression> _value;

	braces_expression(std::unique_ptr<expression> value)
		: _value(std::move(value))
	{}

	friend class parser;
public:
	std::unique_ptr<visitor> walk(std::unique_ptr<visitor> visitor) override {
		visitor->enter(this);
		visitor = _value->walk(std::move(visitor));
		visitor->exit(this);
		return visitor;
	}
};

class op_expression final : public expression {
	std::unique_ptr<expression> _left;
	const char _op;
	std::unique_ptr<expression> _right;

	op_expression(char op, std::unique_ptr<expression> left, std::unique_ptr<expression> right)
		: _left(std::move(left)), _op(op), _right(std::move(right))
	{}

	friend class parser;
public:

	const std::string op() const { return std::string("") + _op; }

	std::unique_ptr<visitor> walk(std::unique_ptr<visitor> visitor) override {
		visitor->enter(this);
		visitor = _left->walk(std::move(visitor));
		visitor->op_next_expression(this);
		visitor = _right->walk(std::move(visitor));
		visitor->exit(this);
		return visitor;
	}
};

class parser {
    std::unique_ptr<lexer> _lexer;
public:
    parser(const std::string &text)
            : _lexer(new lexer(text)) {}

    

    std::unique_ptr<expression> parse() throw(parser_exception);
private:
    std::unique_ptr<expression> create_number(std::shared_ptr<lexer::token> tok) const {
        return std::unique_ptr<expression>(new number_expression( ((lexer::number_token &)(*tok))._value ));
    }

    bool is_character(std::shared_ptr<lexer::token> tok, char ch) {
        return (*tok).is_type(lexer::CHARACTER) && ((lexer::character_token &)(*tok))._value == ch;
    }

    std::unique_ptr<expression> parse_braces_expression()  throw(parser_exception) {
        std::shared_ptr<lexer::token> tok;
        std::unique_ptr<expression> exp;

        exp = parse();
		tok = (*_lexer).current_token();
        if (!tok)
            throw parser_exception("Illegal end of");
        if (!is_character(std::move(tok), ')'))
            throw parser_exception("Illegal state. Must '('");
        return std::unique_ptr<expression>(new braces_expression(std::move(exp)));
    }
};

/*

     expression := number | op_expression | function_expression | braces_expression
     op_expression := expression operation expression
     function_expression := name braces_expression
     braces_expression := '(' expression ')'
     name := alpha (digit | alpha)
     operation := '+' | '-' | '*' | '/'
    */
std::unique_ptr<expression>
parser::parse() throw(parser_exception) {
    std::unique_ptr<expression> root;

    std::shared_ptr<lexer::token> tok;

    tok = (*_lexer).next_token();
    if (!tok)
        return root;
    if ((*tok).is_type(lexer::NUMBER)) {
        root = create_number(std::move(tok));
	} else if ((*tok).is_type(lexer::STRING)) {
		const std::string name = ((lexer::string_token &)(*tok))._value;
		tok = (*_lexer).next_token();
		if (!tok)
			throw parser_exception("Illegal end of");
		if (!is_character(std::move(tok), '(')) {
			(*_lexer).prev();
			root = std::unique_ptr<expression>(new variable_expression(name));
		} else {
			std::unique_ptr<expression> exp = parse_braces_expression();
			root = std::unique_ptr<expression>(new function_expression(name, std::move(exp)));
		}
	} else if ((*tok).is_type(lexer::CHARACTER) && ((lexer::character_token &)(*tok))._value == '(') {
		root = parse_braces_expression();
    } else {
        throw parser_exception("Unknown expression");
    }

    while ( (tok = (*_lexer).next_token()) ) {
        if ((*tok).is_type(lexer::CHARACTER)) {
			if (((lexer::character_token &)(*tok))._value == ')')
				return root;

            root = std::unique_ptr<expression>(new op_expression(
                    (char)((lexer::character_token &) (*tok))._value,
                    std::move(root),
                    std::move(parse())
            ));
			tok = (*_lexer).current_token();
			if (tok && (*tok).is_type(lexer::CHARACTER) && ((lexer::character_token &)(*tok))._value == ')')
				return root;
        } else {
            throw parser_exception("Wat?");
        }
    }
    return root;
}

TEST(MathParser, Lexer) {
    lexer lexer(test_expression);
    std::shared_ptr<lexer::token> tok;
    while ((tok = lexer.next_token())) {
		ASSERT_TRUE(tok);
    }
}

class print_visitor : public visitor {
	std::ostream &output;

public:
	print_visitor(std::ostream &stream)
		: output(stream)
	{}

	void enter(const variable_expression * const ex) override {
		output << ex->value();
	}

	void enter(const number_expression * const ex) override {
		output << ex->value();
	}

	void enter(const braces_expression * const ex)  override {
		output << "(";
	}

	void exit(const braces_expression * const ex)  override {
		output << ")";
	}

	void enter(const function_expression * const ex) override {
		output << ex->name();
	}

	void exit(const function_expression * const ex) override {
	}

	void enter(const op_expression * const ex) override {

	}

	void exit(const op_expression * const ex) override {

	}

	void op_next_expression(const op_expression * const ex)  override {
		output << " " << ex->op() << " ";
	}
};

TEST(MathParser, TreeParser) {
	std::stringstream ss;
	auto v = std::unique_ptr<visitor>(new print_visitor(ss));
    auto root = parser(test_expression).parse();
    EXPECT_TRUE(root);
	std::cerr << std::left << test_expression << std::endl;
	v = root->walk(std::move(v));
	std::cerr << std::left << ss.str() << std::endl;
	EXPECT_TRUE(v);
}

