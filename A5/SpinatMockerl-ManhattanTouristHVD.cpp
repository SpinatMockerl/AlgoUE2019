#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <map>
#include <unistd.h>
#include <algorithm>
using namespace std;

/*

To Do:
Vor isatty auf -h testen, um Help-MSG auszugeben.
Überflüssiges Input entfernen.

*/

typedef vector<vector<int>> Matrix;
typedef vector<vector<string>> Fasta;

map<string, int> parseArgs(int argc, char** argv);
Fasta readFasta();
Matrix scoreNW(string colSeq, string rowSeq, map<string, int> scoringFun);
map<string, string> backtrackNW(vector<string> colSeq, vector<string> rowSeq,
  Matrix scores, map<string, int> scoringFun);
void printAlignment(map<string, string> alignment);
bool testForInt(string number);

int main(int argc, char** argv) {
  map<string, int> scoringFun = parseArgs(argc, argv);

  vector<vector<string>> seqs = readFasta();
  
  Matrix scoringMatrix = scoreNW(seqs[0][1], seqs[1][1], scoringFun);

  map <string, string> alignment = backtrackNW(seqs[0], seqs[1],
    scoringMatrix, scoringFun);
  
  printAlignment(alignment);

  cerr << scoringMatrix[scoringMatrix.size() -1][scoringMatrix[0].size() -1]
    << endl;

  return 0;
}

map<string, int> parseArgs(int argc, char** argv) {
  int bufferSize = 100;
  
  char usage[bufferSize];

  snprintf(usage, bufferSize, "Usage: %s [options] < input_file.fasta",
    argv[0]);

  char more[bufferSize];
  snprintf(more, bufferSize, "Type %s -h | --help for further information",
    argv[0]);
  string help = "Align 2 sequences from one .fasta file using the ";
  help += "Needleman-Wunsch-Algorithm, and output it in CLUSTAL-format. The ";
  help += "Scoring function may be changed using the --match, --mismatch or ";
  help += "--gap options, followed by an integer. Only the first 2 sequences ";
  help += "of the file are used, regardless how many are contained therein.";

  map<string, int> scoringFun = {{"match", 1}, {"mismatch", -1}, {"gap", -2}};

  for (int i = 1; i < argc; i++) {
    string arg = (string)argv[i];
    bool NaN = false;

    if (arg == "--help" || arg == "-h") {
      cout << usage << endl;
      cout << help << endl;
      exit(EXIT_SUCCESS);

    } else if (arg == "--match") {
      if (testForInt(argv[i +1])) {
        scoringFun["match"] = stoi(argv[i +1]);
      } else {
        NaN = true;
      }

    } else if (arg == "--mismatch") {
      if (testForInt(argv[i +1])) {
        scoringFun["mismatch"] = stoi(argv[i +1]);
      } else {
        NaN = true;
      }
    } else if (arg == "--gap") {
      if (testForInt(argv[i +1])) {
        scoringFun["gap"] = stoi(argv[i +1]);
      } else {
        NaN = true;
      }
    }
    
    if (NaN) {
        cerr << "ERROR: A whole number needs to be provided after " <<
          "--match, --mismatch and --gap." << endl << "Aborting" << endl;
        exit(EXIT_FAILURE);
    }
  }

  /* Tests, whether input is coming from a terminal; if so, program is 
  terminated. */
  if (isatty(STDIN_FILENO) == 1) {
    cerr << usage << endl;
    cerr << more << endl;
    exit(EXIT_FAILURE);
  }

  return scoringFun;
}

Fasta readFasta() {
  string line;
  string seq;
  Fasta seqs;
  string header;
  bool firstLine = true;

  int seqCount = 0;

  while (getline(cin, line)) {
    if (line[0] == '>') {
      seqCount++;

      if (seqCount > 2) {
        cerr << endl << "WARNING: Input file contains more than 2 sequences. "
          << "Only the first 2 are used in the alignment." << endl << endl;
        break;
      }
      vector<string> compoundSeq = {header, seq};
      
      if (! firstLine) {
        seqs.push_back(compoundSeq);
      }
      
      // Set to next Sequence:
      header = line.substr(1, line.size() -1);
      seq = "";
    } else if (line[0] == ';') {
      continue;
    } else {
      seq += line;
    }

    firstLine = false;
  }
  
  vector<string> compoundSeq = {header, seq};
  seqs.push_back(compoundSeq);

  return seqs;
}

Matrix scoreNW(string colSeq, string rowSeq, map<string, int> scoringFun) {
  Matrix scores(rowSeq.size() +1, vector<int>(colSeq.size() +1, 0));

  for (int row = 0; row < rowSeq.size() +1; row++) {
    for (int col = 0; col < colSeq.size() +1; col++) {

      if (row == 0 && col == 0) {
        continue;

      } else if (row == 0) {
        scores[row][col] = scores[row][col -1] + scoringFun["gap"];

      } else if (col == 0) {
        scores[row][col] = scores[row -1] [col] + scoringFun["gap"];

      } else {
        int down = scores[row -1][col] + scoringFun["gap"];
        int right = scores[row][col -1] + scoringFun["gap"];
        
        int diagonal = scores[row -1][col -1]; // diagonally preceeding score
        if (rowSeq[row -1] == colSeq[col -1]) { // current base
          diagonal += scoringFun["match"];

        } else {
          diagonal += scoringFun["mismatch"];
        }

        scores[row][col] = max(max(down, right), diagonal);
      }
    }
  }

  return scores;
}

map<string, string> backtrackNW(vector<string> colSeq, vector<string> rowSeq, Matrix scores, map<string, int> scoringFun) {
  vector<char> rowAln;
  vector<char> colAln;
  vector<char> seqDecoration;

  int row = scores.size() -1;
  int col = scores[0].size() -1;

  while (row > 0) {
    if (col < 1) {
      break;
    }

    while (col > 0) {
      if (row < 1) {
        break;
      }

      if (scores[row][col] == scores[row -1][col -1] + scoringFun["match"]
          && rowSeq[1][row -1] == colSeq[1][col -1]) {
        rowAln.insert(rowAln.begin(), rowSeq[1][row -1]);
        colAln.insert(colAln.begin(), colSeq[1][col -1]);
        seqDecoration.insert(seqDecoration.begin(), '*');

        row--;
        col--;

      } else if (scores[row][col] == scores[row -1][col -1] +
                 scoringFun["mismatch"]) {
        rowAln.insert(rowAln.begin(), rowSeq[1][row -1]);
        colAln.insert(colAln.begin(), colSeq[1][col -1]);
        seqDecoration.insert(seqDecoration.begin(), ' ');

        row--;
        col--;

      } else if (scores[row][col] == scores[row][col -1] + scoringFun["gap"]) {
        rowAln.insert(rowAln.begin(), '-');
        colAln.insert(colAln.begin(), colSeq[1][col -1]);
        seqDecoration.insert(seqDecoration.begin(), ' ');

        col--;

      } else if (scores[row][col] == scores[row -1][col] + scoringFun["gap"]) {
        rowAln.insert(rowAln.begin(), rowSeq[1][row -1]);
        colAln.insert(colAln.begin(), '-');
        seqDecoration.insert(seqDecoration.begin(), ' ');

        row--;
      }
    }
  }

  while (row > 0) {
    rowAln.insert(rowAln.begin(), rowSeq[1][row -1]);
    colAln.insert(colAln.begin(), '-');
    seqDecoration.insert(seqDecoration.begin(), ' ');
    
    row--;
  }

  while (col > 0) {
    rowAln.insert(rowAln.begin(), '-');
    colAln.insert(colAln.begin(), colSeq[1][col -1]);
    seqDecoration.insert(seqDecoration.begin(), ' ');
    
    col--;
  }

  map<string, string> alignment;
  alignment["header1"] = colSeq[0];
  alignment["header2"] = rowSeq[0];

  for (int i = 0; i < rowAln.size(); i++) {
    alignment["seq1"] += colAln[i];
    alignment["seq2"] += rowAln[i];
    alignment["seqDecoration"] += seqDecoration[i];
  }
    
  return alignment;
}

void printAlignment(map<string, string> alignment) {
  int seqPos = 0;
  string seqID = "1";
  int maxChars = 0;

  while (seqPos < alignment["seq1"].size()) {
    for (int headerPos = 0; headerPos < 20; headerPos++) {
      if (headerPos != 19 && alignment["header" + seqID].size() > headerPos) {
        cout << alignment["header" + seqID][headerPos];

      } else {
        cout << " ";
      }
    }

      int seq1restLen = alignment["seq1"].size() - seqPos;
      int seq2restLen = alignment["seq2"].size() - seqPos;
      int decorationRestLen = alignment["seqDecoration"].size() - seqPos;

      int lineChars = 0;

    for (int seqLinePos = 0; seqLinePos < 60; seqLinePos++) {
      if ((seqID == "1" && seqLinePos > seq1restLen)
      || (seqID == "2" && seqLinePos > seq2restLen)
      || (seqID == "Decoration" && seqLinePos > decorationRestLen)) {
        break;
      }

      lineChars++;

      if (seqID == "seqDecoration") {
        for (int pos = 0; pos < 20; pos++) {
          cout << " ";
        }
      }
    
      cout << alignment["seq" + seqID][seqPos];
      
      seqPos++;
    }

    
    maxChars = max(maxChars, lineChars);
    
    if (seqID != "Decoration") {
      if (lineChars < 60) {
        seqPos -= lineChars;
      } else {
        seqPos -= 60;
      }

      if (seqID == "1") {
        seqID = "2"; 

      } else if (seqID == "2") {
        seqID = "Decoration";
      }

    } else {
      seqID = "1";
    }
    
    cout << endl;
  }

  return;
}

bool testForInt(string number) {
  string allowed = "0123456789-";
  for (int i = 0; i < number.size(); i++) {
    if (allowed.find(number[i]) >= allowed.size()) {
      cout << "NOT A NUMBER!" << endl;
      return false;
    }
  }

  return true;
}