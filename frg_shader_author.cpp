#include "QtGui"

#include "QGraphicsItem"
#include "QGraphicsView"
#include "QGraphicsScene"
#include "QPainterPath"
#include "QWheelEvent"
#include "QPalette"
#include "QMenu"

#include "math.h"

#include "frg_shader_author.h"
#include "shader_space.h"
#include "node.h"
#include "shader_view.h"

ChangeSpaceAction::ChangeSpaceAction(QGraphicsScene *space, QObject *parent)
    : QAction(parent)
{
    new_space = space;
    connect(this, SIGNAL(triggered()), this, SLOT(emitTriggered()));
}

void ChangeSpaceAction::emitTriggered()
{
    emit triggered(new_space);
}


frg_Shader_Author::frg_Shader_Author(QWidget *parent)
    : QMainWindow(parent)
{
    filename = "";
    setWindowTitle("frg Shader Author");
    resize(800, 600);

    root_scene = new Shader_Space();
    Shader_Space *root_space = (Shader_Space*)root_scene;
    root_space->setName("Root");

    QHBoxLayout *layout = new QHBoxLayout;

    view = new Shader_View(this);

    view->setScene(root_space);
    view->setDragMode(QGraphicsView::RubberBandDrag);
    view->setUpdatesEnabled(true);

    toolbar = addToolBar("");
    updateToolBar();

    layout->addWidget(view);

    QWidget *widget = new QWidget;
    widget->setLayout(layout);

    createMenus();

    statusBar()->showMessage("Welcome to frg Shader Author");
    setCentralWidget(widget);

};

void frg_Shader_Author::moveIntoSpace(QGraphicsScene *space)
{
    view->setScene(space);
    spaces.append(space);
    updateToolBar();
}

void frg_Shader_Author::updateToolBar()
{
    foreach(QAction *action, toolbar->actions())
        toolbar->removeAction(action);

    QAction *rootAction = toolbar->addAction("Root");
    connect(rootAction, SIGNAL(triggered()), this, SLOT(setRoot_Space()));

    Shader_Space *space = dynamic_cast<Shader_Space *>(view->scene());

    Shader_Space *curr_space = space;
    ChangeSpaceAction *chspace;

    for(QList<QGraphicsScene*>::iterator i = spaces.begin(); i != spaces.end(); ++i)
    {
        Shader_Space *cs = (Shader_Space*)*i;
        ChangeSpaceAction *chspace = new ChangeSpaceAction(cs, toolbar);
        chspace->setText(cs->name);
        toolbar->addAction(chspace);
        connect(chspace, SIGNAL(triggered(QGraphicsScene*)), this, SLOT(setShader_Space(QGraphicsScene*)));
    }
}

void frg_Shader_Author::setShader_Space(QGraphicsScene *space)
{
    view->setScene(space);
    QGraphicsScene *tmp;
    while(tmp != space)
        tmp = spaces.takeLast();
    updateToolBar();
}

void frg_Shader_Author::setRoot_Space()
{
    view->setScene(root_scene);
    updateToolBar();
}

void frg_Shader_Author::createMenus()
{
    QMenu *fileMenu = new QMenu("&File");
    QAction *newAction = fileMenu->addAction("&New");
    QAction *openAction = fileMenu->addAction("&Open");
    QAction *saveAction = fileMenu->addAction("&Save");
    QAction *SaveAsAction = fileMenu->addAction("Save &As");
    QAction *quitAction = fileMenu->addAction("&Quit");

    connect(newAction, SIGNAL(triggered()), this, SLOT(newfile()));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));
    connect(SaveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(openAction, SIGNAL(triggered()), this, SLOT(open()));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    menuBar()->addMenu(fileMenu);
};

void frg_Shader_Author::change_window_title(QString title)
{
    QString newtitle("frg Shader Author");
    newtitle.append(" - ");
    newtitle.append(title);
    setWindowTitle(newtitle);
}

void frg_Shader_Author::newfile()
{
    Shader_Space *space = new Shader_Space;
    view->setScene(space);
    root_scene = space;
    foreach(QGraphicsScene *scene, spaces)
        delete scene;
    spaces.clear();
    change_window_title("");
}

void frg_Shader_Author::save()
{
    if(filename == "")
    {
        filename = QFileDialog::getSaveFileName(this);
    }
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    QDataStream stream(&file);
    Shader_Space *space = (Shader_Space*)root_scene;
    stream<<space;
    file.close();
    change_window_title(filename);
}

void frg_Shader_Author::saveAs()
{
    filename = QFileDialog::getSaveFileName(this);
    QFile file(filename);
    file.open(QIODevice::WriteOnly);
    QDataStream stream(&file);
    Shader_Space *space = (Shader_Space*)root_scene;
    stream<<space;
    file.close();
    change_window_title(filename);
}

void frg_Shader_Author::open()
{
    filename = QFileDialog::getOpenFileName(this);
    QFile file(filename);
    file.open(QIODevice::ReadOnly);
    QDataStream stream(&file);
    Shader_Space *space;
    stream>>&space;
    view->setScene(space);
    root_scene = space;
    file.close();
    change_window_title(filename);
}


frg_Shader_Author::~frg_Shader_Author()
{

}
