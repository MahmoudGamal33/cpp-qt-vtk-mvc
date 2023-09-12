#pragma once

#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>

// Base class for all geometric shapes.
class Shape {
public:
    virtual ~Shape() {}

    // Pure virtual function to create the shape.
    virtual vtkSmartPointer<vtkPolyDataMapper> createShape() const = 0;
};

// Class to represent a 3D cube.
class Cube : public Shape {
private:
    double xLength, yLength, zLength;

public:
    Cube(double xLength, double yLength, double zLength);
    virtual ~Cube();

    vtkSmartPointer<vtkPolyDataMapper> createShape() const override;
};

// Class to represent a 3D sphere.
class Sphere : public Shape {
private:
    double radius;

public:
    Sphere(double radius);
    virtual ~Sphere();

    vtkSmartPointer<vtkPolyDataMapper> createShape() const override;
};

// Class to represent a 3D hemisphere.
class Hemisphere : public Shape {
private:
    double radius;

public:
    Hemisphere(double radius);
    virtual ~Hemisphere();

    vtkSmartPointer<vtkPolyDataMapper> createShape() const override;
};

// Class to represent a 3D cone.
class Cone : public Shape {
private:
    double angle;

public:
    Cone(double angle);
    virtual ~Cone();

    vtkSmartPointer<vtkPolyDataMapper> createShape() const override;
};

// Class to represent a 3D pyramid.
class Pyramid : public Shape {
private:
    double baseLength, height;

public:
    Pyramid(double baseLength, double height);
    virtual ~Pyramid();

    vtkSmartPointer<vtkPolyDataMapper> createShape() const override;
};

// Class to represent a 3D cylinder.
class Cylinder : public Shape {
private:
    double radius, height;

public:
    Cylinder(double radius, double height);
    virtual ~Cylinder();

    vtkSmartPointer<vtkPolyDataMapper> createShape() const override;
};

// Class to represent a 3D tube.
class Tube : public Shape {
private:
    double radius, length;

public:
    Tube(double radius, double length);
    virtual ~Tube();

    vtkSmartPointer<vtkPolyDataMapper> createShape() const override;
};

// Class to represent a 3D doughnut (or torus).
class Doughnut : public Shape {
private:
    double radius, height;

public:
    Doughnut(double radius, double height);
    virtual ~Doughnut();

    vtkSmartPointer<vtkPolyDataMapper> createShape() const override;
};

// Class to represent a 3D curved cylinder.
class CurvedCylinder : public Shape {
private:
    double radius;

public:
    CurvedCylinder(double radius);
    virtual ~CurvedCylinder();

    vtkSmartPointer<vtkPolyDataMapper> createShape() const override;
};
