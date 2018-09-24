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
#define REQUEST_CONFIG_LENGTH           195

struct ConfigRequest_s
{
    uint8_t type;

    float depth_k1;
    float depth_k2;
    float depth_k3;
    float depth_k4;
    float depth_iborders;
    float depth_pgain;
    float depth_igain;

    float roll_k1;
    float roll_k2;
    float roll_k3;
    float roll_k4;
    float roll_iborders;
    float roll_pgain;
    float roll_igain;

    float pitch_k1;
    float pitch_k2;
    float pitch_k3;
    float pitch_k4;
    float pitch_iborders;
    float pitch_pgain;
    float pitch_igain;

    float yaw_k1;
    float yaw_k2;
    float yaw_k3;
    float yaw_k4;
    float yaw_iborders;
    float yaw_pgain;
    float yaw_igain;

    uint8_t position_hlb;
    uint8_t position_hlf;
    uint8_t position_hrb;
    uint8_t position_hrf;
    uint8_t position_vb;
    uint8_t position_vf;
    uint8_t position_vl;
    uint8_t position_vr;

    uint8_t setting_hlb;
    uint8_t setting_hlf;
    uint8_t setting_hrb;
    uint8_t setting_hrf;
    uint8_t setting_vb;
    uint8_t setting_vf;
    uint8_t setting_vl;
    uint8_t setting_vr;

    uint8_t kforward_hlb;
    uint8_t kforward_hlf;
    uint8_t kforward_hrb;
    uint8_t kforward_hrf;
    uint8_t kforward_vb;
    uint8_t kforward_vf;
    uint8_t kforward_vl;
    uint8_t kforward_vr;

    uint8_t kbackward_hlb;
    uint8_t kbackward_hlf;
    uint8_t kbackward_hrb;
    uint8_t kbackward_hrf;
    uint8_t kbackward_vb;
    uint8_t kbackward_vf;
    uint8_t kbackward_vl;
    uint8_t kbackward_vr;

    uint16_t checksum;

    friend QDataStream& operator<<(QDataStream &ds, const ConfigRequest_s &req)
    {
        ds.setByteOrder(QDataStream::LittleEndian);
        ds << req.type;

        ds << req.depth_k1;
        ds << req.depth_k2;
        ds << req.depth_k3;
        ds << req.depth_k4;
        ds << req.depth_iborders;
        ds << req.depth_pgain;
        ds << req.depth_igain;

        ds << req.roll_k1;
        ds << req.roll_k2;
        ds << req.roll_k3;
        ds << req.roll_k4;
        ds << req.roll_iborders;
        ds << req.roll_pgain;
        ds << req.roll_igain;

        ds << req.pitch_k1;
        ds << req.pitch_k2;
        ds << req.pitch_k3;
        ds << req.pitch_k4;
        ds << req.pitch_iborders;
        ds << req.pitch_pgain;
        ds << req.pitch_igain;

        ds << req.yaw_k1;
        ds << req.yaw_k2;
        ds << req.yaw_k3;
        ds << req.yaw_k4;
        ds << req.yaw_iborders;
        ds << req.yaw_pgain;
        ds << req.yaw_igain;

        ds << req.position_hlb;
        ds << req.position_hlf;
        ds << req.position_hrb;
        ds << req.position_hrf;
        ds << req.position_vb;
        ds << req.position_vf;
        ds << req.position_vl;
        ds << req.position_vr;

        ds << req.setting_hlb;
        ds << req.setting_hlf;
        ds << req.setting_hrb;
        ds << req.setting_hrf;
        ds << req.setting_vb;
        ds << req.setting_vf;
        ds << req.setting_vl;
        ds << req.setting_vr;

        ds << req.kforward_hlb;
        ds << req.kforward_hlf;
        ds << req.kforward_hrb;
        ds << req.kforward_hrf;
        ds << req.kforward_vb;
        ds << req.kforward_vf;
        ds << req.kforward_vl;
        ds << req.kforward_vr;

        ds << req.kbackward_hlb;
        ds << req.kbackward_hlf;
        ds << req.kbackward_hrb;
        ds << req.kbackward_hrf;
        ds << req.kbackward_vb;
        ds << req.kbackward_vf;
        ds << req.kbackward_vl;
        ds << req.kbackward_vr;
        return ds;
    }

    friend QDataStream& operator>>(QDataStream &ds, ConfigRequest_s &req)
    {
        ds.setByteOrder(QDataStream::LittleEndian);
        ds >> req.type;

        ds >> req.depth_k1;
        ds >> req.depth_k2;
        ds >> req.depth_k3;
        ds >> req.depth_k4;
        ds >> req.depth_iborders;
        ds >> req.depth_pgain;
        ds >> req.depth_igain;

        ds >> req.roll_k1;
        ds >> req.roll_k2;
        ds >> req.roll_k3;
        ds >> req.roll_k4;
        ds >> req.roll_iborders;
        ds >> req.roll_pgain;
        ds >> req.roll_igain;

        ds >> req.pitch_k1;
        ds >> req.pitch_k2;
        ds >> req.pitch_k3;
        ds >> req.pitch_k4;
        ds >> req.pitch_iborders;
        ds >> req.pitch_pgain;
        ds >> req.pitch_igain;

        ds >> req.yaw_k1;
        ds >> req.yaw_k2;
        ds >> req.yaw_k3;
        ds >> req.yaw_k4;
        ds >> req.yaw_iborders;
        ds >> req.yaw_pgain;
        ds >> req.yaw_igain;

        ds >> req.position_hlb;
        ds >> req.position_hlf;
        ds >> req.position_hrb;
        ds >> req.position_hrf;
        ds >> req.position_vb;
        ds >> req.position_vf;
        ds >> req.position_vl;
        ds >> req.position_vr;

        ds >> req.setting_hlb;
        ds >> req.setting_hlf;
        ds >> req.setting_hrb;
        ds >> req.setting_hrf;
        ds >> req.setting_vb;
        ds >> req.setting_vf;
        ds >> req.setting_vl;
        ds >> req.setting_vr;

        ds >> req.kforward_hlb;
        ds >> req.kforward_hlf;
        ds >> req.kforward_hrb;
        ds >> req.kforward_hrf;
        ds >> req.kforward_vb;
        ds >> req.kforward_vf;
        ds >> req.kforward_vl;
        ds >> req.kforward_vr;

        ds >> req.kbackward_hlb;
        ds >> req.kbackward_hlf;
        ds >> req.kbackward_hrb;
        ds >> req.kbackward_hrf;
        ds >> req.kbackward_vb;
        ds >> req.kbackward_vf;
        ds >> req.kbackward_vl;
        ds >> req.kbackward_vr;

        ds >> req.checksum;
        return ds;
    }
};

/* Response values and data structures */
#define RESPONSE_LENGTH                 72

struct Response_s
{
    int16_t roll;
    int16_t pitch;
    int16_t yaw;

    int16_t rollSpeed;
    int16_t pitchSpeed;
    int16_t yawSpeed;

    uint16_t pressure;

    uint8_t wf_type;
    uint8_t wf_tickrate;
    uint8_t wf_voltage;
    float wf_x;
    float wf_y;

    uint8_t dev_state;
    int16_t leak_data;
    int16_t in_pressure;

    uint16_t vma_current_hlb;
    uint16_t vma_current_hlf;
    uint16_t vma_current_hrb;
    uint16_t vma_current_hrf;
    uint16_t vma_current_vb;
    uint16_t vma_current_vf;
    uint16_t vma_current_vl;
    uint16_t vma_current_vr;

    int8_t vma_velocity_hlb;
    int8_t vma_velocity_hlf;
    int8_t vma_velocity_hrb;
    int8_t vma_velocity_hrf;
    int8_t vma_velocity_vb;
    int8_t vma_velocity_vf;
    int8_t vma_velocity_vl;
    int8_t vma_velocity_vr;

    uint16_t dev_current_light;
    uint16_t dev_current_tilt;
    uint16_t dev_current_grab;
    uint16_t dev_current_grab_rotate;
    uint16_t dev_current_dev1;
    uint16_t dev_current_dev2;

    uint16_t vma_errors;
    uint8_t dev_errors;
    uint8_t pc_errors;

    uint16_t checksum;

    friend QDataStream& operator<<(QDataStream &ds, const Response_s &res)
    {
        ds.setByteOrder(QDataStream::LittleEndian);
        ds << res.roll;
        ds << res.pitch;
        ds << res.yaw;

        ds << res.rollSpeed;
        ds << res.pitchSpeed;
        ds << res.yawSpeed;

        ds << res.pressure;

        ds << res.wf_type;
        ds << res.wf_tickrate;
        ds << res.wf_voltage;
        ds << res.wf_x;
        ds << res.wf_y;

        ds << res.dev_state;
        ds << res.leak_data;
        ds << res.in_pressure;

        ds << res.vma_current_hlb;
        ds << res.vma_current_hlf;
        ds << res.vma_current_hrb;
        ds << res.vma_current_hrf;
        ds << res.vma_current_vb;
        ds << res.vma_current_vf;
        ds << res.vma_current_vl;
        ds << res.vma_current_vr;

        ds << res.vma_velocity_hlb;
        ds << res.vma_velocity_hlf;
        ds << res.vma_velocity_hrb;
        ds << res.vma_velocity_hrf;
        ds << res.vma_velocity_vb;
        ds << res.vma_velocity_vf;
        ds << res.vma_velocity_vl;
        ds << res.vma_velocity_vr;

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
        ds >> res.roll;
        ds >> res.pitch;
        ds >> res.yaw;

        ds >> res.rollSpeed;
        ds >> res.pitchSpeed;
        ds >> res.yawSpeed;

        ds >> res.pressure;

        ds >> res.wf_type;
        ds >> res.wf_tickrate;
        ds >> res.wf_voltage;
        ds >> res.wf_x;
        ds >> res.wf_y;

        ds >> res.dev_state;
        ds >> res.leak_data;
        ds >> res.in_pressure;

        ds >> res.vma_current_hlb;
        ds >> res.vma_current_hlf;
        ds >> res.vma_current_hrb;
        ds >> res.vma_current_hrf;
        ds >> res.vma_current_vb;
        ds >> res.vma_current_vf;
        ds >> res.vma_current_vl;
        ds >> res.vma_current_vr;

        ds >> res.vma_velocity_hlb;
        ds >> res.vma_velocity_hlf;
        ds >> res.vma_velocity_hrb;
        ds >> res.vma_velocity_hrf;
        ds >> res.vma_velocity_vb;
        ds >> res.vma_velocity_vf;
        ds >> res.vma_velocity_vl;
        ds >> res.vma_velocity_vr;

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
