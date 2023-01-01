#include "turing_machine.hpp"
using namespace std;
bool verbose;
int main(int argc, char *argv[])
{
    if (argc <= 1 || argc >= 5)
    {
        cerr << "syntax error" << endl;
        cerr << "Wrong number of parameters" << endl;
        exit(1);
    }
    verbose = false;
    int index = 1;
    if ((strcmp(argv[index], "-h") == 0) || (strcmp(argv[index], "--help") == 0))
    {
        cout << "usage: turing [-v|--verbose] [-h|--help] <tm> <input>\n"
             << endl;
        exit(0);
    }
    if ((strcmp(argv[index], "-v") == 0) || (strcmp(argv[index], "--verbose") == 0))
    {
        verbose = true;
        index++;
    }
    parser(argv[index]);
    index++;
    simulator(argv[index]);
    exit(0);
}