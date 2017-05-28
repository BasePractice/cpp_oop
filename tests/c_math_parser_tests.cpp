#include <stack>
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

struct mutator {
	virtual expression *mutate(const expression *owner, const expression *exp) = 0;
};

/** TODO: equals */
struct expression {

	enum type {
		FUNCTION,
		NUMBER,
		VARIABLE,
		BRICES,
		OPERATION
	};

private:
	type _type;
public:
	expression(type type)
		: _type(type)
	{}

	bool is_type(type type) const {
		return _type == type;
	}

	virtual bool has_mutate() const {
		return false;
	}
	virtual void walk(visitor *visitor) = 0;
	virtual bool equals(const expression *) const = 0;
	virtual void mutate(mutator *mutator) = 0;
	virtual expression *copy() const = 0;
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

	function_expression(const std::string &name, std::unique_ptr<expression> __expression)
		: expression(expression::FUNCTION), _name(name), _expression(std::move(__expression))
	{}

	friend class parser;
public:

	const std::string &name() const { return _name; }

	void walk(visitor *visitor) override {
		visitor->enter(this);
		_expression->walk(visitor);
		visitor->exit(this);
	}

	bool equals(const expression *ex) const override {
		if (ex->is_type(expression::FUNCTION)) {
			return ((const function_expression *)ex)->name() == _name &&
				((const function_expression *)ex)->_expression->equals( _expression.get() );
		}
		return false;
	}

	bool has_mutate() const override {
		return true;
	}

	void mutate(mutator *mutator) override {
		_expression.reset(mutator->mutate(this, _expression.release()));
	}

	expression *copy() const override {
		return new function_expression(_name, std::unique_ptr<expression>(_expression->copy()));
	}
};

class number_expression final : public expression {
	const double _value;

	number_expression(double value)
		: expression(expression::NUMBER), _value(value)
	{}

	friend class parser;
public:

	static number_expression *new_number(double v) {
		return new number_expression(v);
	}

	double value() const { return _value; }

	void walk(visitor *visitor) override {
		visitor->enter(this);
	}

	bool equals(const expression *ex) const override {
		if (ex->is_type(expression::NUMBER)) {
			return ((const number_expression *)ex)->_value == _value;
		}
		return false;
	}

	void mutate(mutator *mutator) override {
		//Empty
	}

	expression *copy() const override {
		return new number_expression(_value);
	}
};

class runtime_variable_mutator;

class runtime_context final {
	std::map<std::string, expression *> _variable;
	std::unique_ptr<runtime_variable_mutator> _variable_mutator;
	std::size_t _auto_generated;
public:
	runtime_context();
    virtual ~runtime_context();

	bool add(std::unique_ptr<expression> exp) {
		return add(std::string("_var") + std::to_string(_auto_generated++), std::move(exp));
	}

	bool add(const std::string &variable, std::unique_ptr<expression> exp) {
		if (_variable.find(variable) != _variable.end()) {
			std::cerr << "Variable " << variable << " already defined" << std::endl;
			return false;
		}
		_variable.emplace(variable, exp.release());
		return true;
	}

	std::unique_ptr<expression> variable(const std::string &key);
	void print(std::ostream &output) const;
	void clear();

	std::unique_ptr<expression> mutate(std::unique_ptr<expression> exp);
private:
	friend class runtime_variable_mutator;
};

class runtime_variable_mutator final : public mutator {
	runtime_context *_ctx;
public:
	runtime_variable_mutator(runtime_context *ctx)
		: _ctx(ctx)
	{}

	expression *mutate(const expression *owner, const expression *exp) override;
};

runtime_context::runtime_context()
	: _variable_mutator(new runtime_variable_mutator(this)), _auto_generated(0) {
}

void
runtime_context::clear() {
	for (auto it = _variable.begin(); it != _variable.end(); ++it) {
		if ((*it).second != nullptr)
			delete (*it).second;
	}
	_variable.clear();
}

runtime_context::~runtime_context() {
	clear();
}

std::unique_ptr<expression> 
runtime_context::mutate(std::unique_ptr<expression> exp) {
	exp->mutate(_variable_mutator.get());
	return exp;
}

class variable_expression final : public expression {
	const std::string _value;

	variable_expression(const std::string &value)
		: expression(expression::VARIABLE), _value(value)
	{}

	friend class parser;
public:

	const std::string &value() const { return _value; }

	void walk(visitor *visitor) override {
		visitor->enter(this);
	}

	bool equals(const expression *ex) const override {
		if (ex->is_type(expression::VARIABLE)) {
			return ((const variable_expression *)ex)->_value == _value;
		}
		return false;
	}

	void mutate(mutator *mutator) override {
		//Empty
	}

	expression *copy() const override {
		return new variable_expression(_value);
	}
};

class braces_expression final : public expression {
	std::unique_ptr<expression> _value;

	braces_expression(std::unique_ptr<expression> value)
		: expression(expression::BRICES), _value(std::move(value))
	{}

	friend class parser;
	friend class runtime_variable_mutator;
public:
	void walk(visitor *visitor) override {
		visitor->enter(this);
		_value->walk(visitor);
		visitor->exit(this);
	}

	bool equals(const expression *ex) const override {
		if (ex->is_type(expression::BRICES)) {
			return ((const braces_expression *)ex)->_value->equals(_value.get());
		}
		return false;
	}

	bool has_mutate() const override {
		return true;
	}

	void mutate(mutator *mutator) override {
		auto released = _value.release();
		_value.reset(mutator->mutate(this, released) );
	}

	expression *copy() const override {
		return new braces_expression(std::unique_ptr<expression>(_value->copy()));
	}
};

class op_expression final : public expression {
	std::unique_ptr<expression> _left;
	const char _op;
	std::unique_ptr<expression> _right;

	op_expression(char op, std::unique_ptr<expression> left, std::unique_ptr<expression> right)
		: expression(expression::OPERATION), _left(std::move(left)), _op(op), _right(std::move(right))
	{}

	friend class parser;
public:

	const std::string op() const { return std::string("") + _op; }

	void walk(visitor *visitor) override {
		visitor->enter(this);
		_left->walk(visitor);
		visitor->op_next_expression(this);
		_right->walk(visitor);
		visitor->exit(this);
	}

	bool has_mutate() const override {
		return true;
	}

	bool equals(const expression *ex) const override {
		if (ex->is_type(expression::OPERATION)) {
			return ((const op_expression *)ex)->_left->equals(_left.get()) &&
				((const op_expression *)ex)->_op == _op && 
				((const op_expression *)ex)->_right->equals(_right.get());
		}
		return false;
	}

	void mutate(mutator *mutator) override {
		auto released = _left.release();
		_left.reset(mutator->mutate(this, released));
		released = _right.release();
		_right.reset(mutator->mutate(this, released));
		std::unique_ptr<expression> __unused;
	}

	expression *copy() const override {
		return new op_expression(_op, std::unique_ptr<expression>(_left->copy()), std::unique_ptr<expression>(_right->copy()));
	}
};

expression *
runtime_variable_mutator::mutate(const expression *owner, const expression *exp) {
	if (exp->is_type(expression::VARIABLE)) {
		auto name = ((variable_expression *)exp)->value();
		auto it = _ctx->_variable.find(name);
		if (it != _ctx->_variable.end()) {
			delete exp;
			return new braces_expression(std::unique_ptr<expression>((*it).second->copy()));
		}
	} else if (exp->has_mutate()) {
		const_cast<expression *>(exp)->mutate(this);
	}
	return const_cast<expression *>(exp);
}

std::unique_ptr<expression> 
runtime_context::variable(const std::string &key) {
	auto it = _variable.find(key);
	if (it == _variable.end()) {
		return std::unique_ptr<expression>();
	}
	auto exp = (*it).second->copy();
	return std::unique_ptr<expression>(exp);
}

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
		if (!tok) {
			(*_lexer).prev();
			return std::unique_ptr<expression>(new variable_expression(name));
		} else {
			if (!is_character(std::move(tok), '(')) {
				(*_lexer).prev();
				root = std::unique_ptr<expression>(new variable_expression(name));
			} else {
				std::unique_ptr<expression> exp = parse_braces_expression();
				root = std::unique_ptr<expression>(new function_expression(name, std::move(exp)));
			}
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
			if (!tok)
				return root;
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

class print_visitor final : public visitor {
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

class evaluate_visitor final : public visitor {
	std::shared_ptr<runtime_context> _ctx;
	std::stack<double> _results;
public:
	evaluate_visitor(std::shared_ptr<runtime_context> ctx)
		: _ctx(ctx)
	{}

	double eval(std::unique_ptr<expression> exp) {
		while (_results.size() > 0) {
			_results.pop();
		}
		exp->walk(this);
		return _results.top();
	}

	void enter(const variable_expression * const ex) override {
		auto exp = _ctx->variable(ex->value());
		if (!exp) {
			std::cerr << "Variable " << ex->value() << " not found" << std::endl;
			_results.push(INT_MIN);
		} else {
			if (exp->is_type(expression::NUMBER)) {
				_results.push(((number_expression *)exp.get())->value());
			} else {
				std::cerr << "Variable " << ex->value() << " NaN" << std::endl;
				_results.push(INT_MAX);
			}
		}
	}

	void enter(const number_expression * const ex) override {
		_results.push(ex->value());
	}

	void enter(const braces_expression * const ex)  override {
		//TODO
	}

	void exit(const braces_expression * const ex)  override {
		//TODO
	}

	void enter(const function_expression * const ex) override {
		//TODO
	}

	void exit(const function_expression * const ex) override {
		auto value = _results.top();
		_results.pop();
		//function call
		_results.push(value);
	}

	void enter(const op_expression * const ex) override {
		//TODO
	}

	void exit(const op_expression * const ex) override {
		auto last = _results.top();
		_results.pop();
		auto first = _results.top();
		_results.pop();
		if (ex->op() == "+") {
			_results.push(first + last);
		} else if (ex->op() == "-") {
			_results.push(first - last);
		} else if (ex->op() == "*") {
			_results.push(first * last);
		} else if (ex->op() == "/") {
			_results.push(first / last);
		} else {
			std::cerr << "Unknown operation " << ex->op() << std::endl;
			_results.push(0);
		}
	}

	void op_next_expression(const op_expression * const ex)  override {
		//TODO
	}
};

void
runtime_context::print(std::ostream &output) const {
	for (auto it = _variable.begin(); it != _variable.end(); ++it) {
		std::ostringstream ss;
		print_visitor printer(ss);

		(*it).second->walk(&printer);
		output << (*it).first << " = " << ss.str() << std::endl;
	}
}

TEST(MathParser, TreeParser_PRINT) {
	std::stringstream ss;
	auto v = std::unique_ptr<visitor>(new print_visitor(ss));
    auto root = parser(test_expression).parse();
    EXPECT_TRUE(root);
	std::cerr << std::left << test_expression << std::endl;
	root->walk(v.get());
	std::cerr << std::left << ss.str() << std::endl;
}

TEST(MathParser, TreeParser_EQUAL) {
	auto exp1 = parser(test_expression).parse();
	EXPECT_TRUE(exp1);
	auto exp2 = parser(test_expression).parse();
	EXPECT_TRUE(exp2);
	ASSERT_TRUE(exp1->equals(exp2.get()));
}

TEST(MathParser, TreeParser_TRANSMUTE) {
	std::stringstream root_text;
	auto printer = std::unique_ptr<visitor>(new print_visitor(root_text));
	
	auto runtime = std::unique_ptr<runtime_context>(new runtime_context);
	auto litte = parser("2 + 2 * 2").parse();
	runtime->add("little", std::move(litte));

	auto root = parser("little + 10 * ( 90 + little)").parse();
	EXPECT_TRUE(root);
	root->walk(printer.get());
	std::cerr << std::left << root_text.str() << std::endl;
	
	std::stringstream mutate_root_text;
	root = runtime->mutate(std::move(root));
	EXPECT_TRUE(root);
	printer = std::unique_ptr<visitor>(new print_visitor(mutate_root_text));
	root->walk(printer.get());
	std::cerr << std::left << mutate_root_text.str() << std::endl;
}

TEST(MathParser, TreeParser_CONTEXT) {
	auto runtime = std::unique_ptr<runtime_context>(new runtime_context);
	ASSERT_TRUE(runtime->add("var1", parser("1 + 2 * 2").parse()));
	ASSERT_TRUE(runtime->add("var2", parser("2 + 2 * 2").parse()));
	ASSERT_TRUE(runtime->add("var3", parser("3 + 2 * 2").parse()));
	ASSERT_TRUE(runtime->add("var4", parser("4 + 2 * 2").parse()));
	ASSERT_FALSE(runtime->add("var1", parser("5 + 2 * 2").parse()));
	ASSERT_TRUE(runtime->add(parser("6 + 2 * 2").parse()));
}

TEST(MathParser, TreeParser_EVAL) {
	auto runtime = std::shared_ptr<runtime_context>(new runtime_context);
	ASSERT_TRUE(runtime->add("x", std::unique_ptr<expression>(number_expression::new_number(2))));
	auto root = parser("x + x * x").parse();
	EXPECT_TRUE(root);
	auto evaluator = std::unique_ptr<evaluate_visitor>(new evaluate_visitor(runtime));
	auto result = evaluator->eval(std::unique_ptr<expression>(root->copy()));
	ASSERT_EQ(result, 6);
}

