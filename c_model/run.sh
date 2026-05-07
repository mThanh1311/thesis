#!/bin/bash

LOG=run.log

echo "========== RUN $(date) ==========" | tee $LOG

./test_conv.exe       | tee -a $LOG
./test_relu.exe       | tee -a $LOG
./test_add.exe        | tee -a $LOG
./test_stem.exe       | tee -a $LOG
./test_resblock.exe   | tee -a $LOG
./test_cnn.exe        | tee -a $LOG
./test_classifier.exe | tee -a $LOG