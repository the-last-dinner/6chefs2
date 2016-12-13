#!/bin/sh
if [ ! -e ./targets ]; then
    echo 'Error: Directory is incorrect.'
    exit 0
fi
mv -i ./targets/* ../../cocosstudio/character
