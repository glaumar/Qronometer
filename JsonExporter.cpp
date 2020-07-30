#include "JsonExporter.h"

#include <QFile>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QModelIndex>
#include <QTextStream>
#include <QVariant>

void
JsonExporter::exportTo(QString path) const
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);

    QJsonObject json;
    QJsonArray jarr;
    QModelIndex index;
    QVariant data;
    for (int r = 0; r < model_->rowCount(); r++) {
        index = model_->index(r, 0);
        data = model_->data(index);
        json["number"] = data.toInt();

        index = model_->index(r, 1);
        data = model_->data(index);
        json["Total"] = data.toString();

        index = model_->index(r, 2);
        data = model_->data(index);
        json["Lap"] = data.toString();

        index = model_->index(r, 3);
        data = model_->data(index);
        json["Note"] = data.toString();

        jarr.append(json);
    }

    QJsonDocument jdoc{ jarr };

    out << jdoc.toJson(QJsonDocument::Indented);
}
