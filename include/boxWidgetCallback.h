#pragma once

#include <vtkCommand.h>
#include <vtkBoxWidget2.h>
#include <vtkTransform.h>
#include <vtkActor.h>
#include <vtkBoxRepresentation.h>
#include <vtkTransformPolyDataFilter.h>



/**
 * @class BoxWidgetCallback
 * @brief Callback class derived from vtkCommand, which responds when the vtkBoxWidget2 is manipulated.
 *
 * This class provides functionality to update the associated vtkActor's geometry
 * in response to manipulations on the vtkBoxWidget2. The transformation of the
 * vtkBoxWidget2 is extracted and applied to the vtkActor's underlying polydata.
 */
class BoxWidgetCallback : public vtkCommand
{
public:

    /**
     * @brief Factory method to create a new instance of BoxWidgetCallback.
     *
     * @return BoxWidgetCallback* Pointer to the new instance of BoxWidgetCallback.
     */
    static BoxWidgetCallback* New()
    {
        return new BoxWidgetCallback;
    }


    /**
     * @brief Overridden Execute method which gets triggered when the associated vtkBoxWidget2 is manipulated.
     *
     * This method extracts the transformation from the vtkBoxWidget2, applies it to the associated
     * vtkActor's polydata, and then updates the actor with the transformed polydata.
     *
     * @param caller vtkObject* which triggered the callback (expected to be vtkBoxWidget2).
     * @param event Not used in this implementation.
     * @param clientData Not used in this implementation.
     */
    virtual void Execute(vtkObject* caller, unsigned long, void*) override
    {
        vtkBoxWidget2* boxWidget = reinterpret_cast<vtkBoxWidget2*>(caller);
        vtkSmartPointer<vtkTransform> t = vtkSmartPointer<vtkTransform>::New();
        vtkBoxRepresentation* boxRep = reinterpret_cast<vtkBoxRepresentation*>(boxWidget->GetRepresentation());
        boxRep->GetTransform(t);

        // Extract the actor's polydata
        vtkPolyData* polydata = vtkPolyData::SafeDownCast(this->Actor->GetMapper()->GetInput());

        // Apply the transformation to the polydata
        vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
        transformFilter->SetInputData(polydata);
        transformFilter->SetTransform(t);
        transformFilter->Update();

        // Set the transformed polydata back to the actor
        this->Actor->GetMapper()->SetInputDataObject(transformFilter->GetOutput());

        // Reset the box widget to match the transformed actor
        boxWidget->GetRepresentation()->PlaceWidget(this->Actor->GetBounds());
    }

    /**
     * @brief Default constructor initializing Actor to null.
     */
    BoxWidgetCallback(): Actor(0) {}

    vtkActor* Actor; ///< Pointer to the associated vtkActor whose polydata should be transformed.
};

