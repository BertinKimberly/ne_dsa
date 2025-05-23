/*****************************************************************
 * Rwanda Infrastructure Management System
 * 
 * This program manages infrastructure data for Rwandan cities,
 * including road networks and budget allocations. It provides
 * functionality for adding cities, roads, and their associated
 * budgets, as well as displaying and storing this information.
 * 
 * Data is persisted in real-time to text files:
 * - cities.txt: Contains information about all cities
 * - roads.txt: Contains information about roads and their budgets
 *****************************************************************/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <filesystem>  
#include <limits>

using namespace std;
namespace fs = std::filesystem;

//====================================================================
// UTILITY FUNCTIONS
//====================================================================

/**
 * Gets the absolute path of a file relative to the current directory
 * @param filename The name of the file
 * @return The absolute path as a string
 */
string getAbsolutePath(const string& filename) {
    fs::path currentPath = fs::current_path();
    fs::path filePath = currentPath / filename;
    return filePath.string();
}

/**
 * Clears input buffer and handles invalid input
 */
void clearInputBuffer() {
    cin.clear(); // Clear error flags
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Skip to next newline
}

/**
 * Gets valid integer input from user
 * @param prompt Message to display to user
 * @return Valid integer input
 */
int getValidIntInput(const string& prompt) {
    int input;
    cout << prompt;
    while (!(cin >> input)) {
        cout << "Invalid input. Please enter a number: ";
        clearInputBuffer();
    }
    clearInputBuffer();
    return input;
}

/**
 * Gets valid double input from user
 * @param prompt Message to display to user
 * @return Valid double input
 */
double getValidDoubleInput(const string& prompt) {
    double input;
    cout << prompt;
    while (!(cin >> input) || input < 0) {
        cout << "Invalid input. Please enter a positive number: ";
        clearInputBuffer();
    }
    clearInputBuffer();
    return input;
}

/**
 * Gets non-empty string input from user
 * @param prompt Message to display to user
 * @return Non-empty string input
 */
string getValidStringInput(const string& prompt) {
    string input;
    cout << prompt;
    getline(cin, input);
    while (input.empty()) {
        cout << "Input cannot be empty. Please try again: ";
        getline(cin, input);
    }
    return input;
}

//====================================================================
// STRUCTURES
//====================================================================

/**
 * Represents a city with an index and name
 */
struct City {
    int index;      
    string name;    
};

/**
 * Represents a road connection between two cities with a budget
 */
struct Road {
    int city1;      
    int city2;      
    double budget;  
};

//====================================================================
// RWANDA INFRASTRUCTURE CLASS
//====================================================================

/**
 * Main class for managing Rwanda's infrastructure data
 * Handles cities, roads, and budget allocations
 */
class RwandaInfrastructure {
private:
    vector<City> cities;                  
    vector<vector<int>> roadMatrix;       
    vector<vector<double>> budgetMatrix; 
    
    int findCityIndex(const string& name) {
        for (const auto& city : cities) {
            if (city.name == name) {
                return city.index;
            }
        }
        return -1;
    }
    
    /**
     * Initializes the road and budget matrices
     * Called when the first city is added
     */
    void initializeMatrices() {
        int size = cities.size();
        roadMatrix.resize(size, vector<int>(size, 0));
        budgetMatrix.resize(size, vector<double>(size, 0.0));
    }
    
    /**
     * Resizes the matrices when new cities are added
     * Preserves existing connections and budgets
     */
    void resizeMatrices() {
        int newSize = cities.size();
        // Resize road matrix
        for (auto& row : roadMatrix) {
            row.resize(newSize, 0);
        }
        roadMatrix.resize(newSize, vector<int>(newSize, 0));
        
        // Resize budget matrix
        for (auto& row : budgetMatrix) {
            row.resize(newSize, 0.0);
        }
        budgetMatrix.resize(newSize, vector<double>(newSize, 0.0));
    }
    
public:
    /**
     * Constructor for the RwandaInfrastructure class
     * Initializes with empty matrices
     */
    RwandaInfrastructure() {
        // Initialize with empty matrices
    }
    
    bool addCity(const string& name) {
        // Check if city already exists
        if (findCityIndex(name) != -1) {
            cout << "City " << name << " already exists." << endl;
            return false;
        }
        
        int newIndex = cities.empty() ? 1 : cities.back().index + 1;
        cities.push_back({newIndex, name});
        
        // Resize matrices if they exist
        if (!roadMatrix.empty()) {
            resizeMatrices();
        } else {
            initializeMatrices();
        }
        
        cout << "City " << name << " added with index " << newIndex << endl;
        return true;
    }
    
    bool addRoad(const string& city1, const string& city2) {
        if (city1 == city2) {
            cout << "Cannot add a road between the same city." << endl;
            return false;
        }
        
        int idx1 = findCityIndex(city1);
        int idx2 = findCityIndex(city2);
        
        if (idx1 == -1 || idx2 == -1) {
            cout << "One or both cities not found." << endl;
            return false;
        }
        
        // Adjust for 0-based index in matrix
        int i = idx1 - 1;
        int j = idx2 - 1;
        
        if (roadMatrix[i][j] == 1) {
            cout << "A road already exists between " << city1 << " and " << city2 << endl;
            return false;
        }
        
        roadMatrix[i][j] = 1;
        roadMatrix[j][i] = 1;
        
        cout << "Road added between " << city1 << " and " << city2 << endl;
        return true;
    }
    

    bool addBudget(const string& city1, const string& city2, double budget) {
        if (budget < 0) {
            cout << "Budget cannot be negative." << endl;
            return false;
        }
        
        int idx1 = findCityIndex(city1);
        int idx2 = findCityIndex(city2);
        
        if (idx1 == -1 || idx2 == -1) {
            cout << "One or both cities not found." << endl;
            return false;
        }
        
        // Adjust for 0-based index in matrix
        int i = idx1 - 1;
        int j = idx2 - 1;
        
        if (roadMatrix[i][j] == 0) {
            cout << "No road exists between " << city1 << " and " << city2 << endl;
            return false;
        }
        
        budgetMatrix[i][j] = budget;
        budgetMatrix[j][i] = budget;
        
        cout << "Budget of " << budget << " billion RWF added for road between " 
             << city1 << " and " << city2 << endl;
        return true;
    }
    

    bool editCity(const string& oldName, const string& newName) {
        if (oldName == newName) {
            cout << "New name is the same as the old name." << endl;
            return false;
        }
        
        int idx = findCityIndex(oldName);
        if (idx == -1) {
            cout << "City " << oldName << " not found." << endl;
            return false;
        }
        
        // Check if new name already exists
        if (findCityIndex(newName) != -1) {
            cout << "City " << newName << " already exists." << endl;
            return false;
        }
        
        for (auto& city : cities) {
            if (city.name == oldName) {
                city.name = newName;
                cout << "City renamed from " << oldName << " to " << newName << endl;
                return true;
            }
        }
        
        return false;
    }
    
    void searchCityByIndex(int idx) {
        for (const auto& city : cities) {
            if (city.index == idx) {
                cout << "City found: " << city.index << ": " << city.name << endl;
                return;
            }
        }
        cout << "City with index " << idx << " not found." << endl;
    }
    
    bool hasCities() const {
        return !cities.empty();
    }
    
    /**
     * Displays all cities and their indices
     */
    void displayCities() {
        if (cities.empty()) {
            cout << "No cities recorded yet." << endl;
            return;
        }
        
        cout << "\nCities:\n";
        for (const auto& city : cities) {
            cout << city.index << ": " << city.name << endl;
        }
    }
    
    /**
     * Displays the road network as an adjacency matrix
     */
    void displayRoads() {
        if (cities.empty()) {
            cout << "No cities recorded yet." << endl;
            return;
        }
        
        cout << "\nRoads Adjacency Matrix:\n";
        cout << "    ";
        for (const auto& city : cities) {
            cout << setw(4) << city.index;
        }
        cout << endl;
        
        for (size_t i = 0; i < roadMatrix.size(); ++i) {
            cout << setw(4) << cities[i].index;
            for (size_t j = 0; j < roadMatrix[i].size(); ++j) {
                cout << setw(4) << roadMatrix[i][j];
            }
            cout << endl;
        }
    }
    
    /**
     * Displays the budget allocations as a matrix
     */
    void displayBudgets() {
        if (cities.empty()) {
            cout << "No cities recorded yet." << endl;
            return;
        }
        
        cout << "\nBudgets Adjacency Matrix (in billion RWF):\n";
        cout << fixed << setprecision(1);
        cout << "    ";
        for (const auto& city : cities) {
            cout << setw(8) << city.index;
        }
        cout << endl;
        
        for (size_t i = 0; i < budgetMatrix.size(); ++i) {
            cout << setw(4) << cities[i].index;
            for (size_t j = 0; j < budgetMatrix[i].size(); ++j) {
                cout << setw(8) << budgetMatrix[i][j];
            }
            cout << endl;
        }
    }
    
    /**
     * Displays all data (cities, roads, and budgets)
     */
    void displayAllData() {
        displayCities();
        displayRoads();
        displayBudgets();
    }
    
    /**
     * Saves all data to text files (cities.txt and roads.txt)
     * Creates well-formatted tables with proper column alignment
     */
    void saveToFiles() {
        // Get absolute paths for files
        string cityFilePath = getAbsolutePath("cities.txt");
        string roadFilePath = getAbsolutePath("roads.txt");
        
        // Save cities to cities.txt
        ofstream cityFile(cityFilePath);
        if (!cityFile.is_open()) {
            cerr << "Error: Could not open cities.txt for writing!" << endl;
            return;
        }
        
        // Write header with proper spacing
        cityFile << left << setw(8) << "Index" << setw(20) << "City_Name" << endl;
        // Write city data
        for (const auto& city : cities) {
            cityFile << left << setw(8) << city.index << setw(20) << city.name << endl;
        }
        cityFile.close();
        
        // Save roads to roads.txt
        ofstream roadFile(roadFilePath);
        if (!roadFile.is_open()) {
            cerr << "Error: Could not open roads.txt for writing!" << endl;
            return;
        }
        
        // Write header with proper spacing
        roadFile << left << setw(5) << "Nbr" << setw(25) << "Road" << setw(10) << "Budget" << endl;
        int counter = 1;
        for (size_t i = 0; i < roadMatrix.size(); ++i) {
            for (size_t j = i + 1; j < roadMatrix[i].size(); ++j) {
                if (roadMatrix[i][j] == 1) {
                    string roadName = cities[i].name + "-" + cities[j].name;
                    roadFile << left << setw(5) << (to_string(counter++) + ".")
                           << setw(25) << roadName
                           << setw(10) << budgetMatrix[i][j] << endl;
                }
            }
        }
        roadFile.close();
    }
    
    /**
     * Loads initial data for Rwanda's infrastructure
     * Creates cities and roads with predefined budget allocations
     */
    void loadInitialData() {
        // Add the 7 initial cities
        vector<string> initialCities = {
            "Kigali", "Huye", "Muhanga", "Musanze", 
            "Nyagatare", "Rubavu", "Rusizi"
        };
        
        for (const auto& city : initialCities) {
            addCity(city);
        }
        
        // Add the initial roads with budgets
        vector<tuple<string, string, double>> initialRoads = {
            {"Kigali", "Muhanga", 28.6},
            {"Kigali", "Musanze", 28.6},
            {"Kigali", "Nyagatare", 70.84},
            {"Muhanga", "Huye", 56.7},
            {"Musanze", "Rubavu", 33.7},
            {"Huye", "Rusizi", 80.96},
            {"Muhanga", "Rusizi", 117.5},
            {"Musanze", "Nyagatare", 96.14},
            {"Muhanga", "Musanze", 66.3}
        };
        
        for (const auto& road : initialRoads) {
            addRoad(get<0>(road), get<1>(road));
            addBudget(get<0>(road), get<1>(road), get<2>(road));
        }
        
        // Save to files after initial data is loaded
        saveToFiles();
    }
};

//====================================================================
// MAIN FUNCTION
//====================================================================

/**
 * Main function - Entry point of the program
 * Initializes the infrastructure system 
 */
int main() {
    // Create and initialize the Rwanda infrastructure system
    RwandaInfrastructure rwanda;
    rwanda.loadInitialData();
    
    int choice;
    bool validInput;
    
    do {
        // Display the main menu
        cout << "\nMenu:\n";
        cout << "1. Add new city(ies)\n";
        cout << "2. Add roads between cities\n";
        cout << "3. Add the budget for roads\n";
        cout << "4. Edit city\n";
        cout << "5. Search for a city\n";
        cout << "6. Display cities\n";
        cout << "7. Display roads\n";
        cout << "8. Display recorded data on the console\n";
        cout << "9. Exit\n";
        
        choice = getValidIntInput("Enter your choice: ");
        
        // Process user's choice
        switch (choice) {
            case 1: {
                // Add new cities
                int numCities = getValidIntInput("Enter the number of cities to add: ");
                if (numCities <= 0) {
                    cout << "Number of cities must be positive." << endl;
                    break;
                }
                
                int addedCount = 0;
                for (int i = 0; i < numCities; ++i) {
                    string cityName = getValidStringInput("Enter the name for city " + to_string(i + 1) + ": ");
                    if (rwanda.addCity(cityName)) {
                        addedCount++;
                    }
                }
                
                if (addedCount > 0) {
                    rwanda.saveToFiles(); // Save after cities are added
                    cout << addedCount << " cities added successfully." << endl;
                }
                break;
            }
            case 2: {
                // Add a road between two cities
                if (!rwanda.hasCities()) {
                    cout << "No cities exist yet. Add cities first." << endl;
                    break;
                }
                
                string city1 = getValidStringInput("Enter the name of the first city: ");
                string city2 = getValidStringInput("Enter the name of the second city: ");
                
                if (rwanda.addRoad(city1, city2)) {
                    rwanda.saveToFiles(); // Save after road is added
                }
                break;
            }
            case 3: {
                // Add budget for a road
                if (!rwanda.hasCities()) {
                    cout << "No cities exist yet. Add cities first." << endl;
                    break;
                }
                
                string city1 = getValidStringInput("Enter the name of the first city: ");
                string city2 = getValidStringInput("Enter the name of the second city: ");
                double budget = getValidDoubleInput("Enter the budget for the road (in billion RWF): ");
                
                if (rwanda.addBudget(city1, city2, budget)) {
                    rwanda.saveToFiles(); // Save after budget is added
                }
                break;
            }
            case 4: {
                // Edit city name
                if (!rwanda.hasCities()) {
                    cout << "No cities exist yet. Add cities first." << endl;
                    break;
                }
                
                string oldName = getValidStringInput("Enter the current city name: ");
                string newName = getValidStringInput("Enter the new city name: ");
                
                if (rwanda.editCity(oldName, newName)) {
                    rwanda.saveToFiles(); // Save after city is edited
                }
                break;
            }            
            case 5: {
                // Search for a city by index
                if (!rwanda.hasCities()) {
                    cout << "No cities exist yet. Add cities first." << endl;
                    break;
                }
                
                int idx = getValidIntInput("Enter the city index to search: ");
                rwanda.searchCityByIndex(idx);
                break;
            }
            case 6:
                // Display all cities
                rwanda.displayCities();
                break;
            case 7:
                // Display road network
                rwanda.displayRoads();
                break;
            case 8:
                // Display all infrastructure data
                rwanda.displayAllData();
                break;
            case 9:
                // Exit the program
                cout << "Exiting program.\n";
                break;
            default:
                cout << "Invalid choice. Please enter a number between 1 and 9.\n";
        }
    } while (choice != 9);
    
    return 0;
}