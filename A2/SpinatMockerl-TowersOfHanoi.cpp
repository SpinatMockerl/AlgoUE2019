#include <iostream>
#include <map>
using namespace std;

void Towers(unsigned n, unsigned source, unsigned destination);

int main(int argc, char **argv) {
  string usage = "USAGE: " + (string)argv[0] + " -n number_of_disks";
  string hint = "Type " + (string)argv[0] + " --help for further details.";
  
  unsigned height;
  
    if ((string)argv[1] == "-n" ) {
      cout << "height: "<< endl;
      height = stoi(argv[2]);
      cout << height << endl;
    } else if (argv[1] == "--help" || argv[1] == "-h") {
      cout << usage << endl << endl;
      cout << "This program prints out the steps needed to take, in order to solve";
      cout << " a towers of Hanoi puzzle with given number of disks.";
      return 0;
    } else {
      cerr << "ERROR: Invalid arguments." << endl << endl;
      cout << usage << endl << endl << hint << endl;
      return 1;
    }
  
  unsigned start = 1;
  unsigned goal = 3;

  Towers(height, start, goal);
  
  return 0;
} 

void Towers(unsigned n, unsigned source, unsigned destination) { // LÖSUNG: Towers() übernimmt und returnt map
  if (n == 1) {
      cout << "Move disk "<< n <<" from position "<< source <<" to position "<< destination << endl;
      return;
  }

  unsigned empty = 6 - source - destination;

  Towers(n -1, source, empty);
  cout << "Move disk "<< n <<" from position "<< source <<" to position "<< destination << endl;
  Towers(n -1, empty, destination);

  return;
}