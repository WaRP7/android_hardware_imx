#ifndef __MAGCALIBRATION_H__
#define __MAGCALIBRATION_H__
#include <utils/Mutex.h>
#include <cutils/log.h>

#define MAX_PAIR_SIZE 150
class MagCalibration {
    typedef struct {
        int mag_x;
        int mag_y;
        int mag_z;
        int mag_t;
        int acc_x;
        int acc_y;
        int acc_z;
        int acc_t;
    }mag_acc_data_pair;
private:
    android::Mutex mLock;
    mag_acc_data_pair mag_accel_pairs[MAX_PAIR_SIZE];
    mag_acc_data_pair mag_accel_data_tmp;
    int mag_accel_len=0;
    int mag_accel_size=MAX_PAIR_SIZE;

    int mag_center_x;
    int mag_center_y;
    int mag_center_z;

public:
    MagCalibration();
    ~MagCalibration();
    int insertAccData(int x, int y, int z, int64_t t);
    int insertMagData(int x, int y, int z, int64_t t);
    int getCenter(int &x, int &y, int &z);
    //calculate the center
    int updateCenter();
    int getLength(){return mag_accel_len;};
    int getSize(){return mag_accel_size;};
private:
    int allowMagInsert(int x, int y, int z);
    int mFinished;

};
#endif
