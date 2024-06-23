#include <iostream>
#include <cmath>

class Shape {
public:
    virtual double area() const = 0;
    virtual double perimeter() const = 0;
    virtual ~Shape() {}
};

class Circle : public Shape {
private:
    double radius;

public:
    Circle(double r) : radius(r) {}

    double area() const override {
        return M_PI * radius * radius;
    }

    double perimeter() const override {
        return 2 * M_PI * radius;
    }

    double diameter() const {
        return 2 * radius;
    }
};

// Створення екземпляру класу Circle
int main() {
    Circle circle(5);
    std::cout << "Circle area: " << circle.area() << std::endl;
    std::cout << "Circle perimeter: " << circle.perimeter() << std::endl;
    std::cout << "Circle diameter: " << circle.diameter() << std::endl;

    return 0;
}

class Rectangle : public Shape {
private:
    double width, height;

public:
    Rectangle(double w, double h) : width(w), height(h) {}

    double area() const override {
        return width * height;
    }

    double perimeter() const override {
        return 2 * (width + height);
    }

    bool isSquare() const {
        return width == height;
    }
};

// Створення екземпляру класу Rectangle
int main() {
    Rectangle rectangle(4, 7);
    std::cout << "Rectangle area: " << rectangle.area() << std::endl;
    std::cout << "Rectangle perimeter: " << rectangle.perimeter() << std::endl;
    std::cout << "Is the rectangle a square? " << (rectangle.isSquare() ? "Yes" : "No") << std::endl;

    return 0;
}

class Triangle : public Shape {
private:
    double side1, side2, side3;

public:
    Triangle(double s1, double s2, double s3) : side1(s1), side2(s2), side3(s3) {}

    double area() const override {
        double s = (side1 + side2 + side3) / 2;
        return sqrt(s * (s - side1) * (s - side2) * (s - side3));
    }

    double perimeter() const override {
        return side1 + side2 + side3;
    }

    bool isEquilateral() const {
        return side1 == side2 && side1 == side3;
    }
};

// Створення екземпляру класу Triangle
int main() {
    Triangle triangle(3, 4, 5);
    std::cout << "Triangle area: " << triangle.area() << std::endl;
    std::cout << "Triangle perimeter: " << triangle.perimeter() << std::endl;
    std::cout << "Is the triangle equilateral? " << (triangle.isEquilateral() ? "Yes" : "No") << std::endl;

    return 0;
}
