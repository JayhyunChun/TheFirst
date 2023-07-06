#include <iostream>
#include <cmath>
#include <string>

using namespace std;

int power(int base, int exp) {
    if (exp == 0)
       return 1;
    else if (exp % 2)
       return base * power(base, exp - 1);
    else {
       int temp = power(base, exp / 2);
       return temp * temp;
    }
}

int karatsuba(int x, int y) {
    if (x < 10 || y < 10)
        return x*y;
    
    int n_s = max(to_string(x).length(), to_string(y).length());
    int n_s2 = n_s / 2;

    int x_h = x / power(10, n_s2);
    int x_l = x % power(10, n_s2);

    int y_h = y / power(10, n_s2);
    int y_l = y % power(10, n_s2);

    int z_h = karatsuba(x_h, y_h);
    int z_l = karatsuba(x_l, y_l);
    int z_c = karatsuba(x_h + x_l, y_h + y_l);

    return z_h*power(10, 2*n_s2) + ((z_c - z_h - z_l)*power(10, n_s2)) + z_l;
}

void karatsuba_verbose(int x, int y, int indent = 0) {
    if (x < 10 || y < 10) {
        cout << string(indent, '\t') << "1," << x << ",0," << y << ",0," << x*y << "," << x*y << ",0" << endl;
        return;
    }
    
    int n_s = max(to_string(x).length(), to_string(y).length());
    int n_s2 = n_s / 2;

    int x_h = x / power(10, n_s2);
    int x_l = x % power(10, n_s2);

    int y_h = y / power(10, n_s2);
    int y_l = y % power(10, n_s2);

    karatsuba_verbose(x_h, y_h, indent + 1);
    karatsuba_verbose(x_l, y_l, indent + 1);
    karatsuba_verbose(x_h + x_l, y_h + y_l, indent + 1);

    cout << string(indent, '\t') << n_s2 << "," << x_h << "," << x_l << "," << y_h << "," << y_l << "," << karatsuba(x_h, y_h) << "," << karatsuba(x_h + x_l, y_h + y_l) << "," << karatsuba(x_l, y_l) << endl;
}

int main() {
    cout << "Result: ";
    karatsuba_verbose(17, 11);
    cout << endl;

    cout << "Result: ";
    karatsuba_verbose(192, 237);
    cout << endl;

    return 0;
}
