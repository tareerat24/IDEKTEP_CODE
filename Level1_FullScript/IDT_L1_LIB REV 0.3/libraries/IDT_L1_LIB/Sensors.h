#ifndef SENSORS_H
#define SENSORS_H


class Sensors
{
public:

    void ultraInit();
    static long ultraGetDistance();
    void ultraCheckDistance();
    long ultraMicrosecondsToCentimeters(long microseconds);
    void ultraAdjustDistance(int colision_distance);
    void ultraCollisionCheck();
};
#endif