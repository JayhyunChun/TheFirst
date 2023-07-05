#include <iostream>
#include <vector>

// Helper function to convert an integer to binary representation
std::vector<int> toBinary(int num, int numBits) {
    std::vector<int> binary(numBits);
    for (int i = numBits - 1; i >= 0; i--) {
        binary[i] = num & 1;
        num >>= 1;
    }
    return binary;
}

// Recursive multiplication algorithm
void recursiveMultiply(int x, int y, int n_s, std::vector<int>& output) {
    // Base case: either x or y is 0
    if (x == 0 || y == 0) {
        output = {0, 0, 0, 0, 0, 0, 0, 0};
        return;
    }

    // Split x and y into high and low bits
    int x_h = x >> n_s;
    int x_l = x & ((1 << n_s) - 1);
    int y_h = y >> n_s;
    int y_l = y & ((1 << n_s) - 1);

    // Recursive calls
    std::vector<int> z_h, z_c, z_l;
    recursiveMultiply(x_h, y_h, n_s, z_h);
    recursiveMultiply(x_h + x_l, y_h + y_l, n_s, z_c);
    recursiveMultiply(x_l, y_l, n_s, z_l);

    // Perform the calculations
    std::vector<int> z(output.size());
    for (int i = 0; i < output.size(); i++) {
        if (i < z_h.size()) {
            z[i + 2 * n_s] += z_h[i];
        }
        if (i < z_c.size()) {
            z[i + n_s] += z_c[i] - z_h[i] - z_l[i];
        }
        if (i < z_l.size()) {
            z[i] += z_l[i];
        }
    }

    output = z;
}

int main() {
    int x = 17;
    int y = 11;
    int n_s = 2;  // Change this value according to the desired bit split

    std::vector<int> output;
    recursiveMultiply(x, y, n_s, output);

    // Print the input and output in the desired format
    std::cout << n_s << ",";
    for (int i = 0; i < output.size(); i++) {
        std::cout << output[i];
        if (i != output.size() - 1) {
            std::cout << ",";
        }
    }
    std::cout << std::endl;

    return 0;
}
