#include <iostream>
#include <sstream>

enum Token_value {
    NUMBER,
    PLUS = '+',
    MINUS = '-',
    MUL = '*',
    DIV = '/',
    NOTHING,
    ERROR
};

Token_value curr_tok = NOTHING;
int64_t number_value;
std::string string_value;

int64_t expr(std::istream&, bool);


int error() {
    std::cout << "error"<< std::endl;
    curr_tok = ERROR;
    return 1;
}

Token_value get_token(std::istream& input) {
    char ch;
    
    do {
        if (!input.get(ch)) {
            return curr_tok = NOTHING;
        }
    } while (ch != '\n' && isspace(ch));
    
    switch (ch) {
        case MUL:
        case DIV:
        case PLUS:
        case MINUS:
            return curr_tok = Token_value(ch);
        case '0': case '1': case '2': case '3': case '4':
        case '5': case '6': case '7': case '8': case '9':
            input.putback(ch);
            input >> number_value;
            return curr_tok = NUMBER;
        default:
            error();
            return curr_tok;
    }
}


int64_t prim(std::istream& input, bool get) {
    if (get) {
        get_token(input);
    }
    
    switch (curr_tok) {
        case ERROR:
            return 1;
        case NUMBER: {
            int64_t v = number_value;
            get_token(input);
            return v;
        }
            
        case MINUS:
            return -prim(input, true);
            
        default:
            return error();
    }
}

int64_t term(std::istream& input, bool get) {
    int64_t left = prim(input, get);
    
    for (;;) {
        switch (curr_tok) {
            case ERROR:
                return 1;
            case MUL:
                left *= prim(input, true);
                break;
            case DIV:
                if (double d = prim(input, true)) {
                    left /= d;
                    break;
                }
                return error();
            default:
                return left;
        }
    }
}

int64_t expr(std::istream& input, bool get) {
    int64_t left = term(input, get);
    
    for (;;) {
        switch (curr_tok) {
            case ERROR:
                return 1;
            case PLUS:
                
                left += term(input, true);
                break;
            case MINUS:
                
                left -= term(input, true);
                break;
            default:
                return left;
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        error();
        return 1;
    }
    std::istringstream input(argv[1]);
    
    while (input) {
        get_token(input);
        
        int64_t tmp = expr(input, false);
        if (curr_tok == ERROR) {
            
            return 1;
        }
        std::cout << tmp << std::endl;
    }
    return 0;
}