#include <windows.h>
#include <stdio.h>
#include <lmaccess.h>
#include "beacon.h"
#include "bofdefs.h"
#include "base.c"


DWORD AddUserToGroup(LPCWSTR lpswzServer, LPCWSTR lpswzUserName, LPCWSTR lpswzGroupName)
{
	NET_API_STATUS dwErrorCode = NERR_Success;
	dwErrorCode = NETAPI32$NetGroupAddUser(lpswzServer, lpswzGroupName, lpswzUserName);
	if(NERR_Success != dwErrorCode)
	{
		BeaconPrintf(CALLBACK_ERROR, "Unable to add user to group %lX\n", dwErrorCode);
		return dwErrorCode;
	} 

	internal_printf("User should be added to group\n");

	return dwErrorCode;
}
#ifdef BOF
VOID go( 
	IN PCHAR Buffer, 
	IN ULONG Length 
) 
{
	DWORD dwErrorCode = ERROR_SUCCESS;
	datap parser = {0};
	BeaconDataParse(&parser, Buffer, Length);
	LPCWSTR lpswzHostName = (LPCWSTR)BeaconDataExtract(&parser, NULL); // $4
	LPCWSTR lpswzUserName = (LPCWSTR)BeaconDataExtract(&parser, NULL); // $2
	LPCWSTR lpswzGroupName = (LPCWSTR)BeaconDataExtract(&parser, NULL);// $3

	if(!bofstart())
	{
		return;
	}

	internal_printf("Adding %S to %S\n", lpswzUserName, lpswzGroupName);

	dwErrorCode = AddUserToGroup(lpswzHostName, lpswzUserName, lpswzGroupName);
	if ( ERROR_SUCCESS != dwErrorCode )
	{
		BeaconPrintf(CALLBACK_ERROR, "Adding user to group failed: %lX\n", dwErrorCode);
		goto go_end;
	}
	
	internal_printf("SUCCESS.\n");

go_end:
	printoutput(TRUE);
	
	bofstop();
};
#else
#define TEST_USERNAME L"Guest"
#define TEST_HOSTNAME L""
#define TEST_GROUPNAME L"Administrators"

int main(int argc, char ** argv)
{
	DWORD dwErrorCode = ERROR_SUCCESS;
	LPCWSTR lpswzHostName = TEST_HOSTNAME;
	LPCWSTR lpswzUserName = TEST_USERNAME;
	LPCWSTR lpswzGroupName = TEST_GROUPNAME;
	
	internal_printf("Adding %S to %S\n", lpswzUserName, lpswzGroupName);

	dwErrorCode = AddUserToGroup(lpswzHostName, lpswzUserName, lpswzGroupName);
	if ( ERROR_SUCCESS != dwErrorCode )
	{
		BeaconPrintf(CALLBACK_ERROR, "Adding user to group failed: %lX\n", dwErrorCode);
		goto main_end;
	}

	internal_printf("SUCCESS.\n");

main_end:
	return dwErrorCode;
}
#endif