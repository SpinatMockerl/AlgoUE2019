#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <array>
#include <map>
using namespace std;

int main(/*WAAAAAAAAA*/ int argc, char** argv) {  
  //NEEEEEIIIIIIN:
  string filename = argv[1];
  
  /*string filename;
  getline(cin, filename); // NIMMT KEINE FILE-EXTENSIONS!!!*/
  ifstream file;
  file.open(filename);
  string word;
  file.ignore(256, '\n');/*             */
  /*for (int i = 0; i < 10; i++) {
    file >> word;
    cout << filename << i << ": "<< word << endl;
    cout << "Oida?" << endl;
  }
  cout << "OIDA!?" << endl;
*/

  /*cout << "THE ONE AND ONLY TEST: "<< endl;
  string test = "Es geht umd die Abstaende!";
  test >> word;
  cout << word;*/

  vector<float> row;
  vector<vector<float>> down;
  vector<vector<float>> right;
  map<string, vector<vector<float>>> collector;
  collector["down"] = down;
  collector["right"] = right;
  
  string key = "down";

  

  char delimiter = ' ';
  size_t prevDelim = 0;
  string line;
  bool filled = false;

  cout << filename << endl;
  while (getline(file, line)) {
    if (line[0] != delimiter) {
      key = "right";
      continue;
    }
    cout << line << endl;
    row = {};
    
    //cout << word << endl;
    for (int previous = 0; previous < line.size(); previous++) {
      //cout << "i = "<< i << ": ";
      if (line[previous+1] != delimiter && line[previous] == delimiter) {
        //cout << endl << i << ": '" << line[i]<< "'" << endl;
        //cout << delimiter << endl;

        cout << "so far so good" << endl;
        for (int upcoming = previous+2; upcoming < line.size(); upcoming++) {
          //cout << "upcoming: "<< upcoming << ": ";
          if (line[upcoming] == delimiter || line[upcoming] == '\0') {

cout << "still!" << endl;
            row.push_back(stof(line.substr(previous+1, upcoming-(previous+1))));
            //collector[key][i+1].push_back(stof(line.substr(i+1, upcoming-(i+1))));
            //cout << i+1 <<" - "<< upcoming << "'" << line[upcoming] << "'" << endl;
            //cout << line.substr(i+1, upcoming-(i+1)) << endl;
            //cout << "CURRENT: "<< line.substr(i, upcoming-i) << endl;
            cout << "got it? ";
            /*for (int i = 0; i < row.size(); i++) {
              cout << row[i] <<" ";
            }*/
            
            break;
          }
        }
        /*if (filled) {
          filled = false;
          break;
        }*/
      }
    }
    //collector[key].push_back(100000.789);

    /* try {
      collector[key].push_back(stof(word));
      //cout << word << endl;
    } catch (...) {}*/

    /*if (word != "0.67") {
      cout << "SUCCESS" << endl;
    } else {cout << word << endl;}*/
    //cout << "whatever";
    cout << key << ": ";
    for (int i = 0; i < row.size(); i++) {
      cout << row[i] << endl;
    }
    collector[key].push_back(row);

    contin:;
  }

  down = collector["down"];
  right = collector["right"];

  cout << "test" << endl;
  //cout << "down.size = "<< down.size() <<" down[0].size = "<< down[0].size() << endl;

  /*for (int i = 0; i < 9/*down.size()* /; i++) {
    for (int j = 0; j < 9/*down[i].size()* /; j++) {
      cout << collector["down"][i][j] << endl;
      cout << "row: "<< i <<" column: "<< j << endl;
    }
    cout << "NEWLINE" << endl;
  }*/
  
  cout << "u good?" << endl;


  int matrixSize = right.size();

  int dat = 0 + down.size();
  
  
  float path[matrixSize][matrixSize];
  path[0][0] = 0;

  cout << down.size() << matrixSize << sizeof(path)/sizeof(*path) << endl;

  for (int row = 0; row < matrixSize; row++) {
    for (int col = 0; col < matrixSize; col++) {
      cout << "row-col: "<< row << "-" << col << endl;
      if (row == 0 && col == 0 /*(row & col) == 0*/) {
        cout << "so you noticed..."<<endl;
        continue;
        cout << "BUT YOU DIDN'T JUMP!!!" << endl;
      } else if (row == 0) {
        path[0][col] = path[0][col-1] + right[0][col-1];
      } else if (col == 0) {
        path[row][0] = path[row-1][0] + down[row-1][0];
      } else {      
        path[row][col] = max(
          path[row-1][col] + down[row-1][col],
          path[row][col-1] + right[row][col-1]
        );
      }

      //contin:;
    }
  }





  for (int i = 0; i < matrixSize; i++) {
    for (int j = 0; j < matrixSize; j++) {
      cout << path[i][j] << " ";
      if (path[i][j] < 10) {
        cout << " ";
      }
    }
    cout << endl;
  }
  cout << endl;

  for (int i = 0; i < matrixSize-1; i++) {
    for (int j = 0; j < matrixSize; j++) {
      cout << down[i][j] << " ";
      if (down[i][j] < 10) {
        cout << " ";
      }
    }
    cout << endl;
  }
  cout << endl;

  for (int i = 0; i < matrixSize; i++) {
    for (int j = 0; j < matrixSize-1; j++) {
      cout << right[i][j] << " ";
      if (right[i][j] < 10) {
        cout << " ";
      }
    }
    cout << endl;
  }
  cout << endl;

  cout << "SOLUTION: "<< path[matrixSize-1][matrixSize-1] << endl << endl;

  





  return 0;
}

map<string, vector<vector<int>>> parser(string filename) {
  
}