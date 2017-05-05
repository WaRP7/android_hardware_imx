#include "MagCalibration.h"
#include <stdlib.h>
#include <cutils/properties.h>
#define ACC_Z_THRESHOLD (5)
#define ACC_Z_NORMAL (98)

#define TIME_THRESHOLD 200  //ms
#define MAG_THRESHOLD_X 20//(mag_accel_pairs[0].mag_x/100+1)
#define MAG_THRESHOLD_Y 10//(mag_accel_pairs[0].mag_y/100+1)
#define MAG_THRESHOLD_F(v) (1+v/120)

MagCalibration::MagCalibration():
        mLock()
{
    //ALOGD("%s", __func__);
    mag_center_x=-1;
    mag_center_y=-1;
    mag_center_z=-1;
    mFinished=0;
    memset(mag_accel_pairs, 0, sizeof(mag_acc_data_pair)*mag_accel_size);
}
MagCalibration::~MagCalibration() 
{
    //ALOGD("%s", __func__);
}

int MagCalibration::allowMagInsert(int x, int y, int z)
{
    int i=0;
    for (i=0;i<mag_accel_len;i++)
    {
        if (abs(x-mag_accel_pairs[i].mag_x) < MAG_THRESHOLD_X)
            if (abs(y-mag_accel_pairs[i].mag_y) < MAG_THRESHOLD_Y)
                return 0; //find one
    }
    return 1;
}

int MagCalibration::insertMagData(int x, int y, int z, int64_t t)
{
    android::Mutex::Autolock lock(mLock);
    if (mag_accel_len>=mag_accel_size) //len< MAX
        return -1;    
    //ALOGD("%s:x=%d,y=%d,z=%d,t=%ld", __func__,x,y,z,t);
    mag_accel_data_tmp.mag_x=x;
    mag_accel_data_tmp.mag_y=y;
    mag_accel_data_tmp.mag_z=z;
    mag_accel_data_tmp.mag_t=t;

    int cur = mag_accel_len;
    if (cur>=0) {
        if (abs(mag_accel_data_tmp.acc_z-ACC_Z_NORMAL)< ACC_Z_THRESHOLD) //data in threshold
        if (t>mag_accel_data_tmp.acc_t
                &&(t<mag_accel_data_tmp.acc_t+TIME_THRESHOLD) )
            //has a valid acc and mag is null
            if (cur==0||allowMagInsert(x,y,z)) //not in the arrary
            {
                mag_accel_len++;
                memcpy(&(mag_accel_pairs[cur]),&mag_accel_data_tmp, sizeof(mag_accel_data_tmp));
                //ALOGD("%s:accel:x=%d,y=%d,z=%d,t=%ld", __func__,mag_accel_data_tmp.acc_x,mag_accel_data_tmp.acc_y,mag_accel_data_tmp.acc_z,mag_accel_data_tmp.acc_t);
                //ALOGD("%s:mag:  x=%d,y=%d,z=%d,t=%ld", __func__,x,y,z,t);
                ALOGD("***** INSERTED at %d", cur);
                ALOGD("Magnetic Sensor Calibration complete: %d", 100*(cur+1)/getSize());
                char buf[10];
                sprintf(buf,"%d", 100*(cur+1)/getSize());
                property_set("sys.sensors.magcalibration",buf); //percent of completion 
                return 0;
            }
    }
    return -1;
}
int MagCalibration::insertAccData(int x, int y, int z, int64_t t)
{
    android::Mutex::Autolock lock(mLock);
    if (mag_accel_len>=mag_accel_size) //len< MAX
        return -1;
    //ALOGD("%s:x=%d,y=%d,z=%d,t=%ld", __func__,x,y,z,t);

    mag_accel_data_tmp.acc_x=x;
    mag_accel_data_tmp.acc_y=y;
    mag_accel_data_tmp.acc_z=z;
    mag_accel_data_tmp.acc_t=t;
    return 0;
#if 0
    if (mag_accel_len<mag_accel_size) //len< MAX
    {
        int cur = mag_accel_len;
        if (abs(z-ACC_Z_NORMAL)< ACC_Z_THRESHOLD) //data in threshold
        { //do insert
            mag_accel_pairs[cur].acc_x=x;
            mag_accel_pairs[cur].acc_y=x;
            mag_accel_pairs[cur].acc_z=x;
            mag_accel_pairs[cur].acc_t=x;
            mag_accel_pairs[cur].mag_x=-1;
            mag_accel_pairs[cur].mag_y=-1;
            mag_accel_pairs[cur].mag_z=-1;
            mag_accel_pairs[cur].mag_t=-1;
            return 0; //OK
        }
    }
    return -1; //FAIL
#endif
}

int MagCalibration::updateCenter() 
{
    android::Mutex::Autolock lock(mLock);
    //ALOGD("%s", __func__);
    int i=0;
    int sum_x=0, sum_y=0, sum_z=0;
    if (mFinished==0) {
        if (mag_accel_len>=mag_accel_size/2) {
            for (i=0;i<mag_accel_len;i++) {
                sum_x+=mag_accel_pairs[i].mag_x;
                sum_y+=mag_accel_pairs[i].mag_y;
                sum_z+=mag_accel_pairs[i].mag_z;
            }
            mag_center_x=sum_x/mag_accel_len;
            mag_center_y=sum_y/mag_accel_len;
            mag_center_z=sum_z/mag_accel_len;
            ALOGD("center at (%d,%d,%d)", mag_center_x, mag_center_y, mag_center_z);
            if(mag_accel_len==mag_accel_size)
                mFinished=1;
            return 0;
        }
    }
    return -1;
}

int MagCalibration::getCenter(int &x, int &y, int &z)
{
    x=mag_center_x;
    y=mag_center_y;
    z=mag_center_z;
    return 0;//OK
}
