#include <iostream>
#include <string>
#include <list>
#include <memory>
#include <algorithm>

// Базовий клас Carriage
class Carriage {
protected:
    std::string id;
    std::string type;
    double weight;
    double length;
    std::string owner;
    std::string manufactureDate;
    std::string maintenanceDate;
    int maxSpeed;
    bool inService;
    std::string color;
    std::string material;
    double energyConsumption;
    double manufacturingCost;
    std::string lastStation;
    int currentPassengers;

public:
    Carriage(const std::string& id, const std::string& type, double weight, double length)
        : id(id), type(type), weight(weight), length(length) {}

    virtual ~Carriage() {}

    // Геттери
    std::string getId() const { return id; }
    std::string getType() const { return type; }
    double getWeight() const { return weight; }
    double getLength() const { return length; }
    std::string getOwner() const { return owner; }
    std::string getManufactureDate() const { return manufactureDate; }
    std::string getMaintenanceDate() const { return maintenanceDate; }
    int getMaxSpeed() const { return maxSpeed; }
    bool isInService() const { return inService; }
    std::string getColor() const { return color; }
    std::string getMaterial() const { return material; }
    double getEnergyConsumption() const { return energyConsumption; }
    double getManufacturingCost() const { return manufacturingCost; }
    std::string getLastStation() const { return lastStation; }
    int getCurrentPassengers() const { return currentPassengers; }

    // Сеттери
    void setId(const std::string& id) { this->id = id; }
    void setType(const std::string& type) { this->type = type; }
    void setWeight(double weight) { this->weight = weight; }
    void setLength(double length) { this->length = length; }
    void setOwner(const std::string& owner) { this->owner = owner; }
    void setManufactureDate(const std::string& manufactureDate) { this->manufactureDate = manufactureDate; }
    void setMaintenanceDate(const std::string& maintenanceDate) { this->maintenanceDate = maintenanceDate; }
    void setMaxSpeed(int maxSpeed) { this->maxSpeed = maxSpeed; }
    void setInService(bool inService) { this->inService = inService; }
    void setColor(const std::string& color) { this->color = color; }
    void setMaterial(const std::string& material) { this->material = material; }
    void setEnergyConsumption(double energyConsumption) { this->energyConsumption = energyConsumption; }
    void setManufacturingCost(double manufacturingCost) { this->manufacturingCost = manufacturingCost; }
    void setLastStation(const std::string& lastStation) { this->lastStation = lastStation; }
    void setCurrentPassengers(int currentPassengers) { this->currentPassengers = currentPassengers; }

    virtual void print() const = 0;
};

// Похідні класи від Carriage
class PassengerCarriage : public Carriage {
    int seatsCount;
    std::string comfortLevel;

public:
    PassengerCarriage(const std::string& id, double weight, double length, int seatsCount, const std::string& comfortLevel)
        : Carriage(id, "Passenger", weight, length), seatsCount(seatsCount), comfortLevel(comfortLevel) {}

    int getSeatsCount() const { return seatsCount; }
    void setSeatsCount(int seatsCount) { this->seatsCount = seatsCount; }

    std::string getComfortLevel() const { return comfortLevel; }
    void setComfortLevel(const std::string& comfortLevel) { this->comfortLevel = comfortLevel; }

    void print() const override {
        std::cout << "Passenger Carriage [ID: " << id << ", Weight: " << weight << ", Length: " << length 
                  << ", Seats: " << seatsCount << ", Comfort: " << comfortLevel << "]" << std::endl;
    }
};

class FreightCarriage : public Carriage {
    double maxLoadCapacity;
    std::string cargoType;

public:
    FreightCarriage(const std::string& id, double weight, double length, double maxLoadCapacity, const std::string& cargoType)
        : Carriage(id, "Freight", weight, length), maxLoadCapacity(maxLoadCapacity), cargoType(cargoType) {}

    double getMaxLoadCapacity() const { return maxLoadCapacity; }
    void setMaxLoadCapacity(double maxLoadCapacity) { this->maxLoadCapacity = maxLoadCapacity; }

    std::string getCargoType() const { return cargoType; }
    void setCargoType(const std::string& cargoType) { this->cargoType = cargoType; }

    void print() const override {
        std::cout << "Freight Carriage [ID: " << id << ", Weight: " << weight << ", Length: " << length 
                  << ", Max Load: " << maxLoadCapacity << ", Cargo: " << cargoType << "]" << std::endl;
    }
};

class DiningCarriage : public Carriage {
    int tablesCount;
    bool hasKitchen;

public:
    DiningCarriage(const std::string& id, double weight, double length, int tablesCount, bool hasKitchen)
        : Carriage(id, "Dining", weight, length), tablesCount(tablesCount), hasKitchen(hasKitchen) {}

    int getTablesCount() const { return tablesCount; }
    void setTablesCount(int tablesCount) { this->tablesCount = tablesCount; }

    bool hasKitchenFacility() const { return hasKitchen; }
    void setHasKitchen(bool hasKitchen) { this->hasKitchen = hasKitchen; }

    void print() const override {
        std::cout << "Dining Carriage [ID: " << id << ", Weight: " << weight << ", Length: " << length 
                  << ", Tables: " << tablesCount << ", Kitchen: " << (hasKitchen ? "Yes" : "No") << "]" << std::endl;
    }
};

class SleepingCarriage : public Carriage {
    int compartmentsCount;
    bool hasShowers;

public:
    SleepingCarriage(const std::string& id, double weight, double length, int compartmentsCount, bool hasShowers)
        : Carriage(id, "Sleeping", weight, length), compartmentsCount(compartmentsCount), hasShowers(hasShowers) {}

    int getCompartmentsCount() const { return compartmentsCount; }
    void setCompartmentsCount(int compartmentsCount) { this->compartmentsCount = compartmentsCount; }

    bool hasShowerFacilities() const { return hasShowers; }
    void setHasShowers(bool hasShowers) { this->hasShowers = hasShowers; }

    void print() const override {
        std::cout << "Sleeping Carriage [ID: " << id << ", Weight: " << weight << ", Length: " << length 
                  << ", Compartments: " << compartmentsCount << ", Showers: " << (hasShowers ? "Yes" : "No") << "]" << std::endl;
    }
};

// Клас Train
class Train {
    std::list<std::unique_ptr<Carriage>> carriages;
    std::string name;
    std::string routeNumber;

public:
    Train(const std::string& name, const std::string& routeNumber)
        : name(name), routeNumber(routeNumber) {}

    void addCarriage(std::unique_ptr<Carriage> carriage) {
        carriages.push_back(std::move(carriage));
    }

    void removeCarriage(const std::string& id) {
        carriages.remove_if([&id](const std::unique_ptr<Carriage>& carriage) { return carriage->getId() == id; });
    }

    Carriage* findCarriage(const std::string& id) {
        for (const auto& carriage : carriages) {
            if (carriage->getId() == id) {
                return carriage.get();
            }
        }
        return nullptr;
    }

    void printCarriages() const {
        for (const auto& carriage : carriages) {
            carriage->print();
        }
    }

    int totalPassengerCapacity() const {
        int total = 0;
        for (const auto& carriage : carriages) {
            if (auto p = dynamic_cast<PassengerCarriage*>(carriage.get())) {
                total += p->getSeatsCount();
            }
        }
        return total;
    }

    Carriage* maxCargoCapacityCarriage() const {
        Carriage* maxCarriage = nullptr;
        double maxCapacity = 0;
        for (const auto& carriage : carriages) {
            if (auto f = dynamic_cast<FreightCarriage*>(carriage.get())) {
                if (f->getMaxLoadCapacity() > maxCapacity) {
                    maxCapacity = f->getMaxLoadCapacity();
                    maxCarriage = f;
                }
            }
        }
        return maxCarriage;
    }

    int countCarriagesByType(const std::string& type) const {
        return std::count_if(carriages.begin(), carriages.end(), [&type](const std::unique_ptr<Carriage>& carriage) {
            return carriage->getType() == type;
        });
    }

    double totalTrainWeight() const {
        double totalWeight = 0;
        for (const auto& carriage : carriages) {
            totalWeight += carriage->getWeight();
        }
        return totalWeight;
    }

    void changeRoute(const std::string& newRouteNumber) {
        routeNumber = newRouteNumber;
    }

    bool hasSpecialCarriages() const {
        return std::any_of(carriages.begin(), carriages.end(), [](const std::unique_ptr<Carriage>& carriage) {
            return carriage->getType() == "Special";
        });
    }
};

// Приклад використання
int main() {
    Train train("Express", "12345");

    train.addCarriage(std::make_unique<PassengerCarriage>("1", 20.0, 10.0, 100, "Economy"));
    train.addCarriage(std::make_unique<FreightCarriage>("2", 30.0, 15.0, 200.0, "Coal"));
    train.addCarriage(std::make_unique<DiningCarriage>("3", 25.0, 12.0, 10, true));
    train.addCarriage(std::make_unique<SleepingCarriage>("4", 28.0, 14.0, 20, true));

    std::cout << "All Carriages:" << std::endl;
    train.printCarriages();

    std::cout << "\nTotal Passenger Capacity: " << train.totalPassengerCapacity() << std::endl;

    Carriage* maxCargoCarriage = train.maxCargoCapacityCarriage();
    if (maxCargoCarriage) {
        std::cout << "\nCarriage with Maximum Cargo Capacity:" << std::endl;
        maxCargoCarriage->print();
    }

    std::cout << "\nNumber of Passenger Carriages: " << train.countCarriagesByType("Passenger") << std::endl;
    std::cout << "Total Train Weight: " << train.totalTrainWeight() << " tons" << std::endl;

    std::cout << "\nChanging Route Number to 67890" << std::endl;
    train.changeRoute("67890");

    std::cout << "\nAll Carriages After Route Change:" << std::endl;
    train.printCarriages();

    std::cout << "\nRemoving Carriage with ID 2" << std::endl;
    train.removeCarriage("2");

    std::cout << "\nAll Carriages After Removal:" << std::endl;
    train.printCarriages();

    return 0;
}
