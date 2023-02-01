#include <bits/stdc++.h>
using namespace std;

vector<string> keywords {"void", "int", "long", "float", "double", "char", "string", "if", "else", "for", "while", "do", "continue", "break", "return"};
vector<string> operators {"+", "-", "*", "/", "=", "==", "!=", ">", "<" ">=", "<="};
vector<string> seperators {",", ";", "'"};
vector<string> parenthesis {"(", ")", "{", "}", "[", "]"};

string get_type(string in) {
    bool is_keyword = find(keywords.begin(), keywords.end(), in) != keywords.end();
    bool is_operator = find(operators.begin(), operators.end(), in) != operators.end();
    bool is_seperator = find(seperators.begin(), seperators.end(), in) != seperators.end();
    bool is_parenthesis = find(parenthesis.begin(), parenthesis.end(), in) != parenthesis.end();

    string cat;
    if (is_keyword) {
        cat = "kw";
    } else if (is_operator) {
        cat = "op";
    } else if (is_seperator) {
        cat = "sep";
    } else if (is_parenthesis) {
        cat = "par";
    } else {
        if (isalpha(in[0]) || in[0] == '_') {
            cat = "id";
            for (int i = 1; i < in.length(); ++i) {
                if (!isalnum(in[i]) && in[i] != '_') {
                    cat = "unkn";
                }
            }
        } else if (count(in.begin(), in.end(), '.') > 1 || in[in.length() - 1] == '.') {
            cat = "unkn";
        } else {
            cat = "num";
            for (int i = 0; i < in.length(); ++i) {
                if (!isdigit(in[i]) && in[i] != '.' ) {
                    cat = "unkn";
                    break;
                }
            }
        }
    }
    return cat;
}

int main()
{
    ifstream input("input.c", ios::in);
    ofstream output("first.txt", ios::out);

    string state = "DEFAULT";
    char curr, next, last;

    while (!input.eof()) {
        input.get(curr);
        next = input.peek();
        if (state != "DEFAULT" && curr == '\n') {
            output << curr;
        }
        if (curr == '/' && next == '/') {
            state = "SINGLE_COMMENT";
        } else if (curr == '/' && next == '*') {
            state = "MULTI_COMMENT";
        } else if (state == "SINGLE_COMMENT" && curr == '\n') {
            state = "DEFAULT";
        } else if (state == "MULTI_COMMENT" && curr == '*' && next == '/') {
            state = "DEFAULT";
            input.ignore();
        } else {
            if (state == "DEFAULT") {
                last = curr;
                output << last;
            }
        }
    }

    input.close();
    output.close();

    input.open("first.txt", ios::in);
    output.open("second.txt", ios::out);

    vector<char> delimiters {',', ';', '=', '+', '-', '*', '/', '<', '>', '!', '\'', '(', ')', '{', '}'};
    while (!input.eof()) {
        input.get(curr);
        
        next = input.peek();
        bool is_curr_seperator = find(delimiters.begin(), delimiters.end(), curr) != delimiters.end();
        bool is_next_seperator = find(delimiters.begin(), delimiters.end(), next) != delimiters.end();

        output << curr;
        if ((!is_curr_seperator && is_next_seperator && curr != ' ' && curr != '\n') || (is_curr_seperator && next != '=')) {    
            output << ' ';
        }
    }

    input.close();
    output.close();

    input.open("second.txt", ios::in);
    output.open("second_temp.txt", ios::out);

    string curr_w;
    vector<string> lines, types, codes;
    vector<int> line_no;
    
    while(getline(input, curr_w)) {
        lines.push_back(curr_w);
    }

    input.close();
    output.close();
    
    
    for (int i = 0; i < lines.size(); ++i) {
        stringstream extractor;
        extractor << lines[i];
        string temp;
        while (extractor >> temp) {
            codes.push_back(temp);
            types.push_back(get_type(temp));
            line_no.push_back(i + 1);
        }
    }
    
    output.open("interm.txt", ios::out);
    cout << "Intermediate Ouput:\n";

    for (int i = 0; i < lines.size(); ++i) {
        output << i + 1 << ' ';
        cout << i + 1 << ' ';
        for (int j = 0; j < line_no.size(); ++j) {
            if (i + 1 == line_no[j]) {
                if (types[j] == "id") {
                    output << types[j] << ' ';
                    cout << types[j] << ' ';
                }
                output << codes[j] << ' ';
                cout << codes[j] << ' ';
            }
        }
        output << '\n';
        cout << '\n';
    }
    output.close();
    cout << endl << "Errors:" << endl;

    for (int i = 0; i < codes.size() - 1; ++i) {
        if (codes[i] == ";" && codes[i + 1] == ";") {
            cout << "Duplicate token at line " << line_no[i] << endl; 
        }
    }

    stack<pair<string, int>> paren, print;

    for (int i = 0; i < codes.size(); ++i) {
        if (codes[i] == "(" || codes[i] == "{" || codes[i] == "[") {
            paren.push(make_pair(codes[i], line_no[i]));
        } else if (codes[i] == ")" && !paren.empty() && paren.top().first == "(") {
            paren.pop();
        } else if (codes[i] == "}" && !paren.empty() && paren.top().first == "{") {
            paren.pop();
        } else if (codes[i] == "]" && !paren.empty() && paren.top().first == "[") {
            paren.pop();
        } else if (codes[i] == ")" || codes[i] == "}" || codes[i] == "]") {
            paren.push(make_pair(codes[i], line_no[i]));
        }
    }
    
    while(!paren.empty()) {
        print.push(paren.top());
        paren.pop();
    }

    while (!print.empty()) {
        cout << "Misplaced " << print.top().first << " at line " << print.top().second << endl;
        print.pop();
    }

    stack<string> scope;
    scope.push("global");
    vector<map<string, string>> table;
    for (int i = 0; i < codes.size(); ++i) {
        if (codes[i] == "}" && scope.size() > 1) {
            scope.pop();
        }
        
        if (types[i] == "id" && (codes[i - 1] == "int" || codes[i - 1] == "double" || codes[i - 1] == "long" || codes[i - 1] == "char" || codes[i - 1] == "float")) {
            bool found = false;
            for (int j = 0; j < table.size(); ++j) {
                if (table[j]["name"] == codes[i] && table[j]["scope"] == scope.top()) {
                    cout << "Duplicate identifiers " << table[j]["name"] << " at line " << line_no[i] << endl;
                    found = true;
                }
            }
            if (!found) {
                map<string, string> curr;
                curr["name"] = codes[i];
                curr["scope"] = scope.top();
                table.push_back(curr);
            }
        } else if (types[i] == "id") {
            bool found = false;
            for (int j = 0; j < table.size(); ++j) {
                if (table[j]["name"] == codes[i] && (table[j]["scope"] == "global" || table[j]["scope"] == scope.top())) {
                    found = true;
                }
            }
            if (!found) {
                cout << "Undeclared identifiers " << codes[i] << " at line " << line_no[i] << endl;
            }
        }
        if (types[i] == "id" && codes[i + 1] == "(") {
            for (int j = i; j < codes.size(); ++j) {
                if (codes[j] == ")" && codes[j + 1] == "{") {
                    scope.push(codes[i]);
                    break;
                }
            }
        }
    }

    stack<string> ifs;
    for (int i = 0; i < codes.size(); ++i) {
        if (codes[i] == "if") {
            ifs.push("if");
        }
        else if (codes[i] == "else" && codes[i + 1] == "if" && !ifs.empty() && ifs.top() != "if") {
            cout << "Unmatched 'else if' at line " << line_no[i] << endl;
        } else if (codes[i] == "else") {
            if (!ifs.empty() && ifs.top() == "if") {
                ifs.pop();
            } else {
                cout << "Unmatched 'else' at line " << line_no[i] << endl;
            }
        }
    }

    vector<string> fors {"for", "(", ";", ";", ")", "{", "}"};
    stack<int> for_info;
    int pos = 0;
    for (int i = 0; i < codes.size(); ++i) {
        if (pos < fors.size() && fors[pos] == codes[i]) {
            if (pos == 0) {
                for_info.push(line_no[i]);
            }
            pos++;
        }
        if (pos >= fors.size()) {
            for_info.pop();
            pos = 0;
        }
    }

    while (!for_info.empty()) {
        cout << "Incomplete 'for' at line " << for_info.top() << endl;
        for_info.pop();
    }

    return 0;
}


