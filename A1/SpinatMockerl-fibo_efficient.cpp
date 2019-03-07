#include <iostream>
using namespace std;

int main(int argc, char **argv) {
  // Fibonacci effizient:
  // Argumente: "-n n" Stelle der auszugebenden Fibonacci-Zahl
    // --all oder -a sorgt dafür, dass alle Zahlen bis zur Stelle ausgegeben
    // werden.
    // --help oder -h Hilfefunktion
      
      // time: -p gibt Zeit in Sekunden an

    cout << argc << endl;

/*
    if (argc > 4) {
        cout << "Too many arguments" << endl;
        return 1;
    }
*/
    cout << "Still going strong." << endl;
    cout << "sizeof(argv) = " << sizeof(argv) << endl;
    cout << "sizeof(*argv) = " << sizeof(*argv) << endl;
    cout << "sizeof(argv) / sizeof(*argv) = " << sizeof(argv) / sizeof(*argv) << endl;

    for (int i = 1; i <= argc; i++) {
      cout << argv[i] << endl;
      
    }
  
  return 0;
}