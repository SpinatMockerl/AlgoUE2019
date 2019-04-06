#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

// Delimiter-Ändernde Option einbauen

map<string, vector<vector<float>>> parser();
float needlemanWunschLight(map<string, vector<vector<float>>> collector);

int main() {
  cout << needlemanWunschLight(parser()) << endl;
  return 0;
}

map<string, vector<vector<float>>> parser() {
  vector<float> row; // Temporary storage for each parsed row
  vector<vector<float>> down;
  vector<vector<float>> right;
//////////////////
  vector<vector<float>> diagonal;
//////////////////
  map<string, vector<vector<float>>> collector; /* Unites the two scoring matrices
  in order to simplify later iterations */
  collector["down"] = down;
  collector["right"] = right;
//////////////////
  collector["diagonal"] = diagonal;

  string keychain[3] = {"down", "right", "diagonal"};
  string key = keychain[0];
  unsigned keyID = 0;
//////////////////
  char delimiter = ' ';

  string line;

  getline(cin, line); // Discards the first line of input as header

  while (getline(cin, line)) {
    // Switches to the next scoring matrix and skips a row:
    if (line[0] != delimiter) {
///////////////////////
      if (line[0] == '-') {
        keyID++; // DER GANZE BLOCK IS SCHIRCH!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      }
///////////////////////
      continue;
    }
///////////////////////
    key = keychain[keyID];
///////////////////////

    row = {}; // Empties the temporary row storage before parsing the next row
    
    // Loop for the starting character; previous denotes the one before that:
    for (int previous = 0; previous < line.size(); previous++) {
      // Checks for the beginning character of a score:
      if (line[previous+1] != delimiter && line[previous] == delimiter) {
        /* Loop for the ending character, initiated as the character directly
        following the starting character: */
        for (int upcoming = previous+2; upcoming < line.size(); upcoming++) {
          // Checks for the ending character of a score:
          if (line[upcoming] == delimiter) {
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

float needlemanWunschLight(map<string, vector<vector<float>>> collector) {
  // Unpacks the scoring matrices for readabilities sake:
  vector<vector<float>> down = collector["down"];
  vector<vector<float>> right = collector["right"];

  vector<vector<float>> diagonal = collector["diagonal"];

  int matrixSize = right.size(); /* Helps avoid frequent recalculation in order
  to boost performance */
  
  float path[matrixSize][matrixSize];
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
  the highest possible score and is therefor returned for output: */
  return path[matrixSize -1][matrixSize -1];
}