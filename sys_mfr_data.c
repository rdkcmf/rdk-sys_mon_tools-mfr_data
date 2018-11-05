/*
 * If not stated otherwise in this file or this component's Licenses.txt file the
 * following copyright and licenses apply:
 *
 * Copyright 2017 RDK Management
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "rdk_debug.h"
#include "sys_mfr_data.h"

#if defined(USE_MOT_MFR)
#ifdef __cplusplus
extern "C" {
#endif
    VL_MFR_API_RESULT motorola_mfr_init(void);
    VL_MFR_API_RESULT motorola_mfr_set_notify(VL_MFR_NOTIFY_TYPE_t notifyType, VL_MFR_VOID_CBFUNC_t pfnNotifyFunc);

    VL_MFR_API_RESULT motorola_mfr_get_version_name(VL_PLATFORM_VERSION_TYPE eVersionType, char ** ppString);
    VL_MFR_API_RESULT motorola_mfr_get_version     (VL_PLATFORM_VERSION_TYPE eVersionType, char ** ppString);

    VL_MFR_API_RESULT motorola_mfr_read_normal_nvram (VL_NORMAL_NVRAM_DATA_TYPE eType, VL_NVRAM_DATA * pNvRamData);
    VL_MFR_API_RESULT motorola_mfr_write_normal_nvram(VL_NORMAL_NVRAM_DATA_TYPE eType, const VL_NVRAM_DATA * pNvRamData);

    VL_MFR_API_RESULT motorola_mfr_read_secure_nvram (VL_SECURE_NVRAM_DATA_TYPE eType, VL_NVRAM_DATA * pNvRamData);
    VL_MFR_API_RESULT motorola_mfr_write_secure_nvram(VL_SECURE_NVRAM_DATA_TYPE eType, const VL_NVRAM_DATA * pNvRamData);

    VL_MFR_API_RESULT motorola_mfr_set_mfr_data(VL_STACK_2_MFR_EVENT_TYPE eEvent, void * _pvData);
    VL_MFR_API_RESULT motorola_mfr_get_mfr_data(VL_STACK_2_MFR_EVENT_TYPE eEvent, void * _pvData);
#ifdef __cplusplus
}
#endif
#elif defined(USE_BRCM_MFR)
#include "broadcom_sample_mfr_api.h"
#ifdef LEGACY_NEXUS
#include "broadcom_mfr_nvram_api.c"
#endif
#include "broadcom_DFAST2_prod_gfe_ext_v3.2.h"
#include "broadcom_DFAST2_prod_header_v3.2.h"

#elif defined(USE_PACE_MFR)
#include "pace_mfr_api.h"
#include "pace_mfr_nvram_api.h"
#include "pace_DFAST2_prod_gfe_ext_v3.2.h"
#include "pace_DFAST2_prod_header_v3.2.h"

#elif defined(USE_VIVID_MFR)
#include "vivid_sample_mfr_api.h"
#include "vivid_DFAST2_prod_gfe_ext_v3.2.h"
#include "vivid_DFAST2_prod_header_v3.2.h"

#elif defined(USE_INTEL_MFR)
#include "intel_sample_mfr_api.h"
#include "intel_DFAST2_prod_gfe_ext_v3.2.h"
#include "intel_DFAST2_prod_header_v3.2.h"

#elif defined(USE_THOMSON_MFR)
#include "thomson_sample_mfr_api.h"
#include "thomson_DFAST2_prod_gfe_ext_v3.2.h"
#include "thomson_DFAST2_prod_header_v3.2.h"

#elif defined(USE_SAM_MFR)
#include "sam_mfr_api.h"
#include "iarmUtil.h"
#include "libIARM.h"
#include "libIBus.h"
#define IARM_BUS_SAMMFRDATA_NAME "SamMfrData"
static mfrError_t mfrdata_getSamsungMfrSerializedDataDummy(mfrSerializedType_t mfr_type, mfrSerializedData_t *data);
#elif defined(USE_CISCOXID_MFR)
#include "mfrTypes.h"
#else
#include "vl_sample_mfr_api.h"
#include "vl_DFAST2_eval_gfe_ext_v3_2.h"
#include "vl_DFAST2_eval_header_v3_2.h"
#endif

#if defined(USE_SAM_MFR) || defined(USE_PACEXG1V3_MFR)
static mfrError_t mfrdata_setFailureFlag( );
#endif

/*===================================================================
FUNCTION: MFRData_Init

DESCRIPTION:
    Initialize HAL MFR
RETURN VALUE:
   0 - Success;  Negative values - Failure
SIDE EFFECTS:
   None.
===================================================================*/
VL_MFR_API_RESULT MFRData_Init (void)
{
    VL_MFR_API_RESULT nRet = VL_MFR_API_RESULT_SUCCESS;
    static int iMFRDatataInit = 0;
#if defined(USE_PACEXG1V3_MFR)
    FILE *fbootXreSplash = NULL;
    const char *XRESPLASH_BOOTFPATH="/tmp/.xreSplashDrawn";
#endif

    RDK_LOG(RDK_LOG_DEBUG, "LOG.RDK.SYS_MON_TOOLS", "....................MFR_Init() called....................\n");

    if(!iMFRDatataInit)
    {
#if defined(USE_MOT_MFR)
        nRet = motorola_mfr_init();

#elif defined(USE_BRCM_MFR)
        nRet = broadcom_mfr_init();
        nRet = VL_MFR_API_RESULT_SUCCESS;

#elif defined(USE_PACE_MFR)
        nRet = pace_mfr_init();
        RDK_LOG(RDK_LOG_DEBUG,"pace_mfr_init VL_MFR_API_RESULT_SUCCESS[%d], Actual Ret[%d]", VL_MFR_API_RESULT_SUCCESS, nRet);
        if (VL_MFR_API_RESULT_SUCCESS == nRet)
        {
            nRet = pace_mfr_nvram_init();
            RDK_LOG(RDK_LOG_DEBUG,"pace_mfr_nvram_init VL_MFR_API_RESULT_SUCCESS[%d], Actual Ret[%d]", VL_MFR_API_RESULT_SUCCESS, nRet);
        }
        else
        {
            RDK_LOG(RDK_LOG_DEBUG,"%s: pace_mfr_init returned:%d\n", __FUNCTION__, nRet);
        }

#if defined(USE_PACEXG1V3_MFR)
        /*
        ** On boot SNMP-D certificate decryption is failing on boot due to mfr hook validation failure.
        ** PaceXG1V3 mfr init is failing though API return success during boot time.
        ** .xreSplashDrawn will be set on later part hence checking non-existance of file as workaround.
        */
        fbootXreSplash = fopen(XRESPLASH_BOOTFPATH, "r");
        if((nRet != VL_MFR_API_RESULT_SUCCESS) || (fbootXreSplash == NULL))
        {
            mfrdata_setFailureFlag( );
        }
        if(fbootXreSplash)
        {
            fclose(fbootXreSplash);
            fbootXreSplash = NULL;
        }
#endif

#elif defined(USE_INTEL_MFR)
        intel_mfr_init();

#elif defined(USE_THOMSON_MFR)
        thomson_mfr_init();

#elif defined(USE_VIVID_MFR)
        vivid_mfr_init();

#elif defined(USE_SAM_MFR)
        sam_mfr_init();
#elif defined(USE_CISCOXID_MFR)
        mfr_init();
#else
        RDK_LOG(RDK_LOG_DEBUG, "LOG.RDK.SYS_MON_TOOLS", "vl_mfr_init() Not implemented \n");
#endif
        iMFRDatataInit = 1;
    }

    RDK_LOG(RDK_LOG_DEBUG,"LOG.RDK.SYS_MON_TOOLS","%s(): Exit\n", __FUNCTION__);
    return (VL_MFR_API_RESULT_SUCCESS); /*Always returning success due to issues in boot*/
}

VL_MFR_API_RESULT MFRData_getSerializedData_normalNvram(  VL_NORMAL_NVRAM_DATA_TYPE eType, VL_NVRAM_DATA * pNvRamData)
{

    VL_MFR_API_RESULT result = VL_MFR_API_RESULT_FAILED;

    RDK_LOG(RDK_LOG_DEBUG, "LOG.RDK.SYS_MON_TOOLS", "MFRData_getSerializedData_normalNvram() called \n");
/*
** Cisco XID the API's are not available.
*/
#if !defined(USE_CISCOXID_MFR)

    if(pNvRamData)
    {
#if defined(USE_MOT_MFR)
        result = motorola_mfr_read_normal_nvram(eType, pNvRamData);
#elif defined(USE_BRCM_MFR)
        result = broadcom_mfr_read_normal_nvram(eType, pNvRamData);
#elif defined(USE_PACE_MFR)
        result = pace_mfr_read_normal_nvram(eType, pNvRamData);
#elif defined(USE_INTEL_MFR)
        result = intel_mfr_read_normal_nvram(eType, pNvRamData);
#elif defined(USE_THOMSON_MFR)
        result = thomson_mfr_read_normal_nvram(eType, pNvRamData);
#elif defined(USE_VIVID_MFR)
        result = vivid_mfr_read_normal_nvram(eType, pNvRamData);
#elif defined(USE_SAM_MFR)
        result = sam_mfr_read_normal_nvram(eType, pNvRamData);
#else
        result = VL_MFR_API_RESULT_SUCCESS;
        RDK_LOG(RDK_LOG_DEBUG, "LOG.RDK.SYS_MON_TOOLS", "vl_mfr_read_normal_nvram() Not implemented \n");
#endif
    }

#endif /*!Cisco Xid*/

    return result;
}


VL_MFR_API_RESULT MFRData_getSerializedData_secureNvram( VL_SECURE_NVRAM_DATA_TYPE eType, VL_NVRAM_DATA * pNvRamData)
{

    VL_MFR_API_RESULT result = VL_MFR_API_RESULT_FAILED;
    RDK_LOG(RDK_LOG_DEBUG, "LOG.RDK.SYS_MON_TOOLS", "MFRData_getSerializedData_secureNvram() called \n");

/*
** Cisco XID the API's are not available.
*/
#if !defined(USE_CISCOXID_MFR)

    if(pNvRamData)
    {
#if defined(USE_MOT_MFR)
        result = motorola_mfr_read_secure_nvram(eType, pNvRamData);
        if(VL_MFR_API_RESULT_SUCCESS != result)
        {
            printf("%s(): NOT forcing success, eType: 0x%x\n", __func__, eType);
        }
#elif defined(USE_BRCM_MFR)
        result = broadcom_mfr_read_secure_nvram(eType, pNvRamData);
#elif defined(USE_PACE_MFR)
        result = pace_mfr_read_secure_nvram(eType, pNvRamData);
#elif defined(USE_INTEL_MFR)
        result = intel_mfr_read_secure_nvram(eType, pNvRamData);
#elif defined(USE_THOMSON_MFR)
        result = thomson_mfr_read_secure_nvram(eType, pNvRamData);
#elif defined(USE_VIVID_MFR)
        result = vivid_mfr_read_secure_nvram(eType, pNvRamData);
#elif defined(USE_SAM_MFR)
        result = sam_mfr_read_secure_nvram(eType, pNvRamData);
#else
        result = VL_MFR_API_RESULT_SUCCESS;
        RDK_LOG(RDK_LOG_DEBUG, "LOG.RDK.SYS_MON_TOOLS", "vl_mfr_read_secure_nvram() Not implemented \n");
#endif
    }

#endif /*End !CiscoXiD*/

    return result;
}

#if defined(USE_SAM_MFR) || defined(USE_PACEXG1V3_MFR)

#define MAX_MODELNAME_LEN 14
/*
**
** SAMSUNG XG2V2 MFR initialization require call to iArmBus to get initialized.
** On boot the iArm might not be initialized hence require a work around to init mfr.
** Use this function with descretion, touching file /tmp/MFRfail to indicate mfr call Failure.
** Leaving this test code here in case required to debug DELIA-21795.
**
*/
static mfrError_t mfrdata_setFailureFlag( )
{
    const char *DEVICE_PROPERTIES="/etc/device.properties";
    mfrError_t status = mfrERR_NONE;
    const char *MODELNAME_STRING="FW_VERSION_TAG1";
    char *bufSam = (char *) NULL;
    FILE *devicePropsPtr = NULL ;
    char *line = NULL;
    size_t len = 0;
    char *pModelName = NULL;
    int ModelNameLen = 0;

    system("touch /tmp/MFRfail");
#if 0
    if(data)
    {
        data->bufLen = 0;
        data->freeBuf = NULL;
        data->buf = NULL;

        devicePropsPtr = fopen( DEVICE_PROPERTIES , "r");
        if ( NULL == devicePropsPtr ) {
            printf("Unable to open client config file\n");
        } else {
           while (getline(&line, &len, devicePropsPtr) != -1) {
               char *properties = NULL ;
               if ((properties = strstr(line,MODELNAME_STRING))) {
                   char *tempConfigValue;
                   strtok_r(properties, "=", &tempConfigValue);
                   pModelName = strtok_r(NULL, "=", &tempConfigValue);
                   break ;
               }
           }
           fclose(devicePropsPtr);
           if(pModelName){
               ModelNameLen = strnlen( pModelName, MAX_MODELNAME_LEN);
               data->buf = malloc(ModelNameLen+1);
               if(data->buf) {
                   memset(data->buf, 0, ModelNameLen+1);
                   data->bufLen = ModelNameLen;
                   data->freeBuf = free;
                   snprintf(data->buf, ModelNameLen, "%s", pModelName);
                   printf("MFRData_getSerializedMfrData() failed Using mfrdata_getSamsungMfrSerializedDataDummyData for[%d] is[%s]\n", mfr_type, data->buf);
               }
           }
        }
    }
#endif
    return status;
}
#endif

mfrError_t MFRData_getSerializedMfrData(mfrSerializedType_t mfr_type, mfrSerializedData_t *data)
{
    mfrError_t status = mfrERR_NONE;

#if defined(YOCTO_BUILD)

    printf("LOG.RDK.SYS_MON_TOOLS", "MFRData_getSerializedMfrData() called \n");

#if defined(USE_SAM_MFR)
    IARM_Result_t retCode = IARM_RESULT_SUCCESS;
    retCode = IARM_Bus_Init(IARM_BUS_SAMMFRDATA_NAME);
    if (retCode != IARM_RESULT_SUCCESS)
    {
        printf("Error initializing IARM. error code : %d\n", retCode);
    }
    else
    {
        retCode = IARM_Bus_Connect();
        if (retCode != IARM_RESULT_SUCCESS)
        {
            printf("Error connecting to IARM bus. error code : %d\n", retCode);
            IARM_Bus_Term();
            return mfrERR_GENERAL;
        }
    }
#endif

    if(data)
    {
        status = mfrGetSerializedData( mfr_type, data);
        if( status != mfrERR_NONE ){
            RDK_LOG(RDK_LOG_DEBUG, "LOG.RDK.SYS_MON_TOOLS", "MFRData_getSerializedMfrData() failed \n");
#if defined(USE_SAM_MFR) || defined(USE_PACEXG1V3_MFR)
            status = mfrdata_setFailureFlag( );
#endif
        }
        else{
           RDK_LOG(RDK_LOG_DEBUG, "LOG.RDK.SYS_MON_TOOLS", "MFRData_getSerializedMfrData() Success \n");
        }
    }

#if defined(USE_SAM_MFR)
    IARM_Bus_Disconnect();
    IARM_Bus_Term();
#endif

#endif
    return status;
}

