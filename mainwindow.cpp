#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QPixmap pix_back(":/qfi/images/background.png");
    ui->label_pic->setScaledContents(true);
    ui->label_pic->setPixmap(pix_back);

    //Motors
    QPixmap pix_mot(":/qfi/images/Motors.png");
    ui->label_motors->setScaledContents(true);
    ui->label_motors->setPixmap(pix_mot);


    //Indicator motor
    //motor1
    if (5==5)
    {
        QPixmap pix_green(":/qfi/images/LEDgreen");
        ui->label_indicator1->setScaledContents(true);
        ui->label_indicator1->setPixmap(pix_green);
        ui->textBrowser->setText("Current");
        ui->textBrowser->append("Speed");
        ui->textBrowser->append("Something");
    }
    else //error
    {
       QPixmap pix_red(":/qfi/images/LEDred");
       ui->label_indicator1->setScaledContents(true);
       ui->label_indicator1->setPixmap(pix_red);
       ui->textBrowser->setText("Error");
    }

    //motor2
    if (5>5)
    {
        QPixmap pix_green(":/qfi/images/LEDgreen");
        ui->label_indicator1_2->setScaledContents(true);
        ui->label_indicator1_2->setPixmap(pix_green);
        ui->textBrowser_2->setText("Current");
        ui->textBrowser_2->append("Speed");
        ui->textBrowser_2->append("Something");
    }
    else //error
    {
       QPixmap pix_red(":/qfi/images/LEDred");
       ui->label_indicator1_2->setScaledContents(true);
       ui->label_indicator1_2->setPixmap(pix_red);
       ui->textBrowser_2->setText("Error");
    }

    //motor3
    if (5>5)
    {
        QPixmap pix_green(":/qfi/images/LEDgreen");
        ui->label_indicator1_3->setScaledContents(true);
        ui->label_indicator1_3->setPixmap(pix_green);
        ui->textBrowser_3->setText("Current");
        ui->textBrowser_3->append("Speed");
        ui->textBrowser_3->append("Something");
    }
    else //error
    {
       QPixmap pix_red(":/qfi/images/LEDred");
       ui->label_indicator1_3->setScaledContents(true);
       ui->label_indicator1_3->setPixmap(pix_red);
       ui->textBrowser_3->setText("Error");
    }

    //motor4
    if (5==5)
    {
        QPixmap pix_green(":/qfi/images/LEDgreen");
        ui->label_indicator1_4->setScaledContents(true);
        ui->label_indicator1_4->setPixmap(pix_green);
        ui->textBrowser_4->setText("Current");
        ui->textBrowser_4->append("Speed");
        ui->textBrowser_4->append("Something");
    }
    else //error
    {
       QPixmap pix_red(":/qfi/images/LEDred");
       ui->label_indicator1_4->setScaledContents(true);
       ui->label_indicator1_4->setPixmap(pix_red);
       ui->textBrowser_4->setText("Error");
    }

    //motor5
    if (5==5)
    {
        QPixmap pix_green(":/qfi/images/LEDgreen");
        ui->label_indicator1_5->setScaledContents(true);
        ui->label_indicator1_5->setPixmap(pix_green);
        ui->textBrowser_5->setText("Current");
        ui->textBrowser_5->append("Speed");
        ui->textBrowser_5->append("Something");
    }
    else //error
    {
       QPixmap pix_red(":/qfi/images/LEDred");
       ui->label_indicator1_5->setScaledContents(true);
       ui->label_indicator1_5->setPixmap(pix_red);
       ui->textBrowser_5->setText("Error");
    }

    //motor6
    if (5==5)
    {
        QPixmap pix_green(":/qfi/images/LEDgreen");
        ui->label_indicator1_6->setScaledContents(true);
        ui->label_indicator1_6->setPixmap(pix_green);
        ui->textBrowser_6->setText("Current");
        ui->textBrowser_6->append("Speed");
        ui->textBrowser_6->append("Something");
    }
    else //error
    {
       QPixmap pix_red(":/qfi/images/LEDred");
       ui->label_indicator1_6->setScaledContents(true);
       ui->label_indicator1_6->setPixmap(pix_red);
       ui->textBrowser_6->setText("Error");
    }

    //motor7
    if (5>5)
    {
        QPixmap pix_green(":/qfi/images/LEDgreen");
        ui->label_indicator1_7->setScaledContents(true);
        ui->label_indicator1_7->setPixmap(pix_green);
        ui->textBrowser_7->setText("Current");
        ui->textBrowser_7->append("Speed");
        ui->textBrowser_7->append("Something");
    }
    else //error
    {
       QPixmap pix_red(":/qfi/images/LEDred");
       ui->label_indicator1_7->setScaledContents(true);
       ui->label_indicator1_7->setPixmap(pix_red);
       ui->textBrowser_7->setText("Error");
    }

    //motor8
    if (5==5)
    {
        QPixmap pix_green(":/qfi/images/LEDgreen");
        ui->label_indicator1_8->setScaledContents(true);
        ui->label_indicator1_8->setPixmap(pix_green);
        ui->textBrowser_8->setText("Current");
        ui->textBrowser_8->append("Speed");
        ui->textBrowser_8->append("Something");
    }
    else //error
    {
       QPixmap pix_red(":/qfi/images/LEDred");
       ui->label_indicator1_8->setScaledContents(true);
       ui->label_indicator1_8->setPixmap(pix_red);
       ui->textBrowser_8->setText("Error");
    }


    //Indicator bluetooth
    QPixmap pix_bt(":/qfi/images/BT.png");
    ui->label->setScaledContents(true);
    ui->label->setPixmap(pix_bt);

    if (5==5) //No bluetooth signal
    {
        QPixmap pix_redB(":/qfi/images/LED2");
        ui->label_indicator2->setScaledContents(true);
        ui->label_indicator2->setPixmap(pix_redB);
    }
    else
    {
       QPixmap pix_greenB(":/qfi/images/LED3");
       ui->label_indicator2->setScaledContents(true);
       ui->label_indicator2->setPixmap(pix_greenB);
    }


    //Added
    connect(ui->pushButton_Bluetooth, SIGNAL(click()), this, SLOT(openNewWindow()));
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openNewWindow()
{
    bluetooth_menu = new NewWindow();

    bluetooth_menu->show();

}


void MainWindow::on_pushButton_Bluetooth_clicked()
{
  openNewWindow();
}

void MainWindow::on_pushButton_Connect_released()
{
    emit tryConnect();
}


void MainWindow::on_pushButton_Disconnect_released()
{
    emit disconnect();
}


void MainWindow::on_pushButton_released()
{
    emit connect_fake();
}


void MainWindow::on_radioButton_Joystick_released()
{
    joystick->isControlJoystick = true;
    joystick->isControlKeyboard = false;
    joystick->isControlXbox = false;

    ui->radioButton_Joystick->setChecked(true);
    ui->radioButton_Keyboard->setChecked(false);
    ui->radioButton_XBox->setChecked(false);
}

void MainWindow::on_radioButton_Keyboard_released()
{
    joystick->isControlJoystick = false;
    joystick->isControlKeyboard = true;
    joystick->isControlXbox = false;

    ui->radioButton_Joystick->setChecked(false);
    ui->radioButton_Keyboard->setChecked(true);
    ui->radioButton_XBox->setChecked(false);
}

void MainWindow::on_radioButton_XBox_released()
{
    joystick->isControlJoystick = false;
    joystick->isControlKeyboard = false;
    joystick->isControlXbox = true;

    ui->radioButton_Joystick->setChecked(false);
    ui->radioButton_Keyboard->setChecked(false);
    ui->radioButton_XBox->setChecked(true);
}
