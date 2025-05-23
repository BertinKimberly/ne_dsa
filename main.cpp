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
#include <filesystem>  // For std::filesystem::path and absolute

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

//====================================================================
// DATA STRUCTURES
//====================================================================

/**
 * Represents a city with an index and name
 */
struct City {
    int index;      // Unique identifier for the city
    string name;    // Name of the city
};

/**
 * Represents a road connection between two cities with a budget
 */
struct Road {
    int city1;      // Index of the first city
    int city2;      // Index of the second city
    double budget;  // Budget allocation in billion RWF
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
    vector<City> cities;                  // List of all cities
    vector<vector<int>> roadMatrix;       // Adjacency matrix for roads (0 or 1)
    vector<vector<double>> budgetMatrix;  // Matrix for budget allocations
    
    /**
     * Finds the index of a city by its name
     * @param name The name of the city to find
     * @return The city's index or -1 if not found
     */
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
    
    /**
     * Adds a new city to the system
     * @param name The name of the city to add
     */
    void addCity(const string& name) {
        // Check if city already exists
        if (findCityIndex(name) != -1) {
            cout << "City " << name << " already exists." << endl;
            return;
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
        
        // No saveToFiles() call here to prevent duplicate calls during loadInitialData
    }
    
    /**
     * Adds a road connection between two cities
     * @param city1 The name of the first city
     * @param city2 The name of the second city
     */
    void addRoad(const string& city1, const string& city2) {
        int idx1 = findCityIndex(city1);
        int idx2 = findCityIndex(city2);
        
        if (idx1 == -1 || idx2 == -1) {
            cout << "One or both cities not found." << endl;
            return;
        }
        
        // Adjust for 0-based index in matrix
        int i = idx1 - 1;
        int j = idx2 - 1;
        
        roadMatrix[i][j] = 1;
        roadMatrix[j][i] = 1;
        
        cout << "Road added between " << city1 << " and " << city2 << endl;
        
        // No saveToFiles() call here to prevent duplicate calls during loadInitialData
    }
    
    /**
     * Adds a budget allocation for a road between two cities
     * @param city1 The name of the first city
     * @param city2 The name of the second city
     * @param budget The budget amount in billion RWF
     */
    void addBudget(const string& city1, const string& city2, double budget) {
        int idx1 = findCityIndex(city1);
        int idx2 = findCityIndex(city2);
        
        if (idx1 == -1 || idx2 == -1) {
            cout << "One or both cities not found." << endl;
            return;
        }
        
        // Adjust for 0-based index in matrix
        int i = idx1 - 1;
        int j = idx2 - 1;
        
        if (roadMatrix[i][j] == 0) {
            cout << "No road exists between " << city1 << " and " << city2 << endl;
            return;
        }
        
        budgetMatrix[i][j] = budget;
        budgetMatrix[j][i] = budget;
        
        cout << "Budget of " << budget << " billion RWF added for road between " 
             << city1 << " and " << city2 << endl;
             
        // No saveToFiles() call here to prevent duplicate calls during loadInitialData
    }
    
    /**
     * Edits the name of an existing city
     * @param oldName The current name of the city
     * @param newName The new name for the city
     */
    void editCity(const string& oldName, const string& newName) {
        int idx = findCityIndex(oldName);
        if (idx == -1) {
            cout << "City " << oldName << " not found." << endl;
            return;
        }
        // Check if new name already exists
        if (findCityIndex(newName) != -1) {
            cout << "City " << newName << " already exists." << endl;
            return;
        }
        
        for (auto& city : cities) {
            if (city.name == oldName) {
                city.name = newName;
                cout << "City renamed from " << oldName << " to " << newName << endl;
                return;
            }
        }
    }
    
    /**
     * Searches for a city by its index
     * @param idx The index of the city to find
     */
    void searchCityByIndex(int idx) {
        for (const auto& city : cities) {
            if (city.index == idx) {
                cout << "City found: " << city.index << ": " << city.name << endl;
                return;
            }
        }
        cout << "City with index " << idx << " not found." << endl;
    }
    
    /**
     * Displays all cities and their indices
     */
    void displayCities() {
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
        cout << "     ";
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
        cout << "     ";
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
 * Initializes the infrastructure system and handles the user interface
 */
int main() {
    // Create and initialize the Rwanda infrastructure system
    RwandaInfrastructure rwanda;
    rwanda.loadInitialData();
    
    int choice;
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
        cout << "Enter your choice: ";
        cin >> choice;
        
        cin.ignore(); // Clear newline character
        
        // Process user's choice
        switch (choice) {
            case 1: {
                // Add new cities
                int numCities;
                cout << "Enter the number of cities to add: ";
                cin >> numCities;
                cin.ignore();
                
                for (int i = 0; i < numCities; ++i) {
                    string cityName;
                    cout << "Enter the name for city " << (i + 1) << ": ";
                    getline(cin, cityName);
                    rwanda.addCity(cityName);
                }
                rwanda.saveToFiles(); // Save after cities are added
                break;
            }
            case 2: {
                // Add a road between two cities
                string city1, city2;
                cout << "Enter the name of the first city: ";
                getline(cin, city1);
                cout << "Enter the name of the second city: ";
                getline(cin, city2);
                rwanda.addRoad(city1, city2);
                rwanda.saveToFiles(); // Save after road is added
                break;
            }
            case 3: {
                // Add budget for a road
                string city1, city2;
                double budget;
                cout << "Enter the name of the first city: ";
                getline(cin, city1);
                cout << "Enter the name of the second city: ";
                getline(cin, city2);
                cout << "Enter the budget for the road (in billion RWF): ";
                cin >> budget;
                cin.ignore();
                rwanda.addBudget(city1, city2, budget);
                rwanda.saveToFiles(); // Save after budget is added
                break;
            }
            case 4: {
                // Edit city name
                string oldName, newName;
                cout << "Enter the current city name: ";
                getline(cin, oldName);
                cout << "Enter the new city name: ";
                getline(cin, newName);
                rwanda.editCity(oldName, newName);
                rwanda.saveToFiles(); // Save after city is edited
                break;
            }            
            case 5: {
                // Search for a city by index
                int idx;
                cout << "Enter the city index to search: ";
                cin >> idx;
                cin.ignore(); // Clear newline character
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
                cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 9);
    
    return 0;
}