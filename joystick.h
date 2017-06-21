#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <QObject>
#include <iostream>
#include <math.h>
#include <SFML/Window.hpp>

class Joystick : public QObject
{
    Q_OBJECT
public:
    /*
     * Для любого устройства ввода должны быть соблюдены ограничения в комментариях
     */

    int16_t pitch       = 0;     //[-32767,32767] (вперед)
    int16_t roll        = 0;     //[-32767,32767] (право)
    int16_t yaw         = 0;     //[-32767,32767] (вправо)

    int16_t march       = 0;     //[-32767,32767] (погружение)
    int16_t lag         = 0;     //[-32767,32767] (против ч/ч)
    int16_t depth       = 0;     //[-32767,32767] (носом вниз)

    int8_t light       = 0;      //[0,127]
    int8_t grab        = 0;      //[-127,127]
    int8_t grab_rotate = 0;      //[-127,127]?
    int8_t tilt        = 0;      //[-127,127]
    int8_t agar        = 0;
    int8_t bottom_light= 0;      //[-127, 127]

    int8_t bt           =0;     //BOOL



    float sensitivity = 0.0;    //[0.0,1.0]


    bool btn_depth_inv   = false;  // 6
    bool btn_grab        = false;  // 0
    bool btn_grab_strong = false;  // 14
    bool btn_ungrab      = false;  // 5
    bool btn_agar_forward= false;  // ???
    bool btn_agar_reverse= false;  // ???
    bool btn_bottom_light= false;  // ???


    bool btn_tilt_up      = false;  // 19
    bool btn_tilt_down    = false;  // 21

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
    const int SFML_BTN_AGAR_FORWARD= 23;
    const int SFML_BTN_AGAR_REVERSE= 25;
    const int SFML_BTN_BOTTOM_LIGHT= 15;

    const int SFML_BTN_TILT_UP = 19;
    const int SFML_BTN_TILT_DOWN = 21;

    const int SFML_BTN_BT = 22;

    // Пока не используется
    sf::Joystick::Axis SFML_AXIS_PITCH = sf::Joystick::PovY;
    sf::Joystick::Axis SFML_AXIS_ROLL  = sf::Joystick::PovX;
    sf::Joystick::Axis SFML_AXIS_YAW   = sf::Joystick::R;
    sf::Joystick::Axis SFML_AXIS_MARCH = sf::Joystick::Y;
    sf::Joystick::Axis SFML_AXIS_LAG   = sf::Joystick::X;
    sf::Joystick::Axis SFML_AXIS_DEPTH = sf::Joystick::Z;


    const int XBOX_DEAD_ZONE = 10000;

    int joystick_id = 0;

    void updateJoystick();
    void updateKeyboard();
    void updateXbox();


signals:

public slots:

};

#endif // JOYSTICK_H
