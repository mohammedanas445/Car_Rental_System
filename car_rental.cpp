#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <ctime>
#include <iomanip>
#include <limits>

using namespace std;

#define CostPerDay 300;
#define finePerDay 30;

class Car {
public:
    int id;
    string model;
    string condition;
    string otherDetails;
    bool isRented;
    string rentedTo;
    int renterId;
    string returnDateStr;

    Car() : isRented(false), returnDateStr(""), rentedTo(""), renterId(-1) {}
    Car(const int& id,const string& model, const string& condition, const string& otherDetails)
        : id(id), model(model), condition(condition), otherDetails(otherDetails) {}
    
    int RentRequest();
    void ShowDueDate() const;
};

class CarDatabase {
    vector<Car> cars;
    const string filename = "cars.txt";

public:
    CarDatabase() {
        LoadData();
    }

    ~CarDatabase(){
        SaveData();
    }

    void LoadData() {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return;
        }

        int id, renterId;
        string model, condition, otherDetails, rentedTo;
        bool isRented;
        string returnDateStr;

        while (file >> id >> model >> condition >> otherDetails >> isRented) {
            
            Car car(id, model, condition, otherDetails);
            if(isRented){
                file >> returnDateStr >> rentedTo >> renterId;
                car.returnDateStr = returnDateStr;
                car.rentedTo = rentedTo;
                car.renterId = renterId;
            }
            car.isRented = isRented;
            
            
            cars.push_back(car);
        }

        file.close();
    }

    void SaveData() {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return;
        }
        
        for (const Car& car : cars) {
            file << car.id << " " << car.model << " " << car.condition << " " << car.otherDetails << " "<< car.isRented << " " ;
            if(car.isRented){
                file << car.returnDateStr << " " << car.rentedTo << " " << car.renterId << " ";
            }
            file << endl;
        }

        file.close();
    }

    void AddCar(const Car& car) {
        cars.push_back(car);
        SaveData();
    }

    void UpdateCar(const Car& car) {
        for (Car& c : cars) {
            if (c.id == car.id) {
                c = car;
                SaveData();
                cout << "Car updated successfully." << endl;
                return;
            }
        }
        cout << "Car not found." << endl;
        SaveData();
    }

    void DeleteCar(const Car& car) {
        for (auto it = cars.begin(); it != cars.end(); ++it) {
            if (it->id == car.id) {
                cars.erase(it);
                SaveData();
                cout << "Car deleted successfully." << endl;
                return;
            }
        }
        cout << "Car not found." << endl;
        SaveData();
    }

    Car* SearchCar(const int& id) {
        for (Car& car : cars) {
            if (car.id == id) {
                return &car;
            }
        }
        return nullptr;
    }

    void ShowAvailableCars() const {
        cout << "Available Cars:" << endl;
        for (const Car& car : cars) {
            if (!car.isRented) {
                cout << "Car ID: " << car.id << ", Model: " << car.model << ", Condition: " << car.condition << ", Other Details: " << car.otherDetails << endl;
            }
        }
    }

    void ShowAllCars() const {
        cout << "All Cars:" << endl;
        for (const Car& car : cars) {
            cout << "Car ID: " << car.id << ", Model: " << car.model << ", Condition: " << car.condition << ", Other Details: " << car.otherDetails << ", Is Rented? " << car.isRented << ", Rented To: " << car.rentedTo << ", Renter ID: " << car.renterId << endl << endl;
            
        }
    }
};

class User {
protected:
    string name;
    int id;
    string password;
public:
    User(const string& name, int id, const string& password) : name(name), id(id), password(password) {}
    virtual ~User() {}
    virtual void DisplayInfo() const = 0;
};

class Customer : public User {
    vector<int> rentedCars;
    int numOfCarsRented;
    float fineDue;
    float customerRecord;
public:
    Customer(const string& name, int id, const string& password, float customerRecord)
        : User(name, id, password), fineDue(0), customerRecord(customerRecord), numOfCarsRented(0) {}

    friend class CustomerDatabase;
    friend class Manager;
    bool authentication(string password){return this->password==password;}
    void RentCar();
    void ClearDue();
    void DisplayInfo() const override;
    void ShowRentedCars() const;
    void returnCar();
};

class CustomerDatabase {
    vector<Customer> customers;
    const string filename = "customers.txt";

public:
    CustomerDatabase() {
        LoadData();
    }

    ~CustomerDatabase(){
        SaveData();
    }

    void LoadData() {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return;
        }

        string name, password;
        int id;
        float customerRecord;
        float fineDue;
        int numOfCarsRented;
        vector<int> rentedCars;

        while (file >> name >> id >> password >> customerRecord >> fineDue >> numOfCarsRented ) {
            rentedCars.clear();
            for(int i=0; i<numOfCarsRented; i++) {
                int carId;
                file >> carId;
                rentedCars.push_back(carId);
            }
            Customer customer(name, id, password, customerRecord);
            customer.fineDue = fineDue;
            customer.rentedCars = rentedCars;
            customer.numOfCarsRented= numOfCarsRented;
            customers.push_back(customer);
        }

        file.close();
    }

    void SaveData() {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return;
        }

        for (const Customer& customer : customers) {
            file << customer.name << " " << customer.id << " " << customer.password << " " << customer.customerRecord << " " << customer.fineDue << " " << customer.numOfCarsRented << " ";
            for (int carId : customer.rentedCars) {
                file << carId << " ";
            }
            file << endl;
        }

        file.close();
    }

    void AddCustomer(const Customer& customer) {
        customers.push_back(customer);
        SaveData();
    }

    void UpdateCustomer(const Customer& customer) {
        for (Customer& c : customers) {
            if (c.id == customer.id) {
                c = customer;
                SaveData();
                cout << "Customer updated successfully." << endl;
                return;
            }
        }
        cout << "Customer not found." << endl;
        SaveData();
    }

    void DeleteCustomer(const Customer& customer) {
        for (auto it = customers.begin(); it != customers.end(); ++it) {
            if (it->id == customer.id) {
                customers.erase(it);
                SaveData();
                cout << "Customer deleted successfully." << endl;
                return;
            }
        }
        cout << "Customer not found." << endl;
        SaveData();
    }

    Customer* SearchCustomer(int id) {
        for (Customer& customer : customers) {
            if (customer.id == id) {
                return &customer;
            }
        }
        return nullptr;
    }
};

class Employee : public User {
    vector<int> rentedCars;
    int numOfCarsRented;
    float fineDue;
    float employeeRecord;
public:
    Employee(const string& name, int id, const string& password, float employeeRecord)
        : User(name, id, password), fineDue(0), employeeRecord(employeeRecord), numOfCarsRented(0) {}

    friend class EmployeeDatabase;
    friend class Manager;
    bool authentication(string password){return this->password==password;}
    void RentCar();
    void ClearDue();
    void DisplayInfo() const override;
    void ShowRentedCars() const;
    void returnCar();
};

class EmployeeDatabase {
    vector<Employee> employees;
    const string filename = "employees.txt";

public:
    EmployeeDatabase() {
        LoadData();
    }

    ~EmployeeDatabase(){
        SaveData();
    }

    void LoadData() {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return;
        }

        string name, password;
        int id;
        float employeeRecord;
        float fineDue;
        int numOfCarsRented;
        vector<int> rentedCars;

        while (file >> name >> id >> password >> employeeRecord >> fineDue >> numOfCarsRented ) {
            rentedCars.clear();
            for(int i=0; i<numOfCarsRented; i++) {
                int carId;
                file >> carId;
                rentedCars.push_back(carId);
            }
            Employee employee(name, id, password, employeeRecord);
            employee.fineDue = fineDue;
            employee.rentedCars = rentedCars;
            employee.numOfCarsRented= numOfCarsRented;
            employees.push_back(employee);
        }

        file.close();
    }

    void SaveData() {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Error opening file: " << filename << endl;
            return;
        }

        for (const Employee& employee : employees) {
            file << employee.name << " " << employee.id << " " << employee.password << " " << employee.employeeRecord << " " << employee.fineDue << " " << employee.numOfCarsRented << " ";
            for (int carId : employee.rentedCars) {
                file << carId << " ";
            }
            file << endl;
        }

        file.close();
    }

    void AddEmployee(const Employee& employee) {
        employees.push_back(employee);
        SaveData();
    }

    void UpdateEmployee(const Employee& employee) {
        for (Employee& e : employees) {
            if (e.id == employee.id) {
                e = employee;
                SaveData();
                cout << "Employee updated successfully." << endl;
                return;
            }
        }
        cout << "Employee not found." << endl;
        SaveData();
    }

    void DeleteEmployee(const Employee& employee) {
        for (auto it = employees.begin(); it != employees.end(); ++it) {
            if (it->id == employee.id) {
                employees.erase(it);
                SaveData();
                cout << "Employee deleted successfully." << endl;
                return;
            }
        }
        cout << "Employee not found." << endl;
        SaveData();
    }

    Employee* SearchEmployee(int id) {
        for (Employee& employee : employees) {
            if (employee.id == id) {
                return &employee;
            }
        }
        return nullptr;
    }
};

// Initialize your system with data (e.g., cars, customers, employees, and manager)
    CarDatabase carDB;
    CustomerDatabase customerDB;
    EmployeeDatabase employeeDB;


class Manager : public User {
public:
    Manager(const string& name, int id, const string& password)
        : User(name, id, password) {}

    void ManageCars();
    void ManageCustomers();
    void ManageEmployees();
    void DisplayInfo() const override;
};



int Car::RentRequest() {
    if (isRented) {
        cout << "Car is already rented." << endl;
        return 0;
    }

    cout << "Enter return date (YYYY-MM-DD): ";
    time_t returnDate;
    while(1){
        cin >> returnDateStr;

        // Validate return date format
        struct tm tm = {};
        if (strptime(returnDateStr.c_str(), "%Y-%m-%d", &tm) == nullptr) {
            cerr << "Invalid return date format. Please use YYYY-MM-DD." << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        returnDate = mktime(&tm);

        if (returnDate < time(nullptr)) {
            cerr << "Return date must be in the future." << endl;
            continue;
        }
        break;
    }

    this->isRented = true;

    cout << "Car rented successfully. Return by: " << returnDateStr << endl;
    int numDays=difftime(returnDate,time(0))/(60*60*24);
    carDB.SaveData();
    return numDays;
}

void Car::ShowDueDate() const {
    if (!isRented) {
        cout << "Car is not currently rented." << endl;
        return;
    }

    // Display due date
    cout << "Due date for the rental: ";
    cout << returnDateStr << endl;
}



void Customer::RentCar() {
    // Check if customer has rented too many cars
    if (rentedCars.size() >= customerRecord) {
        cout << "Already rented the maximum number of cars." << endl;
        return;
    }
    carDB.ShowAvailableCars();
    cout << "Enter the Car ID to be rented: ";
    int carId;
    cin >> carId;

    if (cin.fail()) {
        cout << "Invalid input. Please enter a valid integer." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }else if(carDB.SearchCar(carId)==nullptr){
        cout << "There's no Car with this ID!" << endl;
        return;
    }

    Car* car = carDB.SearchCar(carId);
    if (car->isRented) {
        cout << "Car is already rented." << endl;
        return;
    }
    // Rent the car
    int numDays = car->RentRequest();
    int cost = numDays * CostPerDay;
    cout << "Pay: " << cost << endl;

    car->isRented = true;
    car->rentedTo = "Customer";
    car->renterId = id;

    rentedCars.push_back(car->id);
    numOfCarsRented++;
    fineDue += cost;

    carDB.SaveData();
    customerDB.SaveData();
    cout << "Car rented successfully." << endl;
}

void Customer::ClearDue() {
    cout << "Paid: " << fineDue << endl;
    fineDue=0;
    customerDB.SaveData();
}

void Customer::DisplayInfo() const {
    cout << "Customer Info - Name: " << name << ", ID: " << id << ", Customer Record: " << customerRecord << ", Number of Cars Rented: " << numOfCarsRented << endl;
}

void Customer::ShowRentedCars() const {
    if (rentedCars.empty()) {
        cout << "No cars rented." << endl;
        return;
    }

    cout << "Rented Cars:" << endl;
    for (int carId : rentedCars) {
        const Car* car = carDB.SearchCar(carId);
        if (car != nullptr) {
            cout << "Car ID: " << car->id << ", Model: " << car->model << ", Condition: " << car->condition << endl;
        }
    }
    
    cout << endl;
    int choice;
    do{
        cout << "\nMenu:" << endl;
        cout << "1. Check Due Date" << endl;
        cout << "2. Go Back" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail() || (choice != 1 && choice != 2)) {
            cout << "Invalid input. Please enter 1 or 2." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1:{
                cout << "Enter Rented Car ID: " << endl;
                int carId;
                cin >> carId;
                if (cin.fail()) {
                    cout << "Invalid input. Please enter a valid integer." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    return;
                }
                Car* car = carDB.SearchCar(carId);
                if (car == nullptr) {
                    cout << "Car with ID " << carId << " not found." << endl;
                    return;
                }

                if (!car->isRented || car->rentedTo != "Customer" || car->renterId != id) {
                    cout << "You have not rented this car." << endl;
                    return;
                }
                car->ShowDueDate();
                break;
            }
            case 2:
                break;
        }
    }while(choice!=2);
}

void Customer::returnCar() {
    
    Customer::ShowRentedCars();
    cout << endl;
    int carId;
    cout << "Enter the Car ID to be returned: ";
    cin >> carId;
    if (cin.fail()) {
        cout << "Invalid input. Please enter a valid integer." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    Car* car = carDB.SearchCar(carId);
    if (car == nullptr) {
        cout << "Car with ID " << carId << " not found." << endl;
        return;
    }

    if (!car->isRented || car->rentedTo != "Customer" || car->renterId != id) {
        cout << "You have not rented this car." << endl;
        return;
    }

    // Calculate the number of days the car was rented
    struct tm returnDateTm = {};
    strptime(car->returnDateStr.c_str(), "%Y-%m-%d", &returnDateTm);
    time_t returnDate = mktime(&returnDateTm);
    int numDays = difftime(time(nullptr), returnDate) / (60 * 60 * 24);

    // Calculate the total cost based on the number of days and condition of the car
    int cost = numDays * CostPerDay;
    fineDue += cost;
    string carCondition;
    cout << "How is the Car's Condition? " << endl;
    cin >> carCondition;
    car->condition = carCondition;
    if (car->condition == "damaged") {
        fineDue += 500; // Assuming fixed cost for damage
        customerRecord*=0.8;
    }

    // Update the customer's record
    if (numDays>0) {
        customerRecord*=0.9;
        fineDue+= numDays*finePerDay;
    }

    // Update the car's status and save the changes
    car->isRented = false;
    car->rentedTo = "";
    car->renterId = -1;
    carDB.SaveData();

    //updating customer
    for (auto it = rentedCars.begin(); it != rentedCars.end(); ++it) {
            if (*it == carId) {
                rentedCars.erase(it);
            }
    }
    numOfCarsRented--;
    customerDB.SaveData();
    cout << "Car returned successfully. Total cost: " << cost << endl;
}


void Employee::RentCar() {
    // Check if employee has rented too many cars
    if (rentedCars.size() >= employeeRecord) {
        cout << "Already rented the maximum number of cars." << endl;
        return;
    }
    carDB.ShowAvailableCars();
    cout << "Enter the Car ID to be rented: ";
    int carId;
    
    cin >> carId;

    if (cin.fail()) {
        cout << "Invalid input. Please enter a valid integer." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }else if(carDB.SearchCar(carId)==nullptr){
        cout << "There's no Car with this ID!" << endl;
        return;
    }
    
    Car* car = carDB.SearchCar(carId);
    if (car->isRented) {
        cout << "Car is already rented." << endl;
        return;
    }
    // Rent the car
    int numDays = car->RentRequest();
    int cost = numDays * 0.75 * CostPerDay;
    cout << "Pay: " << cost << endl;

    car->isRented = true;
    car->rentedTo = "Employee";
    car->renterId = id;

    rentedCars.push_back(car->id);
    numOfCarsRented++;
    fineDue += cost;

    carDB.SaveData();
    employeeDB.SaveData();
    cout << "Car rented successfully." << endl;
}

void Employee::ClearDue() {

    cout << "Paid: " << fineDue << endl;
    fineDue=0;
    employeeDB.SaveData();
}

void Employee::DisplayInfo() const {
    cout << "Employee Info - Name: " << name << ", ID: " << id << ", Employee Record: " << employeeRecord << ", Number of Cars Rented: " << numOfCarsRented << endl;
}

void Employee::ShowRentedCars() const{
    if (rentedCars.empty()) {
        cout << "No cars rented." << endl;
        return;
    }

    cout << "Rented Cars:" << endl;
    for (int carId : rentedCars) {
        const Car* car = carDB.SearchCar(carId);
        if (car != nullptr) {
            cout << "Car ID: " << car->id << ", Model: " << car->model << ", Condition: " << car->condition << endl;
        }
    }

    cout << endl;
    int choice;
    do{
        cout << "\nMenu:" << endl;
        cout << "1. Check Due Date" << endl;
        cout << "2. Go Back" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail() || (choice != 1 && choice != 2)) {
            cout << "Invalid input. Please enter 1 or 2." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1:{
                cout << "Enter Rented Car ID: " << endl;
                int carId;
                cin >> carId;
                if (cin.fail()) {
                    cout << "Invalid input. Please enter a valid integer." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    return;
                }
                Car* car = carDB.SearchCar(carId);
                if (car == nullptr) {
                    cout << "Car with ID " << carId << " not found." << endl;
                    return;
                }

                if (!car->isRented || car->rentedTo != "Employee" || car->renterId != id) {
                    cout << "You have not rented this car." << endl;
                    return;
                }
                car->ShowDueDate();
                break;
            }
            case 2:
                break;
        }
    }while(choice!=2);
}

void Employee::returnCar() {
    
    Employee::ShowRentedCars();
    cout << endl;
    int carId;
    cout << "Enter the Car ID to be returned: ";
    cin >> carId;
    if (cin.fail()) {
        cout << "Invalid input. Please enter a valid integer." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    Car* car = carDB.SearchCar(carId);
    if (car == nullptr) {
        cout << "Car with ID " << carId << " not found." << endl;
        return;
    }

    if (!car->isRented || car->rentedTo != "Employee" || car->renterId != id) {
        cout << "You have not rented this car." << endl;
        return;
    }

    // Calculate the number of days the car was rented
    struct tm returnDateTm = {};
    strptime(car->returnDateStr.c_str(), "%Y-%m-%d", &returnDateTm);
    time_t returnDate = mktime(&returnDateTm);
    int numDays = difftime(time(nullptr), returnDate) / (60 * 60 * 24);

    // Calculate the total cost based on the number of days and condition of the car
    int cost = numDays * 0.75 * CostPerDay;
    fineDue += cost;
    string carCondition;
    cout << "How is the Car's Condition? " << endl;
    cin >> carCondition;
    car->condition = carCondition;
    if (car->condition == "damaged") {
        fineDue += 500; // Assuming fixed cost for damage
        employeeRecord*=0.8;
    }

    // Update the employee's record
    if (numDays>0) {
        employeeRecord*=0.9;
        fineDue+= numDays*finePerDay;
    }

    // Update the car's status and save the changes
    car->isRented = false;
    car->rentedTo = "";
    car->renterId = -1;
    carDB.SaveData();

    //updating employee
    for (auto it = rentedCars.begin(); it != rentedCars.end(); ++it) {
        if (*it == carId) {
            rentedCars.erase(it);
        }
    }
    numOfCarsRented--;
    employeeDB.SaveData();

    cout << "Car returned successfully. Total cost: " << cost << endl;
}


void Manager::ManageCars(){
    int carChoice;
    do{
        cout << "\nManage Cars" << endl;
        cout << "1. Add Car" << endl;
        cout << "2. Update Car" << endl;
        cout << "3. Delete Car" << endl;
        cout << "4. Go Back" << endl;
        cout << "Enter your choice: ";
        if (!(cin >> carChoice)) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }

        switch (carChoice) {
            case 1: {
                // Add Car
                int id;
                string model, condition, otherDetails;
                cout << "Enter Car ID: ";
                if (!(cin >> id)) {
                    cout << "Invalid input. Please enter a number." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
                cout << "Enter Car Model: ";
                cin >> model;
                cout << "Enter Car Condition: ";
                cin >> condition;
                cout << "Enter Other Details: ";
                cin >> otherDetails;
                Car car(id, model, condition, otherDetails);
                carDB.AddCar( car);
                cout << "Car added successfully." << endl;
                break;
            }
            case 2: {
                // Update Car
                int id;
                cout << "Enter Car ID to update: ";
                if (!(cin >> id)) {
                    cout << "Invalid input. Please enter a number." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
                Car* car = carDB.SearchCar(id);
                if (car != nullptr) {
                    string model, condition, otherDetails;
                    cout << "Enter new Car Model: ";
                    cin >> model;
                    cout << "Enter new Car Condition: ";
                    cin >> condition;
                    cout << "Enter new Other Details: ";
                    cin >> otherDetails;
                    car->model = model;
                    car->condition = condition;
                    car->otherDetails = otherDetails;
                    carDB.UpdateCar( *car);
                } else {
                    cout << "Car with ID " << id << " not found." << endl;
                }
                break;
            }
            case 3: {
                // Delete Car
                int id;
                cout << "Enter Car ID to delete: ";
                if (!(cin >> id)) {
                    cout << "Invalid input. Please enter a number." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
                Car* car = carDB.SearchCar(id);
                if (car != nullptr) {
                    carDB.DeleteCar(*car);
                } else {
                    cout << "Car with ID " << id << " not found." << endl;
                }
                break;
            }
            case 4:
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }while(carChoice!=4);
}

void Manager::ManageCustomers(){
    int customerChoice;
    do{
        cout << "\nManage Customers" << endl;
        cout << "1. Add Customer" << endl;
        cout << "2. Update Customer" << endl;
        cout << "3. Delete Customer" << endl;
        cout << "4. Go Back" << endl;
        cout << "Enter your choice: ";
        if (!(cin >> customerChoice)) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }

        switch (customerChoice) {
            case 1: {
                // Add Customer
                int id;
                float customerRecord;
                string name, password;
                cout << "Enter Customer ID: ";
                if (!(cin >> id)) {
                    cout << "Invalid input. Please enter a number." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
                cout << "Enter Customer Name: ";
                cin >> name;
                cout << "Enter Password: ";
                cin >> password;
                cout << "Enter Customer Record: ";
                cin >> customerRecord;
                if (cin.fail()) {
                    cout << "Invalid input. Please enter a valid integer." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
                Customer customer(name, id, password, customerRecord);
                customerDB.AddCustomer(customer);
                cout << "Customer added successfully." << endl;
                break;
            }
            case 2: {
                // Update Customer
                int customerId;
                cout << "Enter Customer ID to update: ";
                if (!(cin >> customerId)) {
                    cout << "Invalid input. Please enter a number." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
                Customer* customer = customerDB.SearchCustomer(customerId);
                if (customer != nullptr) {
                    string name, password;
                    float customerRecord;
                    cout << "Enter new Customer Name: ";
                    cin >> name;
                    cout << "Enter new Customer Password: ";
                    cin >> password;
                    cout << "Enter new Customer Record: ";
                    cin >> customerRecord;
                    if (cin.fail()) {
                        cout << "Invalid input. Please enter a valid integer." << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        break;
                    }
                    customer->name = name;
                    customer->password = password;
                    customer->customerRecord = customerRecord;
                    customerDB.UpdateCustomer(*customer);
                } else {
                    cout << "Customer not found." << endl;
                }
                break;
            }
            case 3: {
                // Delete Customer
                int customerId;
                cout << "Enter Customer ID to delete: ";
                if (!(cin >> customerId)) {
                    cout << "Invalid input. Please enter a number." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
                Customer* customer = customerDB.SearchCustomer(customerId);
                if (customer != nullptr) {
                    customerDB.DeleteCustomer(*customer);
                } else {
                    cout << "Customer not found." << endl;
                }
                break;
            }
            case 4:
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }while(customerChoice!=4);
}

void Manager::ManageEmployees(){
    int employeeChoice;
    do{
        cout << "\nManage Employees" << endl;
        cout << "1. Add Employee" << endl;
        cout << "2. Update Employee" << endl;
        cout << "3. Delete Employee" << endl;
        cout << "4. Go Back" << endl;
        cout << "Enter your choice: ";
        if (!(cin >> employeeChoice)) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (employeeChoice) {
            case 1: {
                // Add Employee
                int id;
                float employeeRecord;
                string name, password;
                cout << "Enter Employee ID: ";
                if (!(cin >> id)) {
                    cout << "Invalid input. Please enter a number." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
                cout << "Enter Employee Name: ";
                cin >> name;
                cout << "Enter Password: ";
                cin >> password;
                cout << "Enter Employee Record: ";
                cin >> employeeRecord;
                Employee employee(name, id, password, employeeRecord);
                employeeDB.AddEmployee( employee);
                cout << "Employee added successfully." << endl;
                break;
            }
            case 2: {
                // Update Employee
                int employeeId;
                cout << "Enter Employee ID to update: ";
                if (!(cin >> employeeId)) {
                    cout << "Invalid input. Please enter a number." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
                Employee* employee = employeeDB.SearchEmployee(employeeId);
                if (employee != nullptr) {
                    string name, password;
                    float employeeRecord;
                    cout << "Enter new Employee Name: ";
                    cin >> name;
                    cout << "Enter new Employee Password: ";
                    cin >> password;
                    cout << "Enter new Employee Record: ";
                    cin >> employeeRecord;
                    if (cin.fail()) {
                        cout << "Invalid input. Please enter a valid integer." << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        break;
                    }
                    employee->name = name;
                    employee->password = password;
                    employee->employeeRecord = employeeRecord;
                    employeeDB.UpdateEmployee(*employee);
                } else {
                    cout << "Employee not found." << endl;
                }
                break;
            }
            case 3: {
                // Delete Employee
                int employeeId;
                cout << "Enter Employee ID to delete: ";
                if (!(cin >> employeeId)) {
                    cout << "Invalid input. Please enter a number." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    break;
                }
                Employee* employee = employeeDB.SearchEmployee(employeeId);
                if (employee != nullptr) {
                    employeeDB.DeleteEmployee(*employee);
                } else {
                    cout << "Employee not found." << endl;
                }

                break;
            }
            case 4:
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }while (employeeChoice!=4);
    
}

void Manager::DisplayInfo() const {
    cout << "Manager Info - Name: " << name << ", ID: " << id << endl;
}

void CustomerDashboard(Customer& customer, CarDatabase& carDB) {
    int choice;
    cout << "\nCustomer Dashboard" << endl;
    do {
        cout << "\nMenu:" << endl;
        cout << "1. View Rented Cars" << endl;
        cout << "2. Rent a Car" << endl;
        cout << "3. Clear Dues" << endl;
        cout << "4. Return Car" << endl;
        cout << "5. My Profile" << endl;
        cout << "6. Logout" << endl;
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1:
                customer.ShowRentedCars();
                break;
            case 2:
                customer.RentCar();
                break;
            case 3:
                customer.ClearDue();
                break;
            case 4:
                customer.returnCar();
                break;
            case 5:
                customer.DisplayInfo();
                break;
            case 6:
                cout << "Loging Out..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 6);
}

void EmployeeDashboard(Employee& employee, CarDatabase& carDB) {
    int choice;
    cout << "\nEmployee Dashboard" << endl;
    do {
        cout << "\nMenu:" << endl;
        cout << "1. View Rented Cars" << endl;
        cout << "2. Rent a Car" << endl;
        cout << "4. Return Car" << endl;
        cout << "5. My Profile" << endl;
        cout << "6. Logout" << endl;
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1:
                employee.ShowRentedCars();
                break;
            case 2:
                employee.RentCar();
                break;
            case 3:
                employee.ClearDue();
                break;
            case 4:
                employee.returnCar();
            case 5:
                employee.DisplayInfo();
                break;
            case 6:
                cout << "Loging Out..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 6);
}

void ManagerDashboard(Manager& manager, CarDatabase& carDB, CustomerDatabase& customerDB, EmployeeDatabase& employeeDB) {
    int choice;
    cout << "\nManager Dashboard" << endl;
    do {
        cout << "\nMenu:" << endl;
        cout << "1. Manage Cars" << endl;
        cout << "2. Manage Customers" << endl;
        cout << "3. Manage Employees" << endl;
        cout << "4. Show All Cars" << endl;
        cout << "5. Logout" << endl;
        cout << "Enter your choice: ";
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1: {
                // Manage Cars
                manager.ManageCars();
                break;
            }
            case 2: {
                //Manage Customers
                manager.ManageCustomers();
                break;
            }
            case 3: {
                // Manage Employees
                manager.ManageEmployees();
                break;
            }
            case 4: {
                // Show All Cars
                carDB.ShowAllCars();
                break;
            }
            case 5:
                cout << "Loging Out..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 5);
}


int main() {

    // command line interface
    int choice;
    cout << "Welcome to Car Rental System" << endl;
    do {
        cout << "\nMenu:" << endl;
        cout << "1. Login" << endl;
        cout << "2. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail() || (choice != 1 && choice != 2)) {
                    cout << "Invalid input. Please enter 1 or 2." << endl;
                    cin.clear();
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    continue;
        }

        switch (choice) {
            case 1: {
                int userType;
                do{
                    cout << "Select user type:" << endl;
                    cout << "1. Customer" << endl;
                    cout << "2. Employee" << endl;
                    cout << "3. Manager" << endl;
                    cout << "4. Return to Main Menu" << endl;
                    cout << "Enter user type: ";
                    
                    cin >> userType;

                    if (cin.fail() || (userType != 1 && userType != 2 && userType != 3)) {
                        cout << "Invalid input. Please enter 1, 2, or 3." << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        continue;
                    }

                    int id;
                    string password;
                    cout << "Enter ID: ";
                    cin >> id;

                    if (cin.fail()) {
                        cout << "Invalid input. Please enter a valid integer." << endl;
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        continue;
                    }

                    cout << "Enter password: ";
                    cin >> password;

                    bool loggedIn = false;
                    switch (userType) {
                        case 1: {
                            // Check if customer exists
                            Customer* customer = customerDB.SearchCustomer(id);
                            if (customer != nullptr && customer->authentication(password)) {
                                loggedIn = true;
                                cout << "Customer login successful!" << endl;
                                CustomerDashboard(*customer,carDB);
                            } else {
                                cout << "Invalid ID or password." << endl;
                            }
                            break;
                        }
                        case 2: {
                            // Check if employee exists
                            Employee* employee = employeeDB.SearchEmployee(id);
                            if (employee != nullptr && employee->authentication(password)) {
                                loggedIn = true;
                                cout << "Employee login successful!" << endl;
                                EmployeeDashboard(*employee,carDB);
                            } else {
                                cout << "Invalid ID or password." << endl;
                            }
                            break;
                        }
                        case 3: {
                            // Check if manager exists
                            if (id == 220654 && password == "anas123") {
                                loggedIn = true;
                                Manager manager("anas", 220654, "anas123");
                                cout << "Manager login successful!" << endl;
                                ManagerDashboard(manager,carDB,customerDB,employeeDB);
                            } else {
                                cout << "Invalid ID or password." << endl;
                            }
                            break;
                        }
                        case 4:
                            cout << "Returning to main menu..." << endl;
                            break;
                        default: {
                            cout << "Invalid user type." << endl;
                            break;
                        }
                    }
                }while(userType!=4);
                break;
            }
            case 2:
                // Save data to file before exiting
                carDB.SaveData();
                customerDB.SaveData();
                employeeDB.SaveData();
                cout << "Exiting..." << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 2);

    return 0;
}
