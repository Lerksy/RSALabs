#include "des.h"
#include "ui_des.h"

DES::DES(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DES)
{
    ui->setupUi(this);
    ui->textInput->setText("bovkoleks");
    ui->passInput->setText("password");
}

DES::~DES()
{
    delete ui;
}

void DES::on_cipherButton_clicked(){
    QCA::SymmetricKey key(ui->passInput->text().toLocal8Bit());
    QCA::Cipher cipher(QStringLiteral("des"), QCA::Cipher::CBC, QCA::Cipher::DefaultPadding, QCA::Encode, key);
    QCA::SecureArray cipherResult = cipher.update(ui->textInput->text().toLocal8Bit());
    if(cipher.ok()){
        QString hexResult = QCA::arrayToHex(cipherResult.toByteArray());

        QMessageBox::information(this, "Cipher result", "Cipher result of "+ui->textInput->text()+" is: "+QString("%1").arg(hexResult.toULongLong(nullptr, 16), 5, 2, QChar('0')));
    }else{
        QMessageBox::warning(this, "Error occured", "Cipher failed!");
    }
}

