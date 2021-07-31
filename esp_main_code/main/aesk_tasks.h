#ifndef AESK_TASKS_H
#define AESK_TASKS_H

//Bu sıra ile numaralandırılsn

#define TASK_SUSPEND 13
#define TASK_RESUME  5

void UartInit(void);

#define UART_TASK_N 1
void UartReceive_S(void);

#define DATA_TASK_N 2
void DataProcess(void);

#define TELEMERTY_TASK_N 3
void TelemetryTransmit_UC(void);

void CommandListen_TC(void); 

#define VSTREAM_TASK_N 4
void VideoStream_UC(void);

#define VRECORD_TASK_N 5
void VideoRecorder(void);

#define FTP_TASK_N 6

#define COMMAND_TASK_N 4
#endif
