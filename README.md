# picamera-java

## Synopsis

picamera-java provides Java access to the native libcamera stack for Raspberry Pi devices. 

## Description
picamera-java includes a C++ wrapper for the rpicam-apps shared object (included as part of the standard Raspberry Pi distro) and a set of Java FFM (Foreign Function and Memory) API classes to enable native access to these functions. It is currently very much a Proof of Concept, and only supports video streaming (not still capture, etc). For still capture there are many existing Java interfaces.
