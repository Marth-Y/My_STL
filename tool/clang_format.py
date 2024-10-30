# -*- coding:utf-8 -*-

import os
import os.path
rootdir = "."                                   # 指明被遍历的文件夹
import subprocess

for parent,dirnames,filenames in os.walk(rootdir):    #三个参数：分别返回1.父目录 2.所有文件夹名字（不含路径） 3.所有文件名字
#  for dirname in  dirnames:                       #输出文件夹信息
#   print "parent is:" + parent
#   print  "dirname is" + dirname
  for filename in filenames:                        #输出文件信息
#    print "parent is:" + parent
#    print "filename is:" + filename
    suffix = filename.split('.')[-1]
    # print filename
    if "cpp" == suffix or "cc" == suffix or "c" == suffix or "h" == suffix or "hpp" == suffix or "hh" == suffix:
      full_filename = os.path.join(parent,filename)
      #print "the full name of the file is:" + full_filename #输出文件路径信息
      cmd = ["./clang-format","-i", "-style=Google", full_filename]
      print(cmd)
      print(subprocess.check_call(cmd))
