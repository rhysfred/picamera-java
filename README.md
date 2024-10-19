# picamera-java

## Synopsis

picamera-java provides Java access to the native libcamera stack for Raspberry Pi devices. 

## Description
picamera-java includes a C++ wrapper for the rpicam-apps shared object (a version is included as part of the standard Raspberry Pi distro, although you will likely want to recompile this) and a set of Java FFM (Foreign Function and Memory) API classes to enable native access to these functions. It is currently very much a work in progress, and initially only targetting video streaming (not still capture, etc). For still capture there are many existing Java interfaces.

## Installation

You might want to wait a couple of weeks - I'm still writing this and there is no point installing something that doesn't yet work! The plan is to package this all up into a mvn central package which should make installation easy.

TBD - Integrate mvn build process to drive the underlying meson build process
TBD - create maven central package

### libav support

The version of rpicam-apps.so that ships with the current Raspberry Pi OS distro is a little old, and strangely is not compiled with libav support. Libav provides support for outputing stream container formats such as mpegts. If you simply want to stream raw h264, the default distro version is sufficient. However to stream more widely supported formats, you probably want to recompile the rpi-cam applications (which include rpicam-apps.so). https://www.raspberrypi.com/documentation/computers/camera_software.html#build-libcamera-and-rpicam-apps provides instructions on how to do this. Note that you may skip recompiling libcamera (unless you want the most up-to-date version) by starting here: https://www.raspberrypi.com/documentation/computers/camera_software.html#building-rpicam-apps-without-building-libcamera

