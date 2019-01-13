#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "projectsmodel.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    projectsModel(new ProjectsModel)
{
    ui->setupUi(this);

    loadProjects();

    QObject::connect(ui->actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MainWindow::loadProjects()
{
    ui->projectsTableView->setModel(projectsModel);
}

MainWindow::~MainWindow()
{
    delete ui;
}
