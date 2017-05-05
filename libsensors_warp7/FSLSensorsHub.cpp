/*
 * Copyright (C) 2012-2015 Freescale Semiconductor, Inc.
 * Copyright (C) 2008 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <fcntl.h>
#include <errno.h>
#include <math.h>
#include <stdlib.h>
#include <poll.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/select.h>
#include <dlfcn.h>
#include <cutils/log.h>
#include <cutils/properties.h>
#include "FSLSensorsHub.h"
#include <unistd.h>

extern android::Mutex mgLock;
extern MagCalibration gMagCalibration;

#define FSL_SENS_CTRL_NAME   	"FSL_SENS_HUB"
#define FSL_SENS_DATA_NAME    	"FSL_SENS_HUB" 
#define FSL_SENS_SYSFS_PATH   	"/sys/class/misc"
#define FSL_SENS_SYSFS_DELAY  	"poll_delay"
#define FSL_SENS_SYSFS_ENABLE 	"enable"
#define FSL_ACC_DEVICE_NAME	    "FreescaleAccelerometer"
#define FSL_MAG_DEVICE_NAME 	"FreescaleMagnetometer"
#define FSL_GYRO_DEVICE_NAME	"FreescaleGyroscope"

#define EVENT_ACC_X			REL_X
#define EVENT_ACC_Y	   		REL_Y
#define EVENT_ACC_Z			REL_Z		

#define EVENT_MAG_X			REL_RX
#define EVENT_MAG_Y			REL_RY
#define EVENT_MAG_Z			REL_RZ
#define EVENT_MAG_STATUS	REL_MISC

#define EVENT_GYRO_X		(REL_MISC + 1)	/*0x0A*/
#define EVENT_GYRO_Y		(REL_MISC + 2)	/*0x0B*/
#define EVENT_GYRO_Z		(REL_MISC + 3)	/*0x0C*/

#define EVENT_STEP_DETECTED	 	REL_DIAL	
#define EVENT_STEP_COUNT_HIGH	REL_HWHEEL 
#define EVENT_STEP_COUNT_LOW	REL_WHEEL  

#define EVENT_ORNT_X		ABS_X 
#define EVENT_ORNT_Y		ABS_Y 
#define EVENT_ORNT_Z		ABS_Z 
#define EVENT_ORNT_STATUS	EVENT_MAG_STATUS 	/*0x0A*/

#define EVENT_LINEAR_ACC_X	ABS_RX 
#define EVENT_LINEAR_ACC_Y	ABS_RY 
#define EVENT_LINEAR_ACC_Z	ABS_RZ 

#define EVENT_GRAVITY_X		ABS_HAT0X
#define EVENT_GRAVITY_Y		ABS_HAT1X
#define EVENT_GRAVITY_Z		ABS_HAT2X

#define EVENT_ROTATION_VECTOR_W		ABS_MISC
#define EVENT_ROTATION_VECTOR_A		ABS_HAT0Y
#define EVENT_ROTATION_VECTOR_B		ABS_HAT1Y
#define EVENT_ROTATION_VECTOR_C		ABS_HAT2Y



#define ACC_DATA_CONVERSION(value)  ((float)value * GRAVITY_EARTH/0x4000  )
#define MAG_DATA_CONVERSION(value)  ((float)value/10.0f)
#define ORNT_DATA_CONVERSION(value) ((float)value/10.0f)
#define GYRO_DATA_CONVERSION(value) ((float)value/1000.0f /180.0f * M_PI )
#define RV_DATA_CONVERSION(value)   ((float)value /10000.0f)
#define LA_DATA_CONVERSION(value)   ((float)value /10.0f)
#define GRAVT_DATA_CONVERSION(value)((float)value /10.0f)
//2891,865,-1451
#define MAG_DATA_CALIBRATION_X (-2839.4)
#define MAG_DATA_CALIBRATION_Y (-895.21)
#define MAG_DATA_CALIBRATION_Z (1180)

//If reverse, mag and accel should reverse together
#define DATA_REVERSE_X(x) (-(x))
#define DATA_REVERSE_Y(y) (-(y))
#define DATA_REVERSE_Z(z) (+(z))

FSLSensorsHub::FSLSensorsHub( const char* dev_name,
        const char* data_name)
: SensorBase(dev_name, data_name),
  mPendingMask(0),
  mInputReader(16)
{
    memset(&mPendingEvent[0], 0, sensors *sizeof(sensors_event_t));

	mSensorWhat = 0;
	mEnabled[accel] = 0;
	mDelay[accel] = 0;
	mPendingEvent[accel].version = sizeof(sensors_event_t);
    mPendingEvent[accel].sensor  = ID_A;
    mPendingEvent[accel].type    = SENSOR_TYPE_ACCELEROMETER;
    mPendingEvent[accel].magnetic.status = SENSOR_STATUS_ACCURACY_LOW;
    mPendingEvent[accel].version = sizeof(sensors_event_t);

	mEnabled[mag] = 0;
	mDelay[mag] = 0;
    mPendingEvent[mag].version = sizeof(sensors_event_t);
    mPendingEvent[mag].sensor  = ID_M;
    mPendingEvent[mag].type    = SENSOR_TYPE_MAGNETIC_FIELD;
    mPendingEvent[mag].magnetic.status = SENSOR_STATUS_ACCURACY_LOW;
    mPendingEvent[mag].version = sizeof(sensors_event_t);

	mEnabled[gyro] = 0;
	mDelay[gyro] = 0;
    mPendingEvent[gyro].sensor  = ID_GY;
    mPendingEvent[gyro].type    = SENSOR_TYPE_GYROSCOPE;
    mPendingEvent[gyro].orientation.status = SENSOR_STATUS_ACCURACY_LOW;
	mPendingEvent[gyro].version = sizeof(sensors_event_t);

	sprintf(mClassPath[accel],"%s/%s",FSL_SENS_SYSFS_PATH,FSL_ACC_DEVICE_NAME);
	sprintf(mClassPath[mag],"%s/%s",FSL_SENS_SYSFS_PATH,FSL_MAG_DEVICE_NAME);
	sprintf(mClassPath[gyro],"%s/%s",FSL_SENS_SYSFS_PATH,FSL_GYRO_DEVICE_NAME);

}

FSLSensorsHub::~FSLSensorsHub()
{
}

int FSLSensorsHub::setEnable(int32_t handle, int en)
{
	int err = 0;
	int what = accel;
	bool isHaveSensorRun = 0;
    switch(handle){
		case ID_A : what = accel; break;
		case ID_M : what = mag;   break;
		case ID_GY: what = gyro;  break; 	
    }

	mSensorWhat = what;
	
    if(en)
		mEnabled[what]++;
	else
		mEnabled[what]--;
	
	if(mEnabled[what] < 0)
		mEnabled[what] = 0;
	for(int i = 0; i < sensors; i++ ){
		if(mEnabled[i] > 0)
		{
			isHaveSensorRun = 1;
			break;
		}
	}
	if(isHaveSensorRun){
		if(mEnabled[rv] > 0 || mEnabled[gravt] > 0 ||mEnabled[la] > 0 || mEnabled[mag]> 0  || mEnabled[orn] > 0 || mEnabled[gyro]> 0) //need fusion run
		{
			enable_sensor(accel);
			enable_sensor(mag);
			enable_sensor(gyro);
		}else if(mEnabled[accel] > 0)  //only accel enable
		{ 
			enable_sensor(accel);
			disable_sensor(mag);
			disable_sensor(gyro);
		}
	}else
	{
		disable_sensor(accel);
		disable_sensor(mag);
		disable_sensor(gyro);
	}
	ALOGD("FSLSensorsHub sensor what = %d , enable = %d",what,mEnabled[what]);
    return err;
}

int FSLSensorsHub::setDelay(int32_t handle, int64_t ns)
{
    if (ns < 0)
        return -EINVAL;
	int what = accel;
    switch(handle){
		case ID_A : what = accel; break;
		case ID_M : what = mag;   break;
		case ID_O : what = orn;   break;
		case ID_GY: what = gyro;  break; 
		case ID_RV: what = rv;	  break;
		case ID_LA: what = la;	  break;
		case ID_GR: what = gravt; break;
		case ID_SD: what = sd;	  break;
		case ID_SC: what = sc;    break;
    }

    mDelay[what] = ns;
	if(what == accel)
		update_delay(accel,mDelay[accel]);
	else if(what == mag || what == orn)
		update_delay(mag,mDelay[mag]);
	else if(what == gyro)
		update_delay(gyro,mDelay[gyro]);
	else{
		update_delay(accel,mDelay[accel]);
		update_delay(mag,mDelay[mag]);
		update_delay(gyro,mDelay[gyro]);
	}
    return 0;
}

int FSLSensorsHub::update_delay(int sensor_type , int64_t ns)
{
    return writeDelay(sensor_type,ns);
}

int FSLSensorsHub::readEvents(sensors_event_t* data, int count)
{
    //ALOGD("%s:%d", __func__, mSensorWhat);
	int i;
    if (count < 1)
        return -EINVAL;

    // if accel and mag are both working,
    // switch from accel to mag, or mag to accel before report
    // mutex mgLock to make sure the event is correct
#if 1 //az 
    // if this sensor disabled, do not read event
    if (mEnabled[mSensorWhat]==0) {
        //FIXME: this makes sensors.cpp poll always running
        return 0;
    }
    ssize_t n=-1;
    if (mSensorWhat == accel ||  mSensorWhat == mag) {
        mgLock.lock();
        int is_acc_enabled=0, is_mag_enabled=0;
        int err=-1;
        err = readEnable(accel, is_acc_enabled);
        if (err) ALOGE("read accel enable error:%d", err);
        err = readEnable(mag, is_mag_enabled);
        if (err) ALOGE("read mag enable error:%d", err);

        //only acc and mag are both working, make the switch
        if ((mSensorWhat == mag&&is_acc_enabled==1) ||(mSensorWhat == accel && is_mag_enabled==1)) {
            //ALOGD("make switch between accel and mag");
            //flush it firstly
            n = mInputReader.fill(data_fd);
            disable_sensor(accel);
            disable_sensor(mag);
            if (n>0) {
                input_event const* e;
                while (mInputReader.readEvent(&e)) {
                    mInputReader.next();
                }
            }

            if (mSensorWhat == accel) {
                enable_sensor(accel);
                n = mInputReader.fill(data_fd);
            } else if (mSensorWhat == mag) {
                enable_sensor(mag);
                n = mInputReader.fill(data_fd);
            }
        }else {
            n = mInputReader.fill(data_fd);
        }
        mgLock.unlock();
    }else {
        n = mInputReader.fill(data_fd);
    }
#else
    ssize_t n = mInputReader.fill(data_fd);
#endif	
    if (n < 0)
        return n;

    int numEventReceived = 0;
    input_event const* event;

    while (count && mInputReader.readEvent(&event)) {
        int type = event->type;
		//ALOGE("%d fd read %d type data", data_fd, type);
        if ((type == EV_ABS) || (type == EV_REL) || (type == EV_KEY)) {
            processEvent(type,event->code, event->value);
            mInputReader.next();
        } else if (type == EV_SYN) {
            int64_t time = timevalToNano(event->time);
			for(i = 0 ; i< sensors && mPendingMask && count ;i++){
			  	 	if(mPendingMask & (1 << i)){
						mPendingMask &= ~(1 << i);
						mPendingEvent[i].timestamp = time;
						if (mEnabled[i]) {
							*data++ = mPendingEvent[i];
							count--;
							numEventReceived++;
						}
			  	 	}
	       }
		   if (!mPendingMask) {
		       mInputReader.next();
		   }
        } else {
            mInputReader.next();
        }
    }

    return numEventReceived;
}

void FSLSensorsHub::processEvent(int code, int value)
{

}
void FSLSensorsHub::processEvent(int type ,int code, int value){
	static uint64_t steps_high = 0,steps_low = 0;
	//ALOGD("processEvent type:%d code:%x value:%x", type, code, value);

    static int x,y,z;
    static int cx=-MAG_DATA_CALIBRATION_X,cy=-MAG_DATA_CALIBRATION_Y,cz=-MAG_DATA_CALIBRATION_Z;
    int64_t t;
    int ret=-1;
	if(mSensorWhat == accel){
         //ALOGD("processEvent accel type:%d code:%x value:%x processed_value:%d", type, code, value, (int)ACC_DATA_CONVERSION(value));
		 switch (code) {
		 case EVENT_ORNT_X:
            mPendingMask |= 1 << accel;
            mPendingEvent[accel].acceleration.x = ACC_DATA_CONVERSION(DATA_REVERSE_X(value));
            x=mPendingEvent[accel].acceleration.x*10;
            break;
        case EVENT_ORNT_Y:
            mPendingMask |= 1 << accel;
            mPendingEvent[accel].acceleration.y = ACC_DATA_CONVERSION(DATA_REVERSE_Y(value));
            y=mPendingEvent[accel].acceleration.y*10;
            break;
        case EVENT_ORNT_Z:
            mPendingMask |=  1 << accel;
            mPendingEvent[accel].acceleration.z = ACC_DATA_CONVERSION(DATA_REVERSE_Z(value));

            z=mPendingEvent[accel].acceleration.z*10;
            t=0;//t=getTimestamp()/1000; //ms//FIXME: cannot get a right time-stamp
            if (gMagCalibration.getLength()<gMagCalibration.getSize()) {
                gMagCalibration.insertAccData(x,y,z,t);
            }
            break;
		 }
	}
		 
	if(mSensorWhat == mag){
        //ALOGD("processEvent mag type:%d code:%x value:%x processed_value:%d", type, code, value, (int)MAG_DATA_CONVERSION(value));
		switch (code) {
        case EVENT_ORNT_X :
            mPendingMask |= 1 << mag;
            mPendingEvent[mag].magnetic.x = MAG_DATA_CONVERSION(DATA_REVERSE_X(value-cx));
            x=value;
            break;
        case EVENT_ORNT_Y:
            mPendingMask |= 1 << mag;
            mPendingEvent[mag].magnetic.y = MAG_DATA_CONVERSION(DATA_REVERSE_Y(value-cy));
            y=value;
            break;
        case EVENT_ORNT_Z:
            mPendingMask |=  1 << mag;
            mPendingEvent[mag].magnetic.z = MAG_DATA_CONVERSION(DATA_REVERSE_Z(value-cz));
            z=value;
            t=100;//t=getTimestamp()/1000; //ms//FIXME: cannot get a right time-stamp
            if (gMagCalibration.getLength()<gMagCalibration.getSize()) {
                ret = gMagCalibration.insertMagData(x,y,z,/*t*/100);
                if (ret==0) {//insert ok
                    if (gMagCalibration.getLength()>gMagCalibration.getSize()/2) {
                        gMagCalibration.updateCenter(); //FIXME: not always update
                        gMagCalibration.getCenter(cx,cy,cz);
                    }
                }
            }
            break;
		case EVENT_MAG_STATUS:
			mPendingMask |=  1 << mag;
			mPendingEvent[mag].magnetic.status 	= value;
			mPendingEvent[orn].orientation.status	= value;
			break;
		}

	}

	if(mSensorWhat == gyro){
        //ALOGD("processEvent gyro type:%d code:%x value:%x processed_value:%d", type, code, value, (int)GYRO_DATA_CONVERSION(value));
		switch (code) {
		case EVENT_ORNT_X :
            mPendingMask |=  1 << gyro;
            mPendingEvent[gyro].gyro.x= GYRO_DATA_CONVERSION(value);
            break;
        case EVENT_ORNT_Y :
            mPendingMask |=  1 << gyro;
            mPendingEvent[gyro].gyro.y = GYRO_DATA_CONVERSION(value);
            break;
        case EVENT_ORNT_Z :
            mPendingMask |=  1 << gyro;
            mPendingEvent[gyro].gyro.z	= GYRO_DATA_CONVERSION(value);
            break;
		}
    
	}
}


int FSLSensorsHub::writeEnable(int what ,int isEnable) {
	char attr[PATH_MAX] = {'\0'};
	int err = 0;
	if(mClassPath[0] == '\0')
		return -1;

	strcpy(attr, mClassPath[what]);
	strcat(attr,"/");
	strcat(attr,FSL_SENS_SYSFS_ENABLE);

	int fd = open(attr, O_RDWR);
	if (0 > fd) {
		ALOGE("Could not open (write-only) SysFs attribute \"%s\" (%s).", attr, strerror(errno));
		return -errno;
	}

	char buf[16];
	sprintf(buf,"%d",isEnable);
	err = write(fd, buf, sizeof(buf));
	if (0 > err) {
		err = -errno;
		ALOGE("Could not write SysFs attribute \"%s\" (%s).", attr, strerror(errno));
	} else {
		err = 0;
	}

	close(fd);

	return err;
}
int FSLSensorsHub::readEnable(int what ,int &isEnabled) {
	char attr[PATH_MAX] = {'\0'};
	int err = 0;
	if(mClassPath[0] == '\0')
		return -1;

	strcpy(attr, mClassPath[what]);
	strcat(attr,"/");
	strcat(attr,FSL_SENS_SYSFS_ENABLE);

	int fd = open(attr, O_RDONLY);
	if (0 > fd) {
		ALOGE("Could not open (write-only) SysFs attribute \"%s\" (%s).", attr, strerror(errno));
		return -errno;
	}

	char buf[2];
	err = read(fd, buf, sizeof(buf));
	if (0 > err) {
		err = -errno;
		ALOGE("Could not write SysFs attribute \"%s\" (%s).", attr, strerror(errno));
	} else {
		err = 0;
	}
    isEnabled = atoi(buf);
    //ALOGD("%s:%s read from %s:%d", __func__, mClassPath[mSensorWhat], attr, isEnabled);
    
	close(fd);

	return err;
}
int FSLSensorsHub::writeDelay(int what,int64_t ns) {
	char attr[PATH_MAX] = {'\0'};
	int delay;
	if(mClassPath[0] == '\0')
		return -1;

	strcpy(attr, mClassPath[what]);
	strcat(attr,"/");
	strcat(attr,FSL_SENS_SYSFS_DELAY);

	int fd = open(attr, O_RDWR);
	if (0 > fd) {
		ALOGE("Could not open (write-only) SysFs attribute \"%s\" (%s).", attr, strerror(errno));
		return -errno;
	}
	if (ns > 10240000000LL) {
		ns = 10240000000LL; /* maximum delay in nano second. */
	}
	if (ns < 312500LL) {
		ns = 312500LL; /* minimum delay in nano second. */
	}

    char buf[80];
	delay = ns/1000/1000;
    sprintf(buf, "%d",delay);
	ALOGD("FSL_SENS write delay %s\n",buf);
    write(fd, buf, strlen(buf)+1);
    close(fd);
    return 0;

}

int FSLSensorsHub::enable_sensor(int what) {
	return writeEnable(what,1);
}

int FSLSensorsHub::disable_sensor(int what) {
	return writeEnable(what,0);
}
int FSLSensorsHub::getEnable(int32_t handle) {
	int what = accel;
	switch(handle){
		case ID_A : what = accel; break;
		case ID_M : what = mag;   break;
		case ID_O : what = orn;   break;
		case ID_GY: what = gyro;  break; 
		case ID_RV: what = rv;	  break;
		case ID_LA: what = la;	  break;
		case ID_GR: what = gravt; break;
		case ID_SD: what = sd;	  break;
		case ID_SC: what = sc;    break;
    }

	return mEnabled[what];
}

/*****************************************************************************/

