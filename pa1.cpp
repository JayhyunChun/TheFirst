#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

int main(int argc, char* argv[]) {
    ifstream inputFile("test.txt");  // Open the file for reading

    if (inputFile.is_open()) {
        string line;
        while (getline(inputFile, line)) {  // Read lines from the file
            istringstream iss(line);  // Create a string stream from the line

            string token;
            while (getline(iss, token, ' ')) {
                istringstream numStream(token); // Create a new stream to handle comma-separated values

                string numToken;
                while (getline(numStream, numToken, ',')) {
                    // Remove any non-digit characters from the token
                    numToken.erase(remove_if(numToken.begin(), numToken.end(), [](char c) {
                        return !isdigit(c);
                        }), numToken.end());

                    if (!numToken.empty()) {
                        // Convert the token to an integer and print it
                        int number = stoi(numToken);
                        cout << number << endl;
                    }
                }
            }
        }

        inputFile.close();  // Close the file
    }
    else {
        cout << "Failed to open the file." << endl;
    }

    return 0;
}
