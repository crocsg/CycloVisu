
#include "ofApp.h"
#include "mathhelper.h"
#include "mpu_util.h"

const  float q30 = 1073741824.0f;


Quaternion quat_from_dmpquat(float q0, float q1, float q2, float q3)
{
	return Quaternion(q0 / q30, q1 / q30, q2 / q30, q3 / q30);
}

Quaternion quat_from_dmpquat(dmp_data* pdata)
{
	return quat_from_dmpquat(pdata->quat[0], pdata->quat[1], pdata->quat[2], pdata->quat[3]);
}

Quaternion quat_from_dmpquat(float* pquatdmp)
{
	return quat_from_dmpquat(pquatdmp[0], pquatdmp[1], pquatdmp[2], pquatdmp[3]);
}
