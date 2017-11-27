# -*- coding: utf-8 -*-
"""
Created on Sun Nov 26 20:26:11 2017

@author: tranlaman
"""

import xml.etree.ElementTree as ET
import os
import glob
import argparse

def parse_args():
    """Parse input arguments."""
    parser = argparse.ArgumentParser(description='Download SSBD dataset from Youtube.')
    parser.add_argument('--ann_folder', dest='ann_folder', help='annotation folder.', 
                        default='/media/data1/video/ssbd/ssbd-release/Annotations/', type=str)
    parser.add_argument('--out_folder', dest='out_folder', help='out folder', default='/media/data1/video/ssbd/ssbd_raw/',
                        type=int)

    args = parser.parse_args()

    return args

def main():
    args = parse_args()
    ann_folder = args.ann_folder
    out_folder = args.out_folder
    
    xml_files = glob.glob(os.path.join(ann_folder, '*.xml'))
    xml_files.sort()
    
    for fi in xml_files:
        
        filename = os.path.splitext(os.path.basename(fi))[0]
        tree = ET.parse(fi)
        root = tree.getroot()
        url = root[0].text
        
        print 'Downloading Youtube video %s' % (url)
        
        save_file = os.path.join(out_folder, '%s.avi' % filename)
        if os.path.isfile(save_file):
            continue
        cmd = "youtube-dl -f 'bestvideo' %s -o %s" % (url, save_file)
        
        os.system(cmd)