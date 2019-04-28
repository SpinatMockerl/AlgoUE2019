#include <iostream>
#include <string>
#include <string.h>
#include <vector>
#include <map>
#include <unistd.h>
#include <algorithm>
using namespace std;

/*To Do:


Gap-Einbau und Alignment-Länge anpassen



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
map<string, string> backtrackNW(vector<string> colSeq, vector<string> rowSeq, Matrix scores, map<string, int> scoringFun);
void printAlignment(map<string, string> alignment);


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

  /*for (int i = 0; i < argc; i++) {
    cout << argv[i] << endl;
  }*/

  //cout << "works so far" << endl;

  map<string, int> scoringFun = argParser(argc, argv);

  vector<vector<string>> seqs = fastaReader();
  
  //cout << "SEQUENCE 0:" << endl;
  //cout << seqs[0][1] << endl;
  

  int seqCount = seqs.size();

  for (int row = 0; row < seqCount -1; row++) {
    for (int col = row +1; col < seqCount; col++) {
      // anpassen an neues Format von seqs:
      Matrix scoringMatrix = scoreNW(seqs[col][1], seqs[row][1], scoringFun);

      /*for (int i = 0; i < scoringMatrix.size(); i++) {
        for (int j = 0; j < scoringMatrix[0].size(); j++) {
          cout << scoringMatrix[i][j] <<" ";
        }
        cout << endl << endl;
      }*/
/*
      cout << "row - col:" << endl << row << "-" << col << endl;
      cout << "SEQUENCE "<< col << ": "<< endl;
      cout << seqs[col][0] << endl << seqs[col][1] << endl;
      cout << "SEQUENCE "<< row << ": "<< endl;
      cout << seqs[row][0] << endl << seqs[row][1] << endl << endl;
  */    
      /*cout << "SCORING MATRIX:" << endl;
      for (int row = 0; row < scoringMatrix.size(); row++) {
        for (int col = 0; col < scoringMatrix[0].size(); col++) {
          cout << scoringMatrix[row][col] << " ";
        }
        cout << endl;
      }*/

      // ~ vector.push_back(Backtracking)
      map <string, string> alignment = backtrackNW(seqs[col], seqs[row], scoringMatrix, scoringFun);
      printAlignment(alignment);
    }
  }


  //cout << needlemanWunschLight(pathParser(delimiter)) << endl;

  return 0;
}

map<string, int> argParser(int argc, char** argv) {
  map<string, int> scoringFun = {{"match", 1}, {"mismatch", -1}, {"gap", -2}};

  //cout << "inside function" << endl;

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
  bool firstLine = true;

  while (getline(cin, line)) {
    if (line[0] == '>') {
      vector<string> compoundSeq = {header, seq};
      
      if (! firstLine) {
        seqs.push_back(compoundSeq);
      }
      
      // Set to next Sequence:
      header = line;
      seq = "";
    } else if (line[0] == ';') {
      continue;
    } else {
      seq += line;
    }

    //cout << "line read" << endl;

    firstLine = false;
  }
  
  vector<string> compoundSeq = {header, seq};
  seqs.push_back(compoundSeq);

  //cout << "all sequences read" << endl;

  return seqs;
}

Matrix scoreNW(string colSeq, string rowSeq, map<string, int> scoringFun) {
  Matrix scores(rowSeq.size() +1, vector<int>(colSeq.size() +1, 0));

  for (int row = 0; row < rowSeq.size() +1; row++) {
    for (int col = 0; col < colSeq.size() +1; col++) {

      //cout << "while scoring: row - col = " << row << "-" << col << endl;

      if (row == 0 && col == 0) {
        //cout << "ROW AND COL == 0" << endl;

        continue;
      } else if (row == 0) {
        scores[row][col] = scores[row][col -1] + scoringFun["gap"];

        /*cout << "GAP-Cost = " << scoringFun["gap"] << endl;
        cout << "Score to the left = "<< scores[row][col -1] << endl;
        cout << "calculated score = "<< scores[row][col] << endl;*/
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

  //cout << "NW-score calculated" << endl;

  return scores;
}

map<string, string> backtrackNW(vector<string> colSeq, vector<string> rowSeq, Matrix scores, map<string, int> scoringFun) {
  vector<char> rowAln;
  vector<char> colAln;
  vector<char> seqDecoration;

  int row = scores.size() -1;
  int col = scores[0].size() -1;

        /*cout << "ROW:" << endl << rowSeq[1][17] << endl;
        cout << "COL:" << endl << colSeq[1][16] << endl;*/

  while (row > 0) {

    //cout << "while (row > 0) {" << endl;

    if (col < 1) {
      break;
    }

    while (col > 0) {

      /*cout << "while (col > 0) {" << endl;
      cout << row << "-" << col << endl;

      cout << "CURRENT SCORE = "<< scores[row][col] << endl;
      cout << "DIAGONALLY PREVIOUS SCORE = "<< scores[row][col] << endl;

      if (row == 16 && col == 15) {
        cout << "ROW:" << endl << rowSeq[1][15] << endl;
        cout << "COL:" << endl << colSeq[1][14] << endl;
      }*/

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
      } else if (scores[row][col] == scores[row -1][col -1] + scoringFun["mismatch"]) {
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
  // fill in remaining gaps, if row or col == 0
  
  //cout << "filling in remaining gaps..." << endl;

  while (row > 0) {
    rowAln.insert(rowAln.begin(), rowSeq[1][row -1]);
    seqDecoration.insert(seqDecoration.begin(), ' ');
    row--;
  }

  while (col > 0) {
    colAln.insert(colAln.begin(), colSeq[1][col -1]);
    seqDecoration.insert(seqDecoration.begin(), ' ');
    col--;
  }

  //map<string, string> alignment = {{"seq1", colAln}, {"header1", colSeq[0]}, {"seq2", rowAln}, {"header2", rowSeq[0]}, {"seqDecoration", seqDecoration}};

  /*colAln(colAln.begin(), colAln.end());
  rowAln(rowAln.begin(), rowAln.end());*/

  //cout << "building alignment..." << endl;

  map<string, string> alignment;
  //alignment["seq1"] = colAln;
  alignment["header1"] = colSeq[0];
  //alignment["seq2"] = rowAln;
  alignment["header2"] = rowSeq[0];
  //alignment["seqDecoration"] = seqDecoration;

  for (int i = 0; i < rowAln.size(); i++) {
    alignment["seq1"] += colAln[i];
    alignment["seq2"] += rowAln[i];
    alignment["seqDecoration"] += seqDecoration[i];
  }

  //cout << "ALIGNMENT 1: "<< alignment["seq2"] << endl;
  
  
  return alignment;
}

void printAlignment(map<string, string> alignment) {
  //cout << "printing alignment..." << endl;
  
  int seqPos = 0;
  string seqID = "1";

  while (seqPos < alignment["seq1"].size()) {
    for (int headerPos = 0; headerPos < 20; headerPos++) {
      if (alignment["header" + seqID].size() > headerPos) {
        cout << alignment["header" + seqID][headerPos];
      } else {
        cout << " ";
      }
    }

    for (int seqLinePos = 0; seqLinePos < 60; seqLinePos++) {
      if (seqLinePos > alignment["seq" + seqID].size()) {
        break;
      }

      //cout << alignment["seq" + seqID][seqLinePos];

      /*if (seqID == "2") {
        cout << endl;
        */if (seqID == "seqDecoration") {
        for (int pos = 0; pos < 20; pos++) {
          cout << " ";
        //}
        }}
        cout << alignment["seq" + seqID][seqPos];
      //}
      seqPos++;
    }
    
    //cout << seqID << endl;

    //cout << "got it ?" << endl;

    if (seqID != "Decoration") {
      if (seqPos < 60) {
        seqPos -= alignment["seq1"].size();
      } else {
        seqPos -= 60;
      }

      if (seqID == "1") { //cout << endl << "switched to 2" << endl;
        seqID = "2"; 
        //cout << "seqID = "<< seqID << endl;
      } else if (seqID == "2") { //cout << endl << "switched to Decoration" << endl;
        seqID = "Decoration";
      }

    } else /*if (seqID == "2")*/ { //cout << endl << "switched to 1" << endl;
      /*seqID = "Decoration";
    } else {
      */seqID = "1";
      
      /*cout << endl;

      for (int pos = 0; pos < 20; pos++) {
        cout << " ";
      }

      cout << alignment["seqDecoration"] << endl;*/
    }
    cout << endl;


    //cout << "seqID: " << seqID << endl;
    
    
    //cout << seqID << endl;
    //cout << seqPos << endl;
  } // decoration ausgeben, endls


/*
  for (int pos = 0; pos < 80; pos++) {
    for (int headerPos = 0; headerPos < 20) {
      if (alignment["header1"].size() > headerPos) {
        cout << alignment["header1"][headerPos];
        pos++;
      } else {
        cout << " ";
        pos++;
      }
    } //0-19;20-79
    
    while (pos < 80) {
      c
    }
  }*/
}//for (int baseNo = 0; baseNo < alignment["seq1"].size(); baseNo++) {

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