#ifndef CSV_H
#define CSV_H

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QStringList>

QVector<QVector<int>> loadCSV(const QString& file_path)
{
    // NOTE: [ROW][COLUMN] -> [y][x]
    QVector<QVector<int>> csv_mat;

    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open the file:" << file.errorString();
        return csv_mat;
    }

    // Read the CSV content using QTextStream
    QTextStream in(&file);
    while (!in.atEnd()) {
        QVector<int> line_vec;

        QString line = in.readLine();

        // Parse the CSV line into a list of QStrings
        QStringList fields = line.split(',');

        // Process the fields as needed
        for (const QString& field : fields) {
            line_vec.append(field.toInt());
        }

        csv_mat.append(line_vec);
    }

    // Close the file
    file.close();

    qDebug()<<csv_mat;

    return csv_mat;
}

#endif // CSV_H
