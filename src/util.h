#ifndef __UTIL__
#define __UTIL__
#include "DriverMain.h"
#include <tdikrnl.h>

NTSTATUS CallAttachedDevice(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
NTSTATUS IrpNetInternalDeviceControl(IN PDEVICE_OBJECT DeviceObject, IN OUT PIRP Irp);
NTSTATUS TdiOnSendDatagram(IN PDEVICE_OBJECT DeviceObject, IN OUT PIRP Irp);
BOOLEAN IsMyDevice(IN PDEVICE_OBJECT DeviceObject);
NTSTATUS TdiOnSend(IN PDEVICE_OBJECT DeviceObject, IN OUT PIRP Irp);
VOID NetSetHook(BOOLEAN bl);
BOOLEAN NetGetHook();
NTKERNELAPI UCHAR* PsGetProcessImageFileName(__in PEPROCESS Process);
BOOLEAN IsProcessName();
#endif // !1

