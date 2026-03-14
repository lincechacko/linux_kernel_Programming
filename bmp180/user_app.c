#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>

#define DEVICE "/dev/bmp180_device"

struct bmp180_calib {
    int16_t AC1;
    int16_t AC2;
    int16_t AC3;
    uint16_t AC4;
    uint16_t AC5;
    uint16_t AC6;
    int16_t B1;
    int16_t B2;
    int16_t MB;
    int16_t MC;
    int16_t MD;
};

int main()
{
    int fd;
    uint8_t reg;
    uint8_t buf[22];
    struct bmp180_calib calib;

    int32_t UT, UP;
    int32_t X1, X2, B5, B6, X3, B3, p;
    uint32_t B4, B7;
    int OSS = 0;

    fd = open(DEVICE, O_RDWR);
    if(fd < 0)
    {
        perror("Device open failed");
        return -1;
    }

    printf("BMP180 device opened\n");

    /* ---------------- Read Calibration Data ---------------- */

    reg = 0xAA;
    write(fd, &reg, 1);
    read(fd, buf, 22);

    calib.AC1 = (buf[0] << 8) | buf[1];
    calib.AC2 = (buf[2] << 8) | buf[3];
    calib.AC3 = (buf[4] << 8) | buf[5];
    calib.AC4 = (buf[6] << 8) | buf[7];
    calib.AC5 = (buf[8] << 8) | buf[9];
    calib.AC6 = (buf[10] << 8) | buf[11];
    calib.B1  = (buf[12] << 8) | buf[13];
    calib.B2  = (buf[14] << 8) | buf[15];
    calib.MB  = (buf[16] << 8) | buf[17];
    calib.MC  = (buf[18] << 8) | buf[19];
    calib.MD  = (buf[20] << 8) | buf[21];

    /* ---------------- Read Raw Temperature ---------------- */

    uint8_t cmd[2];

    cmd[0] = 0xF4;
    cmd[1] = 0x2E;
    write(fd, cmd, 2);

    usleep(5000);

    reg = 0xF6;
    write(fd, &reg, 1);
    read(fd, buf, 2);

    UT = (buf[0] << 8) | buf[1];

    /* ---------------- Temperature Calculation ---------------- */

    X1 = ((UT - calib.AC6) * calib.AC5) >> 15;
    X2 = (calib.MC << 11) / (X1 + calib.MD);
    B5 = X1 + X2;

    float temperature = ((B5 + 8) >> 4) / 10.0;

    /* ---------------- Read Raw Pressure ---------------- */

    cmd[0] = 0xF4;
    cmd[1] = 0x34;
    write(fd, cmd, 2);

    usleep(8000);

    reg = 0xF6;
    write(fd, &reg, 1);
    read(fd, buf, 3);

    UP = (((buf[0] << 16) | (buf[1] << 8) | buf[2]) >> (8 - OSS));

    /* ---------------- Pressure Calculation ---------------- */

    B6 = B5 - 4000;

    X1 = (calib.B2 * ((B6 * B6) >> 12)) >> 11;
    X2 = (calib.AC2 * B6) >> 11;
    X3 = X1 + X2;

    B3 = (((calib.AC1 * 4 + X3) << OSS) + 2) >> 2;

    X1 = (calib.AC3 * B6) >> 13;
    X2 = (calib.B1 * ((B6 * B6) >> 12)) >> 16;
    X3 = ((X1 + X2) + 2) >> 2;

    B4 = (calib.AC4 * (uint32_t)(X3 + 32768)) >> 15;

    B7 = ((uint32_t)UP - B3) * (50000 >> OSS);

    if (B7 < 0x80000000)
        p = (B7 * 2) / B4;
    else
        p = (B7 / B4) * 2;

    X1 = (p >> 8) * (p >> 8);
    X1 = (X1 * 3038) >> 16;
    X2 = (-7357 * p) >> 16;

    p = p + ((X1 + X2 + 3791) >> 4);

    printf("Temperature = %.2f °C\n", temperature);
    printf("Pressure = %d Pa\n", p);

    close(fd);

    return 0;
}
