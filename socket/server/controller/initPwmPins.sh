#!/bin/sh

fileNamePrefix=/sys/devices/bone_capemgr.
fileNameSuffix=/slots
fileNameIndex=[89]
# Due to boot sequence the index may be 8 or 9 and there will be only one file
filename=${fileNamePrefix}${fileNameIndex}${fileNameSuffix}
echo $filename
echo am33xx_pwm > $filename
echo bone_pwm_P8_13 > $filename
echo bone_pwm_P8_19 > $filename
echo bone_pwm_P9_14 > $filename
echo bone_pwm_P9_16 > $filename
echo bone_pwm_P9_21 > $filename
echo bone_pwm_P9_22 > $filename

