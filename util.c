#include "util.h"





NTSTATUS IrpNetInternalDeviceControl(IN PDEVICE_OBJECT DeviceObject, IN OUT PIRP Irp)
{
	NTSTATUS status;
	switch (IoGetCurrentIrpStackLocation(Irp)->MinorFunction)
	{
	default:
		status = CallAttachedDevice(DeviceObject, Irp);
		break;
		/*
	case TDI_CONNECT:
		status = TdiOnConnect(DeviceObject, Irp);
		break;
		
	case TDI_ASSOCIATE_ADDRESS:
		status = TdiOnAssociateAddress(DeviceObject, Irp);
		break;
	case TDI_DISASSOCIATE_ADDRESS:
		status = TdiOnDisassociateAddress(DeviceObject, Irp);
		break;
	case TDI_SET_EVENT_HANDLER:
		status = TdiOnSetEventHandler(DeviceObject, Irp);
		break;
	case TDI_RECEIVE:
		status = TdiOnReceive(DeviceObject, Irp);
		break;
	case TDI_RECEIVE_DATAGRAM:
		status = TdiOnReceiveDatagram(DeviceObject, Irp);
		break;
		
	case TDI_SEND:
		status = TdiOnSend(DeviceObject, Irp);
		break;
		*/
		
	case TDI_SEND_DATAGRAM:
		status = TdiOnSendDatagram(DeviceObject, Irp);
		break;
		
	}
	return status;
}


NTSTATUS CallAttachedDevice(IN PDEVICE_OBJECT DeviceObject, IN PIRP Irp)
{
	if (!g_objDeviceTcp)
	{
		Irp->IoStatus.Status = STATUS_UNSUCCESSFUL;
		Irp->IoStatus.Information = 0;
		IoCompleteRequest(Irp, IO_NO_INCREMENT);
		return STATUS_UNSUCCESSFUL;
	}
	IoSkipCurrentIrpStackLocation(Irp);
	return IoCallDriver(g_objDeviceTcp, Irp);
}


NTSTATUS TdiOnSendDatagram(IN PDEVICE_OBJECT DeviceObject, IN OUT PIRP Irp)
{
	NTSTATUS					status;
	PFILE_OBJECT				FileObject;
	PIO_STACK_LOCATION			IrpSp;
	PTDI_REQUEST_KERNEL_SENDDG	TdiRequest;
	ULONG						SendLength;

	IrpSp = IoGetCurrentIrpStackLocation(Irp);
	FileObject = IrpSp->FileObject;
	TdiRequest = (PTDI_REQUEST_KERNEL_SENDDG)(&IrpSp->Parameters);
	SendLength = TdiRequest->SendLength;
	//DbgPrint(NF_DEBUG_NAME ": TdiOnSendDatagram: %u\n", SendLength);
	if (IsProcessName() && NetGetHook() && SendLength<30 && SendLength>19)
	{
		DbgPrint(NF_DEBUG_NAME " À¹½Øµ½³¤¶È:%d", SendLength);
		Irp->IoStatus.Status = STATUS_SUCCESS;
		Irp->IoStatus.Information = 0;

		IoCompleteRequest(Irp, IO_NO_INCREMENT);
		return STATUS_SUCCESS;
	}
	return CallAttachedDevice(DeviceObject, Irp);
}

NTSTATUS TdiOnSend(IN PDEVICE_OBJECT DeviceObject, IN OUT PIRP Irp)
{
	NTSTATUS					status;
	PFILE_OBJECT				FileObject;
	PIO_STACK_LOCATION			IrpSp;
	PTDI_REQUEST_KERNEL_SENDDG	TdiRequest;
	ULONG						SendLength;

	IrpSp = IoGetCurrentIrpStackLocation(Irp);
	FileObject = IrpSp->FileObject;
	TdiRequest = (PTDI_REQUEST_KERNEL_SENDDG)(&IrpSp->Parameters);
	SendLength = TdiRequest->SendLength;
	DbgPrint(NF_DEBUG_NAME ": TdiOnSend: %u\n", SendLength);
	return CallAttachedDevice(DeviceObject, Irp);
}

BOOLEAN IsHook = FALSE;
VOID NetSetHook(BOOLEAN bl)
{
	IsHook = bl;

}

BOOLEAN NetGetHook()
{
	return IsHook;

}

BOOLEAN IsProcessName()
{
	UCHAR* pName = PsGetProcessImageFileName(PsGetCurrentProcess());
	UCHAR* pName2 = "crossfire.exe";

		if (pName[0] == pName2[0] && \
			pName[1] == pName2[1] && \
			pName[2] == pName2[2] && \
			pName[3] == pName2[3] && \
			pName[4] == pName2[4] && \
			pName[5] == pName2[5] && \
			pName[6] == pName2[6] && \
			pName[7] == pName2[7] && \
			pName[8] == pName2[8] && \
			pName[9] == pName2[9] && \
			pName[10] == pName2[10] && \
			pName[11] == pName2[11] && \
			pName[12] == pName2[12]

			)
		{
			return TRUE;
		}

	return FALSE;
}