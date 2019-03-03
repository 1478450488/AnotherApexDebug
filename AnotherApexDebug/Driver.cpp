#include <Driver.h>
extern "C"
{
#include <ntifs.h>
#include <ntintsafe.h>
#include <ntddk.h>
#include <intrin.h>
}

#include <Wdk.h>

extern "C" DRIVER_INITIALIZE DriverEntry;
static DRIVER_UNLOAD DriverUnload;

_Use_decl_annotations_ static void DriverUnload(PDRIVER_OBJECT a_driver_object)
{
	//ж������
	UNREFERENCED_PARAMETER(a_driver_object);
	PAGED_CODE();
	//�˳�ǰ����Ҫ�ָ�EAC�������̣߳��һָ�����Ҫ����Ϸ�˳�ǰ������ڶ���������Ϸ��ʧ�ܣ�ֻ������
}
_Use_decl_annotations_ auto DriverEntry(PDRIVER_OBJECT a_driver_object, \
	PUNICODE_STRING a_reg_path) -> NTSTATUS
{
	UNREFERENCED_PARAMETER(a_reg_path);
	PAGED_CODE();
	auto v_ret_status = STATUS_SUCCESS;
	for (;;)
	{
		//Thanks Meesong for WDKExt
		v_ret_status = wdk::WdkInitSystem();
		if (!NT_SUCCESS(v_ret_status))
		{
			break;
		}
		a_driver_object->DriverUnload = DriverUnload;
		break;
	}
	KdPrint(("load\n"));
	//Ϊ�˸���������Ӧ����PCHunter���������������Ǽ��ص�ʱ�����ж�PCHunter�������Ƿ����,
	//���PCHunter�������Ѿ������أ���ô���Ƕ����������أ��Զ��EAC��ɨ��
	return v_ret_status;

}