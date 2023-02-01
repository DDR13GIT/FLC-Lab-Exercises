#include <bits/stdc++.h>
using namespace std;

#define fw setw(10)

enum parse_state {
    DEFAULT,
    SINGLE_COMMENT,
    MULTI_COMMENT,
    STRING_LINE
};

void step_1_function() {

    ifstream input("input.c", ios::in);
    ofstream output("step_1.txt", ios::out);
    
    parse_state state = DEFAULT;
    char curr, next, last;

    while(!input.eof()) {

        input.get(curr);
        next = input.peek();

        if (state == DEFAULT || state == STRING_LINE) {
            if (state == DEFAULT) {
                if (curr == '/' && next == '/') {
                    state = SINGLE_COMMENT;
                }
                if (curr == '/' && next == '*') {
                    state = MULTI_COMMENT;
                } 
                if (curr == '"') {
                    state = STRING_LINE;
                }
                if (curr == '\n' || curr == '\t') {
                    curr = ' ';
                }
            } else {
                if (curr == '"') {
                    state = DEFAULT;
                }
            }
            if (state == DEFAULT || state == STRING_LINE) {
                if (last == ' ' && curr == ' ') {
                    continue;
                }
                last = curr;
                output << curr;
            }
        } else if (state == SINGLE_COMMENT) {
            if (curr == '\n') {
                state = DEFAULT;
            }
        } else if (state == MULTI_COMMENT) {
            if (curr == '*' && next == '/') {
                state = DEFAULT;
                input.ignore();
            }
        }
    }

    input.close();
    output.close();

}

void step_2_function() {
    fstream input("step_1.txt", ios::in);
    fstream output("temp.txt", ios::out);

    vector<char> dels {',', ';', '=', '+', '-', '*', '/', '<', '>', '!', '\'', '(', ')'};
    vector<string> kws {"auto","break", "case", "char", "const", "continue", "default", "do", "double", "else", "enum", "extern", "float", "for", "goto", "if", "int", "long", "register", "return", "short", "signed", "sizeof", "static", "struct", "switch", "typedef", "union", "unsigned", "void", "volatile", "while"};
    vector<string> ops {"+", "-", "*", "/", "=", "==", "!=", ">", "<" ">=", "<="};
    vector<string> seprs {",", ";", "'"};
    vector<string> brackets {"(", ")", "{", "}", "[", "]"};
    
    char curr, next;
    string in;

    if (input.is_open()) {
        while (!input.eof()) {
            input.get(curr);
            next = input.peek();
            bool is_curr_seperator = find(dels.begin(), dels.end(), curr) != dels.end();
            bool is_next_seperator = find(dels.begin(), dels.end(), next) != dels.end();

            output << curr;
            if ((!is_curr_seperator && is_next_seperator && curr != ' ') || (is_curr_seperator && next != '=')) {    
                output << ' ';
            }
        }
    }
    input.close();
    output.close();

    input.open("temp.txt", ios::in);
    output.open("step_2.txt", ios::out);

    while(input >> in) {
        bool is_keyword = find(kws.begin(), kws.end(), in) != kws.end();
        bool is_operator = find(ops.begin(), ops.end(), in) != ops.end();
        bool is_seperator = find(seprs.begin(), seprs.end(), in) != seprs.end();
        bool is_brackets = find(brackets.begin(), brackets.end(), in) != brackets.end();

        string tok;
        if (is_keyword) {
            tok = "[kw " + in + "]";
        } else if (is_operator) {
            tok = "[op " + in + "]";
        } else if (is_seperator) {
            tok = "[sep " + in + "]";
        } else if (is_brackets) {
            tok = "[par " + in + "]";
        } else {
            if (isalpha(in[0]) || in[0] == '_') {
                tok = "[id " + in + "]";
                for (int i = 1; i < in.length(); ++i) {
                    if (!isalnum(in[i]) && in[i] != '_') {
                        tok = "[unkn " + in + "]";
                        break;
                    }
                }
            } else if (count(in.begin(), in.end(), '.') > 1 || in[in.length() - 1] == '.') {
                tok = "[unkn " + in + "]";
            } else {
                tok = "[num " + in + "]";
                for (int i = 0; i < in.length(); ++i) {
                    if (!isdigit(in[i]) && in[i] != '.' ) {
                        tok = "[unkn " + in + "]";
                        break;
                    }
                }
            }
        }
        output << tok << " ";
    }
    input.close();
    output.close();
}

vector<map<string, string>> table;

string i2s(int n) {
    string s;
    stringstream stream;
    stream << n;
    stream >> s;
    return s;
}

void display() {
    cout << fw << "Sl. No." << fw << "Name" << fw << "Id Type" << fw << "Data Type" << fw << "Scope" << fw << "Value" << endl;
    for (int i = 0; i < table.size(); ++i) {
        cout << fw << i + 1 << fw << table[i]["name"] << fw << table[i]["id_type"] << fw << table[i]["data_type"] << fw << table[i]["scope"] << fw << table[i]["value"] << endl;
    }
}

int lookup(string name, string scope) {
    for (int i = 0; i < table.size(); ++i) {
        if (table[i]["name"] == name && table[i]["scope"] == scope) {
            return i;
        }
    }
    return -1;
}

void free() {
    table.clear();
}

void set_attribute(int index, string attr, string value){
    if (index < table.size()) {
        table[index][attr] = value;
    }
}

int insert(map<string, string> curr) {
    int index = lookup(curr["name"], curr["scope"]);
    if (index == -1) {
        table.push_back(curr);
        return table.size();
    } else {
        if (curr["value"].length() > 0) {
            set_attribute(index, "value", curr["value"]);
        }
    }
    return index + 1;
}

void step_3_function() {
    ifstream input("step_2.txt", ios::in);
    vector<string> words, scopes {"global"};

    int cnt = 0;
    string s, curr;
    while(input >> curr) {
        if (curr[curr.length() - 1] == ']') {
            words.push_back(curr.substr(0, curr.length() - 1));
        }
        if (curr == "[id") {
            words.push_back(curr.substr(1, curr.length()));
        }
    }

    for (int i = 0; i < words.size(); ++i) {
        bool is_new_scope = false;
        if (words[i] == "}") {
            scopes.pop_back();
        }
        if (words[i] == "id") {
            map<string, string> curr;
            curr["name"] = words[i + 1];
            bool is_var = words[i + 2] == ";" || words[i + 2] == "=" || words[i + 2] == "," || words[i + 2] == ")";
            if (is_var) {
                curr["id_type"] = "var";
                if (words[i + 2] == "=" && words[i + 3] != "id") {
                    curr["value"] = words[i + 3];
                }
                curr["scope"] = scopes[scopes.size() - 1];
            } else if (words[i + 2] == "(") {
                curr["id_type"] = "func";
                for (int j = i + 2; j < words.size(); ++j) {
                    if (words[j] == ")" && words[j + 1] == "{") {
                        is_new_scope = true;
                        break;
                    }
                }
            }
            if (words[i - 1] == "float" || words[i - 1] == "int" || words[i - 1] == "double" || words[i - 1] == "void" || words[i - 1] == "long") {
                curr["data_type"] = words[i - 1];
            }
            curr["scope"] = scopes[scopes.size() - 1];
            if (is_new_scope) {
                scopes.push_back(curr["name"]);
            }
            if (is_new_scope || is_var) {
                int id_no = insert(curr);
                words[i + 1] = i2s(id_no);
            } else {
                int id_no = lookup(curr["name"], "global") + 1;
                words[i + 1] = i2s(id_no);
            }
        }
    }
}

void f_print(string fn) {
    ifstream input(fn, ios::in);
    char c;
    while (!input.eof()) {
        input.get(c);
        cout << c;
    }
    cout << endl << endl;
}

int main() {
    
    cout << "Input:" << endl;
    cout << "----------------------" << endl;
    f_print("input.c");

    step_1_function();
    cout << "Step 1:" << endl;
    cout << "----------------------" << endl;
    f_print("step_1.txt");

    step_2_function();
    cout << "Step_2:" << endl;
    cout << "----------------------" << endl;
    f_print("step_2.txt");

    step_3_function();
    cout << "Step 3:" << endl;
    cout << "----------------------" << endl;
    display();

    return 0;
}