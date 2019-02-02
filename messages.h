#ifndef MESSAGES_H
#define MESSAGES_H

#include "stdint.h"
#include <QDataStream>

/* Shore send requests and STM send responses */

/* Request values and data structures */

/* Normal mode */
#define REQUEST_NORMAL_CODE             0xA5
#define REQUEST_NORMAL_LENGTH           26

struct Request_s
{
    uint8_t type;
    uint8_t flags;
    int16_t march;
    int16_t lag;
    int16_t depth;
    int16_t roll;
    int16_t pitch;
    int16_t yaw;
    int8_t light;
    int8_t grab;
    int8_t tilt;
    int8_t grab_rotate;
    int8_t dev1;
    int8_t dev2;
    uint8_t dev_flags;
    uint8_t stabilize_flags;
    uint8_t cameras;
    uint8_t pc_reset;
    uint16_t checksum;

    friend QDataStream& operator<<(QDataStream &ds, const Request_s &req)
    {
        ds.setByteOrder(QDataStream::LittleEndian);
        ds << req.type;
        ds << req.flags;
        ds << req.march;
        ds << req.lag;
        ds << req.depth;
        ds << req.roll;
        ds << req.pitch;
        ds << req.yaw;
        ds << req.light;
        ds << req.grab;
        ds << req.tilt;
        ds << req.grab_rotate;
        ds << req.dev1;
        ds << req.dev2;
        ds << req.dev_flags;
        ds << req.stabilize_flags;
        ds << req.cameras;
        ds << req.pc_reset;
        return ds;
    }

    friend QDataStream& operator>>(QDataStream &ds, Request_s &req)
    {
        ds.setByteOrder(QDataStream::LittleEndian);
        ds.setFloatingPointPrecision(QDataStream::SinglePrecision);
        ds >> req.type;
        ds >> req.flags;
        ds >> req.march;
        ds >> req.lag;
        ds >> req.depth;
        ds >> req.roll;
        ds >> req.pitch;
        ds >> req.yaw;
        ds >> req.light;
        ds >> req.grab;
        ds >> req.tilt;
        ds >> req.grab_rotate;
        ds >> req.dev1;
        ds >> req.dev2;
        ds >> req.dev_flags;
        ds >> req.stabilize_flags;
        ds >> req.cameras;
        ds >> req.pc_reset;
        ds >> req.checksum;
        return ds;
    }
};

struct ConfigRequest_s
{
    uint8_t type;
    uint8_t contour;

    int16_t march;
    int16_t lag;
    int16_t depth;
    int16_t roll;
    int16_t pitch;
    int16_t yaw;

    float pJoyUnitCast;
    float pSpeedDyn;
    float pErrGain;

    float posFilterT;
    float posFilterK;
    float speedFilterT;
    float speedFilterK;

    float pid_pGain;
    float pid_iGain;
    float pid_iMax;
    float pid_iMin;

    float pThrustersCast;
    float pThrustersMin;
    float pThrustersMax;

    friend QDataStream& operator<<(QDataStream &ds, const ConfigRequest_s &req)
    {
        ds.setByteOrder(QDataStream::LittleEndian);
        ds.setFloatingPointPrecision(QDataStream::SinglePrecision);
        ds << req.type;
        ds << req.contour;

        ds << req.march;
        ds << req.lag;
        ds << req.depth;
        ds << req.roll;
        ds << req.pitch;
        ds << req.yaw;

        ds << req.pJoyUnitCast;
        ds << req.pSpeedDyn;
        ds << req.pErrGain;

        ds << req.posFilterT;
        ds << req.posFilterK;
        ds << req.speedFilterT;
        ds << req.speedFilterK;

        ds << req.pid_pGain;
        ds << req.pid_iGain;
        ds << req.pid_iMax;
        ds << req.pid_iMin;

        ds << req.pThrustersCast;
        ds << req.pThrustersMin;
        ds << req.pThrustersMax;

        return ds;
    }
};

/* Direct mode */
#define REQUEST_DIRECT_CODE             0xAA
#define REQUEST_DIRECT_LENGTH           11

#define REQUEST_DIRECT_TYPE             0
#define REQUEST_DIRECT_1                1
#define REQUEST_DIRECT_2                2
#define REQUEST_DIRECT_3                3
#define REQUEST_DIRECT_4                4
#define REQUEST_DIRECT_5                5
#define REQUEST_DIRECT_6                6
#define REQUEST_DIRECT_7                7
#define REQUEST_DIRECT_8                8
#define REQUEST_DIRECT_CHECKSUM         9

/* Config mode */
#define REQUEST_CONFIG_CODE             0x55
#define REQUEST_CONFIG_LENGTH           72

/* Response values and data structures */
#define RESPONSE_LENGTH                 70
#define RESPONSE_CONFIG_LENGTH          95

struct Response_s
{
    uint8_t code;

    float roll;
    float pitch;
    float yaw;

    float rollSpeed;
    float pitchSpeed;
    float yawSpeed;

    float pressure;
    float in_pressure;

    uint8_t dev_state;
    int16_t leak_data;

    uint16_t vma_current_hlb;
    uint16_t vma_current_hlf;
    uint16_t vma_current_hrb;
    uint16_t vma_current_hrf;
    uint16_t vma_current_vb;
    uint16_t vma_current_vf;
    uint16_t vma_current_vl;
    uint16_t vma_current_vr;

    uint16_t dev_current_light;
    uint16_t dev_current_tilt;
    uint16_t dev_current_grab;
    uint16_t dev_current_grab_rotate;
    uint16_t dev_current_dev1;
    uint16_t dev_current_dev2;

    uint16_t vma_errors;
    uint16_t dev_errors;
    uint8_t pc_errors;

    uint16_t checksum;

    friend QDataStream& operator<<(QDataStream &ds, const Response_s &res)
    {
        ds.setByteOrder(QDataStream::LittleEndian);
        ds.setFloatingPointPrecision(QDataStream::SinglePrecision);
        ds << res.roll;
        ds << res.pitch;
        ds << res.yaw;

        ds << res.rollSpeed;
        ds << res.pitchSpeed;
        ds << res.yawSpeed;

        ds << res.pressure;
        ds << res.in_pressure;

        ds << res.dev_state;
        ds << res.leak_data;

        ds << res.vma_current_hlb;
        ds << res.vma_current_hlf;
        ds << res.vma_current_hrb;
        ds << res.vma_current_hrf;
        ds << res.vma_current_vb;
        ds << res.vma_current_vf;
        ds << res.vma_current_vl;
        ds << res.vma_current_vr;

        ds << res.dev_current_light;
        ds << res.dev_current_tilt;
        ds << res.dev_current_grab;
        ds << res.dev_current_grab_rotate;
        ds << res.dev_current_dev1;
        ds << res.dev_current_dev2;

        ds << res.vma_errors;
        ds << res.dev_errors;
        ds << res.pc_errors;
        return ds;
    }

    friend QDataStream& operator>>(QDataStream &ds, Response_s &res)
    {
        ds.setByteOrder(QDataStream::LittleEndian);
        ds.setFloatingPointPrecision(QDataStream::SinglePrecision);
        ds >> res.roll;
        ds >> res.pitch;
        ds >> res.yaw;

        ds >> res.rollSpeed;
        ds >> res.pitchSpeed;
        ds >> res.yawSpeed;

        ds >> res.pressure;
        ds >> res.in_pressure;

        ds >> res.dev_state;
        ds >> res.leak_data;

        ds >> res.vma_current_hlb;
        ds >> res.vma_current_hlf;
        ds >> res.vma_current_hrb;
        ds >> res.vma_current_hrf;
        ds >> res.vma_current_vb;
        ds >> res.vma_current_vf;
        ds >> res.vma_current_vl;
        ds >> res.vma_current_vr;

        ds >> res.dev_current_light;
        ds >> res.dev_current_tilt;
        ds >> res.dev_current_grab;
        ds >> res.dev_current_grab_rotate;
        ds >> res.dev_current_dev1;
        ds >> res.dev_current_dev2;

        ds >> res.vma_errors;
        ds >> res.dev_errors;
        ds >> res.pc_errors;

        ds >> res.checksum;
        return ds;
    }
};

struct ConfigResponse_s
{
    uint8_t code;

    float roll;
    float pitch;
    float yaw;

    float rollSpeed;
    float pitchSpeed;
    float yawSpeed;

    float pressure;
    float in_pressure;

    float inputSignal;
    float speedSignal;
    float posSignal;

    float oldSpeed;
    float oldPos;

    float joyUnitCasted;
    float joy_iValue;
    float posError;
    float speedError;
    float dynSummator;
    float pidValue;
    float posErrorAmp;
    float speedFiltered;
    float posFiltered;

    float LastTick;

    uint16_t checksum;

    friend QDataStream& operator>>(QDataStream &ds, ConfigResponse_s &req)
    {
        ds.setByteOrder(QDataStream::LittleEndian);
        ds.setFloatingPointPrecision(QDataStream::SinglePrecision);

        ds >> req.code;

        ds >> req.roll;
        ds >> req.pitch;
        ds >> req.yaw;

        ds >> req.rollSpeed;
        ds >> req.pitchSpeed;
        ds >> req.yawSpeed;

        ds >> req.pressure;
        ds >> req.in_pressure;

        ds >> req.inputSignal;
        ds >> req.speedSignal;
        ds >> req.posSignal;

        ds >> req.oldSpeed;
        ds >> req.oldPos;

        ds >> req.joyUnitCasted;
        ds >> req.joy_iValue;
        ds >> req.posError;
        ds >> req.speedError;
        ds >> req.dynSummator;
        ds >> req.pidValue;
        ds >> req.posErrorAmp;
        ds >> req.speedFiltered;
        ds >> req.posFiltered;

        ds >> req.LastTick;

        ds >> req.checksum;
        return ds;
    }
};

#endif
