#include <iostream>
#include<string>
#include <sstream>
using namespace std;
const int number = 11;
int main() {
    string line, out;
    getline(cin, line);
    istringstream is(line);
    int x;
    while (is >> x) {
        out.append(to_string(x * 7) + " ");
    }
    cout << out << endl;
    return 0;
}
