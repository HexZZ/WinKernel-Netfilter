#ifndef __DriverMain__
#define __DriverMain__
#include <ntddk.h>
#include "util.h"

#define NF_DEVICE_NAME		L"\\Device\\Netfilter"
#define NF_SYMBOLIC_LINK	L"\\DosDevices\\Netfilter"
#define NF_DEBUG_NAME "netfilter"


#define NF_DEVICE_TCP		L"\\Device\\Tcp"
#define NF_DEVICE_UDP		L"\\Device\\Udp"
#define NF_DEVICE_RAWIP		L"\\Device\\RawIp"

#define IOCTL_NETSTART 0x100008
#define IOCTL_NETSTOP 0x100009
extern PDEVICE_OBJECT g_objDevice;
extern PDEVICE_OBJECT g_objDeviceTcp;
extern PDEVICE_OBJECT g_objDeviceUdp;
extern PDEVICE_OBJECT g_objDeviceRawIp;


NTSTATUS NotDispatch(PDEVICE_OBJECT DeviceObject, PIRP Irp);
void DriverUnload(IN PDRIVER_OBJECT DriverObject);
NTSTATUS IrpInternalDeviceControl(PDEVICE_OBJECT DeviceObject, PIRP Irp);
NTSTATUS IrpDeviceControl(PDEVICE_OBJECT DeviceObject, PIRP Irp);
NTSTATUS CreateClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp);
typedef struct _NK_DEVICE_EXTENSION
{
	PDEVICE_OBJECT		DeviceObject;
	PDEVICE_OBJECT		AttachedDevice;
	PFILE_OBJECT		FileObject;
}NK_DEVICE_EXTENSION, * PNK_DEVICE_EXTENSION;


#endif  __DriverMain__
//#define __DriverMain__