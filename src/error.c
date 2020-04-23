/*
 * OCILIB - C Driver for Oracle (C Wrapper for Oracle OCI)
 *
 * Website: http://www.ocilib.net
 *
 * Copyright (c) 2007-2020 Vincent ROGIER <vince.rogier@ocilib.net>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "error.h"

#include "threadkey.h"
#include "macro.h"

/* --------------------------------------------------------------------------------------------- *
 * ErrorCreate
 * --------------------------------------------------------------------------------------------- */

OCI_Error * ErrorCreate
(
    void
)
{
    return (OCI_Error *) calloc(1, sizeof(OCI_Error));
}

/* --------------------------------------------------------------------------------------------- *
 * ErrorFree
 * --------------------------------------------------------------------------------------------- */

void ErrorFree
(
    OCI_Error *err
)
{
    if (err == &OCILib.lib_err)
    {
        return;
    }

    if (err)
    {
        free(err);
    }
}

/* --------------------------------------------------------------------------------------------- *
 * ErrorReset
 * --------------------------------------------------------------------------------------------- */

void ErrorReset
(
    OCI_Error *err
)
{
    if (err)
    {
        err->raise      = FALSE;
        err->active     = FALSE;
        err->con        = NULL;
        err->stmt       = NULL;
        err->sqlcode    = 0;
        err->libcode    = 0;
        err->type       = 0;
        err->str[0]     = 0;
    }
}

/* --------------------------------------------------------------------------------------------- *
 * ErrorGet
 * --------------------------------------------------------------------------------------------- */

OCI_Error * ErrorGet
(
    boolean check,
    boolean reset
)
{
    OCI_Error *err = NULL;

    if (OCILib.loaded && OCI_LIB_THREADED)
    {
        if (OCI_ThreadKeyGet(OCILib.key_errs, (void **)(dvoid *)&err))
        {
            if (!err)
            {
                err = ErrorCreate();

                if (err)
                {
                    OCI_ThreadKeySet(OCILib.key_errs, err);
                }
            }
        }
    }
    else
    {
        err = &OCILib.lib_err;
    }

    if (check && err && err->active)
    {
        err = NULL;
    }

    // Reset error in case OCI_ENV_CONTEXT is no used
    if (reset && err && err->depth == 0 && err->type != OCI_UNKNOWN)
    {
        ErrorReset(err);
    }

    return err;
}

/* --------------------------------------------------------------------------------------------- *
 * ErrorGetString
 * --------------------------------------------------------------------------------------------- */

const otext * ErrorGetString
(
    OCI_Error *err
)
{
    OCI_CHECK(NULL == err, NULL);

    return err->str;
}

/* --------------------------------------------------------------------------------------------- *
 * ErrorGetType
 * --------------------------------------------------------------------------------------------- */

unsigned int ErrorGetType
(
    OCI_Error *err
)
{
    OCI_CHECK(NULL == err, OCI_UNKNOWN);

    return err->type;
}

/* --------------------------------------------------------------------------------------------- *
 * ErrorGetOCICode
 * --------------------------------------------------------------------------------------------- */

int ErrorGetOCICode
(
    OCI_Error *err
)
{
    OCI_CHECK(NULL == err, OCI_UNKNOWN);

    return (int) err->sqlcode;
}

/* --------------------------------------------------------------------------------------------- *
 * ErrorGetInternalCode
 * --------------------------------------------------------------------------------------------- */

int ErrorGetInternalCode
(
    OCI_Error *err
)
{
    OCI_CHECK(NULL == err, 0);

    return err->libcode;
}

/* --------------------------------------------------------------------------------------------- *
 * ErrorGetConnection
 * --------------------------------------------------------------------------------------------- */

OCI_Connection * ErrorGetConnection
(
    OCI_Error *err
)
{
    OCI_CHECK(NULL == err, NULL);

    return err->con;
}

/* --------------------------------------------------------------------------------------------- *
 * ErrorGetStatement
 * --------------------------------------------------------------------------------------------- */

OCI_Statement * ErrorGetStatement
(
    OCI_Error *err
)
{
    OCI_CHECK(NULL == err, NULL);

    return err->stmt;
}

/* --------------------------------------------------------------------------------------------- *
 * ErrorGetRow
 * --------------------------------------------------------------------------------------------- */

unsigned int ErrorGetRow
(
    OCI_Error *err
)
{
    OCI_CHECK(NULL == err, 0);

    return err->row;
}
