//#define REQUEST_NORMAL_CODE             0xFF
#define REQUEST_NORMAL_CODE             0xA5

#define REQUEST_NORMAL_LENGTH           27

#define REQUEST_NORMAL_TYPE             0
#define REQUEST_NORMAL_FLAGS            1
#define REQUEST_NORMAL_MARCH            2
#define REQUEST_NORMAL_LAG              4
#define REQUEST_NORMAL_DEPTH            6
#define REQUEST_NORMAL_ROLL             8
#define REQUEST_NORMAL_PITCH            10
#define REQUEST_NORMAL_YAW              12

#define REQUEST_NORMAL_LIGHT            14
#define REQUEST_NORMAL_GRAB             15
#define REQUEST_NORMAL_TILT             16
#define REQUEST_NORMAL_ROTATE           17
#define REQUEST_NORMAL_BT               18
#define REQUEST_NORMAL_BOTTOM_LIGHT     19

#define REQUEST_NORMAL_STABILIZE_DEPTH  20//ON
#define REQUEST_NORMAL_STABILIZE_ROLL   21//ON
#define REQUEST_NORMAL_STABILIZE_PITCH  22//ON
#define REQUEST_NORMAL_STABILIZE_YAW    23
#define REQUEST_NORMAL_RESET_IMU        24

#define REQUEST_NORMAL_CHECKSUM         25



// Важно оставить в рабочем состоянии!
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

// В разработке
#define REQUEST_CONFIG_CODE             0x55

#define REQUEST_CONFIG_LENGTH           195

#define REQUEST_CONFIG_TYPE             0

#define REQUEST_CONFIG_K1_DEPTH         1
#define REQUEST_CONFIG_K2_DEPTH         5
#define REQUEST_CONFIG_K3_DEPTH         9
#define REQUEST_CONFIG_K4_DEPTH         13
#define REQUEST_CONFIG_IBORDERS_DEPTH   17
#define REQUEST_CONFIG_PGAIN_DEPTH      21
#define REQUEST_CONFIG_IGAIN_DEPTH      25

#define REQUEST_CONFIG_K1_ROLL          29
#define REQUEST_CONFIG_K2_ROLL          33
#define REQUEST_CONFIG_K3_ROLL          37
#define REQUEST_CONFIG_K4_ROLL          41
#define REQUEST_CONFIG_IBORDERS_ROLL    45
#define REQUEST_CONFIG_PGAIN_ROLL       49
#define REQUEST_CONFIG_IGAIN_ROLL       53

#define REQUEST_CONFIG_K1_PITCH         57
#define REQUEST_CONFIG_K2_PITCH         61
#define REQUEST_CONFIG_K3_PITCH         65
#define REQUEST_CONFIG_K4_PITCH         69
#define REQUEST_CONFIG_IBORDERS_PITCH   73
#define REQUEST_CONFIG_PGAIN_PITCH      77
#define REQUEST_CONFIG_IGAIN_PITCH      81

#define REQUEST_CONFIG_K1_YAW           85
#define REQUEST_CONFIG_K2_YAW           89
#define REQUEST_CONFIG_K3_YAW           93
#define REQUEST_CONFIG_K4_YAW           97
#define REQUEST_CONFIG_IBORDERS_YAW     101
#define REQUEST_CONFIG_PGAIN_YAW        105
#define REQUEST_CONFIG_IGAIN_YAW        109


#define REQUEST_CONFIG_POSITION_HLB     113
#define REQUEST_CONFIG_POSITION_HLF     114
#define REQUEST_CONFIG_POSITION_HRB     115
#define REQUEST_CONFIG_POSITION_HRF     116
#define REQUEST_CONFIG_POSITION_VB      117
#define REQUEST_CONFIG_POSITION_VF      118
#define REQUEST_CONFIG_POSITION_VL      119
#define REQUEST_CONFIG_POSITION_VR      120

#define REQUEST_CONFIG_INVERSE_HLB      121
#define REQUEST_CONFIG_INVERSE_HLF      122
#define REQUEST_CONFIG_INVERSE_HRB      123
#define REQUEST_CONFIG_INVERSE_HRF      124
#define REQUEST_CONFIG_INVERSE_VB       125
#define REQUEST_CONFIG_INVERSE_VF       126
#define REQUEST_CONFIG_INVERSE_VL       127
#define REQUEST_CONFIG_INVERSE_VR       128

#define REQUEST_CONFIG_K_FORWARD_HLB    129
#define REQUEST_CONFIG_K_FORWARD_HLF    133
#define REQUEST_CONFIG_K_FORWARD_HRB    137
#define REQUEST_CONFIG_K_FORWARD_HRF    141
#define REQUEST_CONFIG_K_FORWARD_VB     145
#define REQUEST_CONFIG_K_FORWARD_VF     149
#define REQUEST_CONFIG_K_FORWARD_VL     153
#define REQUEST_CONFIG_K_FORWARD_VR     157

#define REQUEST_CONFIG_K_BACKWARD_HLB   161
#define REQUEST_CONFIG_K_BACKWARD_HLF   165
#define REQUEST_CONFIG_K_BACKWARD_HRB   169
#define REQUEST_CONFIG_K_BACKWARD_HRF   173
#define REQUEST_CONFIG_K_BACKWARD_VB    177
#define REQUEST_CONFIG_K_BACKWARD_VF    181
#define REQUEST_CONFIG_K_BACKWARD_VL    185
#define REQUEST_CONFIG_K_BACKWARD_VR    189

#define REQUEST_CONFIG_CHECKSUM         193




#define RESPONSE_LENGTH                 63

#define RESPONSE_ROLL                   0
#define RESPONSE_PITCH                  2
#define RESPONSE_YAW                    4

#define RESPONSE_ROLL_SPEED             6
#define RESPONSE_PITCH_SPEED            8
#define RESPONSE_YAW_SPEED              10

#define RESPONSE_PRESSURE               12

#define RESPONSE_AGAR                   14

#define RESPONSE_VMA_CURRENT_HLB        22
#define RESPONSE_VMA_CURRENT_HLF        24
#define RESPONSE_VMA_CURRENT_HRB        26
#define RESPONSE_VMA_CURRENT_HRF        28
#define RESPONSE_VMA_CURRENT_VB         30
#define RESPONSE_VMA_CURRENT_VF         32
#define RESPONSE_VMA_CURRENT_VL         34
#define RESPONSE_VMA_CURRENT_VR         36


#define RESPONSE_VMA_VELOCITY_HLB       38
#define RESPONSE_VMA_VELOCITY_HLF       39
#define RESPONSE_VMA_VELOCITY_HRB       40
#define RESPONSE_VMA_VELOCITY_HRF       41
#define RESPONSE_VMA_VELOCITY_VB        42
#define RESPONSE_VMA_VELOCITY_VF        43
#define RESPONSE_VMA_VELOCITY_VL        44
#define RESPONSE_VMA_VELOCITY_VR        45

#define RESPONSE_DEV_CURRENT_1          46
#define RESPONSE_DEV_CURRENT_2          48
#define RESPONSE_DEV_CURRENT_3          50
#define RESPONSE_DEV_CURRENT_4          52
#define RESPONSE_DEV_CURRENT_5          54
#define RESPONSE_DEV_CURRENT_6          56

#define RESPONSE_VMA_ERRORS             58

#define RESPONSE_DEV_ERRORS             60

#define RESPONSE_CHECKSUM               61
