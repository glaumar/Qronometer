#include "CSVExporter.h"

#include <QFile>
#include <QModelIndex>
#include <QTextStream>
#include <QVariant>

void
CSVExporter::exportTo(QString path) const
{
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << "#Number, Total, Lap, Note\n";
    for (int r = 0; r < model_->rowCount(); r++) {
        for (int c = 0; c < model_->columnCount(); c++) {
            QModelIndex index = model_->index(r, c);
            QVariant data = model_->data(index);
            out << data.toString();

            if (c < model_->columnCount() - 1)
                out << ",";
        }
        out << "\n";
    }
}
