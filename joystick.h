#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <QObject>
#include <iostream>
#include <SFML/Window.hpp>

class Joystick : public QObject
{
    Q_OBJECT
public:
    int16_t pitch       = 0;     //[-32767,32767]
    int16_t roll        = 0;     //[-32767,32767]
    int16_t yaw         = 0;     //[-32767,32767]

    int16_t march       = 0;     //[-32767,32767]
    int16_t lag         = 0;     //[-32767,32767]
    int16_t depth       = 0;     //[-32767,32767]

    int8_t light       = 0;     //[0,127]
    int8_t grab        = 0;     //[-127,127]
    int8_t tilt        = 0;     //[-127,127]

    int8_t bt           =0; //BOOL

    int8_t bottom_light  = 0;   //[-127, 127]

    float sensitivity = 0.0;       //[0.0,2.0]


    bool btn_depth_inv   = false;  // 6
    bool btn_grab        = false;  // 0
    bool btn_grab_strong = false;  // 14
    bool btn_ungrab      = false;  // 5

    bool btn_bt = false; //22 ПОМЕНЯТЬ ПОЗЖЕ

    bool isControlJoystick = true;
    bool isControlKeyboard = false;
    bool isControlXbox     = false;

    explicit Joystick(QObject *parent = 0);
    bool init(int joystick_id);
    void update();    
    void printButtons();
    void printAxises();

private:
    const int SFML_BTN_DEPTH_INV   = 6;
    const int SFML_BTN_GRAB        = 0;
    const int SFML_BTN_GRAB_STRONG = 14;
    const int SFML_BTN_UNGRAB      = 5;

    const int SFML_BTN_BT = 22;

    int joystick_id = 0;

    void updateJoystick();
    void updateKeyboard();
    void updateXbox();


signals:

public slots:

};

#endif // JOYSTICK_H
