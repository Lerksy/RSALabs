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
    radioAction = actionType::ENCRYPT;
    Q_UNUSED(checked);
}


void Dialog::on_decryptRadio_toggled(bool checked){
    radioAction = actionType::DECRYPT;
    Q_UNUSED(checked);
}

void Dialog::on_signRadio_toggled(bool checked){
    radioAction = actionType::SIGN;
    Q_UNUSED(checked);
}


void Dialog::on_checkSignRadio_toggled(bool checked){
    radioAction = actionType::CHECKSIGN;
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
    }else if(!ui->inputText->toPlainText().isEmpty()){
        QCA::SecureArray result;
        switch (radioAction) {
        case Dialog::ENCRYPT:{
            result = publicKey.encrypt(ui->inputText->toPlainText().toLocal8Bit(), QCA::EME_PKCS1_OAEP);
            if(!result.isEmpty()){
                ui->outputText->setPlainText(QCA::arrayToHex(result.toByteArray()));
            }else{
                error->showMessage("Failed to encrypt");
            }
            break;
        }
        case Dialog::DECRYPT:{
            if(!privateKey.decrypt(QCA::hexToArray(ui->inputText->toPlainText().toLocal8Bit()), &result, QCA::EME_PKCS1_OAEP))
                error->showMessage("Failed to decrypt");
            ui->outputText->setPlainText(result.toByteArray());
            break;
        }
        case Dialog::SIGN:{
            QCA::SecureArray input = ui->inputText->toPlainText().toLocal8Bit();
            ui->outputText->setPlainText(QCA::arrayToHex(privateKey.signMessage(input, QCA::EMSA1_SHA1)));
            break;
        }
        case Dialog::CHECKSIGN:{
            QString origMsg = QInputDialog::getText(this, "Msg asker", "Please input original Message");
            if(publicKey.verifyMessage(origMsg.toLocal8Bit(), QCA::hexToArray(ui->inputText->toPlainText().toLocal8Bit()), QCA::EMSA1_SHA1)){
                ui->outputText->setPlainText("Signature is VALID!");
            }else{
                ui->outputText->setPlainText("Signature is INVALID!");
            }
            break;
        }

        }
    }else{
        error->showMessage("Input is empty!");
    }
}


void Dialog::on_lockKeysButton_clicked(){
    QCA::ConvertResult privateResult, publicResult;
    privateKey = QCA::PrivateKey::fromPEM(ui->privateKeyText->toPlainText(), QCA::SecureArray(), &privateResult);
    publicKey = QCA::PublicKey::fromPEM(ui->publicKeyText->toPlainText(), &publicResult);
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

