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

#include "mfr_types.h"

#if defined(YOCTO_BUILD)
    #include "mfrTypes.h"
#endif

VL_MFR_API_RESULT MFRData_Init (void);

VL_MFR_API_RESULT MFRData_getSerializedData_normalNvram(  VL_NORMAL_NVRAM_DATA_TYPE eType, VL_NVRAM_DATA * pNvRamData);

VL_MFR_API_RESULT MFRData_getSerializedData_secureNvram( VL_SECURE_NVRAM_DATA_TYPE eType, VL_NVRAM_DATA * pNvRamData);

#if defined(YOCTO_BUILD)
    mfrError_t MFRData_getSerializedMfrData(mfrSerializedType_t mfr_type, mfrSerializedData_t *data);
#endif
