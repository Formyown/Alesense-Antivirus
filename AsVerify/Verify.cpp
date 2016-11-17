#include "Verify.h"
bool VerifyFileDigitalSignature(LPWSTR FileName)
{

	HANDLE hFileHandle= CreateFile(FileName,            
		GENERIC_READ,                
		FILE_SHARE_READ  ,           
		NULL,                       
		OPEN_EXISTING,             
		FILE_ATTRIBUTE_NORMAL,     
		NULL);       
	HCATADMIN CatAdmin = 0;
	WINTRUST_DATA TrustData = {0};
	WINTRUST_CATALOG_INFO TrustCatalogInfo = {0};
	WINTRUST_FILE_INFO TrustFileInfo = {0};
	CATALOG_INFO CatalogInfo= {0};
	if(!CryptCATAdminAcquireContext(&CatAdmin,NULL,0))
	{
		CloseHandle(hFileHandle);
		return false;
	}
	DWORD HashCount = 20;
	BYTE HashArry[20];
	WCHAR MemberTag[256] = {0};

	for (unsigned int i = 0; i<HashCount; i++)
	{
		wsprintfW(MemberTag + i*2,L"%2X",HashArry[i]);
	}
	//枚举目录指定的哈希
	HCATINFO CatInfo =NULL;
	CatInfo = CryptCATAdminEnumCatalogFromHash(CatAdmin,HashArry,HashCount,0,&CatInfo);
	if (CatInfo == 0 ) 
	{   //未找到包含此文件的安全编目
		TrustFileInfo.cbStruct = sizeof(WINTRUST_FILE_INFO);
		TrustFileInfo.pcwszFilePath = FileName;
		TrustFileInfo.hFile = NULL;
		TrustFileInfo.pgKnownSubject = NULL;

		TrustData.cbStruct = sizeof(WINTRUST_DATA);
		TrustData.dwUnionChoice = WTD_CHOICE_FILE;
		TrustData.pFile = &TrustFileInfo;
		TrustData.dwUIChoice = WTD_UI_NONE;
		TrustData.fdwRevocationChecks = WTD_REVOKE_NONE;
		TrustData.dwStateAction = WTD_STATEACTION_IGNORE;
		TrustData.dwProvFlags = WTD_SAFER_FLAG;
		TrustData.hWVTStateData = NULL;
		TrustData.pwszURLReference = NULL;

	}
	else
	{   //找到包含此文件的安全编目
		CryptCATCatalogInfoFromContext( CatInfo, &CatalogInfo, 0 );
		TrustCatalogInfo.cbStruct = sizeof( WINTRUST_CATALOG_INFO );
		TrustCatalogInfo.pcwszCatalogFilePath = CatalogInfo.wszCatalogFile;
		TrustCatalogInfo.pcwszMemberFilePath = FileName;
		TrustCatalogInfo.pcwszMemberTag = MemberTag;

		TrustData.cbStruct = sizeof( WINTRUST_DATA );
		TrustData.dwUnionChoice = WTD_CHOICE_CATALOG;
		TrustData.pCatalog = &TrustCatalogInfo;
		TrustData.dwUIChoice = WTD_UI_NONE;
		TrustData.fdwRevocationChecks = WTD_STATEACTION_VERIFY;
		TrustData.dwProvFlags = 0;
		TrustData.hWVTStateData = NULL;
		TrustData.pwszURLReference = NULL;
	}

	GUID ActionGuid = WINTRUST_ACTION_GENERIC_VERIFY_V2;
	HRESULT HRes = WinVerifyTrust(NULL,&ActionGuid,&TrustData);
	bool Res = false;
	Res = SUCCEEDED(HRes);
	if (CatInfo != NULL)
	{
		CryptCATAdminReleaseCatalogContext( CatAdmin, CatInfo, 0 );
	}
	CryptCATAdminReleaseContext( CatAdmin,0); 
	CloseHandle(hFileHandle);
	return Res;
}
bool VerifyFileDigitalSignatureByHandle(PWCHAR FileName,HANDLE FileHandle)
{
	HCATADMIN CatAdmin = 0;
	WINTRUST_DATA TrustData = {0};
	WINTRUST_CATALOG_INFO TrustCatalogInfo = {0};
	WINTRUST_FILE_INFO TrustFileInfo = {0};
	CATALOG_INFO CatalogInfo= {0};
	if(!CryptCATAdminAcquireContext(&CatAdmin,NULL,0))
	{
		CloseHandle(FileHandle);
		return false;
	}
	DWORD HashCount = 20;
	BYTE HashArry[20];
	WCHAR MemberTag[256] = {0};

	for (unsigned int i = 0; i<HashCount; i++)
	{
		wsprintfW(MemberTag + i*2,L"%2X",HashArry[i]);
	}
	//枚举目录指定的哈希
	HCATINFO CatInfo =NULL;
	CatInfo = CryptCATAdminEnumCatalogFromHash(CatAdmin,HashArry,HashCount,0,&CatInfo);
	if (CatInfo == 0 ) 
	{   //未找到包含此文件的安全编目
		TrustFileInfo.cbStruct = sizeof(WINTRUST_FILE_INFO);
		TrustFileInfo.pcwszFilePath = FileName;
		TrustFileInfo.hFile = NULL;
		TrustFileInfo.pgKnownSubject = NULL;

		TrustData.cbStruct = sizeof(WINTRUST_DATA);
		TrustData.dwUnionChoice = WTD_CHOICE_FILE;
		TrustData.pFile = &TrustFileInfo;
		TrustData.dwUIChoice = WTD_UI_NONE;
		TrustData.fdwRevocationChecks = WTD_REVOKE_NONE;
		TrustData.dwStateAction = WTD_STATEACTION_IGNORE;
		TrustData.dwProvFlags = WTD_SAFER_FLAG;
		TrustData.hWVTStateData = NULL;
		TrustData.pwszURLReference = NULL;

	}
	else
	{   //找到包含此文件的安全编目
		CryptCATCatalogInfoFromContext( CatInfo, &CatalogInfo, 0 );
		TrustCatalogInfo.cbStruct = sizeof( WINTRUST_CATALOG_INFO );
		TrustCatalogInfo.pcwszCatalogFilePath = CatalogInfo.wszCatalogFile;
		TrustCatalogInfo.pcwszMemberFilePath = FileName;
		TrustCatalogInfo.pcwszMemberTag = MemberTag;

		TrustData.cbStruct = sizeof( WINTRUST_DATA );
		TrustData.dwUnionChoice = WTD_CHOICE_CATALOG;
		TrustData.pCatalog = &TrustCatalogInfo;
		TrustData.dwUIChoice = WTD_UI_NONE;
		TrustData.fdwRevocationChecks = WTD_STATEACTION_VERIFY;
		TrustData.dwProvFlags = 0;
		TrustData.hWVTStateData = NULL;
		TrustData.pwszURLReference = NULL;
	}

	GUID ActionGuid = WINTRUST_ACTION_GENERIC_VERIFY_V2;
	HRESULT HRes = WinVerifyTrust(NULL,&ActionGuid,&TrustData);
	bool Res = false;
	Res = SUCCEEDED(HRes);
	if (CatInfo != NULL)
	{
		CryptCATAdminReleaseCatalogContext( CatAdmin, CatInfo, 0 );
	}
	CryptCATAdminReleaseContext( CatAdmin,0); 
	CloseHandle(FileHandle);
	return Res;
}