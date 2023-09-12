#include "model.h"

#include <vtkNew.h>
#include <vtkActor.h>

#include <vtkCubeSource.h>
#include <vtkSphereSource.h>
#include <vtkConeSource.h>
#include <vtkCylinderSource.h>
#include <vtkLineSource.h>
#include <vtkTubeFilter.h>
#include <vtkParametricFunctionSource.h>
#include <vtkParametricTorus.h>
#include <vtkParametricSpline.h>
#include <vtkParametricFunctionSource.h>



/**
 * @brief Constructor for the Cube class.
 *
 * @param xLength Length of the cube along the x-axis.
 * @param yLength Length of the cube along the y-axis.
 * @param zLength Length of the cube along the z-axis.
 */
Cube::Cube(double xLength, double yLength, double zLength) : xLength(xLength), yLength(yLength), zLength(zLength)
{
}

/**
 * @brief Destructor for the Cube class.
 */
Cube::~Cube()
{
}

/**
 * @brief Generates and returns a Cube-shaped vtkPolyDataMapper object.
 *
 * @return vtkSmartPointer<vtkPolyDataMapper> The mapper representing the Cube shape.
 */
vtkSmartPointer<vtkPolyDataMapper> Cube::createShape() const
{
    vtkSmartPointer<vtkCubeSource> cubeSource = vtkSmartPointer<vtkCubeSource>::New();

    cubeSource->SetXLength(xLength);
    cubeSource->SetYLength(yLength);
    cubeSource->SetZLength(zLength);

    cubeSource->Update();

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(cubeSource->GetOutput());
    return mapper;
}




/**
 * @brief Constructor for the Sphere class.
 *
 * @param radius Radius of the sphere.
 */
Sphere::Sphere(double radius) : radius(radius)
{
}

/**
 * @brief Destructor for the Sphere class.
 */
Sphere::~Sphere()
{
}

/**
 * @brief Generates and returns a Sphere-shaped vtkPolyDataMapper object.
 *
 * @return vtkSmartPointer<vtkPolyDataMapper> The mapper representing the Sphere shape.
 */
vtkSmartPointer<vtkPolyDataMapper>  Sphere::createShape() const
{
    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetRadius(radius);
    sphereSource->SetPhiResolution(100);
    sphereSource->SetThetaResolution(100);
    sphereSource->Update();

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(sphereSource->GetOutput());
    return mapper;
}




/**
 * @brief Constructor for the Hemisphere class.
 *
 * @param radius Radius of the hemisphere.
 */
Hemisphere::Hemisphere(double radius) : radius(radius)
{
}

/**
 * @brief Destructor for the Sphere class.
 */
Hemisphere::~Hemisphere()
{
}

/**
 * @brief Generates and returns a Hemisphere-shaped vtkPolyDataMapper object.
 *
 * @return vtkSmartPointer<vtkPolyDataMapper> The mapper representing the Hemisphere shape.
 */
vtkSmartPointer<vtkPolyDataMapper> Hemisphere::createShape() const
{
    vtkSmartPointer<vtkSphereSource> sphereSource = vtkSmartPointer<vtkSphereSource>::New();
    sphereSource->SetRadius(radius);
    sphereSource->SetStartTheta(0);
    sphereSource->SetEndTheta(180);
    sphereSource->SetPhiResolution(100);
    sphereSource->SetThetaResolution(100);
    sphereSource->Update();

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(sphereSource->GetOutput());
    return mapper;
}



/**
 * @brief Constructor for the Cone class.
 *
 * @param angle Angle of the Cone.
 */
Cone::Cone(double angle) : angle(angle) 
{
}

/**
 * @brief Destructor for the Cone class.
 */
Cone::~Cone()
{
}

/**
 * @brief Generates and returns a cone-shaped vtkPolyDataMapper object.
 *
 * @return vtkSmartPointer<vtkPolyDataMapper> The mapper representing the Cone shape.
 */
vtkSmartPointer<vtkPolyDataMapper> Cone::createShape() const
{
    vtkSmartPointer<vtkConeSource> coneSource = vtkSmartPointer<vtkConeSource>::New();
    coneSource->SetAngle(angle);
    coneSource->SetResolution(100);
    coneSource->Update();

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(coneSource->GetOutput());
    return mapper;
}



/**
 * @brief Constructor for the Pyramid class.
 *
 * @param baseLength Base length of the Pyramid base.
 * @param height Height of the Pyramid.
 */
Pyramid::Pyramid(double baseLength, double height) : baseLength(baseLength), height(height)
{
}

/**
 * @brief Destructor for the Pyramid class.
 */
Pyramid::~Pyramid()
{
}

/**
 * @brief Generates and returns a Pyramid-shaped vtkPolyDataMapper object.
 *
 * @return vtkSmartPointer<vtkPolyDataMapper> The mapper representing the Pyramid shape.
 */
vtkSmartPointer<vtkPolyDataMapper> Pyramid::createShape() const
{
    // Create pyramid vertices
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(0.0, 0.0, 0.0); // Point 0 - Base corner
    points->InsertNextPoint(baseLength, 0.0, 0.0); // Point 1 - Base corner
    points->InsertNextPoint(baseLength, baseLength, 0.0); // Point 2 - Base corner
    points->InsertNextPoint(0.0, baseLength, 0.0); // Point 3 - Base corner
    points->InsertNextPoint(baseLength/2, baseLength/2, height); // Point 4 - Apex

    // Connect the vertices to create the pyramid
    vtkSmartPointer<vtkCellArray> cells = vtkSmartPointer<vtkCellArray>::New();
    cells->InsertNextCell(3); // Triangle
    cells->InsertCellPoint(0);
    cells->InsertCellPoint(1);
    cells->InsertCellPoint(4);

    cells->InsertNextCell(3); // Triangle
    cells->InsertCellPoint(1);
    cells->InsertCellPoint(2);
    cells->InsertCellPoint(4);

    cells->InsertNextCell(3); // Triangle
    cells->InsertCellPoint(2);
    cells->InsertCellPoint(3);
    cells->InsertCellPoint(4);

    cells->InsertNextCell(3); // Triangle
    cells->InsertCellPoint(3);
    cells->InsertCellPoint(0);
    cells->InsertCellPoint(4);

    cells->InsertNextCell(4); // Square base
    cells->InsertCellPoint(0);
    cells->InsertCellPoint(1);
    cells->InsertCellPoint(2);
    cells->InsertCellPoint(3);

    // Create a polydata object
    vtkSmartPointer<vtkPolyData> pyramid = vtkSmartPointer<vtkPolyData>::New();
    pyramid->SetPoints(points);
    pyramid->SetPolys(cells);

    // Mapper
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(pyramid);

    return mapper;
}



/**
 * @brief Constructor for the Cylinder class.
 *
 * @param radius Radius of the Cylinder.
 * @param height Height of the Cylinder.
 */
Cylinder::Cylinder(double radius, double height) : radius(radius), height(height)
{
}

/**
 * @brief Destructor for the Cylinder class.
 */
Cylinder::~Cylinder()
{
}

/**
 * @brief Generates and returns a Cylinder-shaped vtkPolyDataMapper object.
 *
 * @return vtkSmartPointer<vtkPolyDataMapper> The mapper representing the Cylinder shape.
 */
vtkSmartPointer<vtkPolyDataMapper> Cylinder::createShape() const
{
    vtkSmartPointer<vtkCylinderSource> cylinderSource = vtkSmartPointer<vtkCylinderSource>::New();
    cylinderSource->SetRadius(radius);
    cylinderSource->SetHeight(height);
    cylinderSource->SetResolution(50);
    cylinderSource->Update();

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(cylinderSource->GetOutput());
    return mapper;
}



/**
 * @brief Constructor for the Tube class.
 *
 * @param radius Radius of the Tube.
 * @param length Length of the Tube.
 */
Tube::Tube(double radius, double length) : radius(radius), length(length)
{
}

/**
 * @brief Destructor for the Tube class.
 */
Tube::~Tube()
{
}

/**
 * @brief Generates and returns a Tube-shaped vtkPolyDataMapper object.
 *
 * @return vtkSmartPointer<vtkPolyDataMapper> The mapper representing the Tube shape.
 */
vtkSmartPointer<vtkPolyDataMapper> Tube::createShape() const
{

    // Create a line.
    vtkNew<vtkLineSource> lineSource;
    lineSource->SetPoint1(0.0, 0.0, 0.0);
    lineSource->SetPoint2(0.0, length, 0.0);
    

    // Create a mapper and actor.
    vtkNew<vtkPolyDataMapper> lineMapper;
    lineMapper->SetInputConnection(lineSource->GetOutputPort());
    vtkNew<vtkActor> lineActor;
    lineActor->SetMapper(lineMapper);

    // Create a tube (cylinder) around the line.
    vtkNew<vtkTubeFilter> tubeFilter;
    tubeFilter->SetInputConnection(lineSource->GetOutputPort());
    tubeFilter->SetRadius(radius);
    tubeFilter->SetNumberOfSides(50);
    tubeFilter->Update();

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputData(tubeFilter->GetOutput());
    return mapper;
}



/**
 * @brief Constructor for the Doughnut class.
 *
 * @param radius Radius of the Doughnut.
 * @param height Height of the Doughnut.
 */
Doughnut::Doughnut(double radius, double height) : radius(radius), height(height)
{
}

/**
 * @brief Destructor for the Doughnut class.
 */
Doughnut::~Doughnut()
{
}

/**
 * @brief Generates and returns a Doughnut-shaped vtkPolyDataMapper object.
 *
 * @return vtkSmartPointer<vtkPolyDataMapper> The mapper representing the Doughnut shape.
 */
vtkSmartPointer<vtkPolyDataMapper> Doughnut::createShape() const
{

    // Create a torus
    vtkSmartPointer<vtkParametricTorus> torus = vtkSmartPointer<vtkParametricTorus>::New();
    torus->SetRingRadius(radius);  // Radius from the center of the torus to the center of the tube
    torus->SetCrossSectionRadius(height);  // Radius of the tube

    vtkSmartPointer<vtkParametricFunctionSource> functionSource = vtkSmartPointer<vtkParametricFunctionSource>::New();
    functionSource->SetParametricFunction(torus);

    // Mapper
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(functionSource->GetOutputPort());

    return mapper;
}



/**
 * @brief Constructor for the CurvedCylinder class.
 *
 * @param radius Radius of the Curved Cylinder.
 */
CurvedCylinder::CurvedCylinder(double radius) : radius(radius)
{
}

/**
 * @brief Destructor for the CurvedCylinder class.
 */
CurvedCylinder::~CurvedCylinder()
{
}

/**
 * @brief Generates and returns a CurvedCylinder-shaped vtkPolyDataMapper object.
 *
 * @return vtkSmartPointer<vtkPolyDataMapper> The mapper representing the CurvedCylinder shape.
 */
vtkSmartPointer<vtkPolyDataMapper> CurvedCylinder::createShape() const
{

    // 1. Use vtkParametricSpline to define the curve
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    points->InsertNextPoint(0, 0, 0);
    points->InsertNextPoint(1, 1, 2);
    points->InsertNextPoint(2, 2, 0);

    vtkSmartPointer<vtkParametricSpline> spline = vtkSmartPointer<vtkParametricSpline>::New();
    spline->SetPoints(points);

    vtkSmartPointer<vtkParametricFunctionSource> functionSource = vtkSmartPointer<vtkParametricFunctionSource>::New();
    functionSource->SetParametricFunction(spline);
    functionSource->SetUResolution(50);
    functionSource->Update();

    // 2. Use vtkTubeFilter to sweep a circle along the spline
    vtkSmartPointer<vtkTubeFilter> tubeFilter = vtkSmartPointer<vtkTubeFilter>::New();
    tubeFilter->SetInputConnection(functionSource->GetOutputPort());
    tubeFilter->SetRadius(0.1);
    tubeFilter->SetNumberOfSides(50);
    tubeFilter->Update();

    // Mapper
    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(tubeFilter->GetOutputPort());

    return mapper;
}