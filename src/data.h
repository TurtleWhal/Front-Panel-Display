#if !defined(DATA_H)
#define DATA_H

#define CPU_MIN_TEMP 42
#define CPU_MAX_TEMP 95
#define GPU_MIN_TEMP 36
#define GPU_MAX_TEMP 80

#define CPU_THRESHOLD 20
#define GPU_THRESHOLD 20

struct PerformanceData
{
    float cpuUsage = 0;
    float cpuTemp = 0;
    float gpuUsage = 0;
    float gpuTemp = 0;

    int hour = 12;
    int minute = 0;
    int second = 0;
};

#endif // DATA_H
