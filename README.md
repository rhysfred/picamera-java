# picamera-java

## Synopsis

picamera-java provides Java access to the native libcamera stack for Raspberry Pi devices. 

## Description
picaerma-java includes a C++ wrapper for the rpicam-apps shared object (included as part of the standard Raspberry Pi distro) and a Java FFM (Foreign Function and Memory) API set of classes to enable native access to these functions. It is currently very much a Proof of Concept, and only supports video streaming (not still capture, etc). For still capture there are many existing Java interfaces.
