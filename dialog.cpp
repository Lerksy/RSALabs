#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    ui->outputText->setReadOnly(true);
    ui->encryptRadio->setChecked(true);
    keysAccepted = false;
    error = new QErrorMessage(this);
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::on_encryptRadio_toggled(bool checked){
    encrypt = true;
    Q_UNUSED(checked);
}


void Dialog::on_decryptRadio_toggled(bool checked){
    encrypt = false;
    Q_UNUSED(checked);
}


void Dialog::on_genKeysButton_clicked(){
    privateKey = QCA::KeyGenerator().createRSA(2048);
    publicKey = privateKey.toPublicKey();
    ui->publicKeyText->setReadOnly(true);
    ui->privateKeyText->setReadOnly(true);
    keysAccepted = true;
    ui->lockKeysButton->setEnabled(false);
    ui->genKeysButton->setEnabled(false);
    ui->publicKeyText->setPlainText(publicKey.toPEM());
    ui->privateKeyText->setPlainText(privateKey.toPEM());
}


void Dialog::on_proceedButton_clicked(){
    if(!keysAccepted){
        error->showMessage("You havent provided keys!\nGenerate keys or insert your own keys to continue!");
    }else{
        QCA::SecureArray result;
        if(encrypt){
            if(!ui->inputText->toPlainText().isEmpty()){
                result = publicKey.encrypt(ui->inputText->toPlainText().toLocal8Bit(), QCA::EME_PKCS1_OAEP);
                ui->outputText->setPlainText(QCA::arrayToHex(result.toByteArray()));
            }
        }else{
            if(!privateKey.decrypt(QCA::hexToArray(ui->inputText->toPlainText().toLocal8Bit()), &result, QCA::EME_PKCS1_OAEP))
                error->showMessage("Failed to decrypt");
            ui->outputText->setPlainText(result.toByteArray());
        }
    }
}


void Dialog::on_lockKeysButton_clicked(){
    QCA::ConvertResult privateResult, publicResult;
    privateKey.fromPEM(ui->privateKeyText->toPlainText(), QCA::SecureArray(), &privateResult);
    publicKey.fromPEM(ui->publicKeyText->toPlainText(), &publicResult);
    if(privateResult == QCA::ConvertGood && publicResult == QCA::ConvertGood){
        ui->publicKeyText->setReadOnly(true);
        ui->privateKeyText->setReadOnly(true);
        keysAccepted = true;
        ui->lockKeysButton->hide();
        ui->genKeysButton->setEnabled(false);
    }else{
        error->showMessage("One of your keys is corrupted!");
    }
}

