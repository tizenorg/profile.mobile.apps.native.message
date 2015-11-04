#!/bin/bash
##------------- project config -------------##
# see the *.spec file to configure the script for a specific project
BINNAME=messages-test
APPNAME=org.tizen.message
TESTRESULT=$APPNAME-Test-result.xml
RESULTPATH=/tmp
TARGETPATH=.

##-----------------------------------------##
rm -f $TARGETPATH/$TESTRESULT
sdb root on
#sdb shell "mkdir -p $RESULTPATH"
sdb shell "/usr/apps/$APPNAME/bin/$BINNAME --gtest_output=xml:$RESULTPATH/$TESTRESULT --gtest_color=yes"
sdb pull $RESULTPATH/$TESTRESULT $TARGETPATH/
sdb shell "rm -f $RESULTPATH/$TESTRESULT"
sdb root off
chmod 777 $TARGETPATH/$TESTRESULT
firefox $TARGETPATH/$TESTRESULT
