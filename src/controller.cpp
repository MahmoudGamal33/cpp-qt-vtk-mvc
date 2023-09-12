#include "controller.h"


/**
 * @brief Implementation of the createShape method.
 *
 * This method generates shapes based on the provided shape type.
 * The shapes are initialized with default dimensions.
 *
 * @param shapeType The type of the shape to be generated.
 * @return vtkSmartPointer<vtkPolyDataMapper> The generated shape, or nullptr if the shape type is not recognized.
 */
vtkSmartPointer<vtkPolyDataMapper> ShapeController::createShape(const QString& shapeType)
{
    if (shapeType == "Cube") {
        Cube cube(30, 40, 50);
        return cube.createShape();
    }
    else if (shapeType == "Sphere") {
        Sphere sphere(5);
        return sphere.createShape();
    }
    else if (shapeType == "Hemisphere") {
        Hemisphere hemisphere(5);
        return hemisphere.createShape();
    }
    else if (shapeType == "Cone") {
        Cone cone(30);
        return cone.createShape();
    }
    else if (shapeType == "Pyramid") {
        Pyramid pyramid(4, 15);
        return pyramid.createShape();
    }
    else if (shapeType == "Cylinder") {
        Cylinder cylinder(5, 20);
        return cylinder.createShape();
    }
    else if (shapeType == "Tube") {
        Tube tube(2, 5);
        return tube.createShape();
    }
    else if (shapeType == "Doughnut") {
        Doughnut doughnut(6, 3);
        return doughnut.createShape();
    }
    else if (shapeType == "Curved Cylinder") {
        CurvedCylinder curvedCylinder(5);
        return curvedCylinder.createShape();
    }

    return nullptr;
}
