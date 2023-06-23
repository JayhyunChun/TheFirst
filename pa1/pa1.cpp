#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <algorithm>
#include <vector>
#include <tuple>

using namespace std;

int bandwCap;                               // Bandwidth cap

double calcSublease(double rev, double remainedBW, double BW) {         // For calculating sublease deduction
    return 1.1 * (remainedBW / BW) * rev;
}

int main(int argc, char* argv[]) {
    int numberOfLines;                      // The number of lines
    vector<tuple<int, int, int>> clients;
    /*
    The list of clients:
    1st int: The index of clients
    2nd int: Expected Revenue
    3rd int: Expected BW
    */   

    int maxBW = 0;                                  // Maximum bandwidth

    ifstream inputFile(argv[1]);            // Open the file for reading

    if (inputFile.is_open()) {
        int counter = 0;                    // Determines the value inside the .txt file
        int index = 0;                      // Determines the index of each client
        string line;
        while (getline(inputFile, line)) {  // Read lines from the file
            istringstream iss(line);
            // Create a string stream from the line
            
            string token;
            
            while (getline(iss, token, ' ')) {
                istringstream numStream(token);
                // Create a new stream to handle comma-separated values

                string numToken;
                
                while (getline(numStream, numToken, ',')) {
                    // Remove any non-digit characters from the token
                    numToken.erase(remove_if(numToken.begin(), numToken.end(), [](char c) {
                        return !isdigit(c);
                    }), numToken.end());

                    if (!numToken.empty()) {
                        // Convert the token to an integer and retrieve it
                        int number = stoi(numToken);
                        
                        /*
                        When the counter value is:
                        0:              bandwCap
                        1:              numberOfLines
                        more than 2:    clients
                        */
                        switch(counter)
                        {
                            case 0:
                                bandwCap = number;
                                break;
                            case 1:
                                numberOfLines = number;
                                break;
                            default:
                                if (counter % 2 == 0) {
                                    clients.push_back(make_tuple(index, number, 0));
                                }
                                else {
                                    get<2>(clients[index]) = number;
                                    maxBW += number;
                                    index++;
                                }

                                break;
                        }
                    }

                    counter++;
                }
            }
        }

        inputFile.close();      // Close the file
    }
    else {
        cout << "Failed to open the file." << endl;
    }
    // Done retrieving values from text file.

    // Placeholer
    cout << "The bandwidth cap: " << bandwCap << endl << "Number of lines: "
    << numberOfLines << endl;

    for (const auto& element : clients) {
        int index1 = get<0>(element);
        int index2 = get<1>(element);
        int index3 = get<2>(element);
        cout << "(" << index1 << ", " << index2 << ", " << index3 << ")" << endl;
    }

    cout << "Maximum BW: " << maxBW << endl << endl;

    int currentRev[bandwCap];                       // The revenue for calculating
    int currentBW[bandwCap];                        // The BW for calculating
    int maxRev[bandwCap];                                 // Maximum revenue
    int maxRevWithoutSublease[bandwCap];            // Maximum revenue without sublease
    int theIndex = 0;
    int theIndex2 = 0;
    /*
    Determine "the" index.
    The index indicates whether the calculation of sublease should be taken or not.
    */

    bool subleaseBool = false;                      // The bool of sublease

    // Initializing the currentRev, maxRevWithoutSublease and currentBW array.
    for (int i = 0; i < bandwCap; i++) {
        currentRev[i] = 0;
        currentBW[i] = 0;
        maxRev[i] = 0;
        maxRevWithoutSublease[i] = -1;
    }

    int numberOfCalc = numberOfLines;               // Later determines how many calculations are needed.

    for (int h = 0; h < numberOfLines - 1; h++) {
        int indexes[numberOfCalc][numberOfCalc - 1];
        int indexes2[numberOfCalc][numberOfCalc - 1];
        
        for (int i = 0; i < numberOfCalc; i++) {
            int tmp = maxBW;
            int index2 = 0;

            maxBW -= get<2>(clients[i]);
            currentBW[i] = maxBW;
            cout << "maxBW - BW of client" << get<0>(clients[i]) << ": " << currentBW[i] << "\t";

            for (int j = 0; j < numberOfCalc; j++) {
                if (i != j) {
                    currentRev[i] += get<1>(clients[j]);
                    indexes[i][index2] = get<0>(clients[j]);
                    index2++;
                }
            }

            cout << "currentRev: " << currentRev[i] << endl;

            if (maxBW <= bandwCap && maxRevWithoutSublease[h] == -1) {
                maxRevWithoutSublease[h] = currentRev[i];
            }

            if (i == 0) {
                maxRev[h] = currentRev[i];
                // Initially store the very first current revenue into the maximum revenue.
            }
            else {
                if (currentRev[i] > maxRev[h]) {
                    maxRev[h] = currentRev[i];
                    theIndex = i;
                }

                if (maxBW <= bandwCap && maxRevWithoutSublease[h] != -1 && currentRev[h] > maxRevWithoutSublease[i]) {
                    maxRevWithoutSublease[h] = currentRev[i];
                    
                    for (int j = 0; j < numberOfCalc; j++) {
                        if (i != j) {
                            indexes2[h][j] = get<0>(clients[j]);
                        }
                    }

                    theIndex2 = i;
                }
            }

            maxBW = tmp;
        }

        cout << "max rev (over BW): " << maxRev[h] << endl;
        cout << "max rev (under BW): " << maxRevWithoutSublease[h] << endl;

        if (h > 0 && maxRevWithoutSublease[h - 1] != -1) {
            cout << "max rev with deduction: " << maxRev[h - 1] - calcSublease(get<1>(clients[theIndex]), maxBW - bandwCap, bandwCap) << endl;
            cout << "max rev without deduction: " << maxRevWithoutSublease[h - 1] << endl;
            if (maxRevWithoutSublease[h - 1] > maxRev[h - 1] - calcSublease(get<1>(clients[theIndex]), maxBW - bandwCap, bandwCap)) {
                ofstream dummyFile("dummy.txt");
    
                dummyFile << "0" << endl << numberOfCalc << endl;

                for (int i = 0; i < numberOfCalc; i++) {
                    dummyFile << indexes2[theIndex][i] << endl;
                }

                dummyFile.close();
                break;
            }
            else {
                subleaseBool = true;
            }
        }

        if (currentBW[theIndex] <= bandwCap) {
            ofstream dummyFile("dummy.txt");
            
            if (subleaseBool == false) {
                dummyFile << "0" << endl << numberOfCalc - 1 << endl;
            }
            else {
                dummyFile << "1" << endl << get<0>(clients[theIndex]) << ","
                << maxBW - bandwCap << endl;
            }

            for (int i = 0; i < numberOfCalc - 1; i++) {
                dummyFile << indexes[theIndex][i] << endl;
            }

            dummyFile.close();
            break;
        }
        else {
            numberOfCalc--;
            maxBW -= get<2>(clients[theIndex]);
            clients.erase(clients.begin() + theIndex);

            for (int i = 0; i < bandwCap; i++) {
                currentRev[i] = 0;
                currentBW[i] = 0;
            }
        }
    }

    // Encode the "dummy.txt" into an UTF-8 format "output.txt" file.
    ifstream inFile("dummy.txt", std::ios::in | std::ios::binary);
    ofstream outputFile("output.txt", std::ios::out | std::ios::binary);

    string line;

    while(getline(inFile, line)) {
        outputFile << line << endl;
    }

    inFile.close();
    outputFile.close();
    return 0;
}