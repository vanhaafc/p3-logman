// Project Identifier: 01BD41C3BF016AD7E8B6F837DF18926EC3261350
#include <algorithm>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <deque>
#include <cmath>

using namespace std;

struct LogEntry{
  int entry_id;
  uint64_t time_stamp;
  string og_timestamp;
  string lower_cat;
  string og_cat;
  string message;
};

struct LogCompare{
  bool operator()(const LogEntry &a, const LogEntry &b) const {
      if (a.time_stamp != b.time_stamp) return a.time_stamp < b.time_stamp;
      if (a.lower_cat != b.lower_cat) return a.lower_cat < b.lower_cat;
      return a.entry_id < b.entry_id;
  }
};

class Commands {
  private:
    ifstream in;

    vector<LogEntry> logs;
    vector<int> idToIndex;
    unordered_map<string, vector<int>> categoryMap;
    unordered_map<string, vector<int>> keywordMap;
    deque<int> excerpt;

    // for t and m is true, for c and k is false
    bool lastWasRange = false;
    // for t and m
    pair<size_t, size_t> range;
    // for c and k
    const vector<int>* list = nullptr;
    // for k, holds the intersection
    vector<int> keywordResults;
    bool searched = false;

    uint64_t timestampToInt(string ts){
      uint64_t result = 0;
      for (char c : ts) {
          if (c != ':') {
              result = result * 10 + static_cast<uint64_t>(c - '0');
          }
      }
      return result;
    }

    string lowercase(string s){
      for (char &c : s) {
        c = std::tolower(static_cast<unsigned char>(c));
      }

      return s;
    }

  public:
    // Constructor
    Commands(string file_name){
      in.open(file_name);
      if(in.fail()){
        cerr << "Could not open master log file";
        exit(1);
      }
    }

    void a(){
      int input;
      cin >> input;

      if(input >= static_cast<int>(logs.size()) || input < 0){
        cerr << "Invalid input\n";
        return;
      }
      
      int new_position = idToIndex[input];
      excerpt.push_back(new_position);

      cout << "log entry " << input << " appended\n";
    }

    void t(){
      string input;
      cin >> input;
      if(input.length() != 29 || input[14] != '|'){
        cerr << "Invalid timestamp length\n";
        return;
      }

      string t1, t2;
      t1 = input.substr(0, 14);
      t2 = input.substr(15, 14);
      uint64_t time1 = timestampToInt(t1);
      uint64_t time2 = timestampToInt(t2);

      auto lower = lower_bound(logs.begin(), logs.end(), time1,
        [](const LogEntry &a, uint64_t value) { return a.time_stamp < value;});
      auto upper = upper_bound(logs.begin(), logs.end(), time2,
        [](uint64_t value, const LogEntry &a) { return value < a.time_stamp;});

      int count = static_cast<int>(upper - lower);

      // Update recent searches
      searched = true;
      lastWasRange = true;
      range.first  = static_cast<size_t>(lower - logs.begin());
      range.second = static_cast<size_t>(upper - lower);
      
      cout << "Timestamps search: " << count << " entries found\n";
    }

    void m(){
      string input;
      cin >> input;
      if(input.length() != 14) {
        cerr << "Invalid timestamp length\n";
        return;
      }

      uint64_t t = timestampToInt(input);
      auto lower = lower_bound(logs.begin(), logs.end(), t,
        [](const LogEntry &a, uint64_t value) { return a.time_stamp < value;});
      auto upper = upper_bound(logs.begin(), logs.end(), t,
        [](uint64_t value, const LogEntry &a) { return value < a.time_stamp;});

      int count = static_cast<int>(upper - lower);

      // Update recent searches
      searched = true;
      lastWasRange = true;
      range.first  = static_cast<size_t>(lower - logs.begin());
      range.second = static_cast<size_t>(upper - lower);

      cout << "Timestamp search: " << count << " entries found\n";

    }

    void c(){
      string input;
      getline(cin >> ws, input);

      input = lowercase(input);

      if(auto iter = categoryMap.find(input); iter != categoryMap.end()){
        cout << "Category search: " << iter->second.size() << " entries found\n"; 
        list = &iter->second;
      }
      else{
        cout << "Category search: 0 entries found\n"; 
        list = nullptr;
      }

      searched = true;
      lastWasRange = false;
    }

    void p(){
      for(size_t i = 0; i < excerpt.size(); i++){
        int pos = excerpt[i];
        cout << i << "|" << logs[pos].entry_id << "|" << logs[pos].og_timestamp << 
          "|" << logs[pos].og_cat << "|" << logs[pos].message << "\n";
      }
    }

    void g(){
      // check if a search happened
      if(!searched){
        cerr << "No search has happened\n";
        return;
      }
      if(lastWasRange){
        for(size_t i = range.first; i < range.first + range.second; i++){
          cout << logs[i].entry_id << "|" << logs[i].og_timestamp << 
            "|" << logs[i].og_cat << "|" << logs[i].message << "\n";
        }
      }
      else{
        //do g() for c and k
        if(list == nullptr) return;
          
        for(size_t i = 0; i < list->size(); i++){
          int pos = (*list)[i];
          cout << logs[pos].entry_id << "|" << logs[pos].og_timestamp << 
            "|" << logs[pos].og_cat << "|" << logs[pos].message << "\n";
        }
      }
    }

    // TODO
    void d();

    // TODO
    void b();

    // TODO
    void e();

    // TODO
    void l();

    // TODO
    void s();

    // TODO
    void r();

    // TODO: Needs set_intersection() helper function?
    // TODO
    void k();

    void read(){
      // Read log file
      string ts, cat, msg;
      int count = 0;
      while(getline(in, ts, '|')){
        getline(in, cat, '|');
        getline(in, msg);

        // make ts into an integer
        uint64_t t = timestampToInt(ts);

        // make cat all lowercase (better for comparing)
        string new_cat = lowercase(cat);

        LogEntry l;
        l.time_stamp = t;
        l.og_timestamp = ts;
        l.og_cat = cat;
        l.lower_cat = new_cat;
        l.message = msg;
        l.entry_id = count;

        logs.push_back(l);
        count++;
      }
      cout << count << " entries read\n";

      // TODO: sort the master log on timestamp, then category
      sort(logs.begin(), logs.end(), LogCompare());

      // Holds ID's and where they ended up after sorting
      idToIndex.resize(logs.size());
      for (size_t i = 0; i < logs.size(); ++i) {
          idToIndex[logs[i].entry_id] = static_cast<int>(i);
      }

      // Make Category map
      for (size_t i = 0; i < logs.size(); ++i) {
        categoryMap[logs[i].lower_cat].push_back(logs[i].entry_id);
      }

      // Make keyword map
      // TODO
      
      // Read commands
      char cmd;
      do {
        cout << "% ";
        cin >> cmd;
        if (cin.fail()) {
          cerr << "cin entered fail state: exiting" << endl;
          exit(1);
        } // if
        // TODO: get command argument(s) if necessary
        // TODO: Process command (call function)
        // TODO: check if invalid command character

        // Miscellaneous Commands
        if(cmd == '#'){
          string ignore;
          getline(cin, ignore);
        }
        // Searching Commands
        else if(cmd == 'k') k();
        else if(cmd == 't') t();
        else if(cmd == 'm') m();
        else if(cmd == 'c') c();

        // Excerpt List Editing Commands
        else if(cmd == 'a') a();
        else if(cmd == 'r') r();
        else if(cmd == 'd') d();
        else if(cmd == 'b') b();
        else if(cmd == 'e') e();
        else if(cmd == 's') s();
        else if(cmd == 'l') l();

        // Output Commands
        else if(cmd == 'g') g();
        else if(cmd == 'p') p();

      } while (cmd != 'q');
    }
};