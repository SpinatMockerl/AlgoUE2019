#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <map>
#include <unistd.h>
using namespace std;

/*To Do:
UNNEDIG:
    - neue Funktion zur erstellung von Richtungsmatrizen
      z.B. Matrix(colSeq.size(), vector<int>(rowSeq.size(), 0)) directionScore;
        map<int, Matrix> directionscoringFun
- 
- needlemanWunschLight KÜBELN
- neue Backtracker-Funktion
- Umwandlung in CLUSTAL-Format (eigene Funktion?)
- Ausgeben
*/
typedef enum {
  down = 1,
  right = 2,
  diagonal = 4
} directions;

typedef vector<vector<int>> Matrix;
typedef vector<vector<string>> Fasta;

map<string, int> argParser(int argc, char** argv);
Fasta fastaReader();
Matrix scoreNW(string colSeq, string rowSeq, map<string, int> scoringFun);
map<string, string> backtrackNW()


map<string, vector<vector<int>>> pathParser(char delimiter = ' ');
int needlemanWunschLight(map<string, vector<vector<int>>> collector);

string help = "HELP-MSG\n";

int main(int argc, char** argv) {
  if (isatty(STDIN_FILENO) == 1) {
    cerr << "USAGE!!!!!!!!" << endl;
    exit(EXIT_FAILURE);
  }
  
  char delimiter = ' ';

  //cout << isatty(STDIN_FILENO) << endl;

  /*if (argc > 2 && (strcmp(argv[1], "-d") || strcmp(argv[1], "--delimiter"))) {
    if (strcmp(argv[2], "\t") == 0) {
      delimiter = '\t';
    } else {
      delimiter = *argv[2];
    }
  } */

  for (int i = 0; i < argc; i++) {
    cout << argv[i] << endl;
  }

  cout << "works so far" << endl;

  map<string, int> scoringFun = argParser(argc, argv);

  cout << scoringFun["match"] << endl;

  vector<vector<string>> seqs = fastaReader();

  int seqCount = seqs.size();

  for (int row = 0; row < seqCount -1; row++) {
    for (int col = row +1; col < seqCount; col++) {
      // anpassen an neues Format von seqs:
      Matrix scoringMatrix = scoreNW(seqs[col][1], seqs[row][1], scoringFun);
      // ~ vector.push_back(Backtracking)

    }
  }


  //cout << needlemanWunschLight(pathParser(delimiter)) << endl;

  return 0;
}

map<string, int> argParser(int argc, char** argv) {
  map<string, int> scoringFun = {{"match", 1}, {"mismatch", -1}, {"gap", -2}};

  cout << "inside function" << endl;

  for (int i = 1; i < argc; i++) {
    string arg = (string)argv[i];

    if (arg == "--help" || arg == "-h") {
      cout << help;
      exit(EXIT_SUCCESS);

    } else try {
      if (arg == "--match") {
        scoringFun["match"] = stoi(argv[i +1]);
      } else if (arg == "--mismatch") {
        scoringFun["mismatch"] = stoi(argv[i +1]);
      } else if (arg == "--gap") {
        scoringFun["gap"] = stoi(argv[i +1]);
      }
    } catch (...) {
        cerr << "ERROR: A whole number needs to be provided after " <<
          "--match, --mismatch and --gap.\nAborting\n";
        exit(EXIT_FAILURE);
    }
  }

  return scoringFun;
}

Fasta fastaReader() {
  string line;
  string seq;
  Fasta seqs;
  string header;

  while (getline(cin, line)) {
    cout << "isatty: "<< isatty(STDIN_FILENO) << endl; //STDIN_FILENO = 0

    if (line[0] == '>') {
      vector<string> compoundSeq = {header, seq};
      seqs.push_back(compoundSeq);
      
      // Set to next Sequence:
      header = line;
      seq = "";
    } else if (line[0] == ';') {
      continue;
    } else {
      seq += line;
    }
  }
  
  vector<string> compoundSeq = {header, seq};
  seqs.push_back(compoundSeq);

  return seqs;
}

Matrix scoreNW(string colSeq, string rowSeq, map<string, int> scoringFun) {
  Matrix scores(rowSeq.size() +1, vector<int>(colSeq.size() +1, 0));

  for (int row = 0; row < rowSeq.size() +1; row++) {
    for (int col = 0; col < colSeq.size() +1; col++) {
      if ((row & col) == 0) {
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

/*map<int, Matrix> directionalscoringFun(string colSeq, string rowSeq, map<string, int> scoringFun) {
  map<int, Matrix> directionalMatrix = {
    {directions::down, Matrix(rowSeq.size() -1, vector<int>(colSeq.size(), scoringFun::gap))},
    {directions::right, Matrix(rowSeq.size(), vector<int>(colSeq.size() -1, scores::gap))},
    {directions::diagonal, Matrix(rowSeq.size() -1, vector<int>(colSeq.size() -1, 0))}
  }

  //string::iterator colIt = colSeq.begin();
  //string::iterator rowIt = rowSeq.begin();

  for (int row = 0; row < rowSeq.size(); row++) {
    for (int col = 0; row < colSeq.size(); col++) {
      if (rowSeq[row] == colSeq[col]) {

      }
    }
  }
}*/

/*
map<string, vector<vector<int>>> pathParser(char delimiter) {
  vector<vector<int>> down;
  vector<vector<int>> right;
  vector<vector<int>> diagonal;
  map<string, vector<vector<int>>> collector; /* Unites the two scoring matrices
  in order to simplify later iterations * /
  collector["down"] = down;
  collector["right"] = right;
  collector["diagonal"] = diagonal;

  string keychain[3] = {"down", "right", "diagonal"};
  string key = keychain[0];
  unsigned keyID = 0;

  string line;

  getline(cin, line); // Discards the first line of input as header

  while (getline(cin, line)) {
    // Switches to the next scoring matrix and skips each header row:
    if (line[0] != delimiter) {
      if (line[0] == '-') {
        keyID++;
      }
      continue;
    }

    key = keychain[keyID];

    vector<int> row = {}; // Temporary storage for each parsed row
    
    // Loop for the starting character; previous denotes the one before that:
    for (int previous = 0; previous < line.size(); previous++) {
      // Checks for the beginning character of a score:
      if (line[previous+1] != delimiter && line[previous] == delimiter) {
        /* Loop for the ending character, initiated as the character directly
        following the starting character: * /
        for (int upcoming = previous+2; upcoming <= line.size(); upcoming++) {
          // Checks for the ending character of a score:
          if (line[upcoming] == delimiter || ! line[upcoming]) {
            // Appends the newly parsed score to the row:
            row.push_back(stof(line.substr(previous+1, upcoming-(previous+1))));
            break; // In order to avoid reusing a score
          }
        }
      }
    }
    collector[key].push_back(row);
  }
  return collector;
}

int needlemanWunschLight(map<string, vector<vector<int>>> collector) {
  // Unpacks the scoring matrices for readability's sake:
  vector<vector<int>> down = collector["down"];
  vector<vector<int>> right = collector["right"];

  vector<vector<int>> diagonal = collector["diagonal"];

  int matrixSize = right.size(); /* Helps avoid frequent recalculation in order
  to boost performance * /
  
  int path[matrixSize][matrixSize];
  path[0][0] = 0;

  // The actual simplified Needleman-Wunsch-Algorithm itself:
  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      if (row == 0 && col == 0) {
        continue;
      } else if (row == 0) {
        path[0][col] = path[0][col-1] + right[0][col-1];
      } else if (col == 0) {
        path[row][0] = path[row-1][0] + down[row-1][0];
      } else {      
        path[row][col] = max(
          path[row-1][col-1] + diagonal[row-1][col-1],
          max(path[row-1][col] + down[row-1][col],
          path[row][col-1] + right[row][col-1])
        );
      }
    }
  }
  /* The bottom right element of the cumulative score, which corresponds to
  the highest possible score and is therefor returned for output: * /
  return path[matrixSize -1][matrixSize -1];
}
YPK1            SQLSWKRLLMKGYIPPYKPAVS-----NSMDTSNFDEEFTR---EKPIDSVVDEYLSESV
KRAC_BOVIN      ASIVWQDVYEKKLSPPFKPQVT-----SETDTRYFDEEFTA---QMITITPPDQDDSMEG

*/