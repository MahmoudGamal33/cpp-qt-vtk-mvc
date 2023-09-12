/**
 * @file widget.cpp
 * @brief Implementation of the Widget class.
 */

#include "widget.h"
#include "./ui_widget.h"

#include "boxWidgetCallback.h"

#include <vtkActor.h>
#include <vtkPolyDataMapper.h>
#include <vtkNamedColors.h>
#include <vtkCamera.h>
#include <vtkProperty.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkPolyDataNormals.h>
#include <vtkBoxRepresentation.h>
#include <vtkSTLReader.h>
#include <vtkSTLWriter.h>

#include <QFileDialog>


 /**
  * @brief Constructs the Widget with an optional parent widget.
  *
  * Sets up the UI components and VTK rendering pipeline.
  * @param parent Parent QWidget.
  */
Widget::Widget(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::Widget),
    mRenderWindow(vtkSmartPointer<vtkGenericOpenGLRenderWindow>::New()),
    mRenderer(vtkSmartPointer<vtkRenderer>::New()),
    mInteractor(vtkSmartPointer<QVTKInteractor>::New()),
    mInteractorStyle(vtkSmartPointer<vtkInteractorStyle>::New()),
    mBoxWidget2(vtkSmartPointer<vtkBoxWidget2>::New()),
    callback(vtkSmartPointer<BoxWidgetCallback>::New())
{
    ui->setupUi(this);


    mToolButtonMenu = new QMenu(this);
    mSaveSTLAction = new QAction("Save (STL)", this);
    mLoadSTLAction = new QAction("Load (STL)", this);

    connect(mSaveSTLAction, &QAction::triggered, this, &Widget::onSaveSTL);
    mToolButtonMenu->addAction(mSaveSTLAction);
        
    connect(mLoadSTLAction, &QAction::triggered, this, &Widget::onLoadSTL);
    mToolButtonMenu->addAction(mLoadSTLAction);

    ui->toolButton->setMenu(mToolButtonMenu);


    // Set up the rendering
    mRenderWindow->AddRenderer(mRenderer);
    mRenderWindow->SetInteractor(mInteractor);

    ui->viewWidget->setRenderWindow(mRenderWindow);

    mInteractor->SetInteractorStyle(mInteractorStyle);
    mInteractor->Initialize();

    vtkNew<vtkBoxRepresentation> boxRepresentation;
    boxRepresentation->HandlesOn();
    mBoxWidget2->SetRepresentation(boxRepresentation);
    mBoxWidget2->SetInteractor(mInteractor);


    // Set the UI connections
    QObject::connect(ui->addButton, &QPushButton::clicked, this, &Widget::on_addButton_clicked);
    QObject::connect(ui->editButton, &QPushButton::clicked, this, &Widget::on_editButton_clicked);
    QObject::connect(ui->deleteButton, &QPushButton::clicked, this, &Widget::on_deleteButton_clicked);
    QObject::connect(ui->flipButton, &QPushButton::clicked, this, &Widget::on_flipButton_clicked);
}


/**
 * @brief Destructor for the Widget class.
 *
 * Handles cleanup of dynamically allocated members.
 */
Widget::~Widget()
{
    delete ui;
    delete mToolButtonMenu;
    delete mSaveSTLAction;
    delete mLoadSTLAction;
}


/**
 * @brief Resets all sliders to their default values.
 */
void Widget::reset_sliders(void)
{
    ui->rotateSlider->setValue(0);
    ui->scaleSlider->setValue(0);
    ui->opacitySlider->setValue(100);
    ui->redColorSlider->setValue(0);
    ui->greenColorSlider->setValue(0);
    ui->blueColorSlider->setValue(0);
    ui->xTranslateSlider->setValue(0);
    ui->yTranslateSlider->setValue(0);
    ui->zTranslateSlider->setValue(0);
}


/**
 * @brief Slot triggered when 'addButton' is clicked.
 *
 * Creates a new shape based on the selected option from the combo box.
 */
void Widget::on_addButton_clicked()
{
    vtkNew<vtkNamedColors> colors;

    vtkSmartPointer<vtkPolyDataMapper> shapeMapper = shapeController.createShape(ui->comboBox->currentText());
    if (!shapeMapper) {
        return; // or handle the error
    }

    if (mCurrentShapeActor)
    {
        reset_sliders();

        mRenderer->RemoveViewProp(mCurrentShapeActor);
        mBoxWidget2->Off();
    }

    vtkSmartPointer<vtkActor> shapeActor = vtkSmartPointer<vtkActor>::New();
    shapeActor->SetMapper(shapeMapper);
    shapeActor->GetProperty()->SetColor(0, 0, 0);
    mCurrentShapeActor = shapeActor;

    mRenderer->AddViewProp(shapeActor);
    mRenderer->SetBackground(colors->GetColor3d("Salmon").GetData());
    mRenderer->ResetCamera();
    mRenderer->GetActiveCamera()->Azimuth(5);
    mRenderer->GetActiveCamera()->Elevation(5);

    mRenderWindow->Render();
}


/**
 * @brief Slot triggered when 'editButton' is clicked.
 *
 * Provides a box widget for modifying the current shape.
 */
void Widget::on_editButton_clicked()
{
    if (mCurrentShapeActor)
    {
        callback->Actor = mCurrentShapeActor;
        mBoxWidget2->AddObserver(vtkCommand::InteractionEvent, callback);

        mBoxWidget2->GetRepresentation()->PlaceWidget(mCurrentShapeActor->GetBounds());
        mBoxWidget2->On();
    }
}


/**
 * @brief Slot triggered when 'deleteButton' is clicked.
 *
 * Removes the current shape from the scene.
 */
void Widget::on_deleteButton_clicked()
{
    if (mCurrentShapeActor)
    {
        reset_sliders();

        mBoxWidget2->Off();

        mRenderer->RemoveViewProp(mCurrentShapeActor);
        mRenderWindow->Render();

        mCurrentShapeActor = nullptr;
    }
}


/**
 * @brief Slot triggered when 'mergeButton' is clicked.
 *
 * TODO: Add merge logic.
 */
void Widget::on_mergeButton_clicked()
{

}


/**
 * @brief Slot triggered when 'flipButton' is clicked.
 *
 * Rotates the current shape by 90 degrees around the Y-axis.
 */
void Widget::on_flipButton_clicked()
{
    if (mCurrentShapeActor)
    {
        mCurrentShapeActor->RotateY(90);
        mRenderWindow->Render();
    }
}


/**
 * @brief Slot triggered when the value of 'rotateSlider' changes.
 *
 * Rotates the current shape based on the slider value.
 * @param value New value of the slider.
 */
void Widget::on_rotateSlider_valueChanged(int value)
{
    if (mCurrentShapeActor)
    {
        mCurrentShapeActor->RotateY(value);
        mRenderWindow->Render();
    }
}


/**
 * @brief Slot triggered when the value of 'scaleSlider' changes.
 *
 * Scale the current shape based on the slider value.
 * @param value New value of the slider.
 */
void Widget::on_scaleSlider_valueChanged(int value)
{
    if (mCurrentShapeActor)
    {
        double scaleFactor = 1 + (value / 100.0);

        // Adjust the scale of the shape actor
        mCurrentShapeActor->SetScale(scaleFactor, scaleFactor, scaleFactor);

        // Render the scene again to reflect the scaling change
        mRenderWindow->Render();
    }
}


/**
 * @brief Slot triggered when the value of 'opacitySlider' changes.
 *
 * Change opacity of the current shape based on the slider value.
 * @param value New value of the slider.
 */
void Widget::on_opacitySlider_valueChanged(int value)
{
    // Convert slider value to opacity range [0, 1]
    double opacity = value / 100.0;

    if (mCurrentShapeActor)
    {
        mCurrentShapeActor->GetProperty()->SetOpacity(opacity);
        mRenderWindow->Render();
    }
}


/**
 * @brief Slot triggered when the value of 'redColorSlider' changes.
 *
 * Change color (red in RGB) of the current shape based on the slider value.
 * @param value New value of the slider.
 */
void Widget::on_redColorSlider_valueChanged(int value)
{

    if (mCurrentShapeActor)
    {
        double rgb[3];

        mCurrentShapeActor->GetProperty()->GetColor(rgb);

        rgb[0] = value / 255.0;

        mCurrentShapeActor->GetProperty()->SetColor(rgb);
        mRenderWindow->Render();
    }

}

/**
 * @brief Slot triggered when the value of 'redColorSlider' changes.
 *
 * Change color (green in RGB) of the current shape based on the slider value.
 * @param value New value of the slider.
 */
void Widget::on_greenColorSlider_valueChanged(int value)
{
    if (mCurrentShapeActor)
    {
        double rgb[3];

        mCurrentShapeActor->GetProperty()->GetColor(rgb);

        rgb[1] = value / 255.0;

        mCurrentShapeActor->GetProperty()->SetColor(rgb);
        mRenderWindow->Render();
    }
}

/**
 * @brief Slot triggered when the value of 'redColorSlider' changes.
 *
 * Change color (blue in RGB) of the current shape based on the slider value.
 * @param value New value of the slider.
 */
void Widget::on_blueColorSlider_valueChanged(int value)
{
    if (mCurrentShapeActor)
    {
        double rgb[3];

        mCurrentShapeActor->GetProperty()->GetColor(rgb);

        rgb[2] = value / 255.0;

        mCurrentShapeActor->GetProperty()->SetColor(rgb);
        mRenderWindow->Render();
    }
}


/**
 * @brief Slot triggered when the value of 'xTranslateSlider' changes.
 *
 * Update the x-position of the current shape based on the slider value.
 * @param value New value of the slider.
 */
void Widget::on_xTranslateSlider_valueChanged(int value)
{
    if (mCurrentShapeActor)
    {
        // Get the current position
        double currentPosition[3];
        mCurrentShapeActor->GetPosition(currentPosition);

        // Update the x-position (or y or z, depending on your needs)
        currentPosition[0] = value / 10.0;

        // Set the new position
        mCurrentShapeActor->SetPosition(currentPosition);

        mRenderWindow->Render();
    }
}


/**
 * @brief Slot triggered when the value of 'yTranslateSlider' changes.
 *
 * Update the y-position of the current shape based on the slider value.
 * @param value New value of the slider.
 */
void Widget::on_yTranslateSlider_valueChanged(int value)
{
    if (mCurrentShapeActor)
    {
        // Get the current position
        double currentPosition[3];
        mCurrentShapeActor->GetPosition(currentPosition);

        // Update the x-position (or y or z, depending on your needs)
        currentPosition[1] = value / 10.0;

        // Set the new position
        mCurrentShapeActor->SetPosition(currentPosition);

        mRenderWindow->Render();
    }
}


/**
 * @brief Slot triggered when the value of 'zTranslateSlider' changes.
 *
 * Update the z-position of the current shape based on the slider value.
 * @param value New value of the slider.
 */
void Widget::on_zTranslateSlider_valueChanged(int value)
{
    if (mCurrentShapeActor)
    {
        // Get the current position
        double currentPosition[3];
        mCurrentShapeActor->GetPosition(currentPosition);

        // Update the x-position (or y or z, depending on your needs)
        currentPosition[2] = value / 10.0;

        // Set the new position
        mCurrentShapeActor->SetPosition(currentPosition);

        mRenderWindow->Render();
    }
}


/**
 * @brief Saves the current shape actor to an STL file.
 */
void Widget::onSaveSTL()
{
    // Your save STL logic here.
    if (mCurrentShapeActor)
    {
        // Fetch the actor's geometry data
        vtkPolyData* polyData = vtkPolyData::SafeDownCast(mCurrentShapeActor->GetMapper()->GetInput());

        if (polyData)
        {
            // Open a save file dialog to choose the location and filename
            QString filePath = QFileDialog::getSaveFileName(
                this,
                "Save STL",
                QDir::homePath(),
                "STL Files (*.stl);;All Files (*)"
            );

            if (filePath.isEmpty())
                return; // user canceled

            if (!filePath.endsWith(".stl", Qt::CaseInsensitive))
                filePath += ".stl";  // Append STL extension if not present

            // Use vtkSTLWriter to save the data
            vtkSmartPointer<vtkSTLWriter> stlWriter = vtkSmartPointer<vtkSTLWriter>::New();
            stlWriter->SetFileName(filePath.toStdString().c_str());
            stlWriter->SetInputData(polyData);
            stlWriter->Write();
        }
    }
}


/**
 * @brief Loads a shape from an STL file and sets it as the current shape actor.
 */
void Widget::onLoadSTL()
{
    // Open a file dialog to choose the STL file to load
    QString filePath = QFileDialog::getOpenFileName(
        this,
        "Open STL",
        QDir::homePath(),
        "STL Files (*.stl);;All Files (*)"
    );

    if (filePath.isEmpty())
        return; // user canceled

    // Use vtkSTLReader to read the STL file
    vtkSmartPointer<vtkSTLReader> stlReader = vtkSmartPointer<vtkSTLReader>::New();
    stlReader->SetFileName(filePath.toStdString().c_str());

    // Map the read data to an actor
    vtkSmartPointer<vtkPolyDataMapper> shapeMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    shapeMapper->SetInputConnection(stlReader->GetOutputPort());

    // Create an actor for the shape
    vtkSmartPointer<vtkActor> shape = vtkSmartPointer<vtkActor>::New();
    shape->SetMapper(shapeMapper);

    // Remove the previous shape actor if exists
    if (mCurrentShapeActor)
    {
        mRenderer->RemoveActor(mCurrentShapeActor);
    }

    // Set the newly loaded actor as the current shape actor
    mCurrentShapeActor = shape;

    // Add the shape actor to the renderer
    mRenderer->AddActor(mCurrentShapeActor);

    // Update the rendering
    mRenderer->ResetCamera();
    mRenderWindow->Render();
}
