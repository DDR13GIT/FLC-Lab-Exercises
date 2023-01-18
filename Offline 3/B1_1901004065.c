#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define TABLE_SIZE 10
#define MAX_NAME 256

typedef struct {
    char name[MAX_NAME];
    int sl_no;
} Name;

typedef struct {
    char id_type[MAX_NAME];
    int sl_no;
} ID_type;

typedef struct {
    char data_type[MAX_NAME];
    int sl_no;
} DATA_type;

typedef struct {
    char scope[MAX_NAME];
    int sl_no;
} SCOPE;

typedef struct {
    char value[MAX_NAME];
    int sl_no;
} VALUE;

Name* name_table[TABLE_SIZE];
ID_type* id_type[TABLE_SIZE];
DATA_type* data_type[TABLE_SIZE];
SCOPE* scope[TABLE_SIZE];
VALUE* value[TABLE_SIZE];

int hash(char* name)
{
    int hash_value = 0,i;
    for ( i = 0; i <  MAX_NAME; i++) {
        hash_value += name[i];
        hash_value = (hash_value * name[i]) % 10;
    }
    return hash_value;
}

void free_table()
{
    int i;
    for ( i = 0; i < TABLE_SIZE; i++) {
        name_table[i] = NULL;
        id_type[i] = NULL;
        data_type[i] = NULL;
        scope[i] = NULL;
        value[i] = NULL;
    }
}

int sl_number = 0;
int entry_in_symol_table = 7;
void display()
{
    printf("\nStarting display\n");
    while (true)
    {
        sl_number++;
        int i;
        for ( i = 0; i < TABLE_SIZE; i++)
        {
            if (name_table[i] != NULL) {
                if (name_table[i]->sl_no == sl_number)
                {
                    printf("%d\t", sl_number);
                    printf("%s\t", name_table[i]->name);
                }
                if (id_type[i]->sl_no == sl_number)
                {
                    printf("%s\t", id_type[i]->id_type);
                }
                if (data_type[i]->sl_no == sl_number)
                {
                    printf("%s\t", data_type[i]->data_type);
                }
                if (scope[i]->sl_no == sl_number)
                {
                    printf("%s\t", scope[i]->scope);
                }
                if (value[i]->sl_no == sl_number)
                {
                    if ((strcmp(value[i]->value, "null"))) {
                        printf("%s\t", value[i]->value);
                    }
                    printf("\n");
                    break;
                }
            }
        }
        if (sl_number == entry_in_symol_table) {
            break;
        }
    }
    printf("\nEnding display\n");
}

Name* lookup(char* name)
{
    int index = hash(name);
    if (name_table[index] != NULL && !(strcmp(name_table[index]->name, name)))
    {
        return name_table[index];
    }
    else if (name_table[index] != NULL) {
            int i;
        for ( i = 0; i < TABLE_SIZE; i++) {
            int temp = (i + index) % TABLE_SIZE;
            if (name_table[temp] != NULL) {
                if (name_table[temp]->name == name) {
                    return name_table[temp];
                }
            }
        }
        return NULL;
    }
    else {
        return NULL;
    }
}

int lookup_func(char* name, char* sc)
{
    int index = hash(name);
    if (name_table[index] != NULL && !(strcmp(name_table[index]->name, name)) && !(strcmp(scope[index]->scope, sc)))
    {
        return index;
    }
    else if (name_table[index] != NULL) {
            int i;
        for ( i = 0; i < TABLE_SIZE; i++) {
            int temp = (i + index) % TABLE_SIZE;
            if (name_table[temp] != NULL) {
                if (!(strcmp(name_table[temp]->name, name)) && !(strcmp(scope[temp]->scope, sc))) {
                    return temp;
                }
            }
        }
        return 1111;
    }
    else {
        return 1111;
    }
}

bool insert(Name* n, DATA_type* dt, SCOPE* sc)
{
    if (n == NULL)
    {
        return false;
    }

    if (lookup_func(n->name, sc->scope) == 1111 && name_table[hash(n->name)] != NULL)
    {
        int index = hash(n->name);
        int i;
        for ( i = 0; i < TABLE_SIZE; i++) {
            int temp = (i + index) % TABLE_SIZE;
            if (name_table[temp] == NULL) {
                name_table[temp] = n;
                data_type[temp] = dt;
                scope[temp] = sc;
                return true;
            }
        }
        return false;
    }
    else {
        int index = hash(n->name);
        name_table[index] = n;
        data_type[index] = dt;
        scope[index] = sc;
        return true;
    }
}

int num_rec(char* lex)
{
    int s, i, l;
    i = 0;
    s = 0;
    if (isdigit(lex[i])) {
        s = 1;
        i++;
    }
    else if (lex[i] == '.') {
        s = 2;
        i++;
    }
    else {
        s = 0;
    }
    l = strlen(lex);
    if (s == 1) {
        for (; i < l; i++)
        {
            if (isdigit(lex[i])) {
                s = 1;
            }
            else if (lex[i] == '.') {
                s = 2;
                i++;
                break;
            }
            else {
                s = 0;
                break;
            }
        }
    }
    if (s == 2)
    {
        if (isdigit(lex[i])) {
            s = 3;
            i++;
        }
        else {
            s = 0;
        }
    }
    if (s == 3) {
        for (; i < l; i++)
        {
            if (isdigit(lex[i])) {
                s = 3;
            }
            else {
                s = 0;
                break;
            }
        }
    }
    if (s == 3) {
        s = 1;
    }
    return s;
}

int id_rec(char* lex)
{
    int i, s, l;
    i = 0;
    s = 0;
    if ((lex[i] >= 'a' && lex[i] <= 'z') || (lex[i] >= 'A' && lex[i] <= 'Z')) {
        s = 1;
    }
    else if (lex[i] == '_') {
        s = 1;
    }
    else {
        s = 0;
    }
    l = strlen(lex);
    if (s == 1)
    {
        for (; i < l; i++)
        {
            if (isdigit(lex[i])) {

            }
            else if (lex[i] == '_') {

            }
            else if ((lex[i] >= 'a' && lex[i] <= 'z') || (lex[i] >= 'A' && lex[i] <= 'Z')) {

            }
            else {
                s = 0;
                break;
            }
        }
    }
    return s;
}

int key_rec(char* lex)
{
    int s = 0;
    if (!(strcmp(lex, "char")) || !(strcmp(lex, "int")) ||
        !(strcmp(lex, "void")) || !(strcmp(lex, "double")) ||
        !(strcmp(lex, "return")) || !(strcmp(lex, "float")) ||
        !(strcmp(lex, "if")) || !(strcmp(lex, "else"))) {
        s = 1;
    }
    return s;
}

int sep_rec(char* lex)
{
    int s = 0;
    if (!(strcmp(lex, ",")) || !(strcmp(lex, ";")) || !(strcmp(lex, "\'"))) {
        s = 1;
    }
    return s;
}

int par_rec(char* lex)
{
    int s = 0;
    if (!(strcmp(lex, "(")) || !(strcmp(lex, ")")) || !(strcmp(lex, "{")) || !(strcmp(lex, "}"))) {
        s = 1;
    }
    return s;
}

int op_rec(char* lex)
{
    int s = 0;
    if (!(strcmp(lex, "=")) || !(strcmp(lex, "+")) || !(strcmp(lex, "-"))
        || !(strcmp(lex, "*")) || !(strcmp(lex, "/")) || !(strcmp(lex, "<="))) {
        s = 1;
    }
    return s;
}

void catagorized(FILE* p, char* lexeme, int length, FILE *p1)
{
    char lex[10];
    int i;
    for ( i = 0; i < length; i++) {
        lex[i] = lexeme[i];
    }
    lex[length] = '\0';
    if (key_rec(lex)) {
        fputc('[', p);
        fputs("kw", p);
        fputc(' ', p);
        fputs(lex, p);
        fputc(']', p);


        fputc('[', p1);
        fputs(lex, p1);
        fputc(']', p1);
    }
    else {
        if (id_rec(lex)) {
            fputc('[', p);
            fputs("id", p);
            fputc(' ', p);
            fputs(lex, p);
            fputc(']', p);

            fputc('[', p1);
            fputs("id", p1);
            fputc(' ', p1);
            fputs(lex, p1);
            fputc(']', p1);
        }
        else {
            if (num_rec(lex)) {
                fputc('[', p);
                fputs("num", p);
                fputc(' ', p);
                fputs(lex, p);
                fputc(']', p);

                fputc('[', p1);
                fputs(lex, p1);
                fputc(']', p1);
            }
            else {
                if (sep_rec(lex)) {
                    fputc('[', p);
                    fputs("sep", p);
                    fputc(' ', p);
                    fputs(lex, p);
                    fputc(']', p);

                    fputc('[', p1);
                    fputs(lex, p1);
                    fputc(']', p1);
                }
                else {
                    if (op_rec(lex)) {
                        fputc('[', p);
                        fputs("op", p);
                        fputc(' ', p);
                        fputs(lex, p);
                        fputc(']', p);

                        fputc('[', p1);
                        fputs(lex, p1);
                        fputc(']', p1);
                    }
                    else {
                        if (par_rec(lex)) {
                            fputc('[', p);
                            fputs("par", p);
                            fputc(' ', p);
                            fputs(lex, p);
                            fputc(']', p);

                            fputc('[', p1);
                            fputs(lex, p1);
                            fputc(']', p1);
                        }
                        else {
                            fputc('[', p);
                            fputs("unkn", p);
                            fputc(' ', p);
                            fputs(lex, p);
                            fputc(']', p);

                            fputc('[', p1);
                            fputs("unkn", p1);
                            fputc(' ', p1);
                            fputs(lex, p1);
                            fputc(']', p1);
                            printf("\nError : [ %s", lex);
                            printf(" ]\n");
                        }
                    }
                }
            }
        }
    }
}

int serial_number = 0;
void generation_of_symbol_table(char *lexeme, char *d, char *s, int len)
{
    char id[10];
    int i, j;
    for (i = 3, j = 0; i < len; i++, j++) {
        id[j] = lexeme[i];
    }
    id[j] = '\0';

    if (lookup_func(id, s) == 1111) {
        Name* name = (Name*)malloc(sizeof(Name));
        memcpy(name->name, id, sizeof(id));
        name->sl_no = ++serial_number;

        DATA_type* data = (DATA_type*)malloc(sizeof(DATA_type));
        memcpy(data->data_type, d, sizeof(d) + sizeof(d));
        data->sl_no = serial_number;

        SCOPE* scp = (SCOPE*)malloc(sizeof(SCOPE));
        memcpy(scp->scope, s, sizeof(s) + sizeof(s));
        scp->sl_no = serial_number;
        insert(name, data, scp);
    }
    /*else if (lookup(id) != NULL) {
        int index = hash(id);
        if (name_table[index] != NULL && !(strcmp(name_table[index]->name, id)))
        {
            if (!(strcmp(scope[index]->scope, s))) {

            }
            else {
                Name* name = (Name*)malloc(sizeof(Name));
                memcpy(name->name, id, sizeof(id));
                name->sl_no = ++serial_number;

                DATA_type* data = (DATA_type*)malloc(sizeof(DATA_type));
                memcpy(data->data_type, d, sizeof(d) + sizeof(d));
                data->sl_no = serial_number;

                SCOPE* scp = (SCOPE*)malloc(sizeof(SCOPE));
                memcpy(scp->scope, s, sizeof(s) + sizeof(s));
                scp->sl_no = serial_number;
                insert(name, data, scp);
            }
        }
        else if (name_table[index] != NULL) {
            for (int i = 0; i < TABLE_SIZE; i++) {
                int temp = (i + index) % TABLE_SIZE;
                if (name_table[temp] != NULL) {
                    if (name_table[temp] == id) {
                        if (!(strcmp(scope[temp]->scope, s))) {

                        }
                        else {
                            Name* name = (Name*)malloc(sizeof(Name));
                            memcpy(name->name, id, sizeof(id));
                            name->sl_no = ++serial_number;

                            DATA_type* data = (DATA_type*)malloc(sizeof(DATA_type));
                            memcpy(data->data_type, d, sizeof(d) + sizeof(d));
                            data->sl_no = serial_number;

                            SCOPE* scp = (SCOPE*)malloc(sizeof(SCOPE));
                            memcpy(scp->scope, s, sizeof(s) + sizeof(s));
                            scp->sl_no = serial_number;
                            insert(name, data, scp);
                        }
                    }

                }
            }
        }

    }*/
}

void update_symbol_table(char *lexeme, char *number, char *id_t, char *sc, int len)
{
    char id_st[10];
    int i, j;
    for (i = 3, j = 0; i < len; i++, j++) {
        id_st[j] = lexeme[i];
    }
    id_st[j] = '\0';
    int index = hash(id_st);
    if (name_table[index] != NULL && !(strcmp(name_table[index]->name, id_st)) && !(strcmp(scope[index]->scope, sc)))
    {
        ID_type* id = (ID_type*)malloc(sizeof(ID_type));
        memcpy(id->id_type, id_t, sizeof(id_t) + sizeof(id_t));
        id->sl_no = name_table[index]->sl_no ;
        id_type[index] = id;

        VALUE* v = (VALUE*)malloc(sizeof(VALUE));
        memcpy(v->value, number, sizeof(number) + sizeof(number));
        v->sl_no = name_table[index]->sl_no;
        value[index] = v;
    }
    else if (name_table[index] != NULL) {
            int i;
        for ( i = 0; i < TABLE_SIZE; i++) {
            int temp = (i + index) % TABLE_SIZE;
            if (name_table[temp] != NULL && !(strcmp(name_table[temp]->name, id_st)) && !(strcmp(scope[temp]->scope, sc))) {
                ID_type* id = (ID_type*)malloc(sizeof(ID_type));
                memcpy(id->id_type, id_t, sizeof(id_t) + sizeof(id_t));
                id->sl_no = name_table[temp]->sl_no;
                id_type[temp] = id;

                VALUE* v = (VALUE*)malloc(sizeof(VALUE));
                memcpy(v->value, number, sizeof(number) + sizeof(number));
                v->sl_no = name_table[temp]->sl_no;
                value[temp] = v;
            }
        }
    }
    else {
        return;
    }
}


int modify(char *name)
{
        int i;
        for ( i = 0; i < TABLE_SIZE; i++)
        {
            if (name_table[i] != NULL) {
                if (!(strcmp(name_table[i]->name, name)))
                {
                    return name_table[i]->sl_no;
                }

            }
        }
}

int main()
{
    FILE* p1, * p2, * p3, * p4;
    char c;
    char c2;// = c; //for no space after if
    p1 = fopen("input1.c", "r");
    p2 = fopen("input2.txt", "w");
    while ((c = fgetc(p1)) != EOF) {
        if (c == '\n') {
            c = fgetc(p1);
            if (c == ' ' || c == '\n') {
                while ((c = fgetc(p1)) == ' ') {}
                if ((c != '(' && c != ')' && c != '{' && c != '}') && (c2 != '(' && c2 != ')' && c2 != '{' && c2 != '}')) {
                    fputc(' ', p2); // for separating ==> "else c"
                }
                if (c == '/') {
                    c = fgetc(p1);
                    if (c == '/') {
                        while ((c = fgetc(p1)) != '\n') {}
                    }
                    else if (c == '*') {
                        while ((c = fgetc(p1)) != '/') {}
                    }
                }
                else {
                    fputc(c, p2);
                }
            }
            else if (c != '\n') {
                if (c == '/') {
                    char c1 = c;
                    c = fgetc(p1);
                    if (c == '/') {
                        while ((c = fgetc(p1)) != '\n') {}
                    }
                    else if (c == '*') {
                        while ((c = fgetc(p1)) != '/') {}
                    }
                    else {
                        fputc(c1, p2);
                        fputc(c, p2);
                    }
                }
                else {
                    fputc(c, p2);
                }
            }
        }
        else if (c == ' ') {
            fputc(c, p2);
            while ((c = fgetc(p1)) == ' ') {}
            fputc(c, p2);
        }
        else if (c == '/') {
            char c1 = c;
            c = fgetc(p1);
            if (c == '/') {
                while ((c = fgetc(p1)) != '\n') {}
            }
            else if (c == '*') {
                while ((c = fgetc(p1)) != '/') {}
            }
            else {
                fputc(c1, p2);
                fputc(c, p2);
            }
        }
        else if (c == '"') {
            fputc(c, p2);
            while ((c = fgetc(p1)) != '"')
            {
                fputc(c, p2);
            }
            fputc(c, p2);
        }
        else {
            fputc(c, p2);
            c2 = c;
        }
    }

    fclose(p1);
    fclose(p2);

    p2 = fopen("input2.txt", "r");
    p3 = fopen("output1.txt", "w");
    bool flag = false;
    bool space = false;
    bool just_one_space = false;
    bool already_space = false;
    while ((c = fgetc(p2)) != EOF)
    {
        if (c == ' ') {
            space = true;
            just_one_space = true;
        }
        if (c == ';' || c == ',' || c == '=' || c == '+' || c == '*' ||
            c == '-' || c == '/' || c == '(' || c == ')' || c == '<' ||
            c == '\'' || c == '{' || c == '}') {
            if (c == '<') {
                c = fgetc(p2);
                if (c == '=') {
                    fputc(' ', p3);
                    fputc('<', p3);
                    fputc('=', p3);
                    fputc(' ', p3);
                }
                else {
                    fputc(' ', p3);
                    fputc(c, p3);
                    fputc(' ', p3);
                }
            }
            else if (c == '{' || c == '}') {
                fputc(c, p3);
                fputc(' ', p3);
            }
            else {
                if (c == '\'') {
                    if (flag) {
                        fputc(' ', p3);
                        fputc(c, p3);
                        flag = false;
                    }
                    else {
                        fputc(c, p3);
                        fputc(' ', p3);
                        flag = true;
                    }
                }
                else {
                    if (!space) {
                        fputc(' ', p3);
                        fputc(c, p3);
                        fputc(' ', p3);
                        already_space = true;
                        space = true;
                        just_one_space = true;
                    }
                    else {
                        if (just_one_space && c == ';') {
                            fputc(c, p3);
                            fputc(' ', p3);
                            just_one_space = false;
                        }
                        else {
                            fputc(c, p3);
                        }
                        space = false;
                    }
                }
            }
        }
        else {
            if (c == ' ' && !already_space) {
                fputc(c, p3);
                already_space = false;
            }
            else if (c != ' ') {
                fputc(c, p3);
                already_space = false;
                just_one_space = false;
                space = false;
            }
            if (space && (((c >= 'a') && (c <= 'z')) || isdigit(c))) {
                space = false;
                just_one_space = false;
            }
        }
    }
    fclose(p2);
    fclose(p3);

    // Second Step
    p2 = fopen("output1.txt", "r");
    p3 = fopen("output2.txt", "w");
    p4 = fopen("output3.txt", "w");
    printf("\n");
    while (true) {
        char lexeme[10];
        int index = 0;
        if ((c = fgetc(p2)) == EOF) {
            break;
        }
        else {
            lexeme[index++] = c;
        }
        c = fgetc(p2);
        if (c == EOF) {
            break;
        }
        while ((c != ' ')) {
            lexeme[index++] = c;
            c = fgetc(p2);
        }
        catagorized(p3, lexeme, index, p4);
    }
    fclose(p2);
    fclose(p3);
    fclose(p4);

    printf("\nStep 1 Output: \n");
    p2 = fopen("output3.txt", "r");
    while ((c = fgetc(p2)) != EOF) {
        printf("%c", c);
    }
    fclose(p2);


    //Step 02 : Symbol table generation
    free_table();
    p1 = fopen("output3.txt", "r");
    char scope[7] = "global";
    char find_scope[10];
    char store_lexeme_name[10];
    int update_table_len = 0;
    bool identifier = false;
    bool identifier_for_scope = false;
    bool check_digit = true;
    while ((c = fgetc(p1)) != EOF) {
        if (c == '[') {
            char lexeme[10];
            char data_type[10];
            int i = 0;
            while (c != ']') {
                c = fgetc(p1);
                if (c == ' ') {
                    identifier = true;
                }
                if (c != ']') {
                    lexeme[i++] = c;
                }
            }
            if (identifier) {
                int j;
                for ( j = 0; j < i; j++) {
                    store_lexeme_name[j] = lexeme[j];
                }
                store_lexeme_name[i] = '\0';
                update_table_len = i;
                generation_of_symbol_table(lexeme, data_type, scope, i); //pass scope
                identifier = false;
                identifier_for_scope = true;
            }
            else {
                int j;
                for ( j = 0; j < i; j++) {
                    data_type[j] = lexeme[j];
                }
                data_type[i] = '\0';
                if (key_rec(data_type)) {
                    //it is a data type
                }
                else {
                    if (!(strcmp(data_type, ";"))) {
                        identifier_for_scope = false;
                    }
                    else if (!(strcmp(data_type, "="))) {
                        identifier_for_scope = false;
                        char num[10];
                        i = 0;
                        c = fgetc(p1);
                        while (c != ']') {
                            c = fgetc(p1);
                            if (c != ']') {
                                num[i++] = c;
                            }
                        }
                        num[i] = '\0';
                        if (num_rec(num)) {
                            update_symbol_table(store_lexeme_name, num, "var", scope, update_table_len);
                        }
                        else {
                            update_symbol_table(store_lexeme_name, "null", "var", scope, update_table_len);
                        }
                    }
                    else if (!(strcmp(data_type, "(")) && identifier_for_scope == true) {
                        update_symbol_table(store_lexeme_name, "null", "func", scope, update_table_len);
                        int k, j;
                        for (k = 0, j = 3; j < update_table_len; j++, k++) {
                            scope[k] = store_lexeme_name[j];
                        }
                        scope[k] = '\0';
                        identifier_for_scope = false;
                    }
                    else if (!(strcmp(data_type, "{")) && identifier_for_scope == true) {
                        identifier_for_scope = false;
                        update_symbol_table(store_lexeme_name, "null", "var", scope, update_table_len);
                    }
                    else if (!(strcmp(data_type, "}"))) {
                        scope[0] = 'g';scope[1] = 'l';scope[2] = 'o';scope[3] = 'b';
                        scope[4] = 'a';scope[5] = 'l';scope[6] = '\0';
                    }

                }
            }
        }
    }
    fclose(p1);
    display();


    //step 04 : Modified token stream for Syntax analysis

    p1 = fopen("output3.txt", "r");
    p2 = fopen("output4.txt", "w");
    while ((c = fgetc(p1)) != EOF) {
        fputc(c, p2);
        if (c == ' ') {
            int i = 0;
            char lex[10];
            while (c != ']')
            {
                c = fgetc(p1);
                if (c != ']')
                {
                    lex[i++] = c;
                }
            }
            lex[i] = '\0';
            int x = modify(lex);
            char buffer[10];
            _itoa(x, buffer, 10);
            fputs(buffer, p2);
            fputc(c, p2);
        }
    }
    fclose(p1);
    fclose(p2);

    printf("\nStep 4 Output: \n");
    p2 = fopen("output4.txt", "r");
    while ((c = fgetc(p2)) != EOF) {
        printf("%c", c);
    }
    fclose(p2);

    return 0;
}
