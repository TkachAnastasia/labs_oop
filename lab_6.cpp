#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <nlohmann/json.hpp>

// Визначте макрос для спрощення створення структур дати та часу
#define DATE_FORMAT "%Y-%m-%dT%H:%M:%S"

// FlightStatus Enum
enum class FlightStatus {
    OnTime,
    Delayed,
    Cancelled,
    Boarding,
    InFlight
};

// Flight Class
class Flight {
public:
    std::string FlightNumber;
    std::string Airline;
    std::string Destination;
    std::chrono::system_clock::time_point DepartureTime;
    std::chrono::system_clock::time_point ArrivalTime;
    std::string Gate;
    FlightStatus Status;
    std::chrono::duration<double> Duration;
    std::string AircraftType;
    std::string Terminal;

    // Функція для десеріалізації JSON-даних в об'єкт Flight
    static Flight from_json(const nlohmann::json& j) {
        Flight f;
        f.FlightNumber = j.at("FlightNumber").get<std::string>();
        f.Airline = j.at("Airline").get<std::string>();
        f.Destination = j.at("Destination").get<std::string>();
        f.DepartureTime = parse_date(j.at("DepartureTime").get<std::string>());
        f.ArrivalTime = parse_date(j.at("ArrivalTime").get<std::string>());
        f.Gate = j.at("Gate").get<std::string>();
        f.Status = parse_status(j.at("Status").get<std::string>());
        f.Duration = parse_duration(j.at("Duration").get<std::string>());
        f.AircraftType = j.at("AircraftType").get<std::string>();
        f.Terminal = j.at("Terminal").get<std::string>();
        return f;
    }

    // Функція для серіалізації об'єкта Flight в JSON-дані
    nlohmann::json to_json() const {
        nlohmann::json j;
        j["FlightNumber"] = FlightNumber;
        j["Airline"] = Airline;
        j["Destination"] = Destination;
        j["DepartureTime"] = format_date(DepartureTime);
        j["ArrivalTime"] = format_date(ArrivalTime);
        j["Gate"] = Gate;
        j["Status"] = format_status(Status);
        j["Duration"] = format_duration(Duration);
        j["AircraftType"] = AircraftType;
        j["Terminal"] = Terminal;
        return j;
    }

private:
    // Допоміжні функції для аналізу та форматування дат, статусів і тривалості
    static std::chrono::system_clock::time_point parse_date(const std::string& date) {
        std::tm tm = {};
        std::istringstream ss(date);
        ss >> std::get_time(&tm, DATE_FORMAT);
        return std::chrono::system_clock::from_time_t(std::mktime(&tm));
    }

    static std::string format_date(const std::chrono::system_clock::time_point& tp) {
        std::time_t t = std::chrono::system_clock::to_time_t(tp);
        std::tm tm = *std::localtime(&t);
        std::ostringstream ss;
        ss << std::put_time(&tm, DATE_FORMAT);
        return ss.str();
    }

    static FlightStatus parse_status(const std::string& status) {
        if (status == "OnTime") return FlightStatus::OnTime;
        if (status == "Delayed") return FlightStatus::Delayed;
        if (status == "Cancelled") return FlightStatus::Cancelled;
        if (status == "Boarding") return FlightStatus::Boarding;
        if (status == "InFlight") return FlightStatus::InFlight;
        throw std::invalid_argument("Invalid flight status");
    }

    static std::string format_status(FlightStatus status) {
        switch (status) {
            case FlightStatus::OnTime: return "OnTime";
            case FlightStatus::Delayed: return "Delayed";
            case FlightStatus::Cancelled: return "Cancelled";
            case FlightStatus::Boarding: return "Boarding";
            case FlightStatus::InFlight: return "InFlight";
            default: throw std::invalid_argument("Invalid flight status");
        }
    }

    static std::chrono::duration<double> parse_duration(const std::string& duration) {
        int hours, minutes, seconds;
        sscanf(duration.c_str(), "%d:%d:%d", &hours, &minutes, &seconds);
        return std::chrono::hours(hours) + std::chrono::minutes(minutes) + std::chrono::seconds(seconds);
    }

    static std::string format_duration(const std::chrono::duration<double>& duration) {
        auto hrs = std::chrono::duration_cast<std::chrono::hours>(duration).count();
        auto mins = std::chrono::duration_cast<std::chrono::minutes>(duration).count() % 60;
        auto secs = std::chrono::duration_cast<std::chrono::seconds>(duration).count() % 60;
        std::ostringstream ss;
        ss << hrs << ":" << mins << ":" << secs;
        return ss.str();
    }
};

// FlightInformationSystem Class
class FlightInformationSystem {
public:
    // Function to read flight data from a JSON file
    void read_flights_from_json(const std::string& filename) {
        std::ifstream file(filename);
        nlohmann::json json_data;
        file >> json_data;
        for (const auto& flight_data : json_data["flights"]) {
            flights.push_back(Flight::from_json(flight_data));
        }
    }

    // Функція для отримання всіх рейсів
    const std::vector<Flight>& get_all_flights() const {
        return flights;
    }

private:
    std::vector<Flight> flights;
};

// FlightQueryHandler Class
class FlightQueryHandler {
public:
    FlightQueryHandler(const FlightInformationSystem& system) : system(system) {}

    // Function to get all flights of a specific airline, sorted by departure time
    std::vector<Flight> get_flights_by_airline(const std::string& airline) const {
        std::vector<Flight> result;
        for (const auto& flight : system.get_all_flights()) {
            if (flight.Airline == airline) {
                result.push_back(flight);
            }
        }
        std::sort(result.begin(), result.end(), [](const Flight& a, const Flight& b) {
            return a.DepartureTime < b.DepartureTime;
        });
        return result;
    }

    // Функція для отримання всіх затриманих рейсів, відсортованих за часом затримки
    std::vector<Flight> get_delayed_flights() const {
        std::vector<Flight> result;
        for (const auto& flight : system.get_all_flights()) {
            if (flight.Status == FlightStatus::Delayed) {
                result.push_back(flight);
            }
        }
        std::sort(result.begin(), result.end(), [](const Flight& a, const Flight& b) {
            return a.DepartureTime < b.DepartureTime;
        });
        return result;
    }

    // Функція для отримання всіх рейсів на певний день, відсортованих за часом вильоту
    std::vector<Flight> get_flights_by_day(const std::tm& day) const {
        std::vector<Flight> result;
        for (const auto& flight : system.get_all_flights()) {
            std::time_t dep_time_t = std::chrono::system_clock::to_time_t(flight.DepartureTime);
            std::tm* dep_time_tm = std::localtime(&dep_time_t);
            if (dep_time_tm->tm_year == day.tm_year && dep_time_tm->tm_mon == day.tm_mon && dep_time_tm->tm_mday == day.tm_mday) {
                result.push_back(flight);
            }
        }
        std::sort(result.begin(), result.end(), [](const Flight& a, const Flight& b) {
            return a.DepartureTime < b.DepartureTime;
        });
        return result;
    }

    // Функція для отримання всіх рейсів з певного часового інтервалу до певного пункту призначення, відсортованих за часом вильоту
    std::vector<Flight> get_flights_in_interval_to_destination(const std::chrono::system_clock::time_point& start, const std::chrono::system_clock::time_point& end, const std::string& destination) const {
        std::vector<Flight> result;
        for (const auto& flight : system.get_all_flights()) {
            if (flight.Destination == destination && flight.DepartureTime >= start && flight.DepartureTime <= end) {
                result.push_back(flight);
            }
        }
        std::sort(result.begin(), result.end(), [](const Flight& a, const Flight& b) {
            return a.DepartureTime < b.DepartureTime;
        });
        return result;
    }

    // Функція для отримання всіх рейсів, що прибули за останню годину або протягом певного часового інтервалу, відсортованих за часом прибуття
    std::vector<Flight> get_flights_arriving_recently_or_interval(const std::chrono::system_clock::time_point& start, const std::chrono::system_clock::time_point& end) const {
        std::vector<Flight> result;
        for (const auto& flight : system.get_all_flights()) {
            if (flight.ArrivalTime >= start && flight.ArrivalTime <= end) {
                result.push_back(flight);
            }
        }
        std::sort(result.begin(), result.end(), [](const Flight& a, const Flight& b) {
            return a.ArrivalTime < b.ArrivalTime;
        });
        return result;
    }

private:
    const FlightInformationSystem& system;
};

int main() {
    FlightInformationSystem fis;
    fis.read_flights_from_json("flights.json");

    FlightQueryHandler handler(fis);

    // Приклад використання:
    auto flights = handler.get_flights_by_airline("WizAir");
    for (const auto& flight : flights) {
        std::cout << flight.to_json().dump(4) << std::endl;
    }

    return 0;
}
