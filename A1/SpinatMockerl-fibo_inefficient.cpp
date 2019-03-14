#include <iostream>
#include <vector>
using namespace std;

void argParser(int argc, char** argv, int& n, bool& all, bool& help);
int fibonacci(int n, bool all, vector<int> &nums, bool outermost = true, bool append = true);

// Berechnung von Fibonacci-Zahlen mittels CLI-Arguments:
int main(int argc, char **argv) {
  bool all = false; // Ob eine Sequenz an Zahlen ausgegeben werden soll
  int n = 5; // Als Defaultwert
  bool help = false; // Ob der Hilfetext ausgegeben werden soll.
    // Schließt andere Ausgaben aus.

  // Falls zu viele Argumente angegeben werden:
  if (argc > 4) {
      cout << "Error: Too many arguments" << endl;
      return 1;
  }

  argParser(argc, argv, n, all, help);

  // Hilfetext:
  if (help) {
    cout << "HILFETEXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXT" << endl;
    return 0;
  }

  vector<int> nums = {1, 1};

  cout << n << ": "<< fibonacci(n, all, nums) << endl;




cout << nums.size() << endl;

//cout << "yeh" << endl;





  return 0;
}

// Tatsächliche Berechnung der Fibonacci-Zahlen:
int fibonacci(int n, bool all, vector<int> &nums, bool outermost, bool append) {
  // Initiiert den Array 'nums' welcher die Fibonacci-Zahlen im Falle von
  // all = true sammelt:
  //nums[0] = 1; nums[1] = 1;

                                cout << "SIZE: "<< nums.size() << endl;


if (outermost) {cout << nums.size() << endl;
for (int i= 0; i<10; i++){
    //cout << nums[i] << endl;
}}



  if (n < 1) {
    cout << "Please input a positive integer." << endl;
  } else if (n == 0) {
      return 0;
  } else if (n < 3) {
    //nums.push_back(1);

    //cout << "PUSHED BACK" << endl;
    
    cout << "n < 3" << endl;
    
    return 1;
  } else {

    cout << "else" << endl;
    
    
    int f1 = fibonacci(n -2, all, nums, false, false);
    int f2 = fibonacci(n -1, all, nums, false);
    int f3 = f1 + f2;

                       cout << "f3 == "<< f3 << endl;

    if (append) {nums.push_back(f3);}
               
                                   cout << "SIZEAFTER: "<< nums.size() << endl;


cout << "CLEAR 1"<< endl;
    


    if (all && outermost) { // Gibt die Fibonacci folge bis 'n' aus, oder...
      for (int i = 0; i < nums.size() -1; i++) {
        
        cout << endl << nums.size() << endl << endl;



        cout << i +1 << ": " << nums[i] << endl;
      }
    }

    return f3;
  }
}

// Weist die CLI-Argumente deren Variablen zu:
void argParser(int argc, char **argv, int& n, bool& all, bool& help) {
  for (int i = 1; i <= argc -1; i++) {
    if ((string)argv[i] == "-n") {
      n = stoi(argv[i + 1]);

    } else if ((string)argv[i] == "--all" || (string)argv[i] == "-a") {
      all = true;

    } else if ((string)argv[i] == "--help" || (string)argv[i] == "-h") {
      help = true;
    }
  }
}