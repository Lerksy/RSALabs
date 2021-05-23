#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <Qca-qt5/QtCrypto/QtCrypto>
#include <QErrorMessage>
#include <QDebug>
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();
    enum actionType{
        ENCRYPT,
        DECRYPT,
        SIGN,
        CHECKSIGN
    };

private slots:
    void on_encryptRadio_toggled(bool checked);
    void on_decryptRadio_toggled(bool checked);
    void on_genKeysButton_clicked();
    void on_proceedButton_clicked();
    void on_lockKeysButton_clicked();
    void on_signRadio_toggled(bool checked);
    void on_checkSignRadio_toggled(bool checked);
    void on_copyOutputButton_clicked();

    void on_exportKeysButton_clicked();

    void on_outputText_textChanged();

private:
    Ui::Dialog *ui;
    bool keysAccepted;
    actionType radioAction;
    QCA::PrivateKey privateKey;
    QCA::PublicKey publicKey;
    QErrorMessage *error;
};
#endif // DIALOG_H
