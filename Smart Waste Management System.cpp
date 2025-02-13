#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>

using namespace std;

class WasteBin {
private:
    int binID;
    string location;
    string type; // Organic, Plastic, Metal, etc.
    int wasteLevel; // Percentage (0 - 100)
    bool needsCollection;

public:
    WasteBin(int id, string loc, string t, int level, bool collection = false) {
        binID = id;
        location = loc;
        type = t;
        wasteLevel = level;
        needsCollection = collection;
    }

    int getBinID() const { return binID; }
    string getLocation() const { return location; }
    string getType() const { return type; }
    int getWasteLevel() const { return wasteLevel; }
    bool getNeedsCollection() const { return needsCollection; }

    void updateWasteLevel(int level) {
        wasteLevel = level;
        if (wasteLevel >= 80) {
            needsCollection = true;
        } else {
            needsCollection = false;
        }
    }

    void markForCollection() {
        needsCollection = true;
    }

    void displayBin() const {
        cout << "Bin ID: " << binID << " | Location: " << location << " | Type: " << type
             << " | Waste Level: " << wasteLevel << "% | " 
             << (needsCollection ? "Needs Collection ??" : "Not Full ?") << endl;
    }

    void saveToFile(ofstream &outfile) const {
        outfile << binID << "," << location << "," << type << "," << wasteLevel << "," << needsCollection << endl;
    }

    static WasteBin loadFromFile(ifstream &infile) {
        int id, wasteLevel;
        string location, type;
        bool needsCollection;
        infile >> id;
        infile.ignore();
        getline(infile, location, ',');
        getline(infile, type, ',');
        infile >> wasteLevel >> needsCollection;
        return WasteBin(id, location, type, wasteLevel, needsCollection);
    }
};

// Function to check if bin ID is unique
bool isBinIDUnique(vector<WasteBin> &bins, int id) {
    for (const auto &b : bins) {
        if (b.getBinID() == id) {
            return false;
        }
    }
    return true;
}

// Function to find a bin by ID
WasteBin* findBin(vector<WasteBin> &bins, int id) {
    for (auto &b : bins) {
        if (b.getBinID() == id) {
            return &b;
        }
    }
    return nullptr;
}

// Save bins to file
void saveBinsToFile(vector<WasteBin> &bins) {
    ofstream outfile("waste_bins.txt", ios::trunc);
    for (const auto &b : bins) {
        b.saveToFile(outfile);
    }
    outfile.close();
}

// Load bins from file
void loadBinsFromFile(vector<WasteBin> &bins) {
    ifstream infile("waste_bins.txt");
    if (!infile) return;

    while (!infile.eof()) {
        int id, wasteLevel;
        string location, type;
        bool needsCollection;

        infile >> id;
        infile.ignore();
        if (getline(infile, location, ',') && getline(infile, type, ',') && infile >> wasteLevel >> needsCollection) {
            bins.push_back(WasteBin(id, location, type, wasteLevel, needsCollection));
        }
    }
    infile.close();
}

// Function to delete a waste bin record
void deleteBin(vector<WasteBin> &bins, int id) {
    for (auto it = bins.begin(); it != bins.end(); ++it) {
        if (it->getBinID() == id) {
            bins.erase(it);
            cout << "Waste bin deleted successfully!\n";
            saveBinsToFile(bins);
            return;
        }
    }
    cout << "Waste bin not found!\n";
}

int main() {
    vector<WasteBin> bins;
    loadBinsFromFile(bins);

    int choice;
    while (true) {
        cout << "\n===== Smart Waste Management System =====\n";
        cout << "1. Register a Waste Bin\n";
        cout << "2. View All Waste Bins\n";
        cout << "3. Update Waste Levels\n";
        cout << "4. Mark Bin for Collection\n";
        cout << "5. Delete a Waste Bin\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 1) {
            int id, wasteLevel;
            string location, type;
            cout << "Enter Waste Bin ID: ";
            cin >> id;
            cin.ignore();

            if (!isBinIDUnique(bins, id)) {
                cout << "Error! Waste Bin ID already exists.\n";
                continue;
            }

            cout << "Enter Location: ";
            getline(cin, location);
            cout << "Enter Type (Organic, Plastic, Metal, etc.): ";
            getline(cin, type);
            cout << "Enter Initial Waste Level (0-100%): ";
            cin >> wasteLevel;

            bins.push_back(WasteBin(id, location, type, wasteLevel));
            saveBinsToFile(bins);
            cout << "Waste Bin registered successfully!\n";

        } else if (choice == 2) {
            cout << "\n===== Waste Bins List =====\n";
            for (const auto &b : bins) {
                b.displayBin();
            }

        } else if (choice == 3) {
            int id, wasteLevel;
            cout << "Enter Waste Bin ID to update level: ";
            cin >> id;
            WasteBin* bin = findBin(bins, id);

            if (bin) {
                cout << "Enter New Waste Level (0-100%): ";
                cin >> wasteLevel;
                bin->updateWasteLevel(wasteLevel);
                saveBinsToFile(bins);
                cout << "Waste level updated successfully!\n";
            } else {
                cout << "Waste bin not found!\n";
            }

        } else if (choice == 4) {
            int id;
            cout << "Enter Waste Bin ID to mark for collection: ";
            cin >> id;
            WasteBin* bin = findBin(bins, id);

            if (bin) {
                bin->markForCollection();
                saveBinsToFile(bins);
                cout << "Waste bin marked for collection!\n";
            } else {
                cout << "Waste bin not found!\n";
            }

        } else if (choice == 5) {
            int id;
            cout << "Enter Waste Bin ID to delete: ";
            cin >> id;
            deleteBin(bins, id);

        } else if (choice == 6) {
            cout << "Exiting Smart Waste Management System. Thank you!\n";
            break;
        } else {
            cout << "Invalid choice! Try again.\n";
        }
    }
    return 0;
}
