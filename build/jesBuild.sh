#!/bin/sh
set -e

################################################################################
#  This program and the accompanying materials are
#  made available under the terms of the Eclipse Public License v2.0 which accompanies
#  this distribution, and is available at https://www.eclipse.org/legal/epl-v20.html
#
#  SPDX-License-Identifier: EPL-2.0
#
#  Copyright Contributors to the Zowe Project.
################################################################################


export _LD_SYSLIB="//'SYS1.CSSLIB'://'CEE.SCEELKEX'://'CEE.SCEELKED'://'CEE.SCEERUN'://'CEE.SCEERUN2'://'CSF.SCSFMOD0'"

echo $_LD_SYSLIB

WORKING_DIR=$(dirname "$0")
ZWE="../.."
COMMON="../../deps/zowe-common-c"


echo "********************************************************************************"

mkdir -p "../bin"
mkdir -p "${WORKING_DIR}/tmp-zwe" && cd "$_"

IFS='.' read -r major minor micro < "${ZWE}/version.txt"
date_stamp=$(date +%Y%m%d)
echo "Version: $major.$minor.$micro"
echo "Date stamp: $date_stamp"

echo "testJM01.c ..."

if c89 \
  -DPRODUCT_MAJOR_VERSION="$major" \
  -DPRODUCT_MINOR_VERSION="$minor" \
  -DPRODUCT_REVISION="$micro" \
  -DPRODUCT_VERSION_DATE_STAMP="$date_stamp" \
  -D_XOPEN_SOURCE=600 \
  -DNOIBMHTTP=1 \
  -D_OPEN_THREADS=1 \
  -DUSE_ZOWE_TLS=1 \
  -DHTTPSERVER_BPX_IMPERSONATION=1 \
  -DAPF_AUTHORIZED=0 \
  -Wc,dll,expo,langlvl\(extc99\),gonum,goff,hgpr,roconst,ASM,asmlib\('CEE.SCEEMAC','SYS1.MACLIB','SYS1.MODGEN'\) \
  -Wc,agg,exp,list\(\),so\(\),off,xref \
  -Wl,ac=1,dll \
  -I /usr/include/zos \
  -I ${COMMON}/h \
  -I ${ZWE}/h \
  -o ${ZWE}/bin/testJM01 \
  ${COMMON}/c/alloc.c \
  ${COMMON}/c/as.c \
  ${COMMON}/c/dynalloc.c \
  ${COMMON}/c/idcams.c \
  ${COMMON}/c/vsam.c \
  ${ZWE}/c/testJM01.c
then
  echo "...Build successful"
  exit 0
else
  # remove testJM01 in case the linker had RC=4 and produced the binary
  rm -f ${ZWE}/bin/testJM01
  echo "Build failed"
  exit 8
fi



echo "...Build successful"
