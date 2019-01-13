#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "projectsmodel.h"

namespace  {

QDateTime parseDate(const QString text)
{
    return QDateTime::fromString(text, "yyyy-MM-ddTHH:mm:ss");
}

}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    projectsModel(new ProjectsModel)
{
    ui->setupUi(this);

    ui->projectsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->projectsTableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->projectsTableView->setModel(projectsModel);

    QObject::connect(ui->actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));

    loadProjects();
}

void MainWindow::loadProjects()
{
    QStringList target01;
    target01.append("Afrikaans");

    QStringList target02;
    target02.append("Afrikaans (Namibia)");
    target02.append("Afrikaans (South Africa)");
    target02.append("Balochi");
    target02.append("Bambara");

    QStringList target03;
    target03.append("German");
    target03.append("German (Austria)");
    target03.append("German (Belgium)");
    target03.append("German (Switzerland)");
    target03.append("German (Germany)");
    target03.append("German (Lichtenstein)");
    target03.append("German (Luxembourg)");

    Project project01{"Project01", "French", target01, parseDate("2019-01-05T17:04:56")};
    Project project02{"Project02", "Catalan (Spain, Valencia)", target02, parseDate("2019-01-05T17:13:58")};
    Project project03{"Project03", "Czech (Czech Republic)", target03, parseDate("2019-01-05T17:14:56")};

    ProjectList projectList;
    projectList.append(project01);
    projectList.append(project02);
    projectList.append(project03);

    projectsModel->load(projectList);
}

MainWindow::~MainWindow()
{
    delete ui;
}
