#!/bin/bash

echo "Cleaning old executables..."

rm -f *.exe

echo ""
echo "Start building..."

echo "Build Stem Conv"
gcc -Icommon -Itest common/quant.c test/load_txt.c layers/conv1d_ref.c test/test_stem_conv1d.c -o test_conv -lm
echo ""

echo "Build ReLU"
gcc -Icommon -Itest test/load_txt.c layers/relu_ref.c test/test_relu.c -o test_relu
echo ""

echo "Build Add"
gcc -Icommon -Itest common/quant.c test/load_txt.c layers/add_ref.c test/test_add.c -o test_add -lm
echo ""

echo "Build Stem"
gcc -Icommon -Itest -Imodel common/quant.c test/load_txt.c layers/conv1d_ref.c layers/relu_ref.c model/stem.c test/test_stem.c -o test_stem -lm
echo ""


echo "Build ResBlock"
gcc -Icommon -Itest -Imodel common/quant.c test/load_txt.c layers/conv1d_ref.c layers/relu_ref.c layers/add_ref.c model/resblock.c test/test_resblock.c -o test_resblock -lm
echo ""

echo "Build Full CNN"
gcc -Icommon -Itest -Imodel common/quant.c test/load_txt.c layers/conv1d_ref.c layers/relu_ref.c layers/add_ref.c model/stem.c model/resblock.c model/downsample.c model/resblock2.c model/cnn.c test/test_cnn.c -o test_cnn -lm
echo ""

echo "Build Classifier"
gcc -Itest test/load_txt.c layers/fc_fp32_ref.c test/test_classifier.c -o test_classifier -lm
echo ""

echo "BUILD DONE"
