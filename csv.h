#ifndef CSV_H
#define CSV_H

#include <QDebug>
#include <QFile>
#include <QTextStream>
#include <QStringList>

QVector<QVector<int>> loadCSV(const QString &file_path)
{
    // NOTE: [ROW][COLUMN] -> [y][x]
    QVector<QVector<int>> csv_mat;

    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Failed to open the file:" << file.errorString();
        return csv_mat;
    }

    QTextStream in(&file);

    while (!in.atEnd())
    {
        QVector<int> line_vec;

        QString line = in.readLine();

        QStringList fields = line.split(',');

        for (const QString &field : fields)
        {
            line_vec.append(field.toInt());
        }

        csv_mat.append(line_vec);
    }

    file.close();

    return csv_mat;
}

#endif // CSV_H
