#ifndef DES_H
#define DES_H

#include <QDialog>
#include <Qca-qt5/QtCrypto/QtCrypto>
#include <QMessageBox>

namespace Ui {
class DES;
}

class DES : public QDialog
{
    Q_OBJECT

public:
    explicit DES(QWidget *parent = nullptr);
    ~DES();

private slots:
    void on_cipherButton_clicked();

private:
    Ui::DES *ui;
};

#endif // DES_H
