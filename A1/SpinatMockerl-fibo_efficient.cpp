#include <iostream>
using namespace std;

int main(int argc, char **argv) {
  // Fibonacci effizient:
  // Argumente: "-n n" Stelle der auszugebenden Fibonacci-Zahl
    // --all oder -a sorgt dafür, dass alle Zahlen bis zur Stelle ausgegeben
    // werden.
    // --help oder -h Hilfefunktion
      
      // time: -p gibt Zeit in Sekunden an

  bool all = false;
  int n;
  int f3 = 1; // initiiert das Ergebnis eines Fibonacci-Rechenschrittes
  bool help = false;

  //cout << f3 << endl;

  // Falls zu viele Argumente angegeben werden:
  if (argc > 4) {
      cout << "Error: Too many arguments" << endl;
      return 1;
  }

  // Weist die CLI-Argumente deren Variablen zu:
  for (int i = 1; i <= argc -1; i++) {
    if ((string)argv[i] == "-n") {

      //cout << "argv[i] == '-n'" << endl;

      n = stoi(argv[i + 1]);

    } else if ((string)argv[i] == "--all" || (string)argv[i] == "-a") {

      //cout << "argv[i] == '--all' || argv[i] == '-a'" << endl;

      all = true;
    } else if ((string)argv[i] == "--help" || (string)argv[i] == "-h") {

      //cout << "argv[i] == '--help' || argv[i] == '-h'" << endl;

      help = true;
    }
  }

  // Hilfetext:
  if (help) {
    cout << "HILFETEXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXT" << endl;
    return 0;
  }

  // Initiiert den Array 'nums' welcher die Fibonacci-Zahlen im Falle von
  // all = true sammelt. Kann nicht am Anfang initiiert werden, da zu dem
  // Zeitpunkt 'n' noch nicht bekannt ist:
  int nums[n];
  nums[0] = 1; nums[1] = 1;

  if (n < 1) {
    cout << "Please input a positive integer." << endl;
  } else {

    //cout << "} else {" << endl;

    // Initiiert die erste (f2) und eine theoretische nullte (f1) Fibonacci-Zahl:
    int f1 = 0;
    int f2 = 1;
/*
    if (all) {

      cout << "if (all) {" << endl;

      nums[1 -1] = 1;
      nums[2 -1] = 1;
    }
*/
    // Berechnet die Fibonacci-Zahlen ab der zweiten Zahl:
    for (int i = 2; i <= n; i++) {

      //cout << i << ": " << "for (int i = 3; i <= n -1; i++) {" << endl;

      f3 = f1 + f2;

      // Speichert ggf das Ergebnis des Rechenschrittes im Ergebnis-Array 'nums':
      if (all) {

        //cout << "if (all) {" << endl;

        nums[i -1] = f3;
      }

      // Schiebt das "Leseraster" um eine Stelle weiter:
      f1 = f2;
      f2 = f3;

      //cout << f3 << endl;
    }

    if (all) { // Gibt die Fibonacci folge bis 'n' aus, oder...
      for (int i = 0; i < sizeof(nums) / sizeof(*nums); i++) {
        
        //cout << i << ": " << nums[i] << endl;
      }
    } else { // gibt die Fibonacci-Zahl mit Index 'n' aus
      cout << f3 << endl;
    }
  }
  
  return 0;
}