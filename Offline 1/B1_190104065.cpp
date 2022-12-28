// author: Debopriya Deb Roy
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

string i;
long long int n = 0;

void printOriginal()
{
    char c;
    FILE *fp, *fp2;
    fp2 = fopen("input.txt", "r");
    while ((c = fgetc(fp2)) != EOF)
    {
        printf("%c", c);
    }
    fclose(fp2);
}

void assignment1Answer()
{
    // reading input from this file
    ifstream read;
    read.open("input1.c");
    ofstream write;
    // creating this file to write the changes after compilattion
    write.open("output.txt");
    bool commentFlag = false;
    while (read.eof() == 0)
    {
        getline(read, i);
        for (n = 0; n < i.length(); n++)
        {
            // checking here for single line comment
            if (i[n] == '/' && i[n + 1] == '/')
                commentFlag = true;
            // checking here for multi line comment
            else if (i[n] == '/' && i[n + 1] == '*')
                commentFlag = true;
            // checking here for multi line comment and if the comment finishes, the finishing syntax will not be printed, thats why i have used break
            else if (i[n] == '*' && i[n + 1] == '/')
            {
                commentFlag = false;
                break;
            }
            // checking here for newline escape sequence
            else if (i[n] == '/' && i[n + 1] == 'n')
                break;
            // checking here for tab escape sequence
            else if (i[n] == '/' && i[n + 1] == 't')
                break;

            // printing based on the comment flag without considering any white spaces
            if (commentFlag == false && i[n] != ' ')
            {
                write << i[n];
                cout << i[n];
            }
        }
    }
}

int main()
{
    printOriginal();
    cout << endl;
    assignment1Answer();
    return 0;
}

// save the content below as input.c
// #include <stdio.h>

// int     main(void)
// {

//     // Single Line Comment
//     printf("   Hello");
//     /* Multi
//         Line

//                     Comment

//     */
//     printf("World");
//     return 0;
// }