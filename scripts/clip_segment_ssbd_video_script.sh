#!/bin/bash

python clip_ssbd_video_segments.py --ann_folder=/media/data1/video/ssbd/ssbd-release/Annotations/ \
--origin_folder=/media/data1/video/ssbd/ssbd_raw/ --out_folder=/media/data1/video/ssbd/ssbd_clip_segment/ \
--height=240 --width=320

