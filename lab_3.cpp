#include <iostream>
#include <string>
#include <vector>

class Dish {
    std::string name;
    std::string description;
    double price;
    double weight;
    int calories;
    std::vector<std::string> allergens;

public:
    Dish(const std::string& name, const std::string& description, double price, double weight, int calories, const std::vector<std::string>& allergens)
        : name(name), description(description), price(price), weight(weight), calories(calories), allergens(allergens) {}

    void print() const {
        std::cout << "Dish: " << name << ", " << description << ", Price: " << price << ", Weight: " << weight << ", Calories: " << calories << "\n";
        std::cout << "Allergens: ";
        for (const auto& allergen : allergens) {
            std::cout << allergen << " ";
        }
        std::cout << "\n";
    }
};

class Restaurant {
    std::string name;
    std::string address;
    std::string type;
    double rating;
    std::vector<Dish> dishes;

public:
    Restaurant(const std::string& name, const std::string& address, const std::string& type, double rating, const std::vector<Dish>& dishes)
        : name(name), address(address), type(type), rating(rating), dishes(dishes) {}

    void print() const {
        std::cout << "Restaurant: " << name << ", Address: " << address << ", Type: " << type << ", Rating: " << rating << "\n";
        std::cout << "Dishes:\n";
        for (const auto& dish : dishes) {
            dish.print();
        }
    }

    const std::vector<Dish>& getDishes() const {
        return dishes;
    }
};

class Courier {
    std::string name;
    std::string contactInfo;
    double rating;
    std::string transportType;

public:
    Courier(const std::string& name, const std::string& contactInfo, double rating, const std::string& transportType)
        : name(name), contactInfo(contactInfo), rating(rating), transportType(transportType) {}

    void print() const {
        std::cout << "Courier: " << name << ", Contact: " << contactInfo << ", Rating: " << rating << ", Transport: " << transportType << "\n";
    }
};

class Client {
    std::string name;
    std::string address;
    std::string contactNumber;
    std::vector<std::string> orderHistory;

public:
    Client(const std::string& name, const std::string& address, const std::string& contactNumber)
        : name(name), address(address), contactNumber(contactNumber) {}

    void addOrderToHistory(const std::string& orderDetails) {
        orderHistory.push_back(orderDetails);
    }

    void print() const {
        std::cout << "Client: " << name << ", Address: " << address << ", Contact: " << contactNumber << "\n";
        std::cout << "Order History:\n";
        for (const auto& order : orderHistory) {
            std::cout << order << "\n";
        }
    }
};

class Order {
    std::vector<Dish> dishes;
    double totalAmount;
    std::string status;
    Restaurant restaurant;
    Courier courier;
    Client client;

public:
    Order(const std::vector<Dish>& dishes, double totalAmount, const std::string& status, const Restaurant& restaurant, const Courier& courier, const Client& client)
        : dishes(dishes), totalAmount(totalAmount), status(status), restaurant(restaurant), courier(courier), client(client) {}

    void updateStatus(const std::string& newStatus) {
        status = newStatus;
    }

    void print() const {
        std::cout << "Order:\n";
        restaurant.print();
        std::cout << "Dishes:\n";
        for (const auto& dish : dishes) {
            dish.print();
        }
        std::cout << "Total Amount: " << totalAmount << "\n";
        std::cout << "Status: " << status << "\n";
        courier.print();
        client.print();
    }
};

class DeliveryManager {
    std::vector<Courier> availableCouriers;

public:
    DeliveryManager(const std::vector<Courier>& availableCouriers) : availableCouriers(availableCouriers) {}

    Courier assignCourier() {
        if (availableCouriers.empty()) {
            throw std::runtime_error("No available couriers");
        }
        // Here we can implement more sophisticated logic for selecting a courier
        return availableCouriers.front();
    }

    void processOrder(Order& order) {
        try {
            Courier courier = assignCourier();
            order.updateStatus("In Progress");
            order.print();
            std::cout << "Order is being delivered by: " << courier.print() << "\n";
            order.updateStatus("Delivered");
            std::cout << "Order Status: " << order.getStatus() << "\n";
        } catch (const std::exception& e) {
            std::cerr << e.what() << "\n";
        }
    }
};

#include <cstdlib>
#include <ctime>
#include <vector>

class MockTester {
    std::vector<Client> clients;
    DeliveryManager& deliveryManager;

public:
    MockTester(std::vector<Client> clients, DeliveryManager& deliveryManager)
        : clients(std::move(clients)), deliveryManager(deliveryManager) {}

    void createRandomOrder() {
        srand(time(0));
        std::vector<Dish> dishes = {
            Dish("Burger", "Tasty beef burger", 5.99, 0.3, 500, {"Gluten", "Dairy"}),
            Dish("Pizza", "Delicious cheese pizza", 7.99, 0.5, 800, {"Gluten", "Dairy"}),
            Dish("Salad", "Fresh garden salad", 4.99, 0.2, 200, {"None"})
        };

        Restaurant restaurant("GoodFood", "123 Main St", "Fast Food", 4.5, dishes);
        Courier courier("John Doe", "555-1234", 4.8, "Bicycle");
        Client client = clients[rand() % clients.size()];

        std::vector<Dish> orderDishes = {dishes[rand() % dishes.size()]};
        Order order(orderDishes, orderDishes.front().getPrice(), "Pending", restaurant, courier, client);

        deliveryManager.processOrder(order);
    }
};

int main() {
    std::vector<Client> clients = {
        Client("Alice", "456 Elm St", "555-5678"),
        Client("Bob", "789 Pine St", "555-8765")
    };

    std::vector<Courier> couriers = {
        Courier("John Doe", "555-1234", 4.8, "Bicycle"),
        Courier("Jane Smith", "555-2345", 4.7, "Car")
    };

    DeliveryManager deliveryManager(couriers);

    MockTester tester(clients, deliveryManager);

    tester.createRandomOrder();

    return 0;
}
