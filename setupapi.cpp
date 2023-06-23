bool WINAPI SetupDiGetDevicePropertyW(HDEVINFO hDevInfo,SP_DEVINFO_DATA  *DevInfoData,
                    PDEVPROPKEY DEVPKEY,DEVPROPTYPE *__w64 PropertyType,
					wchar_t *Buffer,int size,int *cbBuffer,int x) {
	ULONG ulRegDataType;
	unsigned long reqSz = (unsigned long)size;	
	int r  = CM_Get_DevNode_Registry_PropertyW(DevInfoData->DevInst,DEVPKEY->pid - 1,
                            &ulRegDataType,Buffer,(unsigned long*) &reqSz,0);
		if(r== CR_SUCCESS ){
	
							if(ulRegDataType == REG_SZ) {
								*PropertyType = DEVPROP_TYPE_STRING;
							} else if(ulRegDataType == REG_EXPAND_SZ) {
								wchar_t *buf = new wchar_t[size+1];
								int sz= ExpandEnvironmentStringsW(Buffer,buf,size);
								if(cbBuffer){
									*cbBuffer = sz;
									if (size>=sz) {
										lstrcpyW(Buffer,buf);
									}
									delete []buf;
								}
								*PropertyType = DEVPROP_TYPE_STRING;
							} else if(ulRegDataType == REG_BINARY) {
								*PropertyType = DEVPROP_TYPE_BINARY;
							} else if(ulRegDataType == REG_DWORD_LITTLE_ENDIAN) {
								*PropertyType = 	DEVPROP_TYPE_UINT32;
							} else if(ulRegDataType == REG_QWORD_LITTLE_ENDIAN) {
								*PropertyType = 	DEVPROP_TYPE_UINT64;
							} else if(ulRegDataType == REG_MULTI_SZ) {
								*PropertyType = 	DEVPROP_TYPE_STRING_LIST;
							}
							return true;
		}	
		return false;
}
