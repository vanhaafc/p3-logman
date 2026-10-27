// Project Identifier: 01BD41C3BF016AD7E8B6F837DF18926EC3261350
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include <getopt.h>

using namespace std;

// Everything the command line can change about a run. The defaults here are
// the spec's defaults, so an option that is never given just keeps its value.
struct Options {
    string input_file;
};  // Options{}


// Print help for the user when requested.
// argv[0] is the name of the currently executing program
void printHelp(char *command) {
    cout << "Usage: " << command << " [options] < inputfile\n";
    cout << "Options:\n";
    cout << "  -h, --help                     Print this message and exit\n";
    cout << "  -f, --file                     Includes the name of the input file\n";
}  // printHelp()

// Member functions if needed

// Process the command line; there is no return value, but the Options
// struct is passed by reference and is modified by this function to send
// information back to the calling function.
void getOptions(int argc, char **argv, Options &options) {
    // These are used with getopt_long()
    opterr = static_cast<int>(false);  // Let us handle all error output for command line options
    int choice = 0;
    int index = 0;

    // NOLINTBEGIN: getopt predates C++ style, this usage is from `man getopt`
    option longOptions[] = {
        {"help",           no_argument, nullptr, 'h'},
        {"file",           required_argument, nullptr, 'f'},
        {nullptr,      0,               nullptr, '\0'},
    };  // longOptions[]
    // NOLINTEND

    while ((choice = getopt_long(argc, argv, "hf:", static_cast<option *>(longOptions), &index)) != -1) {
        switch (choice) {
        case 'h':
            printHelp(*argv);
            exit(0);

        case 'f':
            //TODO: get the input file
            //options.input_file = 
            break;

        default:
            cerr << "Error: Unknown command line option\n";
            exit(1);

        }
    }
}  // getOptions()

int main(int argc, char *argv[]){
  ios_base::sync_with_stdio(false);

  Options options;
  getOptions(argc, argv, options);

  // TODO: finish main
  // Read cmd (switch this to after log file)
  char cmd;
  do {
    cout << "% ";
    cin >> cmd;
    // TODO: Process cmd
  } while (cmd != 'q');

  // Read log file
  string ts, cat, msg;
  while(getline(cin, ts, '|')){
    getline(cin, cat, '|');
    getline(cin, msg);
    // TODO: add to data structure?
  }

  return 0;
} // main()