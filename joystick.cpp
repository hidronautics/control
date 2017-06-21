#include "joystick.h"


Joystick::Joystick(QObject *parent) : QObject(parent)
{

}

int16_t joystickFloatToInt16_t(float n) { // SFML gives numbers in [-100;100] converting to [-32767; 32767]
    return (int16_t)(n*327.67);
}

int8_t joystickFloatToInt8_t(float n) { // SFML gives numbers in [-100;100] converting to [-127; 127]
    return (int8_t)(n*1.27);
}

bool Joystick::init(int joystick_id) { // TODO идентификация джойстика
    sf::Joystick::update();
    if (sf::Joystick::isConnected(joystick_id)) {
        this->joystick_id = joystick_id;
        return true;
    } else {
        return false;
    }
}


void Joystick::update() {
    if (isControlJoystick) {
        updateJoystick();
    } else if (isControlKeyboard) {
        updateKeyboard();
    } else if (isControlXbox) {
        updateXbox();
    }

    std::cout << "pitch=" << pitch << " roll=" << roll << "   yaw="      << yaw   << std::endl;
    std::cout << "march=" << march << "  lag=" << lag  << " depth="      << depth << std::endl;
    std::cout << "light=" << light << " grab=" << grab << "sensitivity=" << sensitivity << std::endl;
    //std::cout << "tilt =" << tilt  << " grab2_squeeze=" << grab2_squeeze << "grab2_rotate" << grab2_rotate << std::endl;
}

void Joystick::printButtons() {
    for (int i = 0; i<=sf::Joystick::ButtonCount; i++) {
        if (sf::Joystick::isButtonPressed(joystick_id, i))
            std::cout << "BUTTON " << i << std::endl;
    }
}

void Joystick::printAxises() {
    std::cout << "Axis X (" << sf::Joystick::X << "): " << sf::Joystick::getAxisPosition(joystick_id, sf::Joystick::X) << std::endl;
    std::cout << "Axis Y (" << sf::Joystick::Y << "): " << sf::Joystick::getAxisPosition(joystick_id, sf::Joystick::Y) << std::endl;
    std::cout << "Axis Z (" << sf::Joystick::Z << "): " << sf::Joystick::getAxisPosition(joystick_id, sf::Joystick::Z) << std::endl;
    std::cout << "Axis R (" << sf::Joystick::R << "): " << sf::Joystick::getAxisPosition(joystick_id, sf::Joystick::R) << std::endl;

    std::cout << "Axis U (" << sf::Joystick::U << "): " << sf::Joystick::getAxisPosition(joystick_id, sf::Joystick::U) << std::endl;
    std::cout << "Axis V (" << sf::Joystick::V << "): " << sf::Joystick::getAxisPosition(joystick_id, sf::Joystick::V) << std::endl;

    std::cout << "Axis PovX (" << sf::Joystick::PovX << "): " << sf::Joystick::getAxisPosition(joystick_id, sf::Joystick::PovX) << std::endl;
    std::cout << "Axis PovY (" << sf::Joystick::PovY << "): " << sf::Joystick::getAxisPosition(joystick_id, sf::Joystick::PovY) << std::endl;

    std::cout << "Axis J1 (" << sf::Joystick::J1 << "): " << sf::Joystick::getAxisPosition(joystick_id, sf::Joystick::J1) << std::endl;
    std::cout << "Axis J2 (" << sf::Joystick::J2 << "): " << sf::Joystick::getAxisPosition(joystick_id, sf::Joystick::J2) << std::endl;
}

void Joystick::updateJoystick() {
    sf::Joystick::update();
    //printAxises();
    //printButtons();
    this->pitch = +
                 joystickFloatToInt16_t(sf::Joystick::getAxisPosition(joystick_id, sf::Joystick::PovY));
    this->roll  = +
                 joystickFloatToInt16_t(sf::Joystick::getAxisPosition(joystick_id, sf::Joystick::PovX));
    this->yaw   = +
                 joystickFloatToInt16_t(sf::Joystick::getAxisPosition(joystick_id, sf::Joystick::R));
    this->march = -
                 joystickFloatToInt16_t(sf::Joystick::getAxisPosition(joystick_id, sf::Joystick::Y));
    this->lag   = +
                 joystickFloatToInt16_t(sf::Joystick::getAxisPosition(joystick_id, sf::Joystick::X));
    this->depth = +
                 joystickFloatToInt16_t((-sf::Joystick::getAxisPosition(joystick_id, sf::Joystick::Z)+100.0)/2);
    if(inverseDepth)depth=-depth;
    //this->light = +
    //             joystickFloatToInt8_t((sf::Joystick::getAxisPosition(joystick_id, sf::Joystick::U) + 100.0)/2);

    this->sensitivity = +
            (sf::Joystick::getAxisPosition(joystick_id, sf::Joystick::U) + 100.)/200.; //Axis Position от -100 до +100

    std::cout << sensitivity << std::endl;

    this->pitch *= sensitivity;
    this->roll *= sensitivity;
    this->yaw *= sensitivity;
    this->march *= sensitivity;
    this->lag *= sensitivity;
    this->depth *= sensitivity;



    this->grab_rotate = +
      (sf::Joystick::getAxisPosition(joystick_id, sf::Joystick::V));

    this->btn_depth_inv   = sf::Joystick::isButtonPressed(joystick_id,  SFML_BTN_DEPTH_INV);
    this->btn_grab        = sf::Joystick::isButtonPressed(joystick_id,  SFML_BTN_GRAB);
    this->btn_grab_strong = sf::Joystick::isButtonPressed(joystick_id,  SFML_BTN_GRAB_STRONG);
    this->btn_ungrab      = sf::Joystick::isButtonPressed(joystick_id,  SFML_BTN_UNGRAB);
    this->btn_agar_forward= sf::Joystick::isButtonPressed(joystick_id,  SFML_BTN_AGAR_FORWARD);
    this->btn_agar_reverse= sf::Joystick::isButtonPressed(joystick_id,  SFML_BTN_AGAR_REVERSE);
    this->btn_bottom_light= sf::Joystick::isButtonPressed(joystick_id,  SFML_BTN_BOTTOM_LIGHT);

    this->btn_tilt_up     = sf::Joystick::isButtonPressed(joystick_id,  SFML_BTN_TILT_UP);
    this->btn_tilt_down     = sf::Joystick::isButtonPressed(joystick_id,  SFML_BTN_TILT_DOWN);

    this->btn_stabilize_depth = sf::Joystick::isButtonPressed(joystick_id,  SFML_BTN_STABILIZE_DEPTH);
    this->btn_stabilize_roll  = sf::Joystick::isButtonPressed(joystick_id,  SFML_BTN_STABILIZE_ROLL);
    this->btn_stabilize_pitch = sf::Joystick::isButtonPressed(joystick_id,  SFML_BTN_STABILIZE_PITCH);

    if (this->btn_stabilize_depth != false) {
        if (btn_stabilize_depth_delay <= 0) {
            stabilize_depth = !stabilize_depth;
            btn_stabilize_depth_delay = BTN_DELAY;
        }
    }
    if (btn_stabilize_depth_delay > 0) btn_stabilize_depth_delay--;

    if (this->btn_stabilize_roll != false) {
        if (btn_stabilize_roll_delay <= 0) {
            stabilize_roll = !stabilize_roll;
            btn_stabilize_roll_delay = BTN_DELAY;
        }
    }
    if (btn_stabilize_roll_delay > 0) btn_stabilize_roll_delay--;

    if (this->btn_stabilize_pitch != false) {
        if (btn_stabilize_pitch_delay <= 0) {
            stabilize_pitch = !stabilize_pitch;
            btn_stabilize_pitch_delay = BTN_DELAY;
        }
    }
    if (btn_stabilize_pitch_delay > 0) btn_stabilize_pitch_delay--;


    if (btn_depth_inv)
        depth = -depth;

    if      (btn_ungrab)
        grab = -127;
    else if (btn_grab_strong)
        grab = 127;
    else if (btn_grab)
        grab = 127;
    else
        grab = 0;

    if      (btn_tilt_up)
        tilt = 60;
    else if (btn_tilt_down)
        tilt = -60;
    else
        tilt = 0;

    if (btn_agar_forward)
        agar = 127;
    else if (btn_agar_reverse)
        agar = -127;
    else
        agar = 0;

    if (btn_bottom_light)
        bottom_light = -126;
    else
        bottom_light = 0;

}

void Joystick::updateKeyboard() {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Equal)) {
        sensitivity += 0.05;
        std::cout << "Sensitivity increased. New value: " << sensitivity << std::endl;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Dash)) {
        sensitivity -= 0.05;
        std::cout << "Sensitivity decreased. New value: " << sensitivity << std::endl;
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        march = 32767;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        march = -32767;
    else
        march = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        lag = 32767;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        lag = -32767;
    else
        lag = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        yaw = 32767;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
        yaw = -32767;
    else
        yaw = 0;


    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        pitch = 32767;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        pitch = -32767;
    else
        pitch = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        roll = 32767;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        roll = -32767;
    else
        roll = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        depth = 32767;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::LControl))
        depth = -32767;
    else
        depth = 0;



    // Light
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::F))
        light += 10;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::C))
        light -= 10;

    // Grab
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::G))
        grab = 100;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::V))
        grab = -100;
    else
        grab = 0;


    // Tilt
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageDown))
        tilt = -50;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::PageUp))
        tilt = 50;
    else
        tilt = 0;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::B))
        bt = 127;
    else
        bt = 0;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::N))
        bottom_light = 127;
    else
        bottom_light = 0;

}

void Joystick::updateXbox() {
    sf::Joystick::update();

    this->pitch = +
                joystickFloatToInt16_t(sf::Joystick::getAxisPosition(joystick_id, sf::Joystick::R));
    this->roll  = +
                 joystickFloatToInt16_t(sf::Joystick::getAxisPosition(joystick_id, sf::Joystick::U));
    //this->yaw   = +
    //             joystickFloatToInt16_t(sf::Joystick::getAxisPosition(joystick_id, sf::Joystick::R));

    this->march = +
                 -joystickFloatToInt16_t(sf::Joystick::getAxisPosition(joystick_id, sf::Joystick::Y));
    this->lag  = +
                 joystickFloatToInt16_t(sf::Joystick::getAxisPosition(joystick_id, sf::Joystick::X));
    this->depth = +
                 joystickFloatToInt16_t(sf::Joystick::getAxisPosition(joystick_id, sf::Joystick::Z));

    //this->light = +
    //            joystickFloatToInt8_t((sf::Joystick::getAxisPosition(joystick_id, sf::Joystick::U) + 100.0)/2);

    if (abs(this->pitch) < XBOX_DEAD_ZONE) this->pitch = 0;
    if (abs(this->roll)  < XBOX_DEAD_ZONE) this->roll = 0;
    if (abs(this->march) < XBOX_DEAD_ZONE) this->march = 0;
    if (abs(this->lag)   < XBOX_DEAD_ZONE) this->lag = 0;
    if (abs(this->depth) < XBOX_DEAD_ZONE) this->depth = 0;


    this->sensitivity = +
            (sf::Joystick::getAxisPosition(joystick_id, sf::Joystick::V) + 100)/2;
    /*
    this->pitch *= sensitivity;
    this->roll *= sensitivity;
    this->yaw *= sensitivity;
    this->march *= sensitivity;
    this->lag *= sensitivity;
    this->depth *= sensitivity;*/



    this->grab_rotate = +
      (sf::Joystick::getAxisPosition(joystick_id, sf::Joystick::V));

    this->btn_depth_inv   = sf::Joystick::isButtonPressed(joystick_id,  SFML_BTN_DEPTH_INV);
    this->btn_grab        = sf::Joystick::isButtonPressed(joystick_id,  SFML_BTN_GRAB);
    this->btn_grab_strong = sf::Joystick::isButtonPressed(joystick_id,  SFML_BTN_GRAB_STRONG);
    this->btn_ungrab      = sf::Joystick::isButtonPressed(joystick_id,  SFML_BTN_UNGRAB);
    this->btn_tilt_up     = sf::Joystick::isButtonPressed(joystick_id,  SFML_BTN_TILT_UP);
    this->btn_tilt_down     = sf::Joystick::isButtonPressed(joystick_id,  SFML_BTN_TILT_DOWN);

    if (btn_depth_inv)
        depth = -depth;

    if      (btn_ungrab)
        grab = -127;
    else if (btn_grab_strong)
        grab = 127;
    else if (btn_grab)
        grab = 127;
    else
        grab = 0;

    if      (btn_tilt_up)
        tilt = 60;
    else if (btn_tilt_down)
        tilt = -60;
    else
        tilt = 0;
}
