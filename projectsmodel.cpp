#include "projectsmodel.h"

ProjectsModel::ProjectsModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

QVariant ProjectsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (section == 0 && orientation == Qt::Horizontal && role == Qt::DisplayRole)
    {
        return QString(tr("Project"));
    }
    return QVariant();
}

int ProjectsModel::rowCount(const QModelIndex &) const
{
    return 1;
}

int ProjectsModel::columnCount(const QModelIndex &) const
{
    return 1;
}

QVariant ProjectsModel::data(const QModelIndex &index, int role) const
{
    if (index.isValid() && role == Qt::DisplayRole)
    {
        return QString("TODO");
    }

    return QVariant();
}
