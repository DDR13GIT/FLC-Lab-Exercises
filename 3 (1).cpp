#include <bits/stdc++.h>
using namespace std;
#define fw setw(10)

vector<map<string, string>> table;

string int_to_string(int n) {
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

int main() {
    ifstream input("input.c", ios::in);
    ofstream output("res.txt", ios::out);
    vector<string> words, scopes {"global"};

    int cnt = 0;
    string s, curr;
    cout << "Input:" << endl << "------" << endl;
    while(input >> curr) {
        cout << curr << " ";
        if (curr[curr.length() - 1] == ']') {
            words.push_back(curr.substr(0, curr.length() - 1));
        }
        if (curr == "[id") {
            words.push_back(curr.substr(1, curr.length()));
        }
    }

    cout << endl << endl << "Step 1:" << endl << "-------" << endl;
    for (int i = 0; i < words.size(); ++i) {
        if (words[i] == "id") {
            cout << "[id " << words[++i] << "]"; 
        } else {
            cout << "[" << words[i] << "]";
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
                words[i + 1] = int_to_string(id_no);
            } else {
                int id_no = lookup(curr["name"], "global") + 1;
                words[i + 1] = int_to_string(id_no);
            }
        }
    }
    cout << endl << endl << "Modified Token Stream:" << endl << "----------------------" << endl;
    for (int i = 0; i < words.size(); ++i) {
        string to_write;
        if (words[i] == "id") {
            to_write = "[id " + words[++i] + "]";
        } else {
            to_write = "[" + words[i] + "]";
        }
        cout << to_write;
        output << to_write;
    }
    output.close();

    cout << endl << endl << "Symbol Table:" << endl << "------------" << endl;
    display();

}