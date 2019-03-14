#include <iostream>
using namespace std;

void argParser(int argc, char** argv, int& n, bool& all, bool& help);
void fibonacci(int n, bool all);

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

  fibonacci(n, all);

  return 0;
}

// Tatsächliche Berechnung der Fibonacci-Zahlen:
void fibonacci(int n, bool all) {
  // Initiiert den Array 'nums' welcher die Fibonacci-Zahlen im Falle von
  // all = true sammelt:
  int nums[n];
  nums[0] = 1; nums[1] = 1;

  if (n < 1) {
    cout << "Please input a positive integer." << endl;
  } else {

    // Initiiert die erste (f2) und eine theoretische nullte (f1) Fibonacci-Zahl:
    int f1 = 0;
    int f2 = 1;
    int f3; // initiiert das Ergebnis eines Fibonacci-Rechenschrittes

    if (all) {
      /* Legt die ersten 2 Zahlen in nums ab. Der Index ist als Rechenoperation
      gegeben, um Die Position in der Folge mathematisch darzustellen
      (1,2,3 statt 0,1,2) */
      nums[1 -1] = 1;
      nums[2 -1] = 1;
    }

    // Berechnet die Fibonacci-Zahlen ab der zweiten Zahl:
    for (int i = 2; i <= n; i++) {
      f3 = f1 + f2;

      // Speichert ggf. das Ergebnis des Rechenschrittes im Ergebnis-Array 'nums':
      if (all) {nums[i -1] = f3;}

      // Schiebt das "Leseraster" um eine Stelle weiter:
      f1 = f2;
      f2 = f3;
    }  

    if (all) { // Gibt die Fibonacci folge bis 'n' aus, oder...
      for (int i = 0; i < sizeof(nums) / sizeof(*nums); i++) {
        
        cout << i << ": " << nums[i] << endl;
      }
    } else { // gibt die Fibonacci-Zahl mit Index 'n' aus
      cout << f3 << endl;
    }
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