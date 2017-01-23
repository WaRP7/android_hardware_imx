/*
 * Copyright (C) 2011-2015 Freescale Semiconductor, Inc.
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

#define LOG_TAG "Sensors"

#include <hardware/sensors.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <math.h>
#include <poll.h>
#include <pthread.h>
#include <stdlib.h>

#include <linux/input.h>

#include <utils/Atomic.h>
#include <utils/Log.h>

#include "sensors.h"
#include "PressSensor.h"

#include "FSLSensorsHub.h"

/*****************************************************************************/

#define DELAY_OUT_TIME 0x7FFFFFFF

#define LIGHT_SENSOR_POLLTIME    2000000000

#define SENSORS_ACCELERATION_HANDLE     ID_A
#define SENSORS_MAGNETIC_FIELD_HANDLE   ID_M
#define SENSORS_ORIENTATION_HANDLE      ID_O
#define SENSORS_GYROSCOPE_HANDLE        ID_GY
#define SENSORS_LIGHT_HANDLE            ID_L
#define SENSORS_PRESSURE_HANDLE         ID_P
#define SENSORS_TEMPERATURE_HANDLE      ID_T
#define SENSORS_PROXIMITY_HANDLE        ID_PX
#define SENSORS_GRAVITY_HANDLE			ID_GR
#define SENSORS_LINEAR_ACCEL_HANDLE		ID_LA
#define SENSORS_ROTATION_VECTOR_HANDLE  ID_RV
#define SENSORS_STEP_DETECTOR_HANDLE  	ID_SD
#define SENSORS_STEP_COUNTER_HANDLE  	ID_SC


/*****************************************************************************/

/* The SENSORS Module */
static const struct sensor_t sSensorList[] = {
        { "Freescale Sensor Hub Accelerometer",
          "Freescale Semiconductor Inc.",
          1, SENSORS_ACCELERATION_HANDLE,
          SENSOR_TYPE_ACCELEROMETER, RANGE_A, CONVERT_A, 0.30f, 10000, 0, 0, "" },

        { "Freescale Sensor Hub Magnetic field sensor",
          "Freescale Semiconductor Inc.",
          1, SENSORS_MAGNETIC_FIELD_HANDLE,
          SENSOR_TYPE_MAGNETIC_FIELD, 1500.0f, CONVERT_M, 0.50f, 10000, 0, 0, "" },

        { "Freescale Sensor Hub Gyroscope sensor",
          "Freescale Semiconductor Inc.",
          1, SENSORS_GYROSCOPE_HANDLE,
          SENSOR_TYPE_GYROSCOPE, 4000.0f, 0.0035, 0.50f, 10000, 0, 0, "" },

        { "Freescale Pressure Sensor ",
          "Freescale Semiconductor Inc.",
          1, SENSORS_PRESSURE_HANDLE,
          SENSOR_TYPE_PRESSURE, 110.0f, 1.5, 0.50f, 10000, 0, 0, "" },
        { "Freescale Temperature Sensor ",
          "Freescale Semiconductor Inc.",
          1, SENSORS_TEMPERATURE_HANDLE,
          SENSOR_TYPE_TEMPERATURE, 110.0f, 1.5, 0.50f, 10000, 0, 0, "" },
};


static int open_sensors(const struct hw_module_t* module, const char* id,
                        struct hw_device_t** device);


static int sensors__get_sensors_list(struct sensors_module_t* module,
                                     struct sensor_t const** list)
{
        *list = sSensorList;
        return ARRAY_SIZE(sSensorList);
}

static struct hw_module_methods_t sensors_module_methods = {
        open: open_sensors
};

struct sensors_module_t HAL_MODULE_INFO_SYM = {
        common: {
                tag: HARDWARE_MODULE_TAG,
                version_major: 1,
                version_minor: 1,
                id: SENSORS_HARDWARE_MODULE_ID,
                name: "Freescale Sensor module",
                author: "Freescale Semiconductor Inc.",
                methods: &sensors_module_methods,
        },
        get_sensors_list: sensors__get_sensors_list,
};
struct sensors_poll_context_t {
    struct sensors_poll_device_1 device; // must be first

        sensors_poll_context_t();
        ~sensors_poll_context_t();
	int fillPollFd();
    int activate(int handle, int enabled);
    int setDelay(int handle, int64_t ns);
    int pollEvents(sensors_event_t* data, int count);
	int batch(int handle, int flags, int64_t period_ns, int64_t timeout);
	int flush(int handle);
	int magRunTimes;
private:
    enum {
        accel           = 0,
		mag,
		gyro,
        press,
        temperature,
        numSensorDrivers,
        numFds,
    };
    static const size_t wake = numFds - 1;
    static const char WAKE_MESSAGE = 'W';
    struct pollfd mPollFds[numFds];
    int mWritePipeFd;
    SensorBase* mSensors[numSensorDrivers];

    int handleToDriver(int handle) const {
        switch (handle) {
            case ID_P:
                return press;
                break;
            case ID_T:
                return temperature;
                break;
            case ID_A:
                return accel;
                break;
          	case ID_M:
	            return mag;
                break;
          	case ID_O:
			case ID_GY:
                return gyro;
                break;
        }
        return -EINVAL;
    }
};

/*****************************************************************************/
int sensors_poll_context_t::fillPollFd(){
	int i = 0;
	for(i = 0 ; i < numSensorDrivers; i++){
		if(mSensors[i] != NULL)
			 mPollFds[i].fd = mSensors[i]->getFd();
   			 mPollFds[i].events = POLLIN;
    		 mPollFds[i].revents = 0;
	}
	return 0;
}
sensors_poll_context_t::sensors_poll_context_t()
{
    mSensors[accel] = new FSLSensorsHub("fxos8700", "fxos8700");
	mSensors[mag] = new FSLSensorsHub("fxos8700", "fxos8700");
	mSensors[gyro] = new FSLSensorsHub("fxas2100x", "fxas2100x");
    mSensors[press] = new PressSensor();
    mSensors[temperature] = new PressSensor();
	fillPollFd();
	magRunTimes = 0;
    int wakeFds[2];
    int result = pipe(wakeFds);
    ALOGE_IF(result<0, "error creating wake pipe (%s)", strerror(errno));
    fcntl(wakeFds[0], F_SETFL, O_NONBLOCK);
    fcntl(wakeFds[1], F_SETFL, O_NONBLOCK);
    mWritePipeFd = wakeFds[1];

    mPollFds[wake].fd = wakeFds[0];
    mPollFds[wake].events = POLLIN;
    mPollFds[wake].revents = 0;
}

sensors_poll_context_t::~sensors_poll_context_t() {
    for (int i=0 ; i<numSensorDrivers ; i++) {
        delete mSensors[i];
    }
    close(mPollFds[wake].fd);
    close(mWritePipeFd);
}

int sensors_poll_context_t::activate(int handle, int enabled) {
    int index = handleToDriver(handle);
    if (index < 0) return index;
    int err = 0 ;
	err =  mSensors[index]->setEnable(handle, enabled);
    if (enabled && !err) {
        const char wakeMessage(WAKE_MESSAGE);
        int result = write(mWritePipeFd, &wakeMessage, 1);
        ALOGE_IF(result<0, "error sending wake message (%s)", strerror(errno));
    }
    return err;
}

int sensors_poll_context_t::setDelay(int handle, int64_t ns) {
    int index = handleToDriver(handle);
    if (index < 0) return index;
    return mSensors[index]->setDelay(handle, ns);
}

int sensors_poll_context_t::pollEvents(sensors_event_t* data, int count)
{
    int nbEvents = 0;
    int n = 0;
    do {
        // see if we have some leftover from the last poll()
        for (int i=0 ; count && i<numSensorDrivers ; i++) {
            SensorBase* const sensor(mSensors[i]);

	   if ((mPollFds[i].revents & POLLIN) || (sensor->hasPendingEvents())) {
                int nb = sensor->readEvents(data, count);
                if (nb < count) {
                    // no more data for this sensor
                    mPollFds[i].revents = 0;
                }
                count -= nb;
                nbEvents += nb;
                data += nb;
            }
        }

        if (count) {
            // we still have some room, so try to see if we can get
            // some events immediately or just wait if we don't have
            // anything to return
            //n = poll(mPollFds, numFds, nbEvents ? 0 : -1);
			do {
				//fillPollFd(); /*reset poll fd , if sensor change between batch mode and continuous mode*/
			 	n = poll(mPollFds, numFds, nbEvents ? 0 : -1);            
			} while (n < 0 && errno == EINTR);
            if (n<0) {
                ALOGE("poll() failed (%s)", strerror(errno));
                return -errno;
            }
            if (mPollFds[wake].revents & POLLIN) {
                char msg;
                int result = read(mPollFds[wake].fd, &msg, 1);
                ALOGE_IF(result<0, "error reading from wake pipe (%s)", strerror(errno));
                ALOGE_IF(msg != WAKE_MESSAGE, "unknown message on wake queue (0x%02x)", int(msg));
                mPollFds[wake].revents = 0;
            }
        }
        // if we have events and space, go read them
    } while (n && count);

    return nbEvents;
}
int sensors_poll_context_t::batch(int handle, int flags, int64_t period_ns, int64_t timeout){
	 int ret;
	 int index = handleToDriver(handle);
     if (index < 0) return index;
	 ret = mSensors[index]->batch(handle,flags,period_ns,timeout);
	 const char wakeMessage(WAKE_MESSAGE); 
     int result = write(mWritePipeFd, &wakeMessage, 1);
     ALOGE_IF(result<0, "error batch sending wake message (%s)", strerror(errno));
	 return ret;
}
int sensors_poll_context_t::flush(int handle){
	int index = handleToDriver(handle);
    if (index < 0) return index;
    return mSensors[index]->flush(handle);
}

/*****************************************************************************/

static int poll__close(struct hw_device_t *dev)
{
    sensors_poll_context_t *ctx = (sensors_poll_context_t *)dev;
    if (ctx) {
        delete ctx;
    }
    return 0;
}

static int poll__activate(struct sensors_poll_device_t *dev,
        int handle, int enabled) {
    sensors_poll_context_t *ctx = (sensors_poll_context_t *)dev;
    return ctx->activate(handle, enabled);
}

static int poll__setDelay(struct sensors_poll_device_t *dev,
        int handle, int64_t ns) {
    sensors_poll_context_t *ctx = (sensors_poll_context_t *)dev;
    return ctx->setDelay(handle, ns);
}

static int poll__poll(struct sensors_poll_device_t *dev,
        sensors_event_t* data, int count) {
    sensors_poll_context_t *ctx = (sensors_poll_context_t *)dev;
    return ctx->pollEvents(data, count);
}
static int poll__batch(struct sensors_poll_device_1* dev,
            int handle, int flags, int64_t period_ns, int64_t timeout){
	sensors_poll_context_t *ctx = (sensors_poll_context_t *)dev;
	return ctx->batch(handle,flags,period_ns,timeout);
}

static int poll__flush(struct sensors_poll_device_1* dev, int handle){
	sensors_poll_context_t *ctx = (sensors_poll_context_t *)dev;
	return ctx->flush(handle);
}
/*****************************************************************************/

/** Open a new instance of a sensor device using name */
static int open_sensors(const struct hw_module_t* module, const char* id,
                        struct hw_device_t** device)
{
        int status = -EINVAL;
        sensors_poll_context_t *dev = new sensors_poll_context_t();

        memset(&dev->device, 0, sizeof(sensors_poll_device_1));

        dev->device.common.tag = HARDWARE_DEVICE_TAG;
        dev->device.common.version  = SENSORS_DEVICE_API_VERSION_1_1;
        dev->device.common.module   = const_cast<hw_module_t*>(module);
        dev->device.common.close    = poll__close;
        dev->device.activate        = poll__activate;
        dev->device.setDelay        = poll__setDelay;
        dev->device.poll            = poll__poll;
		dev->device.batch			= poll__batch;
		dev->device.flush			= poll__flush;
        *device = &dev->device.common;
        status = 0;

        return status;
}