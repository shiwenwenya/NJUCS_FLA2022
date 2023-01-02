#include "turing_machine.hpp"

#define ERR "==================== ERR ===================="
#define END "==================== END ===================="
#define RUN "==================== RUN ===================="

vector<string> Q;        // 状态集 Q
vector<char> S;          // 输入符号集 S
vector<char> G;          // 纸带符号集 G
string q0;               // 初始状态 q0
char B;                  // 空格符号 B
vector<string> F;        // 终结状态集 F
int N;                   // 纸带数 N
vector<trasition> delta; // 转移函数 delta

int step;                    // 步骤数
string state;                // 当前状态
vector<int> head_index;      // 读写头索引
vector<int> left_index;      // 最左非空格符号索引
vector<int> right_index;     // 最右非空格符号索引 + 1
vector<map<int, char>> tape; // 纸带（索引，符号）

void parser(string TM)
{
    if (TM == "")
    {
        cerr << "syntax error" << endl;
        if (verbose)
            cerr << "Please enter the TM's name" << endl;
        exit(1);
    }
    ifstream input;
    input.open(TM);
    if (input.fail())
    {
        cerr << "syntax error" << endl;
        if (verbose)
            cerr << "TM file not exists" << endl;
        exit(1);
    }
    string tmp;
    while (getline(input, tmp))
    {
        if (tmp != "" && tmp[0] != ';')
        {
            handle_line_comment(tmp);
            if (tmp[0] == '#' && tmp[1] == 'Q')
            {
                parser_Q(tmp);
            }
            else if (tmp[0] == '#' && tmp[1] == 'S')
            {
                parser_S(tmp);
            }
            else if (tmp[0] == '#' && tmp[1] == 'G')
            {
                parser_G(tmp);
            }
            else if (tmp[0] == '#' && tmp[1] == 'q' && tmp[2] == '0')
            {
                parser_q0(tmp);
            }
            else if (tmp[0] == '#' && tmp[1] == 'B')
            {
                parser_B(tmp);
            }
            else if (tmp[0] == '#' && tmp[1] == 'F')
            {
                parser_F(tmp);
            }
            else if (tmp[0] == '#' && tmp[1] == 'N')
            {
                parser_N(tmp);
            }
            else if (count(tmp.begin(), tmp.end(), ' ') == 4)
            {
                parser_delta(tmp);
            }
            else
            {
                cerr << "syntax error" << endl;
                if (verbose)
                    cerr << "TM file incorrect format" << endl;
                exit(1);
            }
        }
    }
    input.close();
}
void parser_Q(string input)
{
    if (input[2] == ' ' && input[3] == '=' && input[4] == ' ' && input[5] == '{' && input[input.size() - 1] == '}')
    {
        input.erase(0, 6);
        input.erase(input.size() - 1, 1);
        string strs = input + ',';
        size_t pos = strs.find(',');
        while (pos != strs.npos)
        {
            string temp = strs.substr(0, pos);
            Q.push_back(temp);
            strs = strs.substr(pos + 1, strs.size());
            pos = strs.find(',');
        }
    }
    else
    {
        cerr << "syntax error" << endl;
        if (verbose)
            cerr << "States incorrect format" << endl;
        exit(2);
    }
}
void parser_S(string input)
{
    if (input[2] == ' ' && input[3] == '=' && input[4] == ' ' && input[5] == '{' && input[input.size() - 1] == '}')
    {
        input.erase(0, 6);
        input.erase(input.size() - 1, 1);
        for (int i = 0; i < input.size(); i++)
        {
            if (i % 2 == 0)
            {
                S.push_back(input[i]);
            }
            else
            {
                if (input[i] != ',')
                {
                    cerr << "syntax error" << endl;
                    if (verbose)
                        cerr << "Input symbols incorrect format" << endl;
                    exit(3);
                }
            }
        }
    }
    else
    {
        cerr << "syntax error" << endl;
        if (verbose)
            cerr << "Input symbols incorrect format" << endl;
        exit(3);
    }
}
void parser_G(string input)
{
    if (input[2] == ' ' && input[3] == '=' && input[4] == ' ' && input[5] == '{' && input[input.size() - 1] == '}')
    {
        input.erase(0, 6);
        input.erase(input.size() - 1, 1);
        for (int i = 0; i < input.size(); i++)
        {
            if (i % 2 == 0)
            {
                G.push_back(input[i]);
            }
            else
            {
                if (input[i] != ',')
                {
                    cerr << "syntax error" << endl;
                    if (verbose)
                        cerr << "Tape symbols incorrect format" << endl;
                    exit(4);
                }
            }
        }
    }
    else
    {
        cerr << "syntax error" << endl;
        if (verbose)
            cerr << "Tape symbols incorrect format" << endl;
        exit(4);
    }
}
void parser_q0(string input)
{
    if (input[3] == ' ' && input[4] == '=' && input[5] == ' ' && input[6] != '\0')
    {
        input.erase(0, 6);
        q0 = input;
        if (find(Q.begin(), Q.end(), q0) == Q.end())
        {
            cerr << "syntax error" << endl;
            if (verbose)
                cerr << "Initial state not exists in Q" << endl;
            exit(5);
        }
    }
    else
    {
        cerr << "syntax error" << endl;
        if (verbose)
            cerr << "Initial state incorrect format" << endl;
        exit(5);
    }
}
void parser_B(string input)
{
    if (input[2] == ' ' && input[3] == '=' && input[4] == ' ' && input[5] != '\0')
    {
        input.erase(0, 5);
        B = input[0];
        if (B != '_')
        {
            cerr << "syntax error" << endl;
            if (verbose)
                cerr << "Blank symbol is not _" << endl;
            exit(6);
        }
        if (find(G.begin(), G.end(), B) == G.end())
        {
            cerr << "syntax error" << endl;
            if (verbose)
                cerr << "Blank symbol not exists in G" << endl;
            exit(6);
        }
    }
    else
    {
        cerr << "syntax error" << endl;
        if (verbose)
            cerr << "Blank symbol incorrect format" << endl;
        exit(6);
    }
}
void parser_F(string input)
{
    if (input[2] == ' ' && input[3] == '=' && input[4] == ' ' && input[5] == '{' && input[input.size() - 1] == '}')
    {
        input.erase(0, 6);
        input.erase(input.size() - 1, 1);
        string strs = input + ',';
        size_t pos = strs.find(',');
        while (pos != strs.npos)
        {
            string temp = strs.substr(0, pos);
            if (find(Q.begin(), Q.end(), temp) == Q.end())
            {
                cerr << "syntax error" << endl;
                if (verbose)
                    cerr << "Final state " << temp << " not exists in Q" << endl;
                exit(7);
            }
            F.push_back(temp);
            strs = strs.substr(pos + 1, strs.size());
            pos = strs.find(',');
        }
    }
    else
    {
        cerr << "syntax error" << endl;
        if (verbose)
            cerr << "Final state incorrect format" << endl;
        exit(7);
    }
}
void parser_N(string input)
{
    if (input[2] == ' ' && input[3] == '=' && input[4] == ' ' && input[5] != '\0')
    {
        input.erase(0, 5);
        N = atoi(input.c_str());
    }
    else
    {
        cerr << "syntax error" << endl;
        if (verbose)
            cerr << "Number of tapes incorrect format" << endl;
        exit(8);
    }
}
void parser_delta(string input)
{
    vector<string> result;
    string strs = input + ' ';
    size_t pos = strs.find(' ');
    while (pos != strs.npos)
    {
        string temp = strs.substr(0, pos);
        result.push_back(temp);
        strs = strs.substr(pos + 1, strs.size());
        pos = strs.find(' ');
    }
    trasition new_trasition;
    if (find(Q.begin(), Q.end(), result[0]) == Q.end())
    {
        cerr << "syntax error" << endl;
        if (verbose)
            cerr << "Old state " << result[0] << " not exists in Q" << endl;
        exit(9);
    }
    new_trasition.old_state = result[0];
    if (result[1].size() != N)
    {
        cerr << "syntax error" << endl;
        if (verbose)
            cerr << result[1] << "'s length is not N" << endl;
        exit(9);
    }
    for (int i = 0; i < N; i++)
    {
        if (find(G.begin(), G.end(), result[1][i]) == G.end() && result[1][i] != '*')
        {
            cerr << "syntax error" << endl;
            if (verbose)
                cerr << "Old symbol " << result[1][i] << " not exists in G" << endl;
            exit(9);
        }
    }
    new_trasition.old_symbol_set = result[1];
    if (result[2].size() != N)
    {
        cerr << "syntax error" << endl;
        if (verbose)
            cerr << result[2] << "'s length is not N" << endl;
        exit(9);
    }
    for (int i = 0; i < N; i++)
    {
        if (find(G.begin(), G.end(), result[2][i]) == G.end() && result[2][i] != '*')
        {
            cerr << "syntax error" << endl;
            if (verbose)
                cerr << "New symbol " << result[2][i] << " not exists in G" << endl;
            exit(9);
        }
    }
    new_trasition.new_symbol_set = result[2];
    if (result[3].size() != N)
    {
        cerr << "syntax error" << endl;
        if (verbose)
            cerr << result[3] << "'s length is not N" << endl;
        exit(9);
    }
    if (count(result[3].begin(), result[3].end(), 'l') + count(result[3].begin(), result[3].end(), 'r') + count(result[3].begin(), result[3].end(), '*') != N)
    {
        cerr << "syntax error" << endl;
        if (verbose)
            cerr << "Direction " << result[3] << " should be l or r or *'s string" << endl;
        exit(9);
    }
    new_trasition.direction_set = result[3];
    if (find(Q.begin(), Q.end(), result[4]) == Q.end())
    {
        cerr << "syntax error" << endl;
        if (verbose)
            cerr << "New state " << result[4] << " not exists in Q" << endl;
        exit(9);
    }
    new_trasition.new_state = result[4];
    delta.push_back(new_trasition);
}
void handle_line_comment(string &input)
{
    int i = 0;
    for (; i < input.size(); i++)
    {
        if (input[i] == ';')
        {
            break;
        }
    }
    if (input[i] == ';')
    {
        input.erase(i - 1, input.size() - i + 1);
    }
}
void simulator(string input)
{
    check_input_string(input);
    if (verbose)
    {
        cout << "Input: " << input << endl;
        cout << RUN << endl;
    }
    /* 初始化 */
    step = 0;
    state = q0;
    head_index.resize(N);
    left_index.resize(N);
    right_index.resize(N);
    tape.resize(N);
    if (input.size() == 0)
    {
        tape[0][0] = '_';
        right_index[0] = 1;
    }
    else
    {
        for (int i = 0; i < input.size(); i++)
        {
            tape[0][i] = input[i];
        }
        right_index[0] = input.size();
    }
    for (int i = 1; i < N; i++)
    {
        tape[i][0] = '_';
        right_index[i] = 1;
    }
    /* 运行 */
    while (true)
    {
        if (verbose)
            print_instant_description();
        string symbol;
        for (int i = 0; i < N; i++)
        {
            symbol.push_back(tape[i][head_index[i]]);
        }
        trasition cur_trasition; // 当前转移函数
        int index = 0;
        for (; index < delta.size(); index++)
        {
            if (state.compare(delta[index].old_state) == 0 && symbol_compare(symbol, delta[index].old_symbol_set) == 0)
            {
                cur_trasition = delta[index];
                break;
            }
        }
        if (index == delta.size())
            break; // halt
        state = cur_trasition.new_state;
        for (int i = 0; i < N; i++)
        {
            if (cur_trasition.new_symbol_set[i] != '*')
            {
                tape[i][head_index[i]] = cur_trasition.new_symbol_set[i];
            }
            if (cur_trasition.direction_set[i] == 'l') // 左移
            {
                head_index[i]--;
                if (head_index[i] < left_index[i])
                {
                    tape[i][head_index[i]] = '_';
                    left_index[i] = head_index[i];
                } // 读写头索引不能小于最左非空格符号索引
                for (int j = right_index[i] - 1; j > head_index[i]; j--)
                {
                    if (tape[i][j] == '_')
                    {
                        right_index[i]--;
                    }
                } // 处理最右非空格符号索引
            }
            else if (cur_trasition.direction_set[i] == 'r')
            {
                head_index[i]++;
                if (head_index[i] >= right_index[i])
                {
                    tape[i][head_index[i]] = '_';
                    right_index[i] = head_index[i] + 1;
                } // 读写头索引不能大于等于最右非空格符号索引
                for (int j = left_index[i]; j < head_index[i]; j++)
                {
                    if (tape[i][j] == '_')
                    {
                        left_index[i]++;
                    }
                } // 处理最左非空格符号索引
            }
        }
        step++;
    }
    string result;
    for (int i = left_index[0]; i < right_index[0]; i++)
    {
        result.push_back(tape[0][i]);
    }
    if (verbose)
        cout << "Result: " << result << endl;
    else
        cout << result << endl;
    cout << END << endl;
}
void check_input_string(string input)
{
    for (int i = 0; i < input.size(); i++)
    {
        if (find(S.begin(), S.end(), input[i]) == S.end())
        {
            cerr << "illegal input" << endl;
            if (verbose)
            {
                cerr << "Input: " << input << endl;
                cerr << ERR << endl;
                cerr << "error: '" << input[i] << "' was not declared in the set of input symbols" << endl;
                cerr << "Input: " << input << endl;
                print_spaces(i + 7);
                cerr << "^" << endl;
                cerr << END << endl;
            }
            exit(10);
        }
    }
}
void print_spaces(int count)
{
    for (int i = 0; i < count; i++)
    {
        printf(" ");
    }
}
void print_instant_description()
{
    cout << "Step\t: " << step << endl;
    cout << "State\t: " << state << endl;
    for (int i = 0; i < N; i++)
    {
        cout << "Index" << i << "\t:";
        for (int j = left_index[i]; j < right_index[i]; j++)
        {
            cout << " " << abs(j);
        }
        cout << endl;
        cout << "Tape" << i << "\t:";
        for (int j = left_index[i]; j < right_index[i]; j++)
        {
            int cnt = 1; // index位数，用于对齐
            int num = j;
            while (num /= 10)
            {
                cnt++;
            }
            cout << " " << tape[i][j];
            print_spaces(cnt - 1);
        }
        cout << endl;
        cout << "Head" << i << "\t:";
        for (int j = left_index[i]; j < right_index[i]; j++)
        {
            int cnt = 1; // index位数，用于对齐
            int num = j;
            while (num /= 10)
            {
                cnt++;
            }
            if (j == head_index[i])
            {
                cout << " ^" << endl;
                print_spaces(cnt - 1);
                break;
            }
            print_spaces(2);
            print_spaces(cnt - 1);
        }
    }
    cout << "---------------------------------------------" << endl;
}
int symbol_compare(string a, string b) // b中可能有*
{
    if (a.size() != b.size())
        static_assert(1); // 不会出现size不一致
    for (int i = 0; i < a.size(); i++)
    {
        if (b[i] == '*')
        {
            if (a[i] == '_')
                return -1;
        }
        else
        {
            if (a[i] != b[i])
            {
                return -1;
            }
        }
    }
    return 0;
}