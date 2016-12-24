#!/bin/sh

ROOT_PATH=`pwd`
RESOUCES_PATH=${ROOT_PATH}/../../Resources
TMP_PATH=${ROOT_PATH}/tmp

# generate target list of encryption
for target in config event csv save; do
    ls ${RESOUCES_PATH}/${target} > ${TMP_PATH}/${target}.list
done

# output root path
echo "${RESOUCES_PATH}/" > ${TMP_PATH}/root_path.txt
