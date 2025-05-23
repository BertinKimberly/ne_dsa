# 🌍 Rwanda Infrastructure Management System

A comprehensive C++ application for managing infrastructure data for Rwandan cities, including road networks and budget allocations.

![C++](https://img.shields.io/badge/C%2B%2B-17-blue)
![License](https://img.shields.io/badge/license-MIT-green)
![Status](https://img.shields.io/badge/status-active-success)

## 📋 Table of Contents
- [🌍 Rwanda Infrastructure Management System](#-rwanda-infrastructure-management-system)
  - [📋 Table of Contents](#-table-of-contents)
  - [🎯 Overview](#-overview)
  - [✨ Features](#-features)
  - [🚀 Getting Started](#-getting-started)
    - [Prerequisites](#prerequisites)
    - [Installation](#installation)
  - [💻 Usage](#-usage)
  - [📁 Data Storage](#-data-storage)

## 🎯 Overview

The Rwanda Infrastructure Management System is designed to help manage and track infrastructure development across major cities in Rwanda. It provides a user-friendly interface for managing city data, road connections, and budget allocations for infrastructure projects.

## ✨ Features

- 🏙️ **City Management**
  - Add new cities
  - Edit existing city names
  - Search cities by index
  - View all registered cities

- 🛣️ **Road Network Management**
  - Add road connections between cities
  - Visualize road networks through adjacency matrix
  - Track road connections efficiently

- 💰 **Budget Management**
  - Allocate budgets for road infrastructure
  - Track budget distribution across different routes
  - View budget allocations in a clear matrix format

## 🚀 Getting Started

### Prerequisites

- C++ compiler with C++17 support
- Standard Template Library (STL)
- File system support

### Installation

1. Clone the repository:
```powershell
git clone https://github.com/BertinKimberly/ne_dsa.git
cd ne_dsa
```

2. Compile the source code:
```powershell
g++ -std=c++17 main.cpp -o rwanda
```

3. Run the application:
```powershell
./rwanda
```

## 💻 Usage

The system provides an interactive menu with the following options:

1. Add new city(ies)
2. Add roads between cities
3. Add budget for roads
4. Edit city names
5. Search for a city
6. Display cities
7. Display roads
8. Display all recorded data
9. Exit

## 📁 Data Storage

The system stores data in two main files:

- `cities.txt`: Contains information about all registered cities
- `roads.txt`: Stores road connections and their associated budgets

Data is automatically saved after each operation, ensuring data persistence.


---

Made by Iradukunda Bertin