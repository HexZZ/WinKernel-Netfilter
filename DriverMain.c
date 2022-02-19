#include "DriverMain.h"






PDEVICE_OBJECT g_objDeviceTcp;
PDEVICE_OBJECT g_objDevice;

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{

	NTSTATUS status = STATUS_SUCCESS;
	UNICODE_STRING     DeviceName, Win32Device;
	DEVICE_OBJECT DeviceObject;

	UNICODE_STRING			TargetDeviceString;
	PNK_DEVICE_EXTENSION	DeviceExt;
	RtlInitUnicodeString(&DeviceName, NF_DEVICE_NAME);
	RtlInitUnicodeString(&Win32Device, NF_SYMBOLIC_LINK);
	DriverObject->DriverUnload = DriverUnload;


	for (int i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		DriverObject->MajorFunction[i] = NotDispatch;
	}
	//DriverObject->MajorFunction[IRP_MJ_CREATE] = CreateClose;
	//DriverObject->MajorFunction[IRP_MJ_CLOSE] = CreateClose;
	DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = IrpDeviceControl;
	DriverObject->MajorFunction[IRP_MJ_INTERNAL_DEVICE_CONTROL] = IrpInternalDeviceControl;

	status = IoCreateDevice(DriverObject,
		0,
		&DeviceName,
		FILE_DEVICE_UNKNOWN,
		0,
		TRUE,
		&DeviceObject);
	//DriverObject->Flags |= DO_BUFFERED_IO;
	if (!NT_SUCCESS(status))
	{
		DbgPrint(NF_DEBUG_NAME ": create device failure:  0x%08lx\n", status);
		return status;
	}
	
	//(&DeviceObject)->AlignmentRequirement = FILE_WORD_ALIGNMENT;
	status = IoCreateSymbolicLink(&Win32Device, &DeviceName);
	if (!NT_SUCCESS(status))
	{
		DbgPrint(NF_DEBUG_NAME ": create Symbolic failure: 0x%08lx\n", status);
		IoDeleteDevice(&DeviceObject);
		return status;
	}
	
	RtlInitUnicodeString(&TargetDeviceString, NF_DEVICE_UDP);
	status = IoCreateDevice(DriverObject,
		0,
		NULL,
		FILE_DEVICE_NETWORK,
		FILE_DEVICE_SECURE_OPEN,
		FALSE,
		&g_objDevice);


	status = IoAttachDevice(g_objDevice, &TargetDeviceString, &g_objDeviceTcp);
	
	if (!NT_SUCCESS(status))
	{
		IoDeleteSymbolicLink(&Win32Device);
		IoDeleteDevice(&DeviceObject);
		DbgPrint(NF_DEBUG_NAME "IoAttachDevice Failed 0x%08lx", status);
	}



	return status;

}


NTSTATUS IrpDeviceControl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	PIO_STACK_LOCATION	 irpStack;
	ULONG				 ioControlCode;
	irpStack = IoGetCurrentIrpStackLocation(Irp);
	ioControlCode = irpStack->Parameters.DeviceIoControl.IoControlCode;
	switch (ioControlCode)
	{
	case IOCTL_NETSTART:
		NetSetHook(TRUE);
		break;
	case IOCTL_NETSTOP:
		NetSetHook(FALSE);
		break;


	}

	IoSkipCurrentIrpStackLocation(Irp);
	return IoCallDriver(g_objDeviceTcp, Irp);
}
NTSTATUS IrpInternalDeviceControl(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{

	if (DeviceObject == g_objDevice)
	{
		//IoSkipCurrentIrpStackLocation(Irp);
		return IrpNetInternalDeviceControl(DeviceObject, Irp);
	}
	else
	{
		Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
		Irp->IoStatus.Information = 0;

		IoCompleteRequest(Irp, IO_NO_INCREMENT);
		return STATUS_INVALID_PARAMETER;
	}
	


}

NTSTATUS NotDispatch(PDEVICE_OBJECT DeviceObject, PIRP Irp)
{
	/*
	if (DeviceObject == g_objDevice || DeviceObject == g_objDeviceTcp)
	{
		IoSkipCurrentIrpStackLocation(Irp);
		return IoCallDriver(g_objDeviceTcp, Irp);
	}
	else
	{
		Irp->IoStatus.Status = STATUS_INVALID_PARAMETER;
		Irp->IoStatus.Information = 0;

		IoCompleteRequest(Irp, IO_NO_INCREMENT);
		return STATUS_INVALID_PARAMETER;
	}
	*/
	IoSkipCurrentIrpStackLocation(Irp);
	return IoCallDriver(g_objDeviceTcp, Irp);
}

NTSTATUS CreateClose(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	Irp->IoStatus.Status = STATUS_SUCCESS;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}



void DriverUnload(IN PDRIVER_OBJECT DriverObject)
{
	DbgPrint("Ð¶ÔØ³É¹¦");
	UNICODE_STRING Win32Device;

	IoDetachDevice(g_objDeviceTcp);
	//IoDeleteDevice(g_objDeviceTcp);
	IoDeleteDevice(g_objDevice);
	RtlInitUnicodeString(&Win32Device, NF_SYMBOLIC_LINK);
	IoDeleteSymbolicLink(&Win32Device);
	IoDeleteDevice(DriverObject->DeviceObject);
	
	
	
}