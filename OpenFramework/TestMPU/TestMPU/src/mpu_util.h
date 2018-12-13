#pragma once

#ifndef __mpu_util_h
#define __mpu_util_h

typedef struct
{
	short gyro[3], accel[3], sensors;
	long quat[4];
	short it_freq;
	short ble_freq;
} dmp_data;

Quaternion quat_from_dmpquat(float q0, float q1, float q2, float q3);
Quaternion quat_from_dmpquat(dmp_data* pdata);
Quaternion quat_from_dmpquat(float* pquatdmp);

#endif // !__mpu_util_h
