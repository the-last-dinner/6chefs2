#!/bin/bash

# check argument
if [ -n $1 ];then
    modelName="nadeshiko"
else
    modelName=$1
fi
modelPath="../../cocosstudio/character/"

# check existing model
if [ ! -f "${modelPath}${modelName}.csd" ];then
    echo "Model csd file is not exists!"
    echo "Model name >> ${modelName}"
    exit;
fi

# generate csd files
targets=$(ls targets/*.plist)
for target in $targets;do
    targetName=$(echo $target | cut -d '/' -f 2 | cut -d '.' -f1)
    csdPath="targets/${targetName}.csd"
    udfPath="targets/${targetName}.udf"
    cp ${modelPath}${modelName}.csd ${csdPath}
    cp ${modelPath}${modelName}.udf ${udfPath}
    sed -i '.back' -e "s/${modelName}/${targetName}/g" ${csdPath} ${udfPath}
    echo "${targetName} done!"
    rm targets/${targetName}.*.back
done
