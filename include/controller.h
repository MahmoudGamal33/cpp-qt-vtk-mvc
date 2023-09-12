#pragma once

#include "model.h"
#include <QString>

/**
 * @class ShapeController
 * @brief Controller class responsible for managing and creating different shapes based on the given type.
 *
 * This class provides an interface to create various 3D shapes using the VTK library.
 * The shapes are determined based on their type given as QString.
 */
class ShapeController
{
public:
    /**
     * @brief Default constructor.
     */
    ShapeController() = default;

    /**
     * @brief Creates and returns a shape based on the given type.
     *
     * @param shapeType QString representing the type of shape to create.
     * Supported types are "Cube", "Sphere", "Hemisphere", "Cone", "Pyramid",
     * "Cylinder", "Tube", "Doughnut", and "Curved Cylinder".
     *
     * @return vtkSmartPointer<vtkPolyDataMapper> The created shape or nullptr if the type is unsupported.
     */
    vtkSmartPointer<vtkPolyDataMapper> createShape(const QString& shapeType);
};
