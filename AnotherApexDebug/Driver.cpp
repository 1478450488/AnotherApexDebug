#include <Driver.h>
#include "ObCall.h"

extern "C" DRIVER_INITIALIZE DriverEntry;
static DRIVER_UNLOAD DriverUnload;
static HANDLE g_thread[20] = {nullptr };
static ULONG g_thread_num = 0;
_Use_decl_annotations_ static void DriverUnload(PDRIVER_OBJECT a_driver_object)
{
	//ж������
	UNREFERENCED_PARAMETER(a_driver_object);
	PAGED_CODE();
	//��ע��OB
	UnRegObCall();
	//�˳�ǰ����Ҫ�ָ�EAC�������̣߳��һָ�����Ҫ����Ϸ�˳�ǰ������ڶ���������Ϸ��ʧ�ܣ�ֻ��������
	for (ULONG i = 0; i < g_thread_num; i++)
	{
		BDBig::ResumeThread(g_thread[i]);
		ZwClose(g_thread);
	}
}
_Use_decl_annotations_ auto DriverEntry(PDRIVER_OBJECT a_driver_object, \
	PUNICODE_STRING a_reg_path) -> NTSTATUS
{
	UNREFERENCED_PARAMETER(a_reg_path);
	PAGED_CODE();
	auto v_ret_status = STATUS_SUCCESS;
	util::BypassCheckSign(a_driver_object);
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
	//��д�������ļ�����
	//����PChunter������
	util::HidePCHDriverDepsSelf(a_driver_object);
	//ö���߳��Ժ���ͣ
	ULONG64 v_tid[0x256] = { 0 };
	BDBig::GetDriverThread("EasyAntiCheat.sys", &g_thread_num, v_tid);
	for (ULONG i = 0; i < g_thread_num; i++)
	{
		g_thread[i] = BDBig::OpenThread(THREAD_ALL_ACCESS, FALSE, reinterpret_cast<DWORD>(g_thread[i]));
		BDBig::SuspendThread(g_thread[i]);
	}
	//�ָ���дȨ��
	RegObCall();
	//TODO:
	//Still work in progess
	//Remove EAC Obcall
	//Remove EAC Notify

	return v_ret_status;
}