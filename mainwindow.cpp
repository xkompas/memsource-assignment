#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <functional>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include "projectsmodel.h"
#include "connector.h"
#include "logindialog.h"

namespace  {

QDateTime parseDate(const QString text)
{
    auto lastPlus = text.lastIndexOf("+");
    auto dateString{(lastPlus == -1) ? text : text.left(lastPlus)};
    return QDateTime::fromString(dateString, "yyyy-MM-ddTHH:mm:ss");
}

}

MainWindow::MainWindow(QWidget *parent):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    connector(new Connector(this)),
    projectsModel(new ProjectsModel(this))
{
    ui->setupUi(this);

    ui->projectsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->projectsTableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->projectsTableView->setModel(projectsModel);

    connect(ui->actionExit, SIGNAL(triggered()), qApp, SLOT(quit()));
}

QString MainWindow::codeToLanguage(const QString code) const
{
    auto it = languages.constFind(code);
    if (it == languages.constEnd())
    {
        return tr("Unknown");
    }
    return it.value();
}

void MainWindow::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    QTimer::singleShot(0, this, SLOT(initialize()));
}

void MainWindow::initialize()
{
    performLogin();
}

void MainWindow::performLogin()
{
    LoginDialog *loginDialog = new LoginDialog();

    if (loginDialog->exec() == QDialog::Rejected)
    {
        qApp->quit();
        return;
    }

    QString userName = loginDialog->getUserName();
    QString password = loginDialog->getPassword();

    delete loginDialog;

    login(userName, password);
}

void MainWindow::login(const QString userName, const QString password)
{
    RequestParams params;
    params.insert("userName", userName);
    params.insert("password", password);
    connector->post("/api2/v1/auth/login", params, std::bind(&MainWindow::processLoginResponse, this, std::placeholders::_1));
}

void MainWindow::processLoginResponse(const Response &response)
{
    if (!response.ok)
    {
        performLogin();
        return;
    }

    apiToken = parseLoginJson(response.jsonDocument);
    loadLanguages();
}

QString MainWindow::parseLoginJson(const QJsonDocument &jsonDocument)
{
    QString token;

    if (jsonDocument.isObject())
    {
        QJsonObject root = jsonDocument.object();
        token = root["token"].toString();
    }

    return token;
}

void MainWindow::loadLanguages()
{
    RequestParams params;
    params.insert("token", apiToken);
    connector->get("/api2/v1/languages", params, std::bind(&MainWindow::processLanguagesResponse, this, std::placeholders::_1));
}

void MainWindow::processLanguagesResponse(const Response &response)
{
    if (response.ok)
    {
        languages = parseLanguagesJson(response.jsonDocument);
    }
    // TODO display error

    loadProjects();
}

Languages MainWindow::parseLanguagesJson(const QJsonDocument &jsonDocument)
{
    Languages parsedLanguages;

    if (jsonDocument.isObject())
    {
        QJsonObject root = jsonDocument.object();
        QJsonArray languagesArray = root["languages"].toArray();

        foreach (const QJsonValue &languageValue, languagesArray)
        {
            QJsonObject language = languageValue.toObject();

            QString code = language["code"].toString();
            QString name = language["name"].toString();

            parsedLanguages.insert(code, name);
        }
    }

    return parsedLanguages;
}

void MainWindow::loadProjects()
{
    RequestParams params;
    params.insert("token", apiToken);
    connector->get("/api2/v1/projects", params, std::bind(&MainWindow::processProjectsResponse, this, std::placeholders::_1));
}

void MainWindow::processProjectsResponse(const Response &response)
{
    if (!response.ok)
    {
        // TODO display error
        return;
    }

    ProjectList projectList = parseProjectsJson(response.jsonDocument);
    projectsModel->load(projectList);
}

QList<Project> MainWindow::parseProjectsJson(const QJsonDocument &jsonDocument)
{
    QList<Project> projectList;

    if (jsonDocument.isObject())
    {
        QJsonObject root = jsonDocument.object();
        QJsonArray content = root["content"].toArray();

        foreach (const QJsonValue &projectValue, content)
        {
            QJsonObject projectObject = projectValue.toObject();

            Project project;
            project.name = projectObject["name"].toString();

            project.sourceLanguage = codeToLanguage(projectObject["sourceLang"].toString());

            QJsonArray targetLanguages = projectObject["targetLangs"].toArray();
            foreach (const QJsonValue &targetLanguageValue, targetLanguages)
            {
                project.targetLanguages.append(codeToLanguage(targetLanguageValue.toString()));
            }

            project.created = parseDate(projectObject["dateCreated"].toString());

            projectList.append(project);
        }
    }

    return projectList;
}

MainWindow::~MainWindow()
{
    delete ui;
}
