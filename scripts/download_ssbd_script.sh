#!/bin/bash

ANNO_FOLDER=/media/data1/video/ssbd/ssbd-release/Annotations/
OUT_FOLDER=/media/data1/video/ssbd/ssbd_raw/

python download_ssbd.py --ann_folder=$ANNO_FOLDER --out_folder=$OUT_FOLDER
