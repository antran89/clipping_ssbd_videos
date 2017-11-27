Clip SSBD dataset tool
=======================

HOW-TO USE
----------
This repository contains scripts to clip Self-Stimulatory Behaviours Dataset (SSBD) dataset.
First, download the SSBD dataset release from http://staff.estem-uc.edu.au/roland/research/datasets/ssbd/.

Compile the code:
```
mkdir src-build
cd src-build
cmake ../src
```

Modify the path in `scripts` directories:

* Modify and run download_ssbd_scripts.sh to download the dataset:
```
bash download_ssbd_scripts.sh
```

* Organize the download folders into different folders with 3 classes: ArmFlapping, HeadBanging, Spinning.

* Modify and run clip_segment_ssbd_video_script.sh to clip the dataset:
```
bash clip_segment_ssbd_video_script.sh
```

ALTERNATIVES:
--------------
You can download the dataset from the [link]().

License and Citation
---------------------

The code is available under GPL 3 license.

If you find the tool and data useful in your research, please consider citing:
```
@inproceedings{YeLuo_ICCV_2015,
  author    = {Ye Luo and
               Loong-Fah Cheong and
               An Tran},
  title     = {Actionness-assisted Recognition of Actions},
  booktitle = {The IEEE International Conference on Computer Vision (ICCV)},
  year      = {2015},
}
```

and

```
Self-Stimulatory Behaviours in the Wild for Autism Diagnosis
Shyam Sundar Rajagopalan, Abhinav Dhall, Roland Goecke
The IEEE International Conference on Computer Vision (ICCV), 2013
```