#include "newwindow.h"
#include "ui_reference.h"

NewWindow::NewWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NewWindow)
{
    ui->setupUi(this);

    QPixmap pix_Key(":images/images/Keyboard.png");
    ui->label_K->setScaledContents(true);
    ui->label_K->setPixmap(pix_Key);

    QPixmap pix_Ref(":images/images/Reference.png");
    ui->label_J->setScaledContents(true);
    ui->label_J->setPixmap(pix_Ref);
}

NewWindow::~NewWindow()
{
    delete ui;
}
