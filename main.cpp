#include <stdlib.h>
#include <iostream>
#include <fstream>
using namespace std;

struct district {
    string name;
    string type;
    int num_resident;
    int power;
    bool has_power;                         // true = 1; false = 0
    int num_technician;
    string technicians[10];
};

bool load(string fname, district districts[], int &num, int &power) {
    ifstream ifile(fname);

    if (!ifile.is_open()) {
        cout << "File not found! Exiting program..." << endl;
        return false;
    }
    
    ifile >> power >> num;
    ifile.ignore();

    // load info into array of districts
    for (int i = 0; i < num; ++i) {
        getline(ifile, districts[i].name);
        ifile >> districts[i].type >> districts[i].num_resident >> districts[i].power >> districts[i].has_power >> districts[i].num_technician;
        ifile.ignore();
        for (int j = 0; j < districts[i].num_technician; ++j) {
            getline(ifile, districts[i].technicians[j]);
        }
    }

    // sort districts in ascending order based on num_resident
    district temp;
    for (int j = 0; j < num - 1; ++j) {
        for (int i = 0; i < num - 1; ++i) {
            if (districts[i].num_resident > districts[i+1].num_resident) {
                temp = districts[i];
                districts[i] = districts[i+1];
                districts[i+1] = temp;
            }
        }
    }

    ifile.close();

    return true;
}

void save(string fname, district districts[], int num, int power) {
    ofstream ofile(fname);
    ofile << power << endl << num << endl;
    for (int i = 0; i < num; ++i) {
        ofile << districts[i].name << endl << districts[i].type << endl << districts[i].num_resident << endl << districts[i].power << endl << districts[i].has_power << endl << districts[i].num_technician << endl;
        for (int j = 0; j < districts[i].num_technician; ++j) {
            ofile << districts[i].technicians[j] << endl;
        }
    }
    cout << "Current status has been saved to " << fname << endl;
    ofile.close();
}

void print_all(district districts[], int num, int power) {
    cout << "The current power supply is " << power << "MW." << endl;
    for (int i = 0; i < num; ++i) {
        cout << districts[i].name << endl;
        cout << "District type: " << districts[i].type << endl;
        cout << "Number of residents: " << districts[i].num_resident << endl;
        if (districts[i].has_power) {
            cout << "Power use: " << districts[i].power << "MW" << endl;
        }
        else {
            cout << "Power use: 0MW" << endl;
        }
        cout << "Has power: " << districts[i].has_power << endl;
        if (districts[i].num_technician != 0) {
            cout << "Technicians: ";
            for (int j = 0; j < districts[i].num_technician; ++j) {
                cout << districts[i].technicians[j];
                if (j != districts[i].num_technician - 1) {                                             // if not last technician
                    cout << ", ";
                }
            }
            cout << endl;
        }
        cout << "---" << endl;
    }

    int totalCurrentPowConsump = 0;
    for (int i = 0; i < num; ++i) {
        if (districts[i].has_power) {
            totalCurrentPowConsump += districts[i].power;
        }
    }
    if (power < totalCurrentPowConsump) {
        cout << "Critical Error! The current power supply of " << power << "MW is insufficient to supply " << totalCurrentPowConsump << "MW of power demand. Please shut off one or more districts." << endl << "---" << endl;
    }
}

void assign_technician(string tech_name, string district_name, district districts[], int num_district) {
    for (int i = 0; i < num_district; ++i) {
        if (districts[i].name == district_name) {
            districts[i].technicians[districts[i].num_technician] = tech_name;
            ++districts[i].num_technician;
            cout << "Technician assigned!" << endl;
            return;
        }
    }
    cout << "Error! The district with that name doesn't exist." << endl;
}

bool cut_power(string district_name, district districts[], int num_district) {
    for (int i = 0; i < num_district; ++i) {
        if (districts[i].name == district_name) {
            districts[i].has_power = 0;
            return true;
        }
    }
    return false;
}

bool restore_power(string district_name, district districts[], int num_district) {
    for (int i = 0; i < num_district; ++i) {
        if (districts[i].name == district_name) {
            districts[i].has_power = 1;
            return true;
        }
    }
    return false;
}

int main() {
    system("clear");

    string filename;
    cout << "Please enter the name of the file to load:" << endl;
    cin >> filename;

    district districts[10];
    int numDistricts, totalPowerSupply;

    if (! load(filename, districts, numDistricts, totalPowerSupply)) {
        return 1;
    }
    print_all(districts, numDistricts, totalPowerSupply);

    bool cont = true;
    int option;
    while(cont) {
        cout << "Main Menu\n" << "1- Status\n" << "2- Set Power Supply\n" << "3- Shutoff Power\n" << "4- Restore Power\n" << "5- Assign Technician\n" << "6- Save\n" << "7- Load\n" << "8- Exit\nPlease enter your choice:\n";
        cin >> option;
        if (option == 1) {
            print_all(districts, numDistricts, totalPowerSupply);
        }
        else if (option == 2) {
            cout << "Enter the updated power supply in megawatts:" << endl;
            int newTotalPowerSupply;
            cin >> newTotalPowerSupply;
            totalPowerSupply = newTotalPowerSupply;
            cout << "Power supply updated!" << endl;
        }
        else if (option == 3) {
            cout << "Enter the name of the district:" << endl;
            string searchDistrict;
            cin.ignore();
            getline(cin, searchDistrict);
            if (cut_power(searchDistrict, districts, numDistricts)) {
                cout << "The district's power has been successfully turned off." << endl;
            }
            else {
                cout << "Error! The district with that name doesn't exist." << endl;
            }
        }
        else if (option == 4) {
            cout << "Enter the name of the district:" << endl;
            string searchDistrict;
            cin.ignore();
            getline(cin, searchDistrict);
            if (restore_power(searchDistrict, districts, numDistricts)) {
                cout << "The district's power has been successfully turned back on." << endl;
            }
            else {
                cout << "Error! The district with that name doesn't exist." << endl;
            }
        }
        else if (option == 5) {
            cout << "Enter the name of the technician:" << endl;
            string tech;
            cin.ignore();
            getline(cin, tech);
            cout << "Enter the name of the district to assign them to:" << endl;
            string district;
            getline(cin, district);
            assign_technician(tech, district, districts, numDistricts);
        }
        else if (option == 6) {
            cout << "Enter the file name to save to:" << endl;
            string filename;
            cin >> filename;
            save(filename, districts, numDistricts, totalPowerSupply);
        }
        else if (option == 7) {
            cout << "Enter the file name to load from:" << endl;
            string filename;
            cin >> filename;
            if (! load(filename, districts, numDistricts, totalPowerSupply)) {
                return 1;
            }
        }
        else if (option == 8) {
            cout << "Thank you for using the system! Have a good day!" << endl;
            cont = false;
        }
        else {
            cout << "Invalid option!" << endl;
        }
    }

    return 0;
}
