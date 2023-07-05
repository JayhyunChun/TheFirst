#include <iostream>
#include <cmath>

void recursiveMultiply(int n_s, int x_h, int x_l, int y_h, int y_l, int& z_h, int& z_c, int& z_l) {
    // Base case: either x or y is 0
    if (x_h == 0 || y_h == 0) {
        z_h = 0;
        z_c = 0;
        z_l = 0;
        return;
    }

    // Recursive cases
    recursiveMultiply(n_s, x_h / 2, x_l / 2, y_h / 2, y_l / 2, z_h, z_c, z_l);
    
    z_h *= 4; // Multiply z_h by 2^2*n_s
    
    int a = (x_h + x_l) * (y_h + y_l);
    int b = x_h * y_h;
    int c = x_l * y_l;

    z_c = a - b - c; // z_c = (x_h + x_l)*(y_h + y_l) - z_h - z_l

    z_h += (z_c / 2) * pow(2, n_s); // Add (z_c - z_h - z_l) * 2^n_s to z_h
    z_l = c; // z_l = x_l * y_l
}

int main() {
    int x = 17;
    int y = 11;

    int n_s = log2(x);
    int x_h = x >> n_s;
    int x_l = x & ((1 << n_s) - 1);
    int y_h = y >> n_s;
    int y_l = y & ((1 << n_s) - 1);

    int z_h, z_c, z_l;
    recursiveMultiply(n_s, x_h, x_l, y_h, y_l, z_h, z_c, z_l);

    std::cout << n_s << "," << x_h << "," << x_l << "," << y_h << "," << y_l << "," << z_h << "," << z_c << "," << z_l << std::endl;

    return 0;
}
