#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QMenu>
#include <QAction>

#include "controller.h"

#include <vtkGenericOpenGLRenderWindow.h>
#include <vtkRenderer.h>
#include <QVTKInteractor.h>
#include <vtkInteractorStyle.h>
#include <vtkBoxWidget2.h>
#include <BoxWidgetCallback.h>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

/**
 * @class Widget
 * @brief The Widget class provides a graphical user interface to manipulate and visualize 3D objects.
 */
    class Widget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructs a Widget with the given parent.
     * @param parent The parent QWidget.
     */
    Widget(QWidget* parent = nullptr);

    /// @brief Destroys the Widget.
    ~Widget();

private slots:
    void on_addButton_clicked();
    void on_editButton_clicked();
    void on_deleteButton_clicked();
    void on_mergeButton_clicked();
    void on_flipButton_clicked();
    void on_rotateSlider_valueChanged(int value);
    void on_scaleSlider_valueChanged(int value);
    void on_opacitySlider_valueChanged(int value);
    void on_redColorSlider_valueChanged(int value);
    void on_greenColorSlider_valueChanged(int value);
    void on_blueColorSlider_valueChanged(int value);
    void on_xTranslateSlider_valueChanged(int value);
    void on_yTranslateSlider_valueChanged(int value);
    void on_zTranslateSlider_valueChanged(int value);
    void onSaveSTL();
    void onLoadSTL();

private:
    Ui::Widget* ui;

    QMenu* mToolButtonMenu;
    QAction* mSaveSTLAction;
    QAction* mLoadSTLAction;

    vtkSmartPointer<vtkGenericOpenGLRenderWindow> mRenderWindow;
    vtkSmartPointer<vtkRenderer> mRenderer;
    vtkSmartPointer<QVTKInteractor> mInteractor;
    vtkSmartPointer<vtkInteractorStyle> mInteractorStyle;
    vtkSmartPointer<vtkActor> mCurrentShapeActor;
    vtkSmartPointer<vtkBoxWidget2> mBoxWidget2;
    vtkSmartPointer<BoxWidgetCallback> callback;

    ShapeController shapeController;


    /**
     * @brief Resets all sliders to their default values.
     */
    void reset_sliders(void);
};
#endif // WIDGET_H
