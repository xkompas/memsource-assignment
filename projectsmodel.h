#ifndef PROJECTSMODEL_H
#define PROJECTSMODEL_H

#include <QAbstractTableModel>
#include "project.h"

using ProjectList = QList<Project>;

class ProjectsModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ProjectsModel(QObject *parent = nullptr);

    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void load(const ProjectList &aProjectList);

private:
     ProjectList projectList;
};

#endif // PROJECTSMODEL_H
